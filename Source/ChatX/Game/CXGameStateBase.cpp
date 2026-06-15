// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CXGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CXPlayerController.h"
#include "Net/UnrealNetwork.h"

void ACXGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, CurrentTurnPlayerId );
}

void ACXGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACXPlayerController* CXPC = Cast<ACXPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void ACXGameStateBase::OnRep_MatchState()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ACXPlayerController* CXPC = Cast<ACXPlayerController>(PC);
	if (IsValid(CXPC) == true)
	{
		CXPC->SetChatInputEnabled(IsPlaying());
	}
}
