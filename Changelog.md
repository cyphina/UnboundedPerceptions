# Working on a Boss Part 4
* Continued to rename assets and old source code files more intuitively
* Some collision changes:
    * Fixed collision problems with large enemies which allowed us to launch them.
    * Fixed collisions with projectiles (used on some features mentioned later)
* Added decorator to loop through delayed actions (called latent loop b/c original loop doesn't work with actions like move to or cast spell)
* Added decorator to randomly execute its wrapped node
* Renamed the debug options and added some new utility options:
    * Fixed SeeAll cheat - allows us to see all units on the map.
        * To implement this I also added a way to pause vision updates.
    * Added cheats to buff enemies or allies.
    * Added cheats to show the debug capsules again.
* Actually implemented dodging in terms of logic (dodging now prevents damage and possible execution of gameplay effects). 
    * It also makes projectils whiz past your body.)
* Other small bugfixes to the BTTasks.
