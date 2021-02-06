#include "MyProject.h"
#include "RTSDamageNumberContainer.h"

#include "DamageStructs.h"
#include "Overlay.h"
#include "OverlaySlot.h"
#include "RTSDamageNumberWidget.h"
#include "RTSGlobalCVars.h"
#include "Unit.h"

URTSDamageNumberContainer::URTSDamageNumberContainer(const FObjectInitializer& Initializer) : Super(Initializer), widgetPool(*this)
{
}

void URTSDamageNumberContainer::NativeOnInitialized()
{
   widgetPool.SetWorld(GetWorld());
}

void URTSDamageNumberContainer::SetOwningUnit(AUnit* newOwningUnit)
{
   owningUnit = newOwningUnit;
   if(owningUnit)
   {
      owningUnit->OnUnitDamageReceived().AddUObject(this, &URTSDamageNumberContainer::OnDamageDealt);
   }
}

void URTSDamageNumberContainer::ReleaseDamageNumberWidget(URTSDamageNumberWidget* destroyedDamageNumberWidget)
{
   widgetPool.Release(destroyedDamageNumberWidget);
}

void URTSDamageNumberContainer::ReleaseSlateResources(bool bReleaseChildren)
{
   Super::ReleaseSlateResources(bReleaseChildren);
   widgetPool.ReleaseAllSlateResources();
}

void URTSDamageNumberContainer::OnDamageDealt(const FUpDamage& damageReceived)
{
   if(!AttackCVars::bUseOldDamageNumbers)
   {
      URTSDamageNumberWidget* damageNumber = widgetPool.GetOrCreateInstance(damageNumberWidgetClass);
      damageNumber->SetDamageTextProps(damageReceived);
      UOverlaySlot* damageNumberSlot = damageNumberContainerWidget->AddChildToOverlay(damageNumber);
      damageNumberSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
      damageNumberSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
   }
}
