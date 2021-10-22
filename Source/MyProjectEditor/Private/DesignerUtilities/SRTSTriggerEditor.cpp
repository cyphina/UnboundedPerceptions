#include "SRTSTriggerEditor.h"
#include "AssetEditorSubsystem.h"
#include "AssetToolsModule.h"
#include "Async.h"
#include "Editor.h"
#include "EditorLevelUtils.h"
#include "MultiBoxBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "RTSEditorWidgetBPLibrary.h"
#include "RTSTrigger.h"
#include "SButton.h"
#include "SExpandableArea.h"
#include "SCheckBox.h"
#include "SSearchBox.h"
#include "STableRow.h"
#include "SListView.h"
#include "SComboButton.h"
#include "SHyperlink.h"
#include "STextBlock.h"
#include "SThrobber.h"
#include "SubclassOf.h"
#include "UpFunctionLibrary.h"
#include "Engine/LevelStreaming.h"
#include "Factories/BlueprintFactory.h"
#include "Factories/Factory.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "TriggerManagerEditorUI"

/** An item in our listview of triggers */
struct FRTSTriggerItem : public TSharedFromThis<FRTSTriggerItem>
{
   FRTSTriggerItem(FName NewTriggerName, UObject* NewOuterObject, TSubclassOf<URTSTrigger> NewTriggerType,
                   ULevel* NewTriggerLevel) :
       triggerName(NewTriggerName),
       outerObject(NewOuterObject), triggerType(NewTriggerType), triggerLevel(NewTriggerLevel)
   {
   }

   FName                    triggerName;
   UObject*                 outerObject;
   TSubclassOf<URTSTrigger> triggerType;
   ULevel*                  triggerLevel;
};

typedef SListView<TSharedPtr<FRTSTriggerItem>> SRTSTriggerListView;

/** 
  * This is my first custom Slate widget which is used to list all the Trigger type objects created (in game or in editor).
  * Thus I marked up lot of details 
  */
class SRTSTriggerEditorImpl : public SRTSTriggerEditor
{
 public:
   SRTSTriggerEditorImpl();
   virtual void Construct(const FArguments& InArgs) override;

 protected:
   void         FilterTriggers();
   void         OnSearchTagCommited(const FText& InText, ETextCommit::Type InCommitType);
   void         OnOuterFilterChanged();
   void         OnNameFilterChanged();
   void         OnLevelFilterPicked();
   void         RefreshTriggerData();
   void         OpenEditorForTriggerOuter();
   void         OnLevelLinkClicked();
   virtual void HandleNotify_FindAssetInEditor(FString AssetName, int AssetType) override;
   virtual void HandleNotify_OpenAssetInEditor(FString AssetName, int AssetType) override;

 private:
   SVerticalBox::FSlot& CreateTriggerSectionWidget();

   // ---ListView events---

   /** Create a new Blueprint trigger in the asset browser */
   void CreateNewTrigger();

   /** Generates the data used to populate the ListView when manager is opened or when refresh manually requested */
   void UpdateTriggerListItems();

   TSharedRef<ITableRow> OnGenerateWidgetForListView(TSharedPtr<FRTSTriggerItem>       InItem,
                                                     const TSharedRef<STableViewBase>& OwnerTable);

   EVisibility GetListUpdateThrobberVisibility() const;
   EVisibility GetTriggerListVisibility() const;

   /** Generates a generic submenu when we open a combo button (one we specify unlike the ComboBox)*/
   TSharedRef<SWidget> OnLevelFilterCboOpened();

   FText GetLevelCboText() const;

   /** Generates the text for textblock in the combobox child slot representing the picked item. */
   FText GetSortOrderCboText() const;

   /** Used to generate an entry for the combobox dropdowns.  */
   TSharedRef<SWidget> HandleCboSortWidgetGen(TSharedRef<ETriggerSortOption> Item) const;

   /** Can check multiple fields to sort by them (sorts by multiple fiels in order checked) */
   void OnChkSortFieldChange(ECheckBoxState NewCheckState, FName FilterName);

   /** List of all level streamable from the currently played map in editor */
   const TArray<ULevelStreaming*>* GetStreamedLevels();

   /** Stores value of picked class when choosing a class to filter triggers by. */
   void OnClassFilterPicked(const UClass* PickedClass);

