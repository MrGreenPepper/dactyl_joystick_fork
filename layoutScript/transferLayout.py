import json
from config import getPaths
from writeBasicChipData import writeBasicData
from layoutConfigurator import addLayers

paths = getPaths()
filePath_newLayout_default = defaultValues['filepath_newlayout']
filePath_keymap = defaultValues['filepath_keymap']





#if the user set up a new path change the default path
if(len(filePath_newLayout_input)!=0):
	filePath_newLayout_default = filePath_newLayout_input

print('gonna use this file:', filePath_newLayout_input)

#write basic chip data, like keymap, joystick pins etc.
writeBasicData(filePath_keymap)

#add layout layers to the data


with open(filePath_newLayout_default) as f:
  layoutData = json.load(f)

addLayers(filePath_keymap, layoutData)

