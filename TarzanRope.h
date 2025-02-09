

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/character.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/BoxComponent.h"
#include "CableComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TarzanRope.generated.h"

UCLASS()
class TAG_API ATarzanRope : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATarzanRope();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Setting TarzanRope")
	USplineComponent* RopeSpline;

	UPROPERTY(VisibleAnywhere, Category = "Setting TarzanRope")
	UStaticMeshComponent* RopeMesh;

	UPROPERTY(VisibleAnywhere, Category = "Setting TarzanRope")
	UPhysicsConstraintComponent* RopePhysics;

	UPROPERTY()
	ACharacter* AttachedPlayer;

	UFUNCTION()
	void GrabRope(ACharacter* Player);

	UFUNCTION()
	void ReleaseRope();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex/*, bool bFromSweep, const FHitResult& SweepResult*/);

	UPROPERTY()
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Setting TarzanRope")
	UCableComponent* Cable;

	UPROPERTY(BlueprintReadOnly, Category = "Setting TarzanRope")
	bool bIsGrabbing;

	UPROPERTY(EditAnywhere, Category = "Setting TarzanRope")
	float AddForcePower;

	UPROPERTY(EditAnyWhere, Category = "Setting TarzanRope")
	float PlayerPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Setting TarzanRope")
	TSubclassOf<UUserWidget> PlayerPowerWidgetClass;

	UPROPERTY()
	UUserWidget* PlayerPowerWidget;

	class UTextBlock* PlayerPowerText;

	UFUNCTION()
	void UpdatePlayerPowerUI();

	float CustomTickCounter = 0.0f;
	float CustomTickInterval = 0.5f;

	void PlayerPowerDown(float Amount);

	//float *PlayerPowerPtr;
};