// CXGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGameModeBase.generated.h"

class ACXPlayerController;

enum class EGuessValidity : uint8
{
	None,
	Valid,      
	WrongLength,
	HasNonDigit,
	HasDuplicate,
	End
};

UCLASS()
class CHATX_API ACXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	FString GenerateSecretNumber();
	
	EGuessValidity CheckGuessValidity(const FString& InNumberString);
	
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	void PrintChatMessageString(ACXPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	void IncreaseGuessCount(ACXPlayerController* InChattingPlayerController);
	
	void ResetGame();

	void JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount);
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void StartTurnTimer();
	void OnTurnTimerTick();
	
	void StartTurn();
	void EndTurn();
protected:
	FString SecretNumberString;
	
	TArray<TObjectPtr<ACXPlayerController>> AllPlayerControllers;
	
	FTimerHandle ResetTimerHandle;
	FTimerHandle TurnTimerHandle;
	
	int32 TurnTimeLimit = 30;
	
	int32 CurrentTurnIndex = 0;
};
