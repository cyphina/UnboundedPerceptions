// #include "EnemyDebugCursorClickFunctionality.h"
// #include "RTSPawn.h"
// #include "UserInput.h"
//
// EnemyDebugCursorClickFunctionality::EnemyDebugCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef) : pawnRef(pawnRef), controllerRef(controllerRef)
// {
// }
//
// void EnemyDebugCursorClickFunctionality::HandleLeftClick()
// {
//    float floatX, floatY;
//    controllerRef->GetMousePosition(floatX, floatY);
//    pawnRef->startMousePos = FVector2D{floatX, floatY};
//
//    switch(pawnRef->GetCursorState())
//    {
//       case ECursorStateEnum::Attack:
//       case ECursorStateEnum::Select:
//       case ECursorStateEnum::Moving:
//       case ECursorStateEnum::PanUp:
//       case ECursorStateEnum::PanDown:
//       case ECursorStateEnum::PanLeft:
//       case ECursorStateEnum::PanRight: SelectSingleUnitUnderClick(); break;
//       case ECursorStateEnum::Interact: ClickInteract(); break;
//       case ECursorStateEnum::Item: ClickUseItem(); break;
//       case ECursorStateEnum::Magic: ClickCastSpell(); break;
//       case ECursorStateEnum::Talking: ClickTalk(); break;
//       case ECursorStateEnum::AttackMove: ClickAttackMove(); break;
//       default: break;
//    }
// }
//
// void EnemyDebugCursorClickFunctionality::HandleRightClick()
// {
//    if(GetCursorState() == ECursorStateEnum::Magic)
//    {
//       pawnRef->CompleteHaltSelected();
//       return;
//    }
//
//    if(GetCursorState() != ECursorStateEnum::UI)
//    {
//       pawnRef->GetHitResultRightClick(clickHitResult);
//       if(clickHitResult.bBlockingHit)
//       {
//          switch(clickHitResult.GetComponent()->GetCollisionObjectType())
//          {
//             case ECC_WorldStatic:
//             {
//                const FVector location = clickHitResult.Location;
//                pawnRef->CreateClickVisual(location);
//                pawnRef->CompleteHaltSelected();
//
//                for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies)
//                {
//                   ally->GetUnitController()->Move(FVector(location));
//                   ally->GetUnitController()->StopAutomation();
//                }
//             }
//             break;
//             case ENEMY_CHANNEL:
//             {
//                AEnemy* enemy = Cast<AEnemy>(clickHitResult.GetActor());
//
//                pawnRef->CompleteHaltSelected();
//                for(AAlly* ally : controllerRef->GetBasePlayer()->selectedAllies)
//                {
//                   Cast<AAllyAIController>(ally->GetUnitController())->GetTargetedAttackComponent()->BeginAttack(enemy);
//                   ally->GetUnitController()->StopAutomation();
//                }
//             }
//             break;
//             default: break;
//          }
//       }
//    }
// }
//
// void EnemyDebugCursorClickFunctionality::HandleShiftLeftClick()
// {
// }
//
// void EnemyDebugCursorClickFunctionality::HandleShiftRightClick()
// {
// }
//
// void EnemyDebugCursorClickFunctionality::HandleLeftClickRelease()
// {
// }
