import json



def writeBasicData(layoutFileName):
	with open ("basicData.c", "r") as myfile:
  	  	basicData=myfile.readlines()

	layerFile = open(layoutFileName, 'w')
	layerFile.close()

	layerFile = open(layoutFileName, 'a')
	for basicLine in basicData:
		layerFile.write(basicLine)
	layerFile.close()

def addLayers(layoutFileName, layoutData):
	layoutCount = len(layoutData['layers'])
	layoutName = layoutData['layout']

	layoutFile = open(layoutFileName, 'a')
	for layer in range(layoutCount):
		layoutFile.write('#define layer' + str(layer) + ' '+ str(layer)+ '\n')

	layoutFile.write('const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {')

	for layer in range(layoutCount):


		layoutHeader = '\n' + '[layer' + str(layer) + '] = ' + layoutName + '(' + '\n'
		currentLayer = layoutHeader 

		layerData = layoutData['layers'][layer]
		for dataNumber in range(len(layerData)):
			if(dataNumber + 1 == len(layerData)):
				currentLayer += layerData[dataNumber]
			else:	
				currentLayer += layerData[dataNumber] + ', '

		
		if(layer + 1 == layoutCount):
			currentLayer += ')'
		else:	
			currentLayer += '),'
		layoutFile.write(currentLayer)

	layoutFile.write('\n' + '};')

originalFileName = '/home/nox/Downloads/handwired_dactyl_manuform_4x6_layout_mine.json';
print('Type in the layout-filename: (just ENTER for default is' + originalFileName + ' )')
fileName = input()

if(len(fileName)==0):
	fileName = originalFileName;

print('gonna use this file:', fileName)

layoutFileName = '/home/nox/qmk_firmware/keyboards/handwired/dactyl_manuform/4x6/keymaps/joystick/keymap.c'

writeBasicData(layoutFileName)

with open(fileName) as f:
  layoutData = json.load(f)

addLayers(layoutFileName, layoutData)






layerFile = open('layout.c', 'a')
	









layerFile.close()


