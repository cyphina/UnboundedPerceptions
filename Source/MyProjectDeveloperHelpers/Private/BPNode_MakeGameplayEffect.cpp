#include "BPNode_MakeGameplayEffect.h"

#include "TextProperty.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/SpellFunctionLibrary.h"
#include "GameplayEffect.h"

#include "KismetCompiler.h"
#include "EditorCategoryUtils.h"

#include "EdGraphSchema_K2_Actions.h"
#include "K2Node_CallFunction.h"

#include "GameplayEffects/RTSDamageEffect.h"
#include "GameplayEffects/CoolDownEffect.h"

#include "GameplayTagContainer.h"
#include "RTSHealingEffect.h"

#define LOCTEXT_NAMESPACE "CreateGameplayEffectNode"

struct FBPNode_CreateGameplayEffectHelper
{
   static FString durationPinName;
   static FString periodPinName;
   static FString levelPinName;
   static FString outputPinName;
   static FString classPinName;
   static FString elemPinName;
   static FString namePinName;
   static FString assetPinName;
};

FString FBPNode_CreateGameplayEffectHelper::durationPinName = TEXT("Duration");
FString FBPNode_CreateGameplayEffectHelper::periodPinName   = TEXT("Period");
FString FBPNode_CreateGameplayEffectHelper::levelPinName    = TEXT("Level");
FString FBPNode_CreateGameplayEffectHelper::outputPinName   = TEXT("ReturnValue");
FString FBPNode_CreateGameplayEffectHelper::classPinName    = TEXT("Class");
FString FBPNode_CreateGameplayEffectHelper::elemPinName     = TEXT("Element");
FString FBPNode_CreateGameplayEffectHelper::namePinName     = TEXT("EffectName");
FString FBPNode_CreateGameplayEffectHelper::assetPinName    = TEXT("AssetTags");

UBPNode_MakeGameplayEffect::UBPNode_MakeGameplayEffect(const FObjectInitializer& oI) : Super(oI)
{
   NodeTooltip = LOCTEXT("NodeTooltip", "Create a new gameplay effect");
}

/**Adds default pins to node.  These pins (inputs ?) are always displayed */
void UBPNode_MakeGameplayEffect::AllocateDefaultPins()
{
   const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

   CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, nullptr, UEdGraphSchema_K2::PN_Execute);
   CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, nullptr, UEdGraphSchema_K2::PN_Then);

   UEdGraphPin* ClassPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, GetClassPinBaseClass(), *FBPNode_CreateGameplayEffectHelper::classPinName);
   SetPinToolTip(*ClassPin, LOCTEXT("ClassPinDesc", "The effect class we're going to to create an instance of"));

   UEdGraphPin* durationPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, nullptr, *FBPNode_CreateGameplayEffectHelper::durationPinName);
   SetPinToolTip(*durationPin, LOCTEXT("DurationPinDesc", "Duration of the Effect"));
   durationPin->DefaultValue = "0";

   UEdGraphPin* periodPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, nullptr, *FBPNode_CreateGameplayEffectHelper::periodPinName);
   SetPinToolTip(*periodPin, LOCTEXT("PeriodPinDesc", "Period of the Effect"));
   periodPin->DefaultValue =
       "0"; //Period is usually not a relevant value in timed effects but only periodic effects have executions, but modifiers will not be reset for periodic effects

   UEdGraphPin* levelPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, nullptr, *FBPNode_CreateGameplayEffectHelper::levelPinName);
   SetPinToolTip(*levelPin, LOCTEXT("LevelPinDesc", "Level of the Effect"));
   levelPin->DefaultValue = "1";

   UEdGraphPin* elemPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTag::StaticStruct(), *FBPNode_CreateGameplayEffectHelper::elemPinName);
   SetPinToolTip(*elemPin, LOCTEXT("ElemPinDesc", "Element of the effect"));

   UEdGraphPin* namePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTag::StaticStruct(), *FBPNode_CreateGameplayEffectHelper::namePinName);
   SetPinToolTip(*namePin, LOCTEXT("NamePinDesc", "Name of the effect"));

   UEdGraphPin* assetTagsPin =
       CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTagContainer::StaticStruct(), *FBPNode_CreateGameplayEffectHelper::assetPinName);
   SetPinToolTip(*assetTagsPin, LOCTEXT("NamePinDesc", "Any additional tags this effect may have"));

   UEdGraphPin* resultPin =
       CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FGameplayEffectSpecHandle::StaticStruct(), *FBPNode_CreateGameplayEffectHelper::outputPinName);
   SetPinToolTip(*resultPin, LOCTEXT("ResultPinDesc", "Output is the EffectHandle to the new effect created"));
}

