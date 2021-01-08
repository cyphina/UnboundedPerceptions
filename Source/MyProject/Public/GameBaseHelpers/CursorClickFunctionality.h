#pragma once

/**
 * Interface for a class that controls click functionality depending on the cursor state
 */

class MYPROJECT_API ICursorClickFunctionality
{
 public:
   virtual void HandleLeftClick()        = 0;
   virtual void HandleRightClick()       = 0;
   virtual void HandleShiftLeftClick()   = 0;
   virtual void HandleShiftRightClick()  = 0;
   virtual void HandleLeftClickRelease() = 0;

   virtual ~ICursorClickFunctionality() = default;
};
