import json
from config import getPaths
from writeBasicChipData import writeBasicData
from layoutConfigurator import addLayers

paths = getPaths()



#write basic chip data, like keymap, joystick pins etc.
writeBasicData(paths['filepath_keymap'])

#add layout layers to the data
addLayers(paths)

