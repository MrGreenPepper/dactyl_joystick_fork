from configparser import ConfigParser
import os  

BASEDIR	= os.getcwd()


def getSystemPath():
	systemPath = os.path.split(BASEDIR)
	systemPath = systemPath[0]
	systemPath = systemPath.split(os.sep)
	systemPath = os.path.join(systemPath[1], systemPath[2])
	return systemPath


def getDefaultPaths(filename='config.ini', section='defaultPaths'):
  
	filePath = os.path.join(BASEDIR,filename)
	# create a parser
	parser = ConfigParser()
	# read config file
	parser.read(filePath)

	defaultPaths = {}
	if parser.has_section(section):
		params = parser.items(section)
		#transfer the Data for the mainProgram
		for param in params:
			defaultPaths[param[0]] = param[1]
	else:
		raise Exception('Section {0} not found in the {1} file'.format(section, filename))	

	return defaultPaths

def getPaths():
	defaultPaths = getDefaultPaths()

	print('DefaultPaths are:')
	print('path to the conerning keymap (in the qmk_firmware):\t', defaultPaths['filepath_keymap'])
	print('Type in a new path or just ENTER for using the default one')
	newPath_qmk = input() 
	if(len(newPath_qmk) != 0):
		defaultPaths['filepath_keymap'] = newPath_qmk

	print('qmk firmware path:\t', defaultPaths['filepath_layout'])
	print('Type in a new path or just ENTER for using the default one')
	newPath_layout = input() 
	if(len(newPath_layout) != 0):
		defaultPaths['filepath_layout'] = newPath_layout


	return defaultPaths