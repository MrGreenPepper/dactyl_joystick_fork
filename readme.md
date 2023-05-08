<div>
<img src="/pictures/IMG_5070.JPG" alt="keyboard image"  style="align: left; margin-right: 10px; margin-bottom: 10px; width: 98vw;">
</div>

**Hi,**

this is my own little fork of the **dactyl manuform 4x6 keyboard** modified with a joystick. This repository describes only the changes necessary for a joystick. For the rest please look up at the building guides for the original dactyl manuform keyboards (the size (4x6, 5x6, ...) doesn't really matter, the principles are the same just make sure your chip is supported and have enough pins).

Feel free to ask anything.<br>

**Special thanks to:**<br>
[Leopold Dietrich](https://www.leopold-dietrich.de/) for help with the 3D prints.<br>
And to the [qmk developers](https://github.com/qmk/qmk_firmware) and the concerning [discord community](https://discord.com/invite/Uq7gcHh) for code and help.

# Guide:

**1. Get the qmk firmware**<br>
Go to the [qmk github page](https://github.com/qmk/qmk_firmware), download the firmware and get it running.

**2. adjust the pin varialbes according to your chip**<br>
Go to the [basicData.c](/layoutScript/basicData.c) file. At the start of the file you can see the declaration of 3 variables (xPin, yPin, swPin).
They are used to receive the inputs from the joystick. Change the values of the variables xPin, yPin and swPin according to how you wired your chip
(most likely you can find the signing code of the pins on your chip itself or you need to search your chip layout on the internet).

**3. Configurate your keyboard layout**<br>
Go to the [qmk layout configurator](https://config.qmk.fm/#/handwired/dactyl_manuform/4x6/LAYOUT) website. Setup your layout and download the JSON-file.
Don't compile it! Just download the layout.<br>
_(3.1. lay up your own qmk keymap - not needed but recommended <br>
Go to the qmk_firmware directory. Then dive into the ./keyboards/handwired/dactyl_manuform/4x6/keymaps/ directory.
Copy the default layout directory into a second directory named by you.)_

**4. Setup the filepaths**<br>
Go to [config.ini](/layoutScript/config.ini). Setup the filepaths.
The 'filepath_keymap' variable should point to the keymap.c file in your qmk keymap directory (see step 3.1).
The 'filepath_layout' variable should point to your, already downloaded, layout json-file.

**5. Run the [transferLayout.py](/layoutScript/transferLayout.py) file**<br>
Please make sure to open the (layoutScript)[/layoutScript] directory first and run the file from here to avoid errors with relativ paths.

**6. Flash the chip**<br>
Run the concerning qmk flash command:<br>
_qmk flash -kb [keyboard] -km [keymap]_ <br>
For example in my case it would be (with step 4.1.):<br>
qmk flash -kb handwired/dactyl_manuform/4x6 -km joystick<br>

# How does it work?:

1. the transferLayout script writes the basic chip data (basicData.c) into the qmk file (keymap.c)
2. the script appends the qmk keymap.c with the layout data

# Data overview

| directory                      | description                                               |
| ------------------------------ | --------------------------------------------------------- |
| [3d_models](/3d_models/)       | containing the models for a 3d printer                    |
| [layoutScript](/layoutScript/) | combines your layout with the algorithms for the joystick |
| [pictures](/pictures/)         | some example and workflow pictures                        |
