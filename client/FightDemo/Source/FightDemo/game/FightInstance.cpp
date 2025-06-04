// 


#include "FightInstance.h"

void UFightInstance::Init()
{
	Super::Init();


	SkillDataTable = Cast<UDataTable>(LoadObject<UObject>(nullptr,
		TEXT("/Game/Game/data/AttackTable.AttackTable")));
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
}
