#pragma once

UENUM(BlueprintType)
enum class ECursorStateEnum : uint8 {
   Select UMETA(DisplayName = "Select"),
   Moving,
   Attack,
   PanUp,
   PanDown,
   PanLeft,
   PanRight,
   Interact,
   Talking,
   Item,
   Magic,
   UI,
   AttackMove
};
