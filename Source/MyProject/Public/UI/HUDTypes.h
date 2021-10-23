#pragma once
#include "EnumRange.h"

/**
 * All the toggleable HUDs or HUDs that need some callback when added or hidden
 * in the game should be listed here.
 */
UENUM(BlueprintType)
enum class EHUDs : uint8 {
   /** Used to be used to display all the hero information simultaneously, in a sidebar but replaced by actionbar*/
   HS_Ingame,
   /** Inventory for all heroes in party*/
   HS_Inventory,
   /** Inventory for any storage interactable*/
   HS_Storage,
   /** Inventory for any NPC shops... or whatever has a backpack to sell stuff*/
   HS_Shop_General,
   /** Equipment menu*/
   HS_Equipment,
   /** Character Information Menu - Shows information about one character at a time (whoever is first in selection)*/
   HS_Character,
   /** Action bar for information about selected heroes, commands, AI options, channeling bar, etc.*/
   HS_Actionbar,
   /** Spellbook to level up and change spells in actionbar slot*/
   HS_Spellbook,
   /** Quest journal to see detailed information about what quests we're on, what quests we've finished, etc.*/
   HS_QuestJournal,
   /** Quest list is a widget on the side that can change what quest is being tracked on the minimap as well as conveys condensed information about quest*/
   HS_QuestList,
   /** Dialog prompt for dialog situations*/
   HS_Dialog,
   /** Minimap*/
   HS_Minimap,
   /** More interaction options for sociable NPCs*/
   HS_Social,
   /** Menu that leads to Save/Load/Options menu*/
   HS_Break,
   /** Allows graphical and gameplay settings changes*/
   HS_Settings,
   /** Save/Load Menu*/
   HS_SaveLoad,
   /** Image that shows an item's image in closer details*/
   HS_ExamineMenu,
   /** Shows a confirmation box*/
   HS_Confirmation,
   /** Shows a box that lets you input a number*/
   HS_InputBox,
   /** Chatbox to type in commands to be processed via NLP and converted to actions*/
   HS_ChatBox,
   /** Start menu for the beginning of the game*/
   HS_Start,
   /** Widget which has buttons we can use to map our keys*/
   HS_KeyMap,
   /** Number of widgets managed by the HUDManager*/
   HS_Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EHUDs, static_cast<uint8>(EHUDs::HS_Count));
