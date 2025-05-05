
# Bluetooth Dewise

A malware based on "The Bluetooth device is ready to pair" meme.
Suitable for learning Windows APIs in bizzare, dangerous, (and maybe funny) way.
This program is not suitable for laymen, thus building it might not be easy.
![Screenshot](Screenshot.png)

## DISCLAIMER

WARNING: THIS PROGRAM WILL DESTROY YOUR DATA AND COMPUTER.
IF YOU ARE PLANNING TO PRANK ON WORKING MACHINE, GET AWAY FROM THIS PAGE.
IF YOU ARE TRYING TO TEST THIS PROGRAM, YOU MUST USE A VIRTUAL OR DEDICATED MACHINE FOR TESTING.

THIS PROGRAM IS NOT SUITABLE FOR LAYMEN AND IS INTENDED ONLY FOR ENTERTAINMENT AND EDUCATIONAL PURPOSES. NEVER USE ANY OF THIS TO HARM ANYONE'S COMPUTERS OR PROPERTIES, OR ANY MALICIOUS PURPOSES.
I AM NOT RESPONSIBLE FOR ANY DAMAGES CAUSED BY USING THIS PROGRAM.

CAUTION: To avoid damages to speakers and hearing, set volume to lower level and do not use headphones. Some payload generate low frequency noise which in some cases is able to blowout speakers even in non-destructive mode when volume is set to higher level.

## User Interface and Functions

By running program, you will see a black object, which is imitating a dirt-cheap Bluetooth speaker. Most of buttons are fake, but a button placed on right side is actually a mode button and you can change its mode by clicking it .

 - Bluetooth
 - (MP3 Player)
 - Radio
 - AUX Input

 Again, most of them are fake, but MP3 player mode can play MP3 file(s).
 By default, MP3 function is not activated. To activate it, just put MP3 file(s) in the directory with Bluetooth.exe

Note: In this version, the program doesn't auto-detect while it's running.

Caution: This program is not a proper music player, thus it is not able to use this as music player.




# Non-Destructive Mode (Default)

By default, this program will run in non-destructive mode, which is considered as safe.
This mode is not made to destroy your data or computer.
However I still highly recommend to run on virtual machine.

## Non-Destructive Payloads

These are non-destructive payloads, which are considered as safe.

- Pop-up
- File Opener
- Random Sound Player
- Icon Drawer
- Tunnel Effect
- Window Shaker
- RAW Audio Attack

# Destructive Mode (DANGER)

In destructive mode, the program will destroy your data and make your computer unusable.
By running it in this mode, you will see a warning dialog for 3 times. Please read the warning well and make your decision. DO NOT use this option if you don't know what you're doing or you cannot understand what it says.
To run it in destructive mode, you have to add */DESTRUCTIVE* in command line.

Note: This command line parameter is case-sensitive for safety reason.

## Destructive Payloads

These payloads will not be executed without command line option */DESTRUCTIVE*.
Those destroy data and operating system, which in result make your computer unusable.

- Process Protection
- MBR Writer
- Registry Writer
- Blue Screen Of Death
- RMDIR (Auxiliary Payload)
- Force Shutdown (Auxiliary payload)



# Backdoor Mode

By adding /b option in command line, the program will not show main window at startup.
There is a point to trigger the main window in somewhere.