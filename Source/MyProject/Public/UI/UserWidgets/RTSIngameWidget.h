#pragma once

#include "HUDTypes.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "RTSIngameWidget.generated.h"

class UGameSpeedWidget;
class ARTSGameState;
class URTSSidebarWidget;
class UCharacterMenu;
class UStorageInventory;
class UHeroInventory;
class UStoreInventory;
class UEquipmentMenu;
class UActionbarInterface;
class UQuestList;
class UQuestJournal;
class UMinimap;
class UDialogUI;
class UDialogBox;
class UItemExamineWidget;
class UBackpack;
class USpellbookHUD;
class UConfirmationBox;
class URTSInputBox;
class UUpChatBox;

/**
 * Holds every widget used in the main gamemode Up (the top down game where we control four heroes).
 * Removed whenever we play some kinda mini-game or we go back to the main menu
 */
UCLASS()
class MYPROJECT_API URTSIngameWidget : public UMyUserWidget
{
   GENERATED_BODY()

   URTSIngameWidget();

 public:
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   USpellbookHUD* GetSpellBookMenu() const { return Menu_Spellbook; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UItemExamineWidget* GetExamineMenu() const { return Modal_ItemExamine; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UCharacterMenu* GetCharacterHUD() const { return Menu_CharacterUpgrade; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UEquipmentMenu* GetEquipHUD() const { return Menu_Equips; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UHeroInventory* GetInventoryHUD() const { return Inventory_Hero; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UStoreInventory* GetShopHUD() const { return Inventory_Store; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UStorageInventory* GetStorageHUD() const { return Inventory_Storage; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UActionbarInterface* GetActionbar() const { return Widget_Actionbar; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UQuestList* GetQuestList() const { return Widget_QuestTracker; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UQuestJournal* GetQuestJournal() const { return Menu_QuestJournal; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UMinimap* GetMinimap() const { return Widget_Minimap; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UDialogBox* GetDialogBox() const { return Widget_Dialog; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UDialogUI* GetSocialWindow() const { return Menu_Social; }

   UFUNCTION(Blueprintcallable, BlueprintPure, Category = "HUDManager")
   URTSSidebarWidget* GetSidebarWidget() const { return Sidebar_HeroStatus; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UGameSpeedWidget* GetGamespeedWidget() const { return Widget_Gamespeed; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UUpChatBox* GetChatBox() const { return Chatbox; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   UConfirmationBox* GetConfirmModal() const { return Modal_Confirm; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUDManager")
   URTSInputBox* GetInputModal() const { return Modal_Input; }

   /**
    * @brief TODO: Rewrite BP version to rely on native version instead of vice versa
    * @param worldRef - UObject that calls this function passes in it's reference to the world
    * @param hText - Text to display onto the screen
    */
   static void NativeDisplayHelpText(UWorld* worldRef, const FText& hText);

   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Help")
   void DisplayHelpText(const FText& hText);

 protected:
   void NativeOnInitialized() override;

 private:
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
   class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   USpellbookHUD* Menu_Spellbook;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UItemExamineWidget* Modal_ItemExamine;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UCharacterMenu* Menu_CharacterUpgrade;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UEquipmentMenu* Menu_Equips;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UHeroInventory* Inventory_Hero;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UStoreInventory* Inventory_Store;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UStorageInventory* Inventory_Storage;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UActionbarInterface* Widget_Actionbar;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UQuestList* Widget_QuestTracker;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UQuestJournal* Menu_QuestJournal;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UMinimap* Widget_Minimap;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UDialogBox* Widget_Dialog;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UDialogUI* Menu_Social;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   URTSSidebarWidget* Sidebar_HeroStatus;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UGameSpeedWidget* Widget_Gamespeed;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UConfirmationBox* Modal_Confirm;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   URTSInputBox* Modal_Input;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
   UUpChatBox* Chatbox;
};