   /** Lets class picker know what class the user has selected currently*/
   const UClass* GetTriggerClassFilterValue() const;

   /** Class picker for creating a trigger from a certain type's callback when picking a new class */
   void OnTriggerCreateClassPicked(const UClass* PickedClass);

   /** Class picker for creating a trigger from a certain type's currently selected class type */
   const UClass* GetTriggerClassCreateValue() const;

   TArray<FName> ChkSortFieldNames;

   using SortOrderArray = TArray<TSharedRef<ETriggerSortOption>>;
   SortOrderArray CboxTriggerSortOrders;

   /** Listview shows all the triggers in game */
   TSharedPtr<SRTSTriggerListView> ListView_Trigger;

   /** Find Triggers by Name/Outer object (object trigger lives in) */
   TSharedPtr<SSearchBox> Searchbox_Name;
   TSharedPtr<SSearchBox> Searchbox_Outer;

   /** Actual data used to populate the ListView */
   TArray<TSharedPtr<FRTSTriggerItem>> TriggerData;

   /** Trigger filters/view parameters*/
   FText              NameFilterValue;
   FText              OuterFilterValue;
   const UClass*      ClassFilterValue        = nullptr;
   const UClass*      ClassTriggerCreateValue = nullptr;
   FString            LevelNameFilterValue    = "";
   ETriggerSortOption SelectedSortOrder       = ETriggerSortOption::Ascending;

   bool         IsListUpdating = false;
   FTimerHandle ThrobberDelayTimerHandle;
};

SVerticalBox::FSlot& SRTSTriggerEditorImpl::CreateTriggerSectionWidget()
{
   return SVerticalBox::Slot()
       [SNew(SExpandableArea)
            .HeaderContent()[SNew(STextBlock)
                                 .Text(FText::FromString("Create New Trigger"))
                                 .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))]
            .BodyContent()
                [SNew(SHorizontalBox) +
                 SHorizontalBox::Slot()[SNew(SClassPropertyEntryBox)
                                            .MetaClass(URTSTriggerBase::StaticClass())
                                            .SelectedClass(this, &SRTSTriggerEditorImpl::GetTriggerClassCreateValue)
                                            .AllowAbstract(true)
                                            .OnSetClass_Raw(this, &SRTSTriggerEditorImpl::OnTriggerCreateClassPicked)]
                     .AutoWidth()
                     .Padding(0, 0, 10, 0) +
                 SHorizontalBox::Slot()[SNew(SButton)
                                            .OnPressed(this, &SRTSTriggerEditorImpl::CreateNewTrigger)
                                            .Content()[SNew(STextBlock).Text(FText::FromString("Create Trigger"))]
                                            .ToolTipText(FText::FromString(
                                                "Creates a new trigger in /Game/RTS_Tutorial/Blueprints/Triggers"))]
                     .AutoWidth()]
            .Padding(FMargin(10, 5))]
           .AutoHeight();
}

SRTSTriggerEditorImpl::SRTSTriggerEditorImpl()
{
   CboxTriggerSortOrders.Emplace(MakeShared<ETriggerSortOption>(ETriggerSortOption::Ascending));
   CboxTriggerSortOrders.Emplace(MakeShared<ETriggerSortOption>(ETriggerSortOption::Descending));
}

EVisibility SRTSTriggerEditorImpl::GetListUpdateThrobberVisibility() const
{
   if(IsListUpdating)
   {
      return EVisibility::Visible;
   }
   else
   {
      return EVisibility::Collapsed;
   }
}

EVisibility SRTSTriggerEditorImpl::GetTriggerListVisibility() const
{
   if(!IsListUpdating)
   {
      return EVisibility::Visible;
   }
   else
   {
      return EVisibility::Hidden;
   }
}

void SRTSTriggerEditorImpl::CreateNewTrigger()
{
   const FString AssetName = TEXT("Rename-Temp-Trigger");
   const FString AssetPath = TEXT("/Game/RTS_Tutorial/Blueprints/Triggers/");

   UBlueprintFactory* NewFactory = NewObject<UBlueprintFactory>();
   NewFactory->ParentClass       = const_cast<UClass*>(ClassTriggerCreateValue);

   FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
   UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, AssetPath, NewFactory->SupportedClass, NewFactory);
   TArray<UObject*> ObjectsToSync;
   ObjectsToSync.Add(NewAsset);
   GEditor->SyncBrowserToObjects(ObjectsToSync);
}

