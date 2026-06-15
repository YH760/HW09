// CXPawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CXPawn.generated.h"

UCLASS()
class CHATX_API ACXPawn : public APawn
{
	GENERATED_BODY()

public:
	ACXPawn();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
