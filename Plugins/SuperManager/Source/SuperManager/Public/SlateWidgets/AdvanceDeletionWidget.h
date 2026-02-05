// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAdvanceDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab){}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>> , AssetsDataArray)
		SLATE_ARGUMENT(FString , CurrentSelectedFolder)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
private:
	
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetList();
	TSharedRef<SCheckBox> ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo);
	TSharedRef<SButton> ConstructSyncToContentButton(TSharedPtr<FAssetData>& AssetData);
	TSharedRef<SButton> ConstructDeleteButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	TSharedRef<SButton> ConstructSelectAllButton();
	TSharedRef<SButton> ConstructDeselectAllButton();
	TSharedRef<SButton> ConstructDeleteAllButton();

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);
	FReply OnSyncToContentButtonClicked(TSharedPtr<FAssetData> ClickedAssetData);
	FReply OnDeleteClicked(TSharedPtr<FAssetData> ClickedAssetData);
	FReply OnDeleteAllClicked();
	FReply OnSelectAllClicked();
	FReply OnDeselectAllClicked();

#pragma region ComboBox
	
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructComboBox();
	void OnSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type SelectInfo);

	TSharedPtr<STextBlock> ComboBoxDisplayText;
	TArray<TSharedPtr<FString>> ComboBoxOptionsArray;

#pragma endregion

	void RefreshAssetList();
	FSlateFontInfo GetFontInfo(){return FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));}

	FString CurrentSelectedFolderPath;
	TArray<TSharedPtr<SCheckBox>> CheckboxesArray;
	TArray<TSharedPtr<FAssetData>> AvailableAssetDataArray;
	TArray<TSharedPtr<FAssetData>> DisplayedAssetDataArray;
	TArray<TSharedPtr<FAssetData>> AssetsToDeleteArray;
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	
};