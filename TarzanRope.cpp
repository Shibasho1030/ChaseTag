


#include "TarzanRope.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ATarzanRope::ATarzanRope()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RopeSpline = CreateDefaultSubobject<USplineComponent>(TEXT("RopeSpline"));
	SetRootComponent(RopeSpline);

	RopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeMesh"));
	RopeMesh->SetupAttachment(RootComponent);

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(RopeMesh);

	RopePhysics = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RopePhysics"));
	RopePhysics->SetupAttachment(RopeMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	PlayerPower = 3;
	bIsGrabbing = false;
	AttachedPlayer = nullptr;
	//PlayerPowerPtr = nullptr;
	//PlayerPowerPtr = &PlayerPower;
}

// Called when the game starts or when spawned
void ATarzanRope::BeginPlay()
{
	Super::BeginPlay();
	 
	Cable->SetSimulatePhysics(true);
	Cable->SetEnableGravity(true);
	Cable->SetWorldScale3D(FVector(1, 1, 1));

	RopeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	CollisionBox->SetBoxExtent(FVector(300, 200, 700));
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATarzanRope::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATarzanRope::OnOverlapEnd);

	Cable->bAttachStart = true; 
	Cable->bAttachEnd = false;
	Cable->CableLength = 300.0f;
	Cable->NumSegments = 10;
	Cable->CableWidth = 1.0f;
	Cable->SetSimulatePhysics(true);

	if (PlayerPowerWidgetClass)
	{
		PlayerPowerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerPowerWidgetClass);
		if (PlayerPowerWidget)
		{
			PlayerPowerWidget->AddToViewport();
			PlayerPowerText = Cast<UTextBlock>(PlayerPowerWidget->GetWidgetFromName(TEXT("PlayerPowerText")));
		}
	}

	ATarzanRope::UpdatePlayerPowerUI();
}

// Called every frame
void ATarzanRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	if (bIsGrabbing)
	{
		if (AttachedPlayer)
		{
			//UE_LOG(LogTemp, Warning, TEXT("IfTick"));
			FVector TarzanVelocity = AttachedPlayer->GetActorLocation() - ATarzanRope::GetActorLocation();
			FVector PlayerVelocity = AttachedPlayer->GetVelocity();
			float Dot = FVector::DotProduct(TarzanVelocity, PlayerVelocity);
			FVector NormalizeVelocity = TarzanVelocity.GetSafeNormal();
			AttachedPlayer->GetCharacterMovement()->AddForce(AddForcePower * Dot * NormalizeVelocity);
		}		
	}

	//プレイヤーの残り体力を更新
	CustomTickCounter += DeltaTime;
	if (CustomTickCounter > CustomTickInterval)
	{
		PlayerPower += 0.04f;
		if (bIsGrabbing)
		{
			PlayerPower -= 0.1f;
		}

		if (PlayerPower >= 3)
		{
			PlayerPower = 3;
		}
		if (PlayerPower < 0)
		{
			ATarzanRope::ReleaseRope();
			PlayerPower = 0;
		}
		CustomTickCounter = 0.0f;
		ATarzanRope::UpdatePlayerPowerUI();
		//UE_LOG(LogTemp, Warning, TEXT("PlayerPower: %.2f"), PlayerPower);
	}
}


 
void ATarzanRope::GrabRope(ACharacter* Player)
{
	if (Player && PlayerPower >= 1.0f)
	{
		Cable->bAttachEnd = true;
		bIsGrabbing = true;
		
		PlayerPower -= 1.0f;
		
		AttachedPlayer = Player;

		Cable->SetAttachEndTo(AttachedPlayer, FName(TEXT("GrabPoint"))/*, EAttachLocation::KeepWorldPosition*/);
		Player->AttachToComponent(RopeMesh, FAttachmentTransformRules::KeepWorldTransform);
		//Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void ATarzanRope::ReleaseRope()
{
	if (AttachedPlayer)
	{
		bIsGrabbing = false;
		Cable->bAttachEnd = false;
		Cable->SetAttachEndTo(nullptr, NAME_None/*, EAttachLocation::KeepWorldPosition*/);
		AttachedPlayer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//AttachedPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		AttachedPlayer = nullptr;
	}
}

void ATarzanRope::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		ATarzanRope::GrabRope(Player);
	}
}

void ATarzanRope::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		if (AttachedPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("EndForce"));
			AttachedPlayer->GetCharacterMovement()->AddForce(FVector(0, 0, 2999900));
		}
		ATarzanRope::ReleaseRope();
		Cable->CableLength = 300.0f;
	}
}

void ATarzanRope::UpdatePlayerPowerUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateText"));
	if (PlayerPowerText)
	{
		PlayerPowerText->SetText(FText::FromString(FString::Printf(TEXT("Player Power Left: %.2f"), PlayerPower)));
	}
}

void ATarzanRope::PlayerPowerDown(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerPowerDown"));
	PlayerPower -= Amount;
	PlayerPower = FMath::Clamp(PlayerPower, 0.0f, 3.0f);
}

