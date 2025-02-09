

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameStarter.generated.h"


UCLASS()
class TAG_API AGameStarter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameStarter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting GameStart")
	TSubclassOf<UUserWidget> CountdownWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting GameStart")
	TSubclassOf<UUserWidget> StartCountdownWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Setting GameStart")
	void UpdateStartCountdownText(const FString& NewText);

	UFUNCTION(BlueprintCallable, Category = "Setting GameStart")
	void UpdateCountdownText(const FString& NewText);*/

private:
	UPROPERTY(VisibleAnywhere, Category = "Setting GameStart");
	class UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartCountdown();
	void StartCountdownTick();
	void CountdownTick();
	void GameStart();
	void UpdateStartCountdownUI();
	void UpdateCountdownUI();

	FTimerHandle StartCountdownTimerHandle;
	FTimerHandle CountdownTimerHandle;
	FTimerHandle GameStartTimerHandle;
	int32 StartCountdownTime;
	UPROPERTY(EditAnywhere, Category = "Setting GameStart")
	int32 CountdownTime;
	bool bGameStarted;

	UPROPERTY(EditAnywhere, Category = "Setting GameStart")
	USoundCue* GameStartSound1;

	UPROPERTY(EditAnywhere, Category = "Setting GameStart")
	USoundCue* GameStartSound2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting GameStart", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> StartCountdownWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting GameStart", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CountdownWidgetClass;

	UUserWidget* StartCountdownWidget;
	UUserWidget* CountdownWidget;

	//UPROPERTY(EditAnywhere, Category = "Setting GameStart")
	class UTextBlock* StartCountdownText;

	//UPROPERTY(EditAnywhere, Category = "Setting GameStart")
	class UTextBlock* CountdownText;

};
