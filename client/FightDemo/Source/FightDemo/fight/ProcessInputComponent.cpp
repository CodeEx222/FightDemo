//


#include "ProcessInputComponent.h"

#include "mode/GameFightCharacter.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UProcessInputComponent::UProcessInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProcessInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UProcessInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UProcessInputComponent::ProcessInputAttack(const FInputActionValue& Value)
{

}

void UProcessInputComponent::ProcessInputAttackHeavy(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputDoge(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputLook(const FInputActionValue& Value)
{
	if (const auto Player = Cast<AGameFightCharacter>(GetOwner()))
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		// route the input
		Player->DoLook(LookAxisVector.X, LookAxisVector.Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not a AGameFightCharacter!"));
	}

}

void UProcessInputComponent::ProcessInputMove(const FInputActionValue& Value)
{
	if (const auto Player = Cast<AGameFightCharacter>(GetOwner()))
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		// route the input
		Player->DoMove(LookAxisVector.X, LookAxisVector.Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not a AGameFightCharacter!"));
	}
}

void UProcessInputComponent::ProcessInputMoveF(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputMoveB(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputMoveL(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputMoveR(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputBlockPressed(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputChangeTargetPressed(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputBlockReleased(const FInputActionValue& Value)
{
}

void UProcessInputComponent::ProcessInputChangeTargetReleased(const FInputActionValue& Value)
{
}

