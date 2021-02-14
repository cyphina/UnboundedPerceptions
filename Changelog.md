# Cyphina's Message
Critical Bugfixes and Quest System Checks/Fixes
<br>
# Notes
* Delayed updating healthbar by one tick so other functions can fire off in "OnDamageReceived" delegates properly updating health befor we update the view.
* Added a whole little system allowing us to see what skills an enemy can perform and when we have enemy debug controls we can cast those skills on ourselves.
    * Not the cleanest solution but it'll serve its purpose for now.
* Fixed bug where we moved after level transition.
* Added debug controls to allow enemies to attack friendly targets when under player control
