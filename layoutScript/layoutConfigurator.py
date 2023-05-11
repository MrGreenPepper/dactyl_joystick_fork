import json

def addLayers(paths):
	with open(paths['filepath_layout']) as f:
		layoutData = json.load(f)

	layoutCount = len(layoutData['layers'])
	layoutName = layoutData['layout']

	keymapFile = open(paths['filepath_keymap'], 'a')
	for layer in range(layoutCount):
		keymapFile.write('#define layer' + str(layer) + ' '+ str(layer)+ '\n')

	keymapFile.write('const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {')

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
		keymapFile.write(currentLayer)

	keymapFile.write('\n' + '};')
	keymapFile.close()