#pragma once

#include "UpTreeStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TreeLayoutFunctionLibrary.generated.h"

class UUpTree;
struct FUpTreeNode;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class TREELAYOUT_API UTreeLayoutFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   /**
    * @brief Generates a hierarchy from a relational table (e.g. {a=>b, b=>c, ...})
    * @param Outer - Object which owns the newly created tree (tied lifetimes).
    * @param DataObjects - List of UObjects with UPROPERTY's for the ID and Parent ID references.
    * @param Ids - List of ids for each of the nodes.
    * @param ParentIds - Each ParentId should be a name inside Ids. The ParentId identifies the current node's parent. Root has a ParentId of "".
    * @return - Returns a UObject with data that holds node data generated from the relational table.
    */
   UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Outer", HidePin = "Outer"))
   static UUpTree* StratifyData(UObject* Outer, TArray<UObject*> DataObjects, const TArray<FName>& Ids, const TArray<FName>& ParentIds);

   /**
    * @brief Grabs the position of a node
    * @param Node - Node to get the position of
    * @return Returns the node's position
    */
   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FVector2D GetTreeNodePosition(const FUpTreeNode& Node);

   /**
    * @brief Used to make connections between the center of parent and child node widgets given only the node widget data and not the node data.
    * ! Invariant - For accurate results, both widgets must be on screen. You can also try using UUpTree::MakeNodeConnections which works when the widgets are not all on screen at once.
    * @param ContainingWidget - Widget that contains both parent and child (most likely a canvas)
    * @param Parent - Parent Widget
    * @param Child  - Child Widget
    * @return Returns a connector connecting between the center of the parent and center of the child widget.
    */
   UFUNCTION(BlueprintCallable, BlueprintPure)
   static FUpTreeConnector MakeConnectionBetweenWidgets(const UUserWidget* ContainingWidget, const UUserWidget* Parent, const UUserWidget* Child);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const bool IsNodeValid(const FUpTreeNode& Node);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   static const FUpTreeNode& GetParent(const FUpTreeNode& Node);

   /** Can use this to debug connectors in BPs */
   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (BlueprintAutocast))
   static FString ConnectorToString(const FUpTreeConnector& Connector);

   /** Can use this to debug nodes in BPs */
   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (BlueprintAutocast))
   static FString NodeToString(const FUpTreeNode& Node);

   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (BlueprintAutocast))
   static FString UpTreeToString(UUpTree* Tree);
};
