// Created 1/15/21 9:05 PM

#include "MyProject.h"
#include "Actionbar_EnemyInterface.h"
#include "Enemy.h"

void UActionbar_EnemyInterface::OnWidgetShown(AUnit* focusedUnit)
{
   enemyRef = Cast<AEnemy>(focusedUnit);
}