void SRTSTriggerEditorImpl::OnTriggerCreateClassPicked(const UClass* PickedClass)
{
   ClassTriggerCreateValue = PickedClass;
}

const UClass* SRTSTriggerEditorImpl::GetTriggerClassCreateValue() const
{
   return ClassTriggerCreateValue;
}

FText SRTSTriggerEditorImpl::GetSortOrderCboText() const
{
   return UEnum::GetDisplayValueAsText(SelectedSortOrder);
}

void SRTSTriggerEditorImpl::OnChkSortFieldChange(ECheckBoxState NewCheckState, FName FilterName)
{
   if(NewCheckState == ECheckBoxState::Checked)
   {
      ChkSortFieldNames.Add(FilterName);
   }
   else
   {
      ChkSortFieldNames.Remove(FilterName);
   }
}

TSharedRef<SWidget> SRTSTriggerEditorImpl::HandleCboSortWidgetGen(TSharedRef<ETriggerSortOption> Item) const
{
   TSharedPtr<STextBlock> NewItem = SNew(STextBlock).Text(UEnum::GetDisplayValueAsText(*Item));
   NewItem->SetMargin(FMargin(2));
   return NewItem.ToSharedRef();
}

const UClass* SRTSTriggerEditorImpl::GetTriggerClassFilterValue() const
{
   return ClassFilterValue;
}

void SRTSTriggerEditorImpl::UpdateTriggerListItems()
{
   if(!IsListUpdating)
   {
      IsListUpdating = true;

      TriggerData.Reset();
      const auto Triggers = URTSEditorWidgetBPLibrary::GetTriggerObjects();

      for(const auto Trigger : Triggers)
      {
         if(UObject* TriggerOwner = Trigger->GetOuter()->GetOuter())
         {
            AActor* TriggerOwnerActor = Cast<AActor>(TriggerOwner);

            FString SearchNameFilter  = Searchbox_Name->GetText().ToString();
            FString SearchOuterFilter = Searchbox_Outer->GetText().ToString();

            if(SearchNameFilter.IsEmpty() || Trigger->GetTriggerName().ToString().StartsWith(SearchNameFilter))
            {
               if(SearchOuterFilter.IsEmpty() ||
                  (TriggerOwnerActor && TriggerOwnerActor->GetName().StartsWith(SearchOuterFilter)))
               {
                  if(ClassFilterValue == nullptr || Trigger->GetClass() == ClassFilterValue)
                  {
                     if(LevelNameFilterValue.IsEmpty() ||
                        (TriggerOwnerActor->GetLevel() &&
                         UUpFunctionLibrary::GetStreamingLevelNameFromActor(TriggerOwnerActor).ToString() ==
                             LevelNameFilterValue))
                     {
                        ULevel* TriggerLevel = TriggerOwnerActor ? TriggerOwnerActor->GetLevel() : nullptr;
                        TriggerData.Emplace(MakeShared<FRTSTriggerItem>(Trigger->GetTriggerName(), TriggerOwner,
                                                                        Trigger->GetClass(), TriggerLevel));
                     }
                  }
               }
            }
         }
      }

      static auto TriggerNameProj = [](const TSharedPtr<FRTSTriggerItem>& TriggerItem)
      {
         return TriggerItem->triggerName.ToString();
      };

      static auto TriggerOuterProj = [](const TSharedPtr<FRTSTriggerItem>& TriggerItem)
      {
         return TriggerItem->outerObject->GetName();
      };

      static auto TriggerTypeProj = [](const TSharedPtr<FRTSTriggerItem>& TriggerItem)
      {
         return TriggerItem->triggerType->GetClass()->GetName();
      };

      static auto TriggerLevelProj = [](const TSharedPtr<FRTSTriggerItem>& TriggerItem)
      {
         if(TriggerItem->triggerLevel)
         {
            return TriggerItem->triggerLevel->GetName();
         }
         else
         {
            return FString("");
         }
      };

      Algo::Sort(TriggerData,
                 [this](const TSharedPtr<FRTSTriggerItem>& FirstTriggerItem,
                        const TSharedPtr<FRTSTriggerItem>& SecondTriggerItem)
                 {
                    TFunction<FString(const TSharedPtr<FRTSTriggerItem>&)> ProjFunc;
                    for(FName SortFieldName : ChkSortFieldNames)
                    {
                       if(SortFieldName == "Name")
                       {
                          ProjFunc = TriggerNameProj;
                       }
                       else if(SortFieldName == "Outer")
                       {
                          ProjFunc = TriggerOuterProj;
                       }
                       else if(SortFieldName == "TriggerType")
                       {
                          ProjFunc = TriggerTypeProj;
                       }
                       else if(SortFieldName == "Level")
                       {
                          ProjFunc = TriggerLevelProj;
                       }

                       FString FirstTriggerItemValue  = ProjFunc(FirstTriggerItem);
                       FString SecondTriggerItemValue = ProjFunc(SecondTriggerItem);

                       if(FirstTriggerItemValue != SecondTriggerItemValue)
                       {
                          if(SelectedSortOrder == ETriggerSortOption::Ascending)
                          {
                             return FirstTriggerItemValue < SecondTriggerItemValue;
                          }
                          else
                          {
                             return FirstTriggerItemValue > SecondTriggerItemValue;
                          }
                       }
                    }
                    return true;
                 });

      ListView_Trigger->RequestListRefresh();

      if(!GEditor->GetPIEWorldContext())
      {
         if(UWorld* World = GEditor->GetEditorWorldContext().World())
         {
            World->GetTimerManager().SetTimer(
                ThrobberDelayTimerHandle,
                [this]()
                {
                   IsListUpdating = false;
                },
                1.f, false);
         }
         FEditorDelegates::PreBeginPIE.AddLambda(
             [this](bool bIsSimulating)
             {
                ThrobberDelayTimerHandle.Invalidate();
                IsListUpdating = false;
             });
      }
      else
      {
         IsListUpdating = false;
      }
   }
}

