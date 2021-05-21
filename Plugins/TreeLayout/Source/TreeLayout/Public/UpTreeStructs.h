#pragma once
#include "UpTreeStructs.generated.h"

USTRUCT(BlueprintType)
struct FUpTreeNode
{
   GENERATED_BODY()
	
   FORCEINLINE bool   IsValid() const { return Id != ""; }

   FORCEINLINE bool IsLeaf() const { return Children.Num() == 0; }

   FORCEINLINE bool IsSibling(FUpTreeNode* OtherNode) const { return Parent == OtherNode->Parent; }

   FUpTreeNode* NextLeft();

   FUpTreeNode* NextRight();

   FUpTreeNode* GetLeftSibling() const;

   UPROPERTY(BlueprintReadOnly, EditAnywhere)
   FName Id = "";

   FUpTreeNode* Parent;

   TArray<FUpTreeNode*> Children;

   UPROPERTY(EditAnywhere, BlueprintReadOnly)
   UObject* Data;

   UPROPERTY(BlueprintReadOnly)
   float X = 0;

   UPROPERTY(BlueprintReadOnly)
   float Y = 0;

   FUpTreeNode* Thread = nullptr;

   FUpTreeNode* Ancestor = this;

   uint32 ChildIndex = 0;

   float Shift = 0.f;

   float Change = 0.f;

   /**
    * @brief Used to adjust the location of children
    */
   int Mod = 0;
};

USTRUCT(BlueprintType)
struct FUpTreeConnector
{
   GENERATED_BODY()

   FUpTreeConnector()  = default;
   ~FUpTreeConnector() = default;

   FUpTreeConnector(FVector2D C1, FVector2D C2, FVector2D C3, FVector2D C4) : Control1(C1), Control2(C2), Control3(C3), Control4(C4) {}

   FUpTreeConnector(const FUpTreeConnector& other) : Control1(other.Control1), Control2(other.Control2), Control3(other.Control3), Control4(other.Control4) {}

   FUpTreeConnector(FUpTreeConnector&& other) noexcept :
       Control1(MoveTemp(other.Control1)), Control2(MoveTemp(other.Control2)), Control3(MoveTemp(other.Control3)), Control4(MoveTemp(other.Control4))
   {
   }

   FUpTreeConnector& operator=(const FUpTreeConnector& other);

   FUpTreeConnector& operator=(FUpTreeConnector&& other) noexcept;

   UPROPERTY(BlueprintReadOnly)
   FVector2D Control1;

   UPROPERTY(BlueprintReadOnly)
   FVector2D Control2;

   UPROPERTY(BlueprintReadOnly)
   FVector2D Control3;

   UPROPERTY(BlueprintReadOnly)
   FVector2D Control4;
};
