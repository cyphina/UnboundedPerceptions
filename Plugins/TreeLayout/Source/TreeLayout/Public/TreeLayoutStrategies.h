#pragma once
#include "TreeLayoutStrategies.generated.h"

struct FUpTreeNode;
class UUpTree;

UCLASS(Abstract, BlueprintType)
class UUpTreeLayoutStrategy : public UObject
{
   GENERATED_BODY()

 public:
   /**
    * @brief 
    * @param Tree Given a tree, the nodes of the tree will be arranged to fit a certain layout.
    */
   virtual void LayoutNodes(UUpTree* Tree) const PURE_VIRTUAL(UUpTreeLayoutStrategy::LayoutNodes, );
};

UCLASS()
class UUpTreeKnuthStrategy : public UUpTreeLayoutStrategy
{
   GENERATED_BODY()

 public:
   virtual void LayoutNodes(UUpTree* Tree) const override;
};

UCLASS()
class UUpTreeKnuthWSNaive : public UUpTreeLayoutStrategy
{
   GENERATED_BODY()

 public:
   virtual void LayoutNodes(UUpTree* Tree) const override;

 private:
   static void PositionNodesMinimalWidthNoCentering(FUpTreeNode* Node, TArray<int>& NextNodeIndex);
};

UCLASS()
class UUpTreeKnuthWSCentered : public UUpTreeLayoutStrategy
{
   GENERATED_BODY()

 public:
   virtual void LayoutNodes(UUpTree* Tree) const override;

 private:
   static void PositionNodesMinimalWidthCentering(FUpTreeNode* Node, TArray<int>& NextNodeIndex, TArray<int>& Offsets);
};

UCLASS()
class UUpTreeReingoldTilfordStrategy : public UUpTreeLayoutStrategy
{
   GENERATED_BODY()

 public:
   virtual void LayoutNodes(UUpTree* Tree) const override;

 private:
   static FUpTreeNode* ReingoldTilfordSetup(FUpTreeNode* Node);
   static void         AddMods(FUpTreeNode* node, int mod = 0);
   static int          FixSubtrees(FUpTreeNode& Left, FUpTreeNode& Right);

   /*
    * @brief Walks along the inside and outside of the subtrees.
    * Inside moves along contours (max or minimum coordinates of side of tree) to find overlaps.
    * Outside is to set "Thread" nodes.
    * @param Left 
    * @param Right 
    * @param MaxOffset 
    * @param LeftOuter 
    * @param RightOuter 
    * @return 
    */
   static void Contour(FUpTreeNode*& Left, FUpTreeNode*& Right, float& MaxOffset, float& LeftOffset, float& RightOffset, FUpTreeNode*& LeftOuter,
                       FUpTreeNode*& RightOuter);
};

UCLASS()
class UUpTreeBucheimLayoutStrategy : public UUpTreeLayoutStrategy
{
   GENERATED_BODY()

 public:
   virtual void LayoutNodes(UUpTree* Tree) const override;

 private:
   static void FirstWalk(FUpTreeNode* Node, int Distance = 1);

   /**
    * @brief Combine a new subtree with previous subtrees similar to that in RT algorithm.
    * @param Node 
    * @param DefaultAncestor 
    * @param Distance 
    */
   static void Apportion(FUpTreeNode* Node, FUpTreeNode*& DefaultAncestor, int Distance);
   static void MoveSubtree(FUpTreeNode* GreatestLeftAncestor, FUpTreeNode* GreatestRightAncestor, int Shift);
   static void ExecuteShifts(FUpTreeNode* Node);
   /**
    * @brief 
    * @param NodeInsideLST 
    * @param Node 
    * @param DefaultAncestor 
    * @return 
    */
   static FUpTreeNode* Ancestor(FUpTreeNode* NodeInsideLST, FUpTreeNode* Node, FUpTreeNode* DefaultAncestor);
   static void         SecondWalk(FUpTreeNode* Node, int Mod = 0);
};