FText SRTSTriggerEditorImpl::GetLevelCboText() const
{
   return FText::FromString(LevelNameFilterValue);
}

TSharedRef<SWidget> SRTSTriggerEditorImpl::OnLevelFilterCboOpened()
{
   FMenuBuilder MenuBuilder(true, nullptr);

   const FUIAction DefaultLevelFilterAction(FExecuteAction::CreateLambda(
       [this]()
       {
          LevelNameFilterValue = "";
       }));
   MenuBuilder.AddMenuEntry(FText::FromString("No Filter"), FText::GetEmpty(), FSlateIcon(), DefaultLevelFilterAction);

   if(const auto StreamedLevels = GetStreamedLevels())
   {
      for(ULevelStreaming* StreamedLevel : *StreamedLevels)
      {
         int LastFolderSepIndex;
         StreamedLevel->GetWorldAssetPackageName().FindLastChar('/', LastFolderSepIndex);

         const FString LevelName = StreamedLevel->GetWorldAssetPackageName().RightChop(LastFolderSepIndex + 1);

         FUIAction LevelFilterAction(FExecuteAction::CreateLambda(
             [this, LevelName]()
             {
                LevelNameFilterValue = LevelName;
             }));

         MenuBuilder.AddMenuEntry(FText::FromString(LevelName), FText::GetEmpty(), FSlateIcon(), LevelFilterAction);
      }
      return MenuBuilder.MakeWidget();
   }
   return SNullWidget::NullWidget;
}

const TArray<ULevelStreaming*>* SRTSTriggerEditorImpl::GetStreamedLevels()
{
   const FWorldContext& WorldContext = GEditor->GetEditorWorldContext();
   if(UWorld* World = WorldContext.World())
   {
      return &World->GetStreamingLevels();
   }

   return nullptr;
}

