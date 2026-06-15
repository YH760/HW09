// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CXTurnTimer.generated.h"

/**
 * 
 */
UCLASS()
class CHATX_API UCXTurnTimer : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_RemainingTime;

	UFUNCTION(BlueprintPure)
	FText GetRemainingTimeText() const;
	
	UFUNCTION(BlueprintPure)
	FText GetTurnText() const;
	
	UFUNCTION(BlueprintPure)
	FText GetGuessCountText() const;
};
