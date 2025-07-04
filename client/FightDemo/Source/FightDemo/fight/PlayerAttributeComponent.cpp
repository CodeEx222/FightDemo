//


#include "PlayerAttributeComponent.h"

#include "FightDemo/mode/GameFightCharacter.h"
#include "FightDemo/ui/HeadView.h"
#include "mode/GameFightNpc.h"


// Sets default values for this component's properties
UPlayerAttributeComponent::UPlayerAttributeComponent(): HPValue(), BlockValue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();



	ChangeHpValue(HPValue.MaxValue);
	ChangeBlockValue(0);

}


// Called every frame
void UPlayerAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...


	const auto currentTime = GetWorld()->GetTimeSeconds();
	if (currentTime - LastChangeBlockTime > BlockValue.RecoverTime)
	{
		if (BlockValue.Value > 0)
		{
			// Recover block value over time
			const double RecoverValue = (BlockValue.RecoverValue) * DeltaTime ;
			ChangeBlockValue(-RecoverValue,false);
		}

	}

}

void UPlayerAttributeComponent::ChangeHpValue(const double Value)
{
	HPValue.Value += Value;

	HPValue.Value = FMath::Clamp(HPValue.Value, 0.0, HPValue.MaxValue);

	GetHeadViewWidget();
	if (HeadViewWidget.IsValid())
	{
		HeadViewWidget->SetHpPercentage(HPValue.Value / HPValue.MaxValue);
	}
}

void UPlayerAttributeComponent::ChangeBlockValue(const double Value, const bool bIsSetTime)
{

	BlockValue.Value += Value;
	BlockValue.Value = FMath::Clamp(BlockValue.Value, 0.0, BlockValue.MaxValue);

	if (bIsSetTime)
	{
		LastChangeBlockTime = GetWorld()->GetTimeSeconds();
	}

	GetHeadViewWidget();
	if (HeadViewWidget.IsValid())
	{
		HeadViewWidget->SetBlockPercentage(BlockValue.Value / BlockValue.MaxValue);
	}
}

void UPlayerAttributeComponent::GetHeadViewWidget()
{
	if (!HeadViewWidget.IsValid())
	{

		if (const auto GameFightCharacter = Cast<AGameFightCharacter>(GetOwner()); GameFightCharacter)
		{
			HeadViewWidget = Cast<UHeadView>(GameFightCharacter->HeadViewUI->GetUserWidgetObject());
		}
		else if (const auto GameFightNpc = Cast<AGameFightNpc>(GetOwner()); GameFightNpc)
		{
			HeadViewWidget = Cast<UHeadView>(GameFightNpc->HeadViewUI->GetUserWidgetObject());
		}
	}
}

