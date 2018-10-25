// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "ShootingEnemy.generated.h"

/**
 * Class for enemy that shoots projectiles for its auto attacks
 */

class ARTSProjectile;

UCLASS()
class MYPROJECT_API AShootingEnemy : public AEnemy
{
   GENERATED_BODY()

   /**Element of standard attacks*/
   UPROPERTY(EditAnywhere, Category = "ShootingStats")
   TSubclassOf<ARTSProjectile> projectileType;

   void BeginPlay() override;
   void Attack() override;
};
