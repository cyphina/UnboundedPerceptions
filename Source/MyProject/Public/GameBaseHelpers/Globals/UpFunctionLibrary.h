// Created 2/11/21 12:50 AM

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DEPRECATED_Trigger.h"
#include "UpFunctionLibrary.generated.h"

class AHUDManager;
class ABaseHero;
class AUnit;

UCLASS()
class MYPROJECT_API UUpFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, Category = "Trigger", meta = (WorldContext = "WorldContextObject"))
   static void ActivateTrigger(const FTriggerData& triggerData, const UObject* worldContextObject);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global Accessors", meta = (WorldContext = "WorldContextObject"))
   static TArray<ABaseHero*> GetAllHeroes(const UObject* WorldContextObject);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global Accessors", meta = (WorldContext = "WorldContextObject"))
   static const TArray<ABaseHero*>& GetHeroes(const UObject* worldContextObject);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global Accessors", meta = (WorldContext = "WorldContextObject"))
   static AUnit* GetFocusedUnit(const UObject* worldContextObject);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global Accessors", meta = (WorldContext = "WorldContextObject"))
   static ABaseHero* GetHeroFromIndex(const UObject* worldContextObject, int heroIndex);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global Accessors", meta = (WorldContext = "WorldContextObject"))
   static AHUDManager* GetHUDManager(const UObject* worldContextObject);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BP Helpers", meta = (WorldContext = "WorldContextObject"))
   static FName GetStreamingLevelNameFromActor(const AActor* Actor);
};
