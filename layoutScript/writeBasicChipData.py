def writeBasicData(filePath_keymap):
	with open ("basicData.c", "r") as myfile:
		basicData=myfile.readlines()

	keyMapFile = open(filePath_keymap, 'w')
	keyMapFile.close()

	keyMapFile = open(filePath_keymap, 'a')
	for basicLine in basicData:
		keyMapFile.write(basicLine)
	keyMapFile.close()