


#include "GameFightCharacter.h"

#include "EnhancedInputComponent.h"
#include "Components/WidgetComponent.h"
#include "FightDemo/Anim/AnimDefine.h"
#include "fight/FightComponent.h"
#include "fight/GameAnimInstance.h"
#include "fight/PlayerAttributeComponent.h"
#include "fight/ProcessInputComponent.h"
#include "mode/FightInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ui/HeadView.h"

// Sets default values
AGameFightCharacter::AGameFightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FightComponent = CreateDefaultSubobject<UFightComponent>(TEXT("FightComponent"));
	ProcessInputComponent = CreateDefaultSubobject<UProcessInputComponent>(TEXT("ProcessInputComponent"));
	PlayerAttributeComponent = CreateDefaultSubobject<UPlayerAttributeComponent>(TEXT("PlayerAttributeComponent"));

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
		EnhancedInputComponent->BindAction(ProcessInputComponent->InputChangeTarget, ETriggerEvent::Completed,
			ProcessInputComponent, &UProcessInputComponent::ProcessInputChangeTargetReleased);
	}

}

void AGameFightCharacter::UpdateActorRotator()
{
	// 处于移动中 actor朝向向摄影机朝向旋转
	auto camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	auto cameraRot = camera->GetCameraRotation();
	// 获取 cameraRot xy平面上的旋转值
	cameraRot.Pitch = 0;
	cameraRot.Roll = 0;

	auto actorRot = GetActorRotation();
	auto setActorRot = actorRot;
	setActorRot.Pitch = 0;
	setActorRot.Roll = 0;


	auto gameDelta = GetWorld()->GetDeltaSeconds();
	auto rot = FMath::RInterpTo(setActorRot, cameraRot, gameDelta  , 5);
	actorRot.Yaw = rot.Yaw;
	SetActorRotation(actorRot);
}

bool AGameFightCharacter::IsAttackPlayer(AGameFightCharacter* Target)
{
	auto selfPos = this->GetActorLocation();
	auto targetPos = Target->GetActorLocation();

	auto dir = targetPos - selfPos;


	// 计算距离
	auto len = dir.Length();
	if (len < 200)
	{
		dir.Normalize();
		auto selfVertor = this->GetActorForwardVector();

		float angle = acos(FVector::DotProduct(selfVertor, dir)/ (selfVertor.Size()*selfVertor.Size()) ) * 180 / PI;

		if (angle <= 40)
		{
			return true;
		}

	}

	return false;

}

void AGameFightCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);

		UAnimInstance * AnimInstance =  GetMesh() ?  GetMesh()->GetAnimInstance() : nullptr;
		check(AnimInstance != nullptr);
		const auto OwnAnimInstance = Cast<UGameAnimInstance>(AnimInstance);
		check(OwnAnimInstance != nullptr);
		OwnAnimInstance->SetMoveDirection(Right,Forward);

		UpdateActorRotator();
	}
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

void AGameFightCharacter::DoAnimNotify(UFightAnimNotify* AnimNotify)
{
	if (FightComponent == nullptr || AnimNotify == nullptr)
	{
		return;
	}
	FightComponent->OnAnimNotify(AnimNotify);
}

void AGameFightCharacter::DoAnimNotifyState(UFightAnimNotifyState* AnimNotyfy, bool bState)
{
	if (FightComponent == nullptr || AnimNotyfy == nullptr)
	{
		return;
	}

	FightComponent->OnAnimNotifyState(AnimNotyfy, bState);
}



