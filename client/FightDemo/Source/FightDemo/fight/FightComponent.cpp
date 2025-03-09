// 


#include "FightComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UFightComponent::UFightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFightComponent::AddInput(EInputEnum inputEnum)
{
	double GameTime = UGameplayStatics::GetTimeSeconds(this);
}

