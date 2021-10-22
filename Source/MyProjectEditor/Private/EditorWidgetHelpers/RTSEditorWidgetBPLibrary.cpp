#include "EditorWidgetHelpers/RTSEditorWidgetBPLibrary.h"
#include "Package.h"
#include "MyProject/Public/EventSystem/RTSTrigger.h"
#include "UObjectIterator.h"
#include "Engine/Blueprint.h"

TArray<URTSTriggerBase*> URTSEditorWidgetBPLibrary::GetTriggerObjects()
{
   TArray<URTSTriggerBase*> TriggerObjects;
   for(TObjectIterator<URTSTriggerBase> ObjectIt; ObjectIt; ++ObjectIt)
   {
      if(URTSTriggerBase* Obj = *ObjectIt)
      {
         // RF_ArchetypeObject
         if(Obj->HasAnyFlags(EObjectFlags::RF_ClassDefaultObject | EObjectFlags::RF_DefaultSubObject | EObjectFlags::RF_Transient))
         {
            continue;
         }

         UE_LOG(LogTemp, Warning, TEXT("%s"), *Obj->GetFullName());

         if(UObject* Outer = Obj->GetPackage())
         {
            UE_LOG(LogTemp, Warning, TEXT("Outer %s"), *Outer->GetName());
         }

         TriggerObjects.Add(Obj);
      }
   }

   return TriggerObjects;
}