void SRTSTriggerEditorImpl::Construct(const FArguments& InArgs)
{
   ChildSlot
       [SNew(SVerticalBox) + CreateTriggerSectionWidget() +
        SVerticalBox::Slot()
            [SNew(SExpandableArea)
                 .HeaderContent()[SNew(STextBlock)
                                      .Text(FText::FromString("Filters"))
                                      .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))]
                 .BodyContent()
                     [SNew(SVerticalBox) +
                      SVerticalBox::Slot()[SNew(SHorizontalBox) +
                                           SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Name"))]
                                               .AutoWidth()
                                               .VAlign(VAlign_Center) +
                                           SHorizontalBox::Slot()[SAssignNew(Searchbox_Name, SSearchBox)
                                                                      .HintText(FText::FromString("Name"))]
                                               .MaxWidth(250)
                                               .VAlign(VAlign_Center)
                                               .Padding(FMargin(10, 0)) +
                                           SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Outer"))]
                                               .AutoWidth()
                                               .VAlign(VAlign_Center)
                                               .Padding(FMargin(0, 0, 10, 0)) +
                                           SHorizontalBox::Slot()[SAssignNew(Searchbox_Outer, SSearchBox)
                                                                      .HintText(FText::FromString("Outer"))]
                                               .MaxWidth(250)] +
                      SVerticalBox::Slot()
                          [SNew(SHorizontalBox) +
                           SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Trigger Type Filter"))]
                               .AutoWidth()
                               .VAlign(VAlign_Center) +
                           SHorizontalBox::Slot()
                               [SNew(SClassPropertyEntryBox)
                                    .MetaClass(URTSTriggerBase::StaticClass())
                                    .SelectedClass(this, &SRTSTriggerEditorImpl::GetTriggerClassFilterValue)
                                    .AllowAbstract(false)
                                    .OnSetClass_Raw(this, &SRTSTriggerEditorImpl::OnClassFilterPicked)]
                                   .AutoWidth()
                                   .Padding(10, 0) +
                           SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Level"))]
                               .AutoWidth()
                               .VAlign(VAlign_Center)
                               .Padding(0, 0, 10, 0) +
                           SHorizontalBox::Slot()
                               [SNew(SComboButton)
                                    .OnGetMenuContent(this, &SRTSTriggerEditorImpl::OnLevelFilterCboOpened)
                                    .ButtonContent()[SNew(STextBlock)
                                                         .Text(this, &SRTSTriggerEditorImpl::GetLevelCboText)]
                                    .ContentPadding(FMargin(2.0, 2.0))
                                    .VAlign(VAlign_Center)]
                                   .MaxWidth(300)]]
                 .Padding(FMargin(10, 5))]
                .AutoHeight() +
        SVerticalBox::Slot()
            [SNew(SExpandableArea)
                 .HeaderContent()[SNew(STextBlock)
                                      .Text(FText::FromString("Sort Options"))
                                      .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))]
                 .BodyContent()
                     [SNew(SHorizontalBox) +
                      SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Name"))]
                          .VAlign(VAlign_Center)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(SCheckBox).OnCheckStateChanged_Raw(
                                                 this, &SRTSTriggerEditorImpl::OnChkSortFieldChange, FName("Name"))]
                          .Padding(2, 0, 10, 0)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Outer"))]
                          .VAlign(VAlign_Center)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(SCheckBox).OnCheckStateChanged_Raw(
                                                 this, &SRTSTriggerEditorImpl::OnChkSortFieldChange, FName("Outer"))]
                          .Padding(2, 0, 10, 0)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Trigger Type"))]
                          .VAlign(VAlign_Center)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(SCheckBox).OnCheckStateChanged_Raw(
                                                 this, &SRTSTriggerEditorImpl::OnChkSortFieldChange,
                                                 FName("TriggerType"))]
                          .Padding(2, 0, 10, 0)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Level"))]
                          .VAlign(VAlign_Center)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(SCheckBox).OnCheckStateChanged_Raw(
                                                 this, &SRTSTriggerEditorImpl::OnChkSortFieldChange, FName("Level"))]
                          .Padding(2, 0, 10, 0)
                          .AutoWidth() +
                      SHorizontalBox::Slot()[SNew(STextBlock).Text(FText::FromString("Sort Order"))]
                          .VAlign(VAlign_Center)
                          .AutoWidth() +
                      SHorizontalBox::Slot()
                          [SNew(SComboBox<TSharedRef<ETriggerSortOption>>)
                               .OptionsSource(&CboxTriggerSortOrders)
                               .OnGenerateWidget_Raw(this, &SRTSTriggerEditorImpl::HandleCboSortWidgetGen)
                               .OnSelectionChanged_Lambda(
                                   [this](TSharedPtr<ETriggerSortOption> NewItem, ESelectInfo::Type SelectInfo)
                                   {
                                      SelectedSortOrder = *NewItem;
                                   })
                               .InitiallySelectedItem(CboxTriggerSortOrders[0])
                                   [SNew(STextBlock).Text(this, &SRTSTriggerEditorImpl::GetSortOrderCboText)]]
                              .Padding(2, 0)
                              .AutoWidth()]
                 .Padding(FMargin(10, 5))]
                .AutoHeight() +
        SVerticalBox::Slot()
            [SNew(SExpandableArea)
                 .HeaderContent()[SNew(STextBlock)
                                      .Text(FText::FromString("Triggers"))
                                      .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))]
                 .BodyContent()
                     [SNew(SVerticalBox) +
                      SVerticalBox::Slot()[SNew(SButton)
                                               .OnPressed(this, &SRTSTriggerEditorImpl::UpdateTriggerListItems)
                                               .Content()[SNew(STextBlock)
                                                              .Text(FText::FromString(
                                                                  "Refresh Trigger Data (and Apply New Filters)"))]]
                          .AutoHeight()
                          .HAlign(EHorizontalAlignment::HAlign_Center)
                          .Padding(FMargin(0, 5, 0, 10)) +
                      SVerticalBox::Slot()
                          [SNew(SOverlay) +
                           SOverlay::Slot()
                               [SAssignNew(ListView_Trigger, SRTSTriggerListView)
                                    .ItemHeight(24)
                                    .ListItemsSource(&TriggerData)
                                    .OnGenerateRow(this, &SRTSTriggerEditorImpl::OnGenerateWidgetForListView)
                                    .HeaderRow(
                                        SNew(SHeaderRow) +
                                        SHeaderRow::Column("Trigger Name")
                                            .DefaultLabel(LOCTEXT("TriggerNameColumn", "Trigger Name"))
                                            .FillWidth(.25f) +
                                        SHeaderRow::Column("Outer Object")
                                            .DefaultLabel(LOCTEXT("OuterObjectColumn", "Outer Object"))
                                            .FillWidth(.25f)
                                            .DefaultTooltip(FText::FromString(
                                                "Click link to select object if in level or open blueprint if CDO")) +
                                        SHeaderRow::Column("Trigger Type")
                                            .DefaultLabel(LOCTEXT("TriggerTypeColumn", "Trigger Type"))
                                            .FillWidth(.25f) +
                                        SHeaderRow::Column("Level")
                                            .DefaultLabel(LOCTEXT("LevelColumn", "Level"))
                                            .FillWidth(.25f)
                                            .DefaultTooltip(
                                                FText::FromString("Click link to select level and make it visible.")))
                                    .Visibility(this, &SRTSTriggerEditorImpl::GetTriggerListVisibility)] +
                           SOverlay::Slot()[SNew(SCircularThrobber)
                                                .Visibility(this,
                                                            &SRTSTriggerEditorImpl::GetListUpdateThrobberVisibility)
                                                .Period(1.f)
                                                .Radius(20.f)]
                               .Padding(10)
                               .HAlign(HAlign_Center)
                               .VAlign(VAlign_Center)]]]
                .AutoHeight()];

   UpdateTriggerListItems();
}

