

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Sound/SoundBase.h"
//#include "Materials/MaterialInstanceDynamic.h"
#include "FallingPlatform.generated.h"

UCLASS()
class TAG_API AFallingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlatformMesh;

	FTimerHandle FallTimerHandle;
	FTimerHandle ResetTimerHandle;
	FTimerHandle BlockTimerHandle;
	FTimerHandle DestroyTimerHandle;
	FTimerHandle RespawnTimerHandle;
	FTimerHandle PawnIgnoreTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Setting Falling") 
	float TimeBeforeFall = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Setting Falling")
	float ResetTime = 5.0f;
	
	FVector InitialLocation;
	FRotator InitialRotation;

	UFUNCTION()
	void StartShake();

	UFUNCTION()
	void StartFall();

	UFUNCTION()
	void ResetPlatform();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Timeline
	UPROPERTY()
	UTimelineComponent* ShakeTimeline;

	//�^�C�����C���̃J�[�u�쐬
	UPROPERTY(EditAnywhere, Category = "Setting Falling")
	UCurveFloat* ShakeCurve;

	//�^�C�����C���̍X�V�֐�
	UFUNCTION()
	void HandleShakeProgress(float Value);

	//�T�E���h
	UPROPERTY(EditAnywhere, Category = "Setting Falling")
	USoundBase* WarningSound;

	/*UFUNCTION()
	void RespawnActor();*/

	UFUNCTION()
	void DestroyActor();

	//�o��Ȃ��悤��
	UFUNCTION()
	void PawnIgnore();

	//�}�e���A��
	/*UPROPERTY(EditAnywhere, Category = "Setting Falling")
	UMaterialInstanceDynamic* DynamicMaterial;

	void ChangeColor(FLinearColor NewColor);*/

	



};
