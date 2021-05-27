#pragma once
#include "K2Node_ConstructObjectFromClass.h"
#include "BPNode_MakeGameplayEffect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYPROJECTDEVELOPERHELPERS_API UBPNode_MakeGameplayEffect : public UK2Node_ConstructObjectFromClass
{
   GENERATED_UCLASS_BODY()

   ///UEdGraphNode Interface
   void         AllocateDefaultPins() override;     //Creates the default pins on the node
   FLinearColor GetNodeTitleColor() const override; //Color of the node
   /**Expand the node with additional pins*/
   void  ExpandNode(class FKismetCompilerContext& compilerContext,
                    UEdGraph* sourceGraph) override;         //Used to create functionality (programs a node graph to represent this one node like a collapsed graph)
   FText GetNodeTitle(ENodeTitleType::Type TitleType) const; //Get title of the node
   ///End UEdGraph interface

   ///Begin UK2Node interface
   void GetMenuActions(
       FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override; //Changes certain things but needs to have call to super function to see it in the node browser
   FText GetMenuCategory() const override;                                 //Changes category inside the node browser

   ///Pin Accessors
   UEdGraphPin* GetDurationPin() const;
   UEdGraphPin* GetPeriodPin() const;
   UEdGraphPin* GetLevelPin() const;
   UEdGraphPin* GetResultPin() const;
   UEdGraphPin* GetElemPin() const;
   UEdGraphPin* GetNamePin() const;
   UEdGraphPin* GetClassPin() const;
   UEdGraphPin* GetAssetTagsPin() const;

   /*These functions are overriden and do not call the parent function because they change options we do not want
	due to the nature of how this node was created*/
   void         GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;
   virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
   virtual void PostPlacedNewNode() override;
   virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
   virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

 protected:
   /**Gets the default node title when no class is selected */
   virtual FText GetBaseNodeTitle() const override;
   /**Gets the node title when a class has been selected */
   virtual FText GetNodeTitleFormat() const override;
   /**Gets base class to use for the 'class' pin.  UObject by default. */
   virtual UClass* GetClassPinBaseClass() const override;
   bool            IsSpawnVarPin(UEdGraphPin* pin) const override;
};
