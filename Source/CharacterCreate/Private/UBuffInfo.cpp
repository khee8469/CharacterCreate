#include "UBuffInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

//CreateWidget()으로 생성될 떄 호출
void UUBuffInfo::NativeConstruct()
{
	Super::NativeConstruct();

	TotalDuration = 0.0f;
	RemainingTime = 0.0f;

	BuffNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuffNameText")));

	if (BuffNameText)
	{
		BuffNameText->SetText(FText::FromString(TEXT("")));
	}
	if (BuffDurationBar)
	{
		BuffDurationBar->SetPercent(1.0f);
	}
}

void UUBuffInfo::InitializeBuffInfo(const FString& InBuffName, float InDuration)
{
	TotalDuration = InDuration;
	RemainingTime = InDuration;

	if (BuffNameText)
	{
		BuffNameText->SetText(FText::FromString(InBuffName));
	}
	if (BuffDurationBar)
	{
		BuffDurationBar->SetPercent(1.0f);
	}
}

void UUBuffInfo::ExtendBuffDuration(float AdditionalTime)
{
	TotalDuration += AdditionalTime;
	RemainingTime = FMath::Max(RemainingTime + AdditionalTime, AdditionalTime);

	UpdateBuffUI();
}

const FString& UUBuffInfo::GetBuffName() const
{
	return BuffNameText->GetText().ToString();
}

void UUBuffInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RemainingTime > 0.0f)
	{
		RemainingTime -= InDeltaTime;
		UpdateBuffUI();

		if (RemainingTime <= 0.0f)
		{
			RemoveFromParent();
		}
	}
}

void UUBuffInfo::UpdateBuffUI()
{
	if (BuffDurationBar && TotalDuration > 0.0f)
	{
		BuffDurationBar->SetPercent(RemainingTime / TotalDuration);
	}
}
