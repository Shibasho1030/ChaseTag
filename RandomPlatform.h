
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomPlatform.generated.h"






UCLASS()
class TAG_API ARandomPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARandomPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�����ʒu
	UPROPERTY(EditAnywhere, Category = "RandomPlatform Setting")
	FVector StartLocation;

	//�ړ��X�s�[�h
	UPROPERTY(EditAnywhere, Category = "RandomPlatform Setting")
	float MoveSpeed;

	//�ړ��͈�
	UPROPERTY(EditAnywhere, Category = "RandomPlatform Setting")
	float MoveDistance;

	//��������
	UPROPERTY(EditAnywhere, Category = "RandomPlatform Setting")
	FVector MoveDirection;

	UFUNCTION()
	FVector RotateVectorZ(double x, double y);

	//UFUNCTION()
	//FVector RotateVector-Z(double x, double y);

};
