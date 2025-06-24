// 


#include "FightInstance.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UFightInstance::Init()
{
	Super::Init();

	auto SkillDataTable = CommonFightTable;
	if (!IsUseCommon) {
		SkillDataTable = GameFightTable;
	}

	if (SkillDataTable) {
		// 循环表
		for (TArray<FName> RowNames = SkillDataTable->GetRowNames(); const FName RowName : RowNames) {
			// 获取行数据
			FAttackAnimTable* SkillActionInfo = SkillDataTable->FindRow<FAttackAnimTable>(RowName, TEXT(""));
			if (SkillActionInfo) {
				AttackAnimTableArray.Add(SkillActionInfo);
			}
		}
	}

	LoadedAnims.Empty();
	// 预加载所有角色动画
	LoadAllAnimSequencesAsync(
		"/Game/common/FightAnimations/Hit", true);
}

TArray<FString> UFightInstance::GetAllAnimSequencePaths(const FString& Directory, bool bRecursive)
{
	TArray<FString> AnimPaths;

	// 获取资产注册表模块
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// 递归获取所有资产数据
	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*Directory), AssetDataList, bRecursive);

	// 筛选 AnimSequence 资源
	for (const FAssetData& AssetData : AssetDataList)
	{
		if (AssetData.AssetClassPath == UAnimSequence::StaticClass()->GetClassPathName())
		{
			AnimPaths.Add(AssetData.GetObjectPathString());
		}
	}

	return AnimPaths;
}

void UFightInstance::LoadAllAnimSequencesAsync(const FString& RootDirectory, bool bRecursive)
{
	// 获取所有动画路径
	TArray<FString> AnimPaths = GetAllAnimSequencePaths(RootDirectory, bRecursive);

	if (AnimPaths.Num() == 0)
	{
		return;
	}

	// 创建流式加载管理器
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	// 转换路径为软引用列表
	TArray<FSoftObjectPath> AssetsToLoad;
	for (const FString& Path : AnimPaths)
	{
		AssetsToLoad.Add(FSoftObjectPath(Path));
	}

	// 异步加载回调
	FStreamableDelegate Delegate = FStreamableDelegate::CreateLambda(
		[this,AssetsToLoad]()
		{
			for (const FSoftObjectPath& Path : AssetsToLoad)
			{
				UAnimSequence* AnimSequence = Cast<UAnimSequence>(Path.ResolveObject());
				if (AnimSequence)
				{
					LoadedAnims.Add(Path.ToString(), AnimSequence);
				}
			}
		}
	);

	// 开始异步加载
	Streamable.RequestAsyncLoad(AssetsToLoad, Delegate);
}

EHitDirection8 UFightInstance::CalculateHitDirection8(const FVector& VictimLocation, const FRotator& VictimRotation,
                                                      const FVector& AttackerLocation)
{
	// 1. 计算攻击者相对于自身的方向向量（世界空间）
	FVector ToAttacker = AttackerLocation - VictimLocation;
	ToAttacker.Z = 0; // 忽略垂直方向
	ToAttacker.Normalize();

	// 2. 将方向向量转换到自身坐标系
	FVector LocalDirection = VictimRotation.UnrotateVector(ToAttacker);
	LocalDirection.Normalize();

	// 3. 计算角度（0-360度）
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(LocalDirection.Y, LocalDirection.X));
	if (Angle < 0) Angle += 360;

	// 4. 映射到8个方向（每个方向45度）
	// 方向划分：
	//   Front: [337.5, 22.5)
	//   FrontRight: [22.5, 67.5)
	//   Right: [67.5, 112.5)
	//   BackRight: [112.5, 157.5)
	//   Back: [157.5, 202.5)
	//   BackLeft: [202.5, 247.5)
	//   Left: [247.5, 292.5)
	//   FrontLeft: [292.5, 337.5)

	// 调整角度使0度对应正前方
	Angle = FMath::Fmod(Angle + 22.5f, 360.0f);

	if (Angle < 45) return EHitDirection8::Front;
	if (Angle < 90) return EHitDirection8::FrontRight;
	if (Angle < 135) return EHitDirection8::Right;
	if (Angle < 180) return EHitDirection8::BackRight;
	if (Angle < 225) return EHitDirection8::Back;
	if (Angle < 270) return EHitDirection8::BackLeft;
	if (Angle < 315) return EHitDirection8::Left;
	return EHitDirection8::FrontLeft;
}

EHitDirection8 UFightInstance::CalculateHitDirection(AActor* Victim, AActor* Attacker)
{
	if (!Victim || !Attacker)
		return EHitDirection8::Front; // 默认值

	return CalculateHitDirection8(
		Victim->GetActorLocation(),
		Victim->GetActorRotation(),
		Attacker->GetActorLocation()
	);
}
