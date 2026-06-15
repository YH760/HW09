// CXGameModeBase.cpp


#include "Game/CXGameModeBase.h"

#include "CXGameStateBase.h"
#include "EngineUtils.h"
#include "Player/CXPlayerController.h"
#include "Player/CXPlayerState.h"

void ACXGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
}

FString ACXGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	
	return Result;
}

EGuessValidity ACXGameModeBase::CheckGuessValidity(const FString& InNumberString)
{
	if (InNumberString.Len() != 3)
	{
		return EGuessValidity::WrongLength;
	}

	TSet<TCHAR> UniqueDigits;
	for (TCHAR C : InNumberString)
	{
		if (FChar::IsDigit(C) == false || C == '0')
		{
			return EGuessValidity::HasNonDigit;
		}
		UniqueDigits.Add(C);
	}

	if (UniqueDigits.Num() != 3)
	{
		return EGuessValidity::HasDuplicate;
	}

	return EGuessValidity::Valid;
}

FString ACXGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACXGameModeBase::PrintChatMessageString(ACXPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	EGuessValidity Validity = CheckGuessValidity(InChatMessageString);

	if (Validity == EGuessValidity::Valid)
	{
		ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
		
		if (IsValid(CXGS) && CXGS->IsPlaying() == false)
		{
			return;
		}
		
		if (IsValid(CXGS) && CXGS->GetRemainingTime() <= 0)
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("시간이 초과되었습니다."));
			return;
		}
		
		ACXPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPS) && CXPS->HasUsedAllGuesses())
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("횟수를 모두 소모했습니다."));
			return;
		}
		
		if (IsValid(CXGS) && IsValid(CXPS) && CXGS->GetCurrentTurnPlayerId() != CXPS->GetPlayerId())
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("당신의 턴이 아닙니다."));
			return;
		}
		
		FString JudgeResultString = JudgeResult(SecretNumberString, InChatMessageString);
		
		IncreaseGuessCount(InChattingPlayerController);
		
		FString PlayerInfoString = IsValid(CXPS) ? CXPS->GetPlayerInfoString() : TEXT("");
		FString CombinedMessageString = PlayerInfoString + TEXT(": ") + InChatMessageString + TEXT(" -> ") + JudgeResultString;
		
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
		
		if (IsValid(CXGS) && CXGS->IsPlaying())
		{
			EndTurn();
		}
	}
	else
	{
		FString Msg;
		switch (Validity)
		{
		case EGuessValidity::WrongLength:
			{
				Msg = TEXT("3자리 숫자를 입력해주세요."); break;
			}
		case EGuessValidity::HasNonDigit:
			{
				Msg = TEXT("1~9 사이 숫자만 입력해주세요."); break;
			}
		case EGuessValidity::HasDuplicate:
			{
				Msg = TEXT("중복되지 않은 숫자를 입력해주세요."); break;
			}
		default:
			{
				Msg = TEXT("잘못된 입력입니다."); break;
			}
		}
		InChattingPlayerController->ClientRPCPrintChatMessageString(Msg);
	}
}

void ACXGameModeBase::IncreaseGuessCount(ACXPlayerController* InChattingPlayerController)
{
	ACXPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void ACXGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(NewPlayer);
	if (IsValid(CXPlayerController) == true)
	{
		CXPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(CXPlayerController);
		
		if (AllPlayerControllers.Num() == 1)
		{
			ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
			if (IsValid(CXGS))
			{
				CXGS->SetMatchState(EMatchState::Playing);
			}
			StartTurn();
		}
		
		ACXPlayerState* CXPS = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		
		ACXGameStateBase* CXGameStateBase = GetGameState<ACXGameStateBase>();
		if (IsValid(CXGameStateBase) == true)
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}
	}
}

void ACXGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	CurrentTurnIndex = 0;
	
	for (const auto& CXPlayerController : AllPlayerControllers)
	{
		CXPlayerController->NotificationText = FText::GetEmpty();
		ACXPlayerState* CXPS = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->CurrentGuessCount = 0;
		}
	}
	
	ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
	if (IsValid(CXGS) == true)
	{
		CXGS->SetMatchState(EMatchState::Playing);
		StartTurn();
	}
}

void ACXGameModeBase::JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
		if (IsValid(CXGS) == true)
		{
			CXGS->SetMatchState(EMatchState::Ending);
			GetWorldTimerManager().ClearTimer(TurnTimerHandle);
		}
		
		ACXPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXPS) == true)
			{
				FString CombinedMessageString = CXPS->PlayerNameString + TEXT(" has won the game.");
				CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}
		}
		
		GetWorldTimerManager().SetTimer(
			ResetTimerHandle,
			this,
			&ACXGameModeBase::ResetGame,
			3.0f,
			false
		);
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			ACXPlayerState* CXPS = CXPlayerController->GetPlayerState<ACXPlayerState>();
			if (IsValid(CXPS) == true)
			{
				if (CXPS->CurrentGuessCount < CXPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
			if (IsValid(CXGS) == true)
			{
				CXGS->SetMatchState(EMatchState::Ending);
				GetWorldTimerManager().ClearTimer(TurnTimerHandle);
			}
			
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			
			GetWorldTimerManager().SetTimer(
				ResetTimerHandle,
				this,
				&ACXGameModeBase::ResetGame,
				3.0f,
				false
			);
		}
	}
}

void ACXGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(ResetTimerHandle);
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ACXGameModeBase::StartTurnTimer()
{
	ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
	if (IsValid(CXGS) == true)
	{
		CXGS->SetRemainingTime(TurnTimeLimit);
	}

	GetWorldTimerManager().SetTimer(
		TurnTimerHandle,
		this,
		&ACXGameModeBase::OnTurnTimerTick,
		1.0f,
		true
	);
}

void ACXGameModeBase::OnTurnTimerTick()
{
	ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
	if (IsValid(CXGS) == false)
	{
		return;
	}

	int32 NewTime = CXGS->GetRemainingTime() - 1;
	CXGS->SetRemainingTime(NewTime);

	if (NewTime <= 0)
	{
		if (AllPlayerControllers.IsValidIndex(CurrentTurnIndex))
		{
			IncreaseGuessCount(AllPlayerControllers[CurrentTurnIndex]);
		}
		EndTurn();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("RemainingTime: %d"), NewTime);
}

void ACXGameModeBase::StartTurn()
{
	if (AllPlayerControllers.Num() == 0) return;

	CurrentTurnIndex = CurrentTurnIndex % AllPlayerControllers.Num();

	ACXPlayerController* TurnPC = AllPlayerControllers[CurrentTurnIndex];
	ACXPlayerState* TurnPS = TurnPC->GetPlayerState<ACXPlayerState>();

	ACXGameStateBase* CXGS = GetGameState<ACXGameStateBase>();
	if (IsValid(CXGS) && IsValid(TurnPS))
	{
		CXGS->SetCurrentTurnPlayerId(TurnPS->GetPlayerId());
	}

	StartTurnTimer();
}

void ACXGameModeBase::EndTurn()
{
	CurrentTurnIndex++;
	StartTurn();
}
