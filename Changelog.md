# Skill Tree Update
* Some various bugfixes I forgot to keep track of.
    * One of them is certainly fixing the damage indicator widget positioning
    * Another one is fixing the logic with the spellbook hud.
* Added a spawner for characters so they don't have to be part of the map (allowing them to get the spawned actor timeline)
* Added a plugin which allows us to layout widgets using Bucheim's tree layout algorithm.
    * Added a custom slate widget that allows us to pan around a limited view of a larger widget (kind of like the blueprint graph)
    * Added a custom user widget that draws connections between the nodes (still needs fixing to draw behind the nodes instead of in front...)