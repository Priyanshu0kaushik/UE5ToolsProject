// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAdvanceDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab){}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>> , AssetsDataArray)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
private:
	
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetList();
	TSharedRef<SCheckBox> ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo);
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay);

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);
	FReply OnDeleteClicked(TSharedPtr<FAssetData> ClickedAssetData);

	void RefreshAssetList();
	FSlateFontInfo GetFontInfo(){return FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));}
	
	TArray<TSharedPtr<FAssetData>> AvailableAssetDataArray;
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	
};