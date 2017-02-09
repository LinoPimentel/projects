ServerMap - Attempt at developing a tile based map logic for game development use. C# and Unity
Autopot - Autohotkey script, 3rd party program to use in a particular game. It's objecting was an easily configurable program to use game consumables automaticly. Makes use of readProcessMemory from Windows.
Cheats - 3rd party program, for the same game as above. Console application for windows for easy toggling on some client cheats by overwriting client assembly on runtime. C and windows API (Eg: VirtualProtect, WriteProcessMemory).
CheatsBlock - dll to be loaded by the game client. It would make use of a shell code that would hook into X address and call a function from the dll. It would check if certain sprite files were tampered with for exploiting by using checksums.
