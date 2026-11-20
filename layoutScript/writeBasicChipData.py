import os

def writeBasicData(filePath_keymap):
	with open (os.path.join(os.path.dirname(os.path.abspath(__file__)), "basicData.c"), "r") as myfile:
		basicData=myfile.readlines()

	keyMapFile = open(filePath_keymap, 'w')
	keyMapFile.close()

	keyMapFile = open(filePath_keymap, 'a')
	for basicLine in basicData:
		keyMapFile.write(basicLine)
	keyMapFile.close()