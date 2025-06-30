


#include "GameFightCharacter.h"

#include "EnhancedInputComponent.h"
#include "Components/WidgetComponent.h"
#include "FightDemo/Anim/AnimDefine.h"
#include "fight/FightComponent.h"
#include "fight/GameAnimInstance.h"
#include "fight/PlayerAttributeComponent.h"
#include "fight/ProcessInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "mode/FightInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ui/HeadView.h"

// Sets default values
AGameFightCharacter::AGameFightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProcessInputComponent = CreateDefaultSubobject<UProcessInputComponent>(TEXT("ProcessInputComponent"));


	HeadViewUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadViewUI"));
	HeadViewUI->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGameFightCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameFightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGameFightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


		// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputMove, ETriggerEvent::Triggered,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputMove);

		// Looking
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputLook, ETriggerEvent::Triggered,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputLook);

		// Combo Attack
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputAttack, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputAttack);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputAttackHeavy, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputAttackHeavy);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputMoveF, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputMoveF);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputMoveB, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputMoveB);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputMoveL, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputMoveL);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputMoveR, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputMoveR);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputDoge, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputDoge);

		// Charged Attack
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputBlock, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputBlockPressed);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputBlock, ETriggerEvent::Completed,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputBlockReleased);

		EnhancedInputComponent->BindAction(ProcessInputComponent->InputChangeTarget, ETriggerEvent::Started,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputChangeTargetPressed);
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputChangeTarget, ETriggerEvent::Ongoing,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputChangeTargetLongPressed);
	}



}

void AGameFightCharacter::UpdateActorRotator()
{
	FRotator TargetRot;
	if (FightTarget == nullptr)
	{
		// 处于移动中 actor朝向向摄影机朝向旋转
		auto camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		TargetRot = camera->GetCameraRotation();
		// 获取 cameraRot xy平面上的旋转值

	}
	else
	{
		auto targetPos = FightTarget->GetActorLocation();
		auto selfPos = this->GetActorLocation();
		auto dir = targetPos - selfPos;
		// 计算方向向量
		dir.Normalize();
		TargetRot = dir.Rotation();
	}

	TargetRot.Pitch = 0;
	TargetRot.Roll = 0;



	auto actorRot = GetActorRotation();
	auto setActorRot = actorRot;
	setActorRot.Pitch = 0;
	setActorRot.Roll = 0;


	auto gameDelta = GetWorld()->GetDeltaSeconds();
	auto rot = FMath::RInterpTo(setActorRot, TargetRot, gameDelta  , 15);
	actorRot.Yaw = rot.Yaw;
	SetActorRotation(actorRot);
}



bool AGameFightCharacter::IsAttackPlayer(AGameFightCharacter* Target)
{
	const auto SelfPos = this->GetActorLocation();
	const auto TargetPos = Target->GetActorLocation();

	auto Dir = TargetPos - SelfPos;


	// 计算距离
	const auto len = Dir.Length();
	if (len < 200)
	{
		Dir.Normalize();
		auto selfVertor = this->GetActorForwardVector();

		float angle = acos(FVector::DotProduct(selfVertor, Dir)/ (selfVertor.Size()*selfVertor.Size()) ) * 180 / PI;

		if (angle <= 40)
		{
			return true;
		}

	}

	return false;

}

void AGameFightCharacter::DoMove(float Right, float Forward)
{
	FRotator MoveRotation = FRotator::ZeroRotator;
	// find out which way is forward
	MoveRotation = GetController()->GetControlRotation();

	const FRotator YawRotation(0, MoveRotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);

	UpdateActorRotator();
}

void AGameFightCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}




