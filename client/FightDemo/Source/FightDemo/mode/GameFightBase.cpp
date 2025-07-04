//


#include "GameFightBase.h"

#include "Anim/AnimDefine.h"
#include "fight/FightComponent.h"
#include "fight/GameAnimInstance.h"
#include "fight/PlayerAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGameFightBase::AGameFightBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FightComponent = CreateDefaultSubobject<UFightComponent>(TEXT("FightComponent"));
	PlayerAttributeComponent = CreateDefaultSubobject<UPlayerAttributeComponent>(TEXT("PlayerAttributeComponent"));
}

// Called when the game starts or when spawned
void AGameFightBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameFightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity.Size() > 0.0f)
	{
		// 如果有移动速度, Mesh 旋转值旋转到actor旋转
		const auto Mesh = GetMesh();
		const auto OldMeshRotation = Mesh->GetComponentRotation();
		const auto NewRotation = GetActorRotation();

	}
}

void AGameFightBase::SetGameActorRotation(const FRotator NewRotation)
{
	const auto Mesh = GetMesh();
	const auto OldMeshRotation = Mesh->GetComponentRotation();
	SetActorRotation(NewRotation);
	Mesh->SetWorldRotation(OldMeshRotation);
}

void AGameFightBase::MoveToDir(FRotator MoveDir, float Right, float Forward)
{

	SetGameActorRotation(MoveDir);

	const FRotator YawRotation(0, MoveDir.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);

}


void AGameFightBase::ChangeTarget()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameFightBase::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		if (Actor == this)
		{
			continue;
		}
		FightTarget = Cast<AGameFightBase>(Actor);
		if (FightTarget != nullptr)
		{
			GetCharacterMovement()->MaxWalkSpeed = 130.0f; // 恢复默认移动速度
			break;
		}

	}
}

void AGameFightBase::ClearTarget()
{
	FightTarget = nullptr;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f; // 恢复默认移动速度
}

void AGameFightBase::DoAnimNotify(UFightAnimNotify* AnimNotify)
{
	if (FightComponent == nullptr || AnimNotify == nullptr)
	{
		return;
	}
	FightComponent->OnAnimNotify(AnimNotify);
}

void AGameFightBase::DoAnimNotifyState(UFightAnimNotifyState* AnimNotyfy, bool bState)
{
	if (FightComponent == nullptr || AnimNotyfy == nullptr)
	{
		return;
	}

	FightComponent->OnAnimNotifyState(AnimNotyfy, bState);
}

