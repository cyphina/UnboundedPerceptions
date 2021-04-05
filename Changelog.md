# Combat Fixing Part 1
* Cleaned up unit movement a bit
    * Involved several tweaks to the navigational mesh
    * Changed several collision settins on the unit actors
    * Added some pushback between allies when they are done moving (so they won't clump and glitch into each other which launches them far away)

* Improved Effect Bar
    * Nativized UI
    * Now effect slots have their descriptions parsed so they'll show the appropriate values depending on the GE level.
    * Can handle stacking effects and pseudo stacking effects (like huskar burning spears)

* Added several features to the ability API for easier ability creation:
    * Ways to conditionally activate secondary effects of an ability depending on if the damage effect is a hit or miss.
    * Limited the gameplaytag choices in several locations.
    * Added more options to the projectile strategy.
    * And more...

* Added several new skills which showcase how to design unconventional abilities.
    * Discovery of a New Blast - Accumulates blast charges. Each charge has its independent timer. Each charge gives a buff, and is used as resources for other cosmic element spells.
    * Gone with the Blast - Like teleport but creates an explosion where you teleported from.
    * Anaphorica - Buffs units around caster. Throughout the buffs duration, the strength varies. The closer the unit is to the caster, the stronger the buff's effect.
    * Fire Breath - Spews a cone shape hitbox that burns units that walk into it. Hits a unit every 0.4 seconds they are in the flames, and each unit has independent burn timers.
    * Abyssal Torrent - Channels dark energy over 1.5 seconds. Each 0.4 seconds after the channel start time a pillar of energy formulates in front of the caster. 
        * Spawns 3 pillars, and each pillar spawns further away than the previous and deals more damage.

* Added cues for some of the different GEs.
* Added a cosmic element.

* Some stat functionality was fixed/added
    * Attack speed now makes the attack animation and mock attack animations faster
    * Fixed accuracy and pierce calculations in the damage formula to be more balanced.
    * Fixed critical damage multiplier being rediculously high.

* Fixed some other combat related bugs
    * Healing effects are now reflected on the status bar.

* Fixed some other bugs and QOL changes
    * Fixed the workflow involved so that we can set heroes as being active (makes for easier testing if we can activate heroes based on the map)
        * Now it's ok to have them located anywhere initially as opposed to having to manually set their location after they spawn.
    * Hide a lot of unnecessary properties/categories on the unit details panel.
    * Slots now don't show tooltips if they are empty.
    * Fixed swapping spells on actionbar slots.
    * More debug options for easier ability testing (most debug options start with Up_ or Up.)
    * And more...
