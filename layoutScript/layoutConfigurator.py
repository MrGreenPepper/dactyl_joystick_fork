
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
	layoutFile.close()