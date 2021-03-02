# Working on a boss part 2
* Developed more of the bosses abilities:
    * Added a psychic headband like ability (acts as a pushback)
    * Added some other really standard abilities (some aoe attack and debuff)
    * Fixed projectile logic and improved projectile creating flow.
* Created bosses BT.
* Started fixing/cleaning existing BT nodes (focusing on ones used in the BT I mapped out).
# Bugfixes/General Improvements
* Removed a lot of unnecessary assets:
    * Removed useless animations
    * Removed useless AI BPs
    * Removed patrol and follow trees for NPCs. Planning to replace them with components (we already have the follow component)
    * Removed useless spell effects
* Used some particle effects from infinity blade for different spells.
* Performed some linting and renamed assets acording to a scheme.
* Refactored some code (better comments, consolidating interfaces) and added some debug logging for crashes that were hard to replicate.
* Fixed glitch with no channeling bar showing for abilities with no targeting.
* Added text replacement for the spell descriptions.
* Fixed indexing functionality in spells.
* Fixed montage attack animation notifies not dealing damage.