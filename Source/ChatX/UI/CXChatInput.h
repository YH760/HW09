// CXChatInput.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "CXChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class CHATX_API UCXChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void SetInputEnabled(bool bEnabled);
	
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
};
