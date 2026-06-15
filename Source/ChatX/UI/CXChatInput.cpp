// CXChatInput.cpp

#include "UI/CXChatInput.h"

#include "Components/EditableTextBox.h"
#include "Game/CXGameStateBase.h"
#include "Player/CXPlayerController.h"
#include "Player/CXPlayerState.h"

void UCXChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCXChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCXChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ACXPlayerController* OwningCXPlayerController = Cast<ACXPlayerController>(OwningPlayerController);
			if (IsValid(OwningCXPlayerController) == true)
			{
				OwningCXPlayerController->ServerRPCPrintChatMessageString(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}

void UCXChatInput::SetInputEnabled(bool bEnabled)
{
	if (IsValid(EditableTextBox_ChatInput) == true)
	{
		EditableTextBox_ChatInput->SetIsEnabled(bEnabled);
	}
}