TSharedRef<ITableRow> SRTSTriggerEditorImpl::OnGenerateWidgetForListView(TSharedPtr<FRTSTriggerItem>       InItem,
                                                                         const TSharedRef<STableViewBase>& OwnerTable)
{
   class SRTSTriggerItemWidget : public SMultiColumnTableRow<TSharedPtr<FRTSTriggerItem>>
   {
    public:
      SLATE_BEGIN_ARGS(SRTSTriggerItemWidget) {}

      SLATE_END_ARGS()

      void Construct(const FArguments& InArgs, const TSharedRef<SRTSTriggerListView>& InOwnerTable,
                     TSharedPtr<FRTSTriggerItem> InListItem, SRTSTriggerEditorImpl* InTriggerEditor)
      {
         Item          = InListItem;
         TriggerEditor = InTriggerEditor;
         SMultiColumnTableRow<TSharedPtr<FRTSTriggerItem>>::Construct(FSuperRowType::FArguments(), InOwnerTable);
      }

    private:
      virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
      {
         if(ColumnName == "Trigger Name")
         {
            return SNew(STextBlock).Text(FText::FromString(Item->triggerName.ToString()));
         }
         else if(ColumnName == "Outer Object")
         {
            return SNew(SBox).HAlign(HAlign_Left)[SNew(SHyperlink)
                                                      .Style(FEditorStyle::Get(), "Common.GotoBlueprintHyperlink")
                                                      .Text(FText::FromString(Item->outerObject->GetName()))
                                                      .OnNavigate(this, &SRTSTriggerItemWidget::NavigateToOuter)];
         }
         else if(ColumnName == "Trigger Type")
         {
            return SNew(SBox).HAlign(HAlign_Left)[SNew(STextBlock)

                                                      .Text(FText::FromString(Item->triggerType->GetName()))];
         }
         else if(ColumnName == "Level")
         {
            FName LevelName = "Outer not in Level";
            if(AActor* Actor = Cast<AActor>(Item->outerObject))
            {
               LevelName = UUpFunctionLibrary::GetStreamingLevelNameFromActor(Actor);
            }
            return SNew(SBox).HAlign(
                HAlign_Left)[SNew(SHyperlink)
                                 .Style(FEditorStyle::Get(), "Common.GotoBlueprintHyperlink")
                                 .Text(FText::FromString(LevelName.ToString()))
                                 .OnNavigate(this, &SRTSTriggerItemWidget::NavigateToOuterLevel, Item->triggerLevel)];
         }
         else
         {
            return SNew(STextBlock).Text(LOCTEXT("UnknownColumn", "Unknown Column"));
         }
      }

      void NavigateToOuter()
      {
         if(IsValid(Item->outerObject))
         {
            UPackage* pkg = Cast<UPackage>(Item->outerObject->GetOuter());
            if(pkg)
            {
               const FString AssetName   = FPaths::GetBaseFilename(Item->outerObject->GetFullName());
               UObject*      AssetObject = FindObject<UObject>(pkg, *AssetName);
               GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(AssetObject);
            }
            else
            {
               if(AActor* OuterActor = Cast<AActor>(Item->outerObject))
               {
                  GEditor->SelectNone(false, true);
                  GEditor->SelectActor(OuterActor, true, true, true, true);
               }
            }
         }
      }

      void NavigateToOuterLevel(ULevel* Level)
      {
         if(UWorld* World = GEditor->GetEditorWorldContext().World())
         {
            if(!World->IsPlayInEditor())
            {
               if(Level)
               {
                  UEditorLevelUtils::MakeLevelCurrent(Level);
                  UEditorLevelUtils::SetLevelVisibility(Level, true, true);
               }
            }
         }
      }

      TSharedPtr<FRTSTriggerItem> Item;
      SRTSTriggerEditorImpl*      TriggerEditor = nullptr;
   };

   if(InItem.IsValid())
   {
      return SNew(SRTSTriggerItemWidget, ListView_Trigger.ToSharedRef(), InItem, this);
   }
   else
   {
      return SNew(STableRow<TSharedPtr<FRTSTriggerItem>>,
                  OwnerTable)[SNew(STextBlock).Text(LOCTEXT("UnknownTrigger", "Unknown Trigger"))];
   }
}