FLinearColor UBPNode_MakeGameplayEffect::GetNodeTitleColor() const
{
   return FLinearColor::Black;
}

FText UBPNode_MakeGameplayEffect::GetBaseNodeTitle() const
{
   return LOCTEXT("CreateGameplayEffectBaseTitle", "Create Gameplay Effect");
}

FText UBPNode_MakeGameplayEffect::GetNodeTitleFormat() const
{
   return LOCTEXT("CreateGameplayEffectData", "Create {ClassName} Effect");
}

/**Which class can use this node to create objects*/
UClass* UBPNode_MakeGameplayEffect::GetClassPinBaseClass() const
{
   return UGameplayEffect::StaticClass();
}

/**Category which node is under in menu*/
FText UBPNode_MakeGameplayEffect::GetMenuCategory() const
{
   return FText::FromString("Spell Helper");
}

void UBPNode_MakeGameplayEffect::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
   Super::GetMenuActions(ActionRegistrar);
}

FText UBPNode_MakeGameplayEffect::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
   return LOCTEXT("CreateGameplayEffecTitle", "Create Gameplay Effect");
}

bool UBPNode_MakeGameplayEffect::IsSpawnVarPin(UEdGraphPin* pin) const
{
   return Super::IsSpawnVarPin(pin) && pin->PinName != *FBPNode_CreateGameplayEffectHelper::periodPinName &&
          pin->PinName != *FBPNode_CreateGameplayEffectHelper::durationPinName && pin->PinName != *FBPNode_CreateGameplayEffectHelper::levelPinName &&
          pin->PinName != *FBPNode_CreateGameplayEffectHelper::elemPinName && pin->PinName != *FBPNode_CreateGameplayEffectHelper::namePinName &&
          pin->PinName != *FBPNode_CreateGameplayEffectHelper::assetPinName;
}

