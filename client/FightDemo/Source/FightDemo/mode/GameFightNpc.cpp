//


#include "GameFightNpc.h"


// Sets default values
AGameFightNpc::AGameFightNpc()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameFightNpc::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameFightNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