void SRTSTriggerEditorImpl::FilterTriggers()
{
}

void SRTSTriggerEditorImpl::OnSearchTagCommited(const FText& InText, ETextCommit::Type InCommitType)
{
}

void SRTSTriggerEditorImpl::OnOuterFilterChanged()
{
}

void SRTSTriggerEditorImpl::OnNameFilterChanged()
{
}

void SRTSTriggerEditorImpl::OnLevelFilterPicked()
{
}

void SRTSTriggerEditorImpl::RefreshTriggerData()
{
}

void SRTSTriggerEditorImpl::OnClassFilterPicked(const UClass* PickedClass)
{
   ClassFilterValue = PickedClass;
}

void SRTSTriggerEditorImpl::OpenEditorForTriggerOuter()
{
}

void SRTSTriggerEditorImpl::OnLevelLinkClicked()
{
}

void SRTSTriggerEditorImpl::HandleNotify_FindAssetInEditor(FString AssetName, int AssetType)
{
}

void SRTSTriggerEditorImpl::HandleNotify_OpenAssetInEditor(FString AssetName, int AssetType)
{
}

TSharedRef<SRTSTriggerEditor> SRTSTriggerEditor::New()
{
   return MakeShareable(new SRTSTriggerEditorImpl());
}

#undef LOCTEXT_NAMESPACE
