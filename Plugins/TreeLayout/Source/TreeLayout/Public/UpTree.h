#pragma once
#include "UpTreeStructs.h"
#include "UpTree.generated.h"

class UUpTreeLayoutStrategy;

/**
 * @brief Represents data with a hierarchical relationship in a tree-like format.
 */
UCLASS(BlueprintType)
class UUpTree : public UObject
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
   static UUpTree* CreateTree(UObject* Outer, TArray<UObject*> DataObjects, const TArray<FName>& Ids, const TArray<FName>& ParentIds);

   UFUNCTION(BlueprintCallable)
   int GetMaxDepth();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FString MakeStringFromTree() const;

   UFUNCTION(BlueprintCallable, BlueprintPure)
   const FUpTreeNode& GetNodeById(FName Id) const;

   FUpTreeNode* GetRoot() const;

   UFUNCTION(BlueprintCallable)
   void PositionNodes(TSubclassOf<UUpTreeLayoutStrategy> LayoutStrategy, FVector2D TreeSize);

   UFUNCTION(BlueprintCallable)
   const TMap<FName, FUpTreeNode>& GetHierarchy() const { return Hierarchy; }

   UFUNCTION(BlueprintCallable)
   TArray<FUpTreeConnector> MakeNodeConnections() const;

   /**
    * @brief Resets all the node's positions and threads
    */
   UFUNCTION(BlueprintCallable)
   void ResetTree();

#if WITH_EDITORONLY_DATA
   /**
    * @brief Creates connectors between threads (links between nodes that some of the tree layout algorithms generate for optimization purposes)
    * @return Returns connectors used to build curves between threads.
    */
   UFUNCTION(BlueprintCallable)
   TArray<FUpTreeConnector> MakeThreadConnections() const;
#endif

   static void PostOrderTraversal(FUpTreeNode* node, const TFunction<void(FUpTreeNode*)>& f);
   static void PreOrderTraversal(FUpTreeNode* node, const TFunction<void(FUpTreeNode*)>& f);

   static inline const FUpTreeNode INVALID_NODE = FUpTreeNode();

 protected:
   UFUNCTION(BlueprintCallable, BlueprintPure, Meta = (DisplayName = "Get Tree Root"))
   const FUpTreeNode& BP_GetRoot() const { return *Root; }

 private:
   void GetTreeExtents(FUpTreeNode*& Left, FUpTreeNode*& Right, FUpTreeNode*& Bottom) const;

   /**
    * @brief Adjusts the nodes of the tree to have coordinates that scale based on the TreeSize (instead of normalized coordinates)
    * @param Left Left most node of the tree
    * @param Right Right most node of the tree
    * @param Bottom Bottom most node fo the tree
    * @param TreeSize Desired size of the tree (when drawing the tree the nodes will be spread amongst these bounds)
    */
   void OffsetTreeNodes(const FUpTreeNode& Left, const FUpTreeNode& Right, const FUpTreeNode& Bottom, const FVector2D TreeSize);

   TMap<FName, FUpTreeNode> Hierarchy;
   FUpTreeNode*             Root     = nullptr;
   int                      MaxDepth = 0;
};
