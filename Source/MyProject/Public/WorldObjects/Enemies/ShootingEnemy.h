#pragma once

#include "WorldObjects/Enemies/Enemy.h"
#include "ShootingEnemy.generated.h"

/**
 * Class for enemy that shoots projectiles for its auto attacks
 */
UCLASS()
class MYPROJECT_API AShootingEnemy : public AEnemy
{
   GENERATED_BODY()

   void BeginPlay() override final;
};
