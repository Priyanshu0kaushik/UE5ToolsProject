#pragma once
#include "Engine/Engine.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHelper
{
	static void PrintOnScreen(const FString& Message, const FColor& Color = FColor::Red){
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
		
		}
	}

	static void PrintLog(const FString& Message){
		UE_LOG(LogTemp, Display, TEXT("%s"), *Message);	
	}

	static EAppReturnType::Type ShowDialogMessage(EAppMsgType::Type MsgType, const FString& Message, bool bIsMsgWarning)
	{
		FText Title = bIsMsgWarning ? FText::FromString("Warning") : FText::FromString("Confirmation");
		return FMessageDialog::Open(MsgType, FText::FromString(Message), Title);
	}

	static void ShowNotifyInfo(const FString& Msg){
		FNotificationInfo NotificationInfo(FText::FromString(Msg));
		NotificationInfo.bUseLargeFont = true;
		NotificationInfo.FadeOutDuration = 5.f;
	
		FSlateNotificationManager::Get().AddNotification(NotificationInfo);
	}

}
