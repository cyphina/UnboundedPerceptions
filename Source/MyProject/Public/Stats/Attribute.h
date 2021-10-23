#pragma once

/**
 * ! Deprecated - Relic of the past before I used the gameplay ability system
 * Attribute are the fundamental stats which are modifyable by leveling up and affect other useful combat properties of a unit
 */

namespace CombatInfo
{
   class RTSUnitStat;

   class MYPROJECT_API Attribute {
      char* name;
      float baseValue; // base value only increased by leveling up or equipment
      float buffValue; // increased by some effects

   public:
      Attribute()
      {
         name      = "";
         baseValue = 0;
         buffValue = 0;
      }

      Attribute(char* name, int baseV)
      {
         name      = name;
         baseValue = baseV;
         buffValue = 0;
      }

      int GetBaseValue() const { return baseValue; }

      void SetBaseValue(int value) { baseValue = value; }

      int GetBuffValue() const { return buffValue; }

      void SetBuffValue(int value) { buffValue = value; }

      // gets complete values
      int GetAdjustedValue() const { return baseValue + buffValue; }

      char* GetName() const { return name; }

      ~Attribute() {}
   };
} // namespace CombatInfo
