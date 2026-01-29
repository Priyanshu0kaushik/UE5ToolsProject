// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHelper.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	AvailableAssetDataArray = InArgs._AssetsDataArray;
	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;

	ChildSlot
	[
		// Main widget
		SNew(SVerticalBox)

		// first slot for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Advance Deletion"))
			.Font(TitleTextFont)
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FColor::White)
		]
		
		// second slot for the condition options
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
			]
		]
		
		// third slot for the asset list
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(SListView<TSharedPtr<FAssetData>>)
				.ItemHeight(24.f)
				.ListItemsSource(&AvailableAssetDataArray)
				.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
			]
			
		]

		// fourth slot for the buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]
		
	];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if(!AssetDataToDisplay.IsValid()) return TSharedRef<ITableRow>();
	const FString AssetName = AssetDataToDisplay->AssetName.ToString();
	TSharedRef<ITableRow> AssetRowWidget =
		 SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			// 1st slot Checkbox
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(0.05f)
			[
				ConstructCheckbox(AssetDataToDisplay)
			]
			// 2nd slot Asset Class Name
			// 3rd slot Asset Name

			// 4th slot direct delete button
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(AssetName))
			]
			
		];
	return AssetRowWidget;
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> checkBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);
	return checkBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch(NewState)
	{
	case ECheckBoxState::Checked:
		break;

	case ECheckBoxState::Unchecked:
		break;

	case ECheckBoxState::Undetermined:
		break;
		
	default:
		break;
	}
	DebugHelper::PrintOnScreen ("Working.....");
}
