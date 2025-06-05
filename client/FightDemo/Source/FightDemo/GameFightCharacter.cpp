


#include "GameFightCharacter.h"

#include "fight/AnimDefine.h"
#include "fight/FightComponent.h"
#include "game/FightInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameFightCharacter::AGameFightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FightComponent = CreateDefaultSubobject<UFightComponent>(TEXT("FightComponent"));

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

bool AGameFightCharacter::IsAttackPlayer(AGameFightCharacter* target)
{
	auto selfPos = this->GetActorLocation();
	auto targetPos = target->GetActorLocation();

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



