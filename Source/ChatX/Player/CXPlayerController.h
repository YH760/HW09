// CXPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CXPlayerController.generated.h"

class UCXChatInput;
class UUserWidget;

UCLASS()
class CHATX_API ACXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACXPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
		
	void PrintChatMessageString(const FString& InChatMessageString);
	
	void SetChatInputEnabled(bool bEnabled);
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCXChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCXChatInput> ChatInputWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TurnTimerWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> TurnTimerWidgetInstance;
	
	FString ChatMessageString;
};
