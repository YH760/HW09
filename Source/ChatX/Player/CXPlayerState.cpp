// CXPlayerState.cpp


#include "Player/CXPlayerState.h"

#include "CXPlayerController.h"
#include "Net/UnrealNetwork.h"

ACXPlayerState::ACXPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
    , MaxGuessCount(3)
{
	bReplicates = true;
}

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

void ACXPlayerState::OnRep_CurrentGuessCount()
{
	ACXPlayerController* PC = Cast<ACXPlayerController>(GetOwningController());
	if (IsValid(PC) == true && PC->IsLocalController() == true)
	{
		PC->SetChatInputEnabled(!HasUsedAllGuesses());
	}
}

FString ACXPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
