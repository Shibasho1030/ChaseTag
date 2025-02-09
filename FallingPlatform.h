

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

	//タイムラインのカーブ作成
	UPROPERTY(EditAnywhere, Category = "Setting Falling")
	UCurveFloat* ShakeCurve;

	//タイムラインの更新関数
	UFUNCTION()
	void HandleShakeProgress(float Value);

	//サウンド
	UPROPERTY(EditAnywhere, Category = "Setting Falling")
	USoundBase* WarningSound;

	/*UFUNCTION()
	void RespawnActor();*/

	UFUNCTION()
	void DestroyActor();

	//登らないように
	UFUNCTION()
	void PawnIgnore();

	//マテリアル
	/*UPROPERTY(EditAnywhere, Category = "Setting Falling")
	UMaterialInstanceDynamic* DynamicMaterial;

	void ChangeColor(FLinearColor NewColor);*/

	



};
