


#include "WarpActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TarzanRope.h"
#include "TimerManager.h"

// Sets default values
AWarpActor::AWarpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWarpActor::OnOverlapBegin);

	bHasWarped = false;
}

// Called when the game starts or when spawned
void AWarpActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWarpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarpActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasWarped)
	{
		return;
	}
	ATarzanRope* TarzanRopeInstance = Cast<ATarzanRope>(UGameplayStatics::GetActorOfClass(GetWorld(), ATarzanRope::StaticClass()));
	if (!TarzanRopeInstance)
	{
		return;
	}
	if (OtherActor && TargetWarpActor && (TarzanRopeInstance->PlayerPower > 1.0f) )
	{
		//FRotator  CurrentRotation = OtherActor->GetActorRotation();

		OtherActor->SetActorLocation(TargetWarpActor->GetActorLocation());
		//OtherActor->SetActorRotation(CurrentRotation);
		TarzanRopeInstance->PlayerPowerDown(1.0f);
		UE_LOG(LogTemp, Warning, TEXT("PlayerPowerAfter: %.2f"), TarzanRopeInstance->PlayerPower);
		if (ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			/*AController* Controller = Pawn->GetController();
			if (Controller)
			{
				Controller->SetControlRotation(CurrentRotation);
			}*/
			if (Character->GetMovementComponent()/* && UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)*/)
			{
				Character->GetMovementComponent()->StopMovementImmediately();
			}
		}
	}
	bHasWarped = true;
	FTimerHandle ResetTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, this, &AWarpActor::ResetWarpFlag, 0.5f, false);
	TarzanRopeInstance->UpdatePlayerPowerUI();
}

void AWarpActor::ResetWarpFlag()
{
	bHasWarped = false;
}