// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CXTurnTimer.h"
#include "Game/CXGameStateBase.h"
#include "Player/CXPlayerState.h" 

FText UCXTurnTimer::GetRemainingTimeText() const
{
	ACXGameStateBase* CXGS = GetWorld()->GetGameState<ACXGameStateBase>();
	if (IsValid(CXGS) == true)
	{
		int32 Time = CXGS->GetRemainingTime();
		return FText::FromString(FString::Printf(TEXT("남은 시간: %d"), Time));
	}
	return FText::GetEmpty();
}

FText UCXTurnTimer::GetTurnText() const
{
	ACXGameStateBase* CXGS = GetWorld()->GetGameState<ACXGameStateBase>();
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(CXGS) && IsValid(PC))
	{
		ACXPlayerState* CXPS = PC->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPS))
		{
			if (CXGS->GetCurrentTurnPlayerId() == CXPS->GetPlayerId())
			{
				return FText::FromString(TEXT("당신의 턴입니다"));
			}
			else
			{
				return FText::FromString(TEXT("상대 턴입니다"));
			}
		}
	}
	return FText::GetEmpty();
}

FText UCXTurnTimer::GetGuessCountText() const
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		ACXPlayerState* CXPS = PC->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPS))
		{
			return FText::FromString(FString::Printf(TEXT("시도: %d / %d"),
				CXPS->CurrentGuessCount, CXPS->MaxGuessCount));
		}
	}
	return FText::GetEmpty();
}
