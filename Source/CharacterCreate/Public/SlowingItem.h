#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class CHARACTERCREATE_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	//float SlowPer;

	virtual void ActivateItem(AActor* Activator) override;
};
