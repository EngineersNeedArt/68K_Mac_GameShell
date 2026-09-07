# 68K_Mac_GameShell
A small shell of an application that can be repurposed for creating 68K Macintosh sprite-based games.

<p align="center">
  <img src="Images/FlappyToaster.png" width="512" alt="FlappyToaster screenshot">
</p>

### A note about the files

1) The 68K Macintosh used a <CR> (carriage return) to indicate the end of a line of text. Modern computing (and GitHub) prefer using the <LF> (line feed) character to indicate line breaks. To make the sources more readable in GitHub I have converted line endings to Unix's <LF>. If though you choose to pull down the sources and use them in a 68K project, you will want to covert the lines endings of the files to <CR>.

`mac2unix` and `unix2mac` are two command lines tools that do this conversion. To make the source files 68K Mac ready:

`find . -type f \( -name "*.c" -o -name "*.h" \) -exec unix2mac {} +`

To clean them back up for GitHub:

`find . -type f \( -name "*.c" -o -name "*.h" \) -exec mac2unix {} +`

2) Other files, the THINK C project files, resource files, paint files, are not plaintext—they have resource forks. These were compressed (zipped) on MacOS before uploading to this repo. You will have to uncompress them on the Mac to restore the resource fork before moving them to a 68K Mac (or emulator).

## Included

### 68K_GameShellBW
A directory containing a THINK C 5.0 project file, a resource file and sources for a "game shell". It is intended to be a shell you would use to write your own black and white, sprite-based, 68K Macintosh game.

The project contains utilities for setting up and initializing the basic Macintosh Toolbox. It sets up a number of offscreen bitmaps for rendering as well as for sprites. It initializes the Sound Manger. It creates a basic menu bar and drops into an event loop.

You can start a game and a it will drop into a game loop. It will animate the game and respond to keyboard events until you pause or quit the game.

The game itself is little more than a sprite moving slowly across the screen, rotating when you press the left and right cursor keys.

To create your own game you would replace the artwork, sounds with your own game-specific assets. Game logic you'll have to completely implement yourself. But the game shell provides all the scaffolding you need to get up and running quickly.

### FlappyToaster
A directory containing a (rather lame) game created from the game shell. It shows you the kinds of changes you might require to do something like this on your own.

(Note: some artwork borrowed from a popular vintage screensaver.)

### GameShells.dsk.zip
A compressed disk image that will mount with the [Basillisk II](https://basilisk.cebix.net) Macintosh emulator. In addition to the sources and project files for the **68K_GameShellBW** above, it also includes all the tools needed to compile, link and run the game shell and your game.

Additionally, there is a color game shell for creating color games for the early Macintosh. I have not at this time been able to clean up the sources however—they're likely to be a bit rough.

*Note: the source files in disk image have the standard Mac line-endings.*
