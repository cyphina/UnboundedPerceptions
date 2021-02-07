# Cyphina's Message
Sorry it's been a while since I did formal changelog notes. I massively changed up the way the game's code is structured; I think it will be good for the long run
<br>
# Notes
* Somehow I created a regression in spellcast component involving beginplay unit reference setup by accidentally uncommenting reset target. After some days of testing I don't think we have to ever reset the target so I removed that line of code.
* Fixed choppy movement due to removing calls to StopMovement() which canceled out all velocity.
* Fixed asset tags not being passed through the spell function library functions.
* Fixed incorrect pin name for AssetTags in CreateEffect_BP custom node. This was preventing our cure spell from healing us.
* Refactored part of the settings menu (decoupled from game logic) and added three new settings:
    * Added setting and functionality for moving units in formation .
    * Added setting and functionality and for autoclicking.
    * Added setting and functionality for locking the actionbar.
* Some small visual changes to the help text to make it more apparant.
* Fixed some bugs with the unit selection slot and tab selection.
* Fixed misalignment of components for some of the hero BPs
* Some other refactorings made to C++ and affected BPs.