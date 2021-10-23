#include "MyProject.h"
#include "DraggedActionWidget.h"

#include "Image.h"

void UDraggedActionWidget::SetDraggedImage(UTexture2D* newImage)
{
	Image_Item->SetBrushFromTexture(newImage);
}
