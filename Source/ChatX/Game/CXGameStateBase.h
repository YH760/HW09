// CXGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

UCLASS()
class CHATX_API ACXGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
	EMatchState GetMatchState() const { return MatchState; }
	void SetMatchState(EMatchState InState) { MatchState = InState; }
	bool IsPlaying() const { return MatchState == EMatchState::Playing; }
	
	int32 GetRemainingTime() const { return RemainingTime; }
	void SetRemainingTime(int32 InTime) { RemainingTime = InTime; }
	
	int32 GetCurrentTurnPlayerId() const { return CurrentTurnPlayerId; }
	void SetCurrentTurnPlayerId(int32 InId) { CurrentTurnPlayerId = InId; }
	
private:
	UFUNCTION()
	void OnRep_MatchState();
	
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	EMatchState MatchState = EMatchState::Waiting;

	UPROPERTY(Replicated)
	int32 RemainingTime = 0;
	
	UPROPERTY(Replicated)
	int32 CurrentTurnPlayerId = -1;
};
