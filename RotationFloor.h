

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotationFloor.generated.h"

UCLASS()
class TAG_API ARotationFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotationFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationSpeed;

};
