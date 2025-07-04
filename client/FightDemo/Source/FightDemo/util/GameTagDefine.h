
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"



#define TAG(x) UGameplayTagsManager::Get().RequestGameplayTag(TEXT(x))