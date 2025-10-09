#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControlItem.generated.h"

UCLASS()
class CHARACTERCREATE_API AReverseControlItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AReverseControlItem();


	virtual void ActivateItem(AActor* Activator) override;
};
