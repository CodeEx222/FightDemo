//


#include "ProcessInputComponent.h"

#include "FightComponent.h"
#include "mode/GameFightCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UProcessInputComponent::UProcessInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::NormalAttack);
}

void UProcessInputComponent::ProcessInputAttackHeavy(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::HeavyAttack);

}

void UProcessInputComponent::ProcessInputDoge(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::Doge);
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
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::MoveForward);
}

void UProcessInputComponent::ProcessInputMoveB(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::MoveBackward);
}

void UProcessInputComponent::ProcessInputMoveL(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::MoveLeft);
}

void UProcessInputComponent::ProcessInputMoveR(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::MoveRight);
}

void UProcessInputComponent::ProcessInputBlockPressed(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::Defend);
}

void UProcessInputComponent::ProcessInputBlockReleased(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	const auto FightComponent = FightCharacter->FightComponent;
	check(FightComponent);
	FightComponent->AddInput(EInputEnum::DefendRelease);
}

void UProcessInputComponent::ProcessInputChangeTargetPressed(const FInputActionValue& Value)
{
	const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
	check(FightCharacter);
	StartPressTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	FightCharacter->ChangeTarget();
	//UE_LOG(LogTemp, Warning, TEXT("Player changed target"));
}



void UProcessInputComponent::ProcessInputChangeTargetLongPressed(const FInputActionValue& Value)
{
	auto EndPressTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if ((EndPressTime - StartPressTime) > 2.0f)
	{
		const auto FightCharacter = Cast<AGameFightCharacter>(GetOwner());
		check(FightCharacter);
		FightCharacter->ChangeTarget();
	}
}

