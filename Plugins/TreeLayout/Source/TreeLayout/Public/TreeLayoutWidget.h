#pragma once

#include "Blueprint/UserWidget.h"
#include "TreeLayoutWidget.generated.h"

class UUpTree;
struct FUpTreeConnector;
class UUpTreeLayoutStrategy;
class UCanvasPanel;
class UCanvasPanelSlot;

/**
 * A widget that generates homogeneous sub widgets laid out on a tree formation given an array of FUpTreeNode's that describe the hierarchical relationship between nodes.
 * It can also draw connections between the widgets.
 * We can change what tree gets shown by this widget at runtime.
 * Does not handle the underlying logic for the widgets it layers out.
 */
UCLASS()
class TREELAYOUT_API UTreeLayoutWidget : public UUserWidget
{
   GENERATED_BODY()

 public:
   /**
    * @brief Creates a visual for this tree which involves generating widgets and positioning them.
    * @param Tree - Tree used to figure out the relationship between nodes.
    * @param TreeSizeX - Used to space nodes horizontally.
    * @param TreeSizeY - Used to space nodes vertically.
    * @return Returns the generated widgets in case some other system wants to handle them while the canvas in this widget retains ownership of them.
    */
   UFUNCTION(BlueprintCallable)
   TArray<UUserWidget*> ShowTree(UUpTree* Tree, float TreeSizeX, float TreeSizeY);

   /**
    * @brief Type of widgets that are generated and laid out in a tree formation.
    */
   UPROPERTY(EditAnywhere)
   TSubclassOf<UUserWidget> WidgetClass;

   UPROPERTY(EditAnywhere)
   FLinearColor BranchColor = FLinearColor::Green;

   UPROPERTY(EditAnywhere)
   FLinearColor ThreadColor = FLinearColor::Red;

 protected:
   /** Draws the connection between nodes */
   virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                             int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

   /**
    * @brief Show connections between parent and children nodes
    */
   UPROPERTY(EditAnywhere)
   bool bShowConnections;

   /**
    * @brief Threads are connections made as part to optimize the layout algorithm. You can see enable seeing these to understand how the algorithm works (if it uses threads).
    */
   UPROPERTY(EditAnywhere)
   bool bShowThreads;

   /**
    * @brief The strategy used to layout the tree node widgets
    */
   UPROPERTY(EditAnywhere)
   TSubclassOf<UUpTreeLayoutStrategy> LayoutStrategy;

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
   UCanvasPanel* Can_Node;

 private:
   /** Used to center the nodes on the branch points */
   void OffsetNodes(TArray<UCanvasPanelSlot*>& NodeSlots);

   TArray<FUpTreeConnector> Branches;

   TArray<FUpTreeConnector> Threads;
};
