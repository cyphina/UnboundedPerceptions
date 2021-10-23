#include "MyProject.h"
#include "MovingState.h"
#include "WorldObjects/Unit.h"

void MovingState::Enter(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Entering Move"));
}

void MovingState::Exit(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Exiting Move"));
}

void MovingState::Update(AUnit& unit, float deltaSeconds) {}
