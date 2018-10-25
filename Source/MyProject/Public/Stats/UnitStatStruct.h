#pragma once

/**Used to setup initial stats for a unit*/
USTRUCT(BlueprintType, NoExport)
struct FUnitStatStruct {
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   TArray<Attributes> defaultAttributes;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   TArray<UnitStats> defaultUnitStats;
   /**Shooting range*/
   UPROPERTY(EditAnywhere, Category = "ShootingStats")
   TArray<Vitals> defaultVitals;
   UPROPERTY(EditAnywhere, Category = "ShootingStats")
   TArray<Mechanics> defaultMechanics;
   UPROPERTY(EditAnywhere, Category = "ShootingStats")
   TArray<int> defaultValues;

   /**Element of standard attacks*/
   UPROPERTY(EditAnywhere, Category = "ShootingStats")
   FGameplayTag element;
};