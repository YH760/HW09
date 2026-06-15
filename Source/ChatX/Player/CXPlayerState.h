// CXPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerState.generated.h"

UCLASS()
class CHATX_API ACXPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACXPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_CurrentGuessCount();
	
	bool HasUsedAllGuesses() const { return CurrentGuessCount >= MaxGuessCount; }
	
	FString GetPlayerInfoString();
	
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGuessCount)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
