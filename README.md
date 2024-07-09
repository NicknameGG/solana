# Solana
Geometry Dash random pathfinding algorithm POC

## How to use
> Solana is NOT intended to be use as a normal mod, it's so unstable you can't play any level with it.
> Solana was also never tested on other plataforms than Windows.
1. Build the project using the provided CMakeLists.txt
2. Add manually to your Geode mods folder
3. In order to use Solana you must need to load the level in the local levels tab, you will see the NCS button to the right, that's the UI.
4. Everything in the UI is self explanatory.

## Known issues
- Checkpoints are inaccurate as hell meaning the playback is often wrong. Use Train from File to fix any train issues.
- `removeObjectFromSection` tends to crash the game very often, specially with 2.2 or really trigger heavy 2.113 levels.

## Media
Most media can be found on the Geode's discord server #dev-chat channel.
However, i uploaded some to youtube:
* [Geometry Dash Pathfinder beats Decode](https://www.youtube.com/watch?v=hhrsAnlPvEo)
* [Geometry Dash Pathfinder beats Geometrical Dominator](https://www.youtube.com/watch?v=-shOlmxVBEQ)

## Disclaimer
The code is a hot mess, I got really bored with the project and the crashes drove me crazy.
I decided to open source it though, maybe someone can find it useful.

## Credits
- Eclipse Menu for the [PracticeFix](https://github.com/EclipseMenu/EclipseMenu/blob/main/src/hacks/Bot/PracticeFix.cpp) code