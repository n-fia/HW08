#include "LampItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ALampItem::ALampItem() :InterectWidgetClass(nullptr) , InterectWidgetInstance(nullptr), HUDWidget(nullptr){}

void ALampItem::BeginPlay()
{
	//if (InterectWidgetClass)
	//{
	//	//test
	//	UE_LOG(LogTemp, Warning, TEXT("InterectWidgetClass is Completed"));
	//	InterectWidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InterectWidgetClass);
	//	if (InterectWidgetInstance)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("InterectWidgetInstance is Completed"));
	//		HUDWidget = InterectWidgetInstance;
	//		if (HUDWidget)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("HUDWidget is Completed"));
	//			if (UTextBlock* InterText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("InterectText"))))
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("UTextBlock InterText is Completed"));
	//				InterText->SetVisibility(ESlateVisibility::Hidden);
	//			}
	//		}
	//	}
	//}
}

void ALampItem::ActivateItem(AActor * Activator)
{
	ShowLampInterectHUD();
}

void ALampItem::ShowLampInterectHUD()
{
	//if (HUDWidget)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("HUDWidget is Completed"));
	//	if (UTextBlock* InterText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("InterectText"))))
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("UTextBlock InterText is Completed"));
	//		InterText->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}
}