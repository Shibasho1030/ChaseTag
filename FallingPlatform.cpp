


#include "FallingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
//#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AFallingPlatform::AFallingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Box Collision�̐���
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	//���b�V���R���|�[�l���g���쐬
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	//PlatformMesh->SetWorldLocation(GetActorLocation());
	PlatformMesh->SetupAttachment(TriggerBox);
	RootComponent = PlatformMesh;
	//PlatformMesh->SetCollisionResponseToAllChannels(ECR_Block);

	//Timeline�̍쐬
	ShakeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ShakeTimeline"));


}

// Called when the game starts or when spawned
void AFallingPlatform::BeginPlay()
{
	Super::BeginPlay();

	PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//�R���W������ݒ�
	TriggerBox->SetBoxExtent(FVector(151.0f, 151.0f, 51.0f));
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	//TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	UE_LOG(LogTemp, Warning, TEXT("this is spawned at location: %s"), *GetActorLocation().ToString());

	PlatformMesh->SetWorldScale3D(FVector(3.0f, 3.0f, 1.0f));
	PlatformMesh->SetWorldLocation(GetActorLocation());

	//�I�[�o�[���b�v�C�x���g�̐ݒ�
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlatform::OnOverlapBegin);

	InitialLocation = GetActorLocation();
	InitialRotation = GetActorRotation();

	//Timeline�̐ݒ�
	if (ShakeCurve)
	{
		//�^�C�����C���̍X�V���̃R�[���o�b�N��ݒ�
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleShakeProgress"));
		

		ShakeTimeline->AddInterpFloat(ShakeCurve, ProgressFunction);
	}
	
	//�}�e���A���̓��I�C���X�^���X�쐬�i�F�ύX�p�j
	/*if (PlatformMesh)
	{
		UMaterialInterface* Material = PlatformMesh->GetMaterial(0);
		if (Material)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			PlatformMesh->SetMaterial(0, DynamicMaterial);
		}
	}*/

}

// Called every frame
//void AFallingPlatform::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AFallingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *OtherActor->GetName());
		//����O�̌x�������Đ�
		/*if (WarningSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, WarningSound, GetActorLocation());
		}*/

		StartShake();

		TriggerBox->SetGenerateOverlapEvents(false);

		PlatformMesh->SetVisibility(true);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Non OtherActor"));
	}
}

void AFallingPlatform::StartShake()
{
	if (ShakeTimeline)
	{
		ShakeTimeline->PlayFromStart();
	}

	GetWorldTimerManager().SetTimer(PawnIgnoreTimerHandle, this, &AFallingPlatform::PawnIgnore, TimeBeforeFall - 0.01f, false);

	//��莞�Ԍ�ɗ���
	GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AFallingPlatform::StartFall, TimeBeforeFall, false);

}

void AFallingPlatform::HandleShakeProgress(float Value)
{
	FVector NewLocation = InitialLocation;
	NewLocation.X += FMath::Sin(Value * 20.0f) * 10.0f; //X�������ɗh���
	NewLocation.Y += FMath::Cos(Value * 20.0f) * 10.0f; //Y
	SetActorLocation(NewLocation);
	PlatformMesh->SetWorldLocation(NewLocation);
	SetActorRotation(InitialRotation);

}

void AFallingPlatform::StartFall()
{
	UE_LOG(LogTemp, Warning, TEXT("StartFall"));

	//����Ƃ��F��Ԃɂ���
	//ChangeColor(FLinearColor::Red);

	//Sound
	if (WarningSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WarningSound, GetActorLocation());
	}

	

	//��������[�V������L����
	TriggerBox->SetSimulatePhysics(true);
	PlatformMesh->SetSimulatePhysics(true);

	//PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	//Destroy���邩�炢��Ȃ� GetWorldTimerManager().SetTimer(BlockTimerHandle, this, &AFallingPlatform::PawnBlock, ResetTime, false);

	//��莞�Ԍ�Ƀ��Z�b�g
	GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AFallingPlatform::ResetPlatform, ResetTime, false);

	//GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFallingPlatform::RespawnActor, 5.0f, false);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AFallingPlatform::DestroyActor, 1.5f, false);

}

//void AFallingPlatform::RespawnActor()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Respawn"));
//	GetWorld()->SpawnActor<AFallingPlatform>(GetClass(), FVector(0, 0, 0), InitialRotation);
//}

void AFallingPlatform::ResetPlatform()
{
	/*PlatformMesh->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;
	TriggerBox->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;*/

	PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	//PlatformMesh->SetCollisionResponseToChannel(ECC_Climbable, ECR_Block);
	//TriggerBox->SetCollisionResponseToChannel(ECC_Climbable, ECR_Overlap);

	TriggerBox->SetSimulatePhysics(false);
	PlatformMesh->SetSimulatePhysics(false);

	TriggerBox->SetWorldRotation(InitialRotation);
	PlatformMesh->SetWorldRotation(InitialRotation);
	
	PlatformMesh->SetWorldLocation(InitialLocation);
	TriggerBox->SetWorldLocation(InitialLocation);
	SetActorLocation(InitialLocation);
	SetActorRotation(InitialRotation);

	//���x�Ɖ�]���[���ɂ���
	TriggerBox->SetPhysicsLinearVelocity(FVector::ZeroVector);
	TriggerBox->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	TriggerBox->SetGenerateOverlapEvents(true);

	//PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//�F�����ɖ߂�
	//ChangeColor(FLinearColor::White);

}

void AFallingPlatform::PawnIgnore()
{
	UE_LOG(LogTemp, Warning, TEXT("PawnIgnore"));
	PlatformMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//PlatformMesh->SetCollisionResponseToChannel(ECC_Climbable, ECR_Ignore);
	//TriggerBox->SetCollisionResponseToChannel(ECC_Climbable, ECR_Ignore);
}

//void AFallingPlatform::ChangeColor(FLinearColor NewColor)
//{
//	if (DynamicMaterial)
//	{
//		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), NewColor);
//	}
//}


void AFallingPlatform::DestroyActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn"));
	PlatformMesh->SetWorldLocation(FVector(0, 0, 0));
	TriggerBox->SetWorldLocation(FVector(0, 0, 0));
	//SetActorLocation(FVector(0, 0, 0));
	//SetLifeSpan(0.5f);
	//GetWorld()->SpawnActor<AFallingPlatform>(GetClass(), FVector(0, 0, 0), InitialRotation);
	//PlatformMesh->DestroyComponent();
	//TriggerBox->DestroyComponent();
}
