


#include "GameStarter.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AGameStarter::AGameStarter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGameStarted = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGameStarter::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AGameStarter::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartCountdownWidgetClass)
	{
		StartCountdownWidget = CreateWidget<UUserWidget>(GetWorld(), StartCountdownWidgetClass);
		if (StartCountdownWidget)
		{
			StartCountdownWidget->AddToViewport();
			StartCountdownText = Cast<UTextBlock>(StartCountdownWidget->GetWidgetFromName(TEXT("StartCountdownText")));
		}
	}

	if (CountdownWidgetClass)
	{
		CountdownWidget = CreateWidget<UUserWidget>(GetWorld(), CountdownWidgetClass);
		if (CountdownWidget)
		{
			CountdownWidget->AddToViewport();
			CountdownText = Cast<UTextBlock>(CountdownWidget->GetWidgetFromName(TEXT("CountdownText")));
		}
	}
}

 //Called every frame
void AGameStarter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameStarter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Pawn)
	{
		if (!bGameStarted)
		{
			bGameStarted = true;
			StartCountdown();
		}
	}
}

void AGameStarter::StartCountdown()
{
	UE_LOG(LogTemp, Warning, TEXT("StartCountdown"));
	StartCountdownTime = 3;
	GetWorld()->GetTimerManager().SetTimer(StartCountdownTimerHandle, this, &AGameStarter::StartCountdownTick, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &AGameStarter::GameStart, 3.9f, false);
}

void AGameStarter::GameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("GameStart"));
	if (GameStartSound2)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, GameStartSound2, FVector(-3885, -1405, -555), FRotator::ZeroRotator, 2.0f, 1.0f, 0.0f, nullptr);
	}
	//CountdownTime = 60;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AGameStarter::CountdownTick, 1.0f, true);

	if (StartCountdownWidget)
	{
		StartCountdownWidget->RemoveFromParent();
	}
}

void AGameStarter::StartCountdownTick()
{
	UpdateStartCountdownUI();

	if (GameStartSound1)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, GameStartSound1, FVector(-3885, -1405, -555), FRotator::ZeroRotator, 2.0f, 1.0f, 0.0f, nullptr);
	}

	if (--StartCountdownTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartCountdownTimerHandle);
	}
}

void AGameStarter::CountdownTick()
{
	UpdateCountdownUI();

	if (--CountdownTime < 0)
	{
		/*if (CountdownWidget)
		{
			CountdownWidget->RemoveFromParent();
		}*/
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void AGameStarter::UpdateStartCountdownUI()
{
	if (StartCountdownText)
	{
		StartCountdownText->SetText(FText::FromString(FString::Printf(TEXT("Time Left: %d"), StartCountdownTime)));
	}
}

void AGameStarter::UpdateCountdownUI()
{
	if (CountdownText)
	{
		CountdownText->SetText(FText::FromString(FString::Printf(TEXT("Time Left: %d"), CountdownTime)));
	}

}



