/**Expands node for our custom object, with properties set as EditAnywhere and meta=(ExposeOnSpawn)*/
void UBPNode_MakeGameplayEffect::ExpandNode(FKismetCompilerContext& compilerContext, UEdGraph* sourceGraph)
{
   Super::ExpandNode(compilerContext, sourceGraph);
   static FName createFunctionName;

   ///SETUP NAMES
   //Look for static function in BP Library depending on class type (factory pattern)
   if(GetClassToSpawn()->IsChildOf(URTSDamageEffect::StaticClass()) || GetClassToSpawn()->IsChildOf(URTSHealingEffect::StaticClass()))
   {
      createFunctionName = GET_FUNCTION_NAME_CHECKED(USpellFunctionLibrary, MakeDamageOrHealingEffect);
   }
   else if(GetClassToSpawn()->IsChildOf(UStatChangeEffect::StaticClass()))
   {
      createFunctionName = GET_FUNCTION_NAME_CHECKED(USpellFunctionLibrary, MakeStatChangeEffect);
   }
   else
   {
      createFunctionName = GET_FUNCTION_NAME_CHECKED(USpellFunctionLibrary, MakeGameplayEffect);
   }

   //inputs for that function
   static const FString abilityRefParamName  = FString(TEXT("AbilityRef"));
   static const FString effectClassParamName = FString(TEXT("EffectClass"));
   static const FString durationParamName    = FString(TEXT("Duration"));
   static const FString periodParamName      = FString(TEXT("Period"));
   static const FString levelParamName       = FString(TEXT("Level"));
   static const FString elemParamName        = FString(TEXT("Elem"));
   static const FString nameParamName        = FString(TEXT("EffectName"));
   static const FString assetTagsParamName   = FString(TEXT("AssetTags"));

   //Create pin we see when we get CreateGameplayEffect
   UBPNode_MakeGameplayEffect* createGameplayEffectNode = this;

   ///Prepare pins
   //get pointer to pins on the node we're creating
   UEdGraphPin* spawnNodeExec = createGameplayEffectNode->GetExecPin();
   UEdGraphPin* durationPin   = createGameplayEffectNode->GetDurationPin();
   UEdGraphPin* periodPin     = createGameplayEffectNode->GetPeriodPin();
   UEdGraphPin* levelPin      = createGameplayEffectNode->GetLevelPin();
   UEdGraphPin* elemPin       = createGameplayEffectNode->GetElemPin();
   UEdGraphPin* namePin       = createGameplayEffectNode->GetNamePin();
   UEdGraphPin* assetTagsPin  = createGameplayEffectNode->GetAssetTagsPin();
   UEdGraphPin* spawnClassPin = createGameplayEffectNode->GetClassPin();
   UEdGraphPin* spawnNodeThen = createGameplayEffectNode->GetThenPin();
   UEdGraphPin* spawnNodeRes  = createGameplayEffectNode->GetResultPin();

   UClass* spawnClass = (spawnClassPin != NULL) ? Cast<UClass>(spawnClassPin->DefaultObject) : NULL;
   if(0 == spawnClassPin->LinkedTo.Num() && (NULL == spawnClass))
   {
      compilerContext.MessageLog.Error(TEXT("Spawn node @@ must have a class specified."), createGameplayEffectNode);
      createGameplayEffectNode->BreakAllNodeLinks();
      return;
   }

   //Create a function calling node calling the function that our new node is going to be linked to
   UK2Node_CallFunction* callCreateNode = compilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(createGameplayEffectNode, sourceGraph);
   callCreateNode->FunctionReference.SetExternalMember(createFunctionName, USpellFunctionLibrary::StaticClass());
   callCreateNode->AllocateDefaultPins();

   //Prepare the function calling pin's nodes
   UEdGraphPin* callCreateExec           = callCreateNode->GetExecPin();
   UEdGraphPin* callCreateDurationPin    = callCreateNode->FindPinChecked(durationParamName);
   UEdGraphPin* callCreatePeriodPin      = callCreateNode->FindPinChecked(periodParamName);
   UEdGraphPin* callCreateLevelPin       = callCreateNode->FindPinChecked(levelParamName);
   UEdGraphPin* callCreateEffectClassPin = callCreateNode->FindPinChecked(effectClassParamName);
   UEdGraphPin* callCreateElemPin        = callCreateNode->FindPinChecked(elemParamName);
   UEdGraphPin* callCreateNamePin        = callCreateNode->FindPinChecked(nameParamName);
   UEdGraphPin* callCreateAssetTagsPin   = callCreateNode->FindPinChecked(assetTagsParamName);
   UEdGraphPin* callCreateRes            = callCreateNode->GetReturnValuePin();

   ///Creating links from our node we're creating (The one in blueprints) to a function call node we're creating now

   //Move exec connection from this node to the node in our SpellFunctionLibrary
   compilerContext.MovePinLinksToIntermediate(*spawnNodeExec, *callCreateExec);

   if(spawnClassPin->LinkedTo.Num() > 0)
   {
      //copy the blueprint connection from spawn node to the widget library connection
      compilerContext.MovePinLinksToIntermediate(*spawnClassPin, *callCreateEffectClassPin);
   }
   else
   {
      //Copy blueprint literal onto the BP library makeeffect call
      callCreateEffectClassPin->DefaultObject = spawnClass;
   }

   compilerContext.MovePinLinksToIntermediate(*durationPin, *callCreateDurationPin);
   compilerContext.MovePinLinksToIntermediate(*periodPin, *callCreatePeriodPin);
   compilerContext.MovePinLinksToIntermediate(*levelPin, *callCreateLevelPin);
   compilerContext.MovePinLinksToIntermediate(*elemPin, *callCreateElemPin);
   compilerContext.MovePinLinksToIntermediate(*namePin, *callCreateNamePin);
   compilerContext.MovePinLinksToIntermediate(*assetTagsPin, *callCreateAssetTagsPin);

   callCreateRes->PinType = spawnNodeRes->PinType;

   ///Create set variable nodes (the setter nodes in blueprints) for all the extra pins we have exposed
   ///Typically we do this by calling another function FKismetCompilerUtilities::GenerateAssignmentNodes but we get issues doing it so this is how we can do it manually
   ///with less code and do this correctly.

   //If we can find a corresponding input pin in our generated function (from our BP library) pins, then link our pin from this node to that pin
   UEdGraphPin *callPin, *objPin;
   for(int32 PinIdx = 0; PinIdx < createGameplayEffectNode->Pins.Num(); PinIdx++)
   {
      objPin  = createGameplayEffectNode->Pins[PinIdx]; //get pin from our special node
      callPin = callCreateNode->FindPin(createGameplayEffectNode->Pins[PinIdx]->PinName);
      if(callPin)
      {
         compilerContext.MovePinLinksToIntermediate(*objPin, *callPin); //set them to the pins in our function
      }
   }

   compilerContext.MovePinLinksToIntermediate(*spawnNodeThen, *callCreateNode->GetThenPin());
   compilerContext.MovePinLinksToIntermediate(*spawnNodeRes, *callCreateRes);
   createGameplayEffectNode->BreakAllNodeLinks();
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetDurationPin() const
{
   UEdGraphPin* pin = FindPin(FBPNode_CreateGameplayEffectHelper::durationPinName);
   check(pin == NULL || pin->Direction == EGPD_Input) return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetPeriodPin() const
{
   UEdGraphPin* pin = FindPin(FBPNode_CreateGameplayEffectHelper::periodPinName);
   check(pin == NULL || pin->Direction == EGPD_Input) return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetLevelPin() const
{
   UEdGraphPin* pin = FindPin(FBPNode_CreateGameplayEffectHelper::levelPinName);
   check(pin == NULL || pin->Direction == EGPD_Input) return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetResultPin() const
{
   UEdGraphPin* pin = FindPinChecked(FBPNode_CreateGameplayEffectHelper::outputPinName);
   check(pin->Direction == EGPD_Output);
   return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetElemPin() const
{
   UEdGraphPin* pin = FindPinChecked(FBPNode_CreateGameplayEffectHelper::elemPinName);
   check(pin == NULL || pin->Direction == EGPD_Input);
   return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetNamePin() const
{
   UEdGraphPin* pin = FindPinChecked(FBPNode_CreateGameplayEffectHelper::namePinName);
   check(pin == NULL || pin->Direction == EGPD_Input);
   return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetAssetTagsPin() const
{
   UEdGraphPin* pin = FindPinChecked(FBPNode_CreateGameplayEffectHelper::assetPinName);
   check(pin->Direction == EGPD_Input);
   return pin;
}

UEdGraphPin* UBPNode_MakeGameplayEffect::GetClassPin() const
{
   UEdGraphPin* pin = FindPinChecked(FBPNode_CreateGameplayEffectHelper::classPinName);
   check(pin->Direction == EGPD_Input);
   return pin;
}

void UBPNode_MakeGameplayEffect::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
   Super::GetPinHoverText(Pin, HoverTextOut);

   if(UEdGraphPin* ClassPin = GetClassPin())
   {
      SetPinToolTip(*ClassPin, LOCTEXT("ClassPinDescription", "The effect class that the spec to be spawned is based off of"));
   }
   if(UEdGraphPin* ResultPin = GetResultPin())
   {
      SetPinToolTip(*ResultPin, LOCTEXT("ResultPinDescription", "The constructed effect spec's handle"));
   }
   if(UEdGraphPin* DurationPin = GetDurationPin())
   {
      SetPinToolTip(*DurationPin, LOCTEXT("DurationPinDescription", "Duration of the effect."));
   }
   if(UEdGraphPin* PeriodPin = GetPeriodPin())
   {
      SetPinToolTip(
          *PeriodPin,
          LOCTEXT(
              "PeriodPinDescription",
              "Over the duration of the effect, will trigger the effect every time the period elapses.  0 means non periodic or instant effect, so set to higher than duration if this effect is not set to instant for duration policy but not periodic as well"));
   }
   if(UEdGraphPin* ElemPin = GetElemPin())
   {
      SetPinToolTip(*ElemPin, LOCTEXT("ElemPinDescription", "This is the element tag for the effect"));
   }
   if(UEdGraphPin* NamePin = GetNamePin())
   {
      SetPinToolTip(*NamePin, LOCTEXT("NamePinDescription", "This is the effect's name tag"));
   }
   if(UEdGraphPin* AssetTagsPin = GetAssetTagsPin())
   {
      SetPinToolTip(*AssetTagsPin, LOCTEXT("AssetTagsPinDescription", "Put any additional asset tags here the effect may need"));
   }
}

void UBPNode_MakeGameplayEffect::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
   Super::ReallocatePinsDuringReconstruction(OldPins);
   GetResultPin()->PinType.PinSubCategoryObject = FGameplayEffectSpecHandle::StaticStruct();
}

void UBPNode_MakeGameplayEffect::PostPlacedNewNode()
{
   Super::PostPlacedNewNode();
   GetResultPin()->PinType.PinSubCategoryObject = FGameplayEffectSpecHandle::StaticStruct();
}

void UBPNode_MakeGameplayEffect::PinDefaultValueChanged(UEdGraphPin* Pin)
{
   Super::PinDefaultValueChanged(Pin);
   GetResultPin()->PinType.PinSubCategoryObject = FGameplayEffectSpecHandle::StaticStruct();
}

void UBPNode_MakeGameplayEffect::PinConnectionListChanged(UEdGraphPin* Pin)
{
   Super::PinConnectionListChanged(Pin);
   GetResultPin()->PinType.PinSubCategoryObject = FGameplayEffectSpecHandle::StaticStruct();
}
#undef LOCTEXT_NAMESPACE
