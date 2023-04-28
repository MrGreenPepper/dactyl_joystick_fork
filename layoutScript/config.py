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
	filepath_keymap = defaultPaths['filepath_keymap']
	filepath_layout = defaultPaths['filepath_layout']
	systemPath = getSystemPath()
	defaultPaths['filepath_keymap'] = os.path.join(systemPath, filepath_keymap)
	defaultPaths['filepath_layout'] = os.path.join(BASEDIR, filepath_layout)
	test1 = os.path.join(systemPath, filepath_keymap)
	test2 = os.path.join(BASEDIR, filepath_layout)

	return defaultPaths

def getPaths():
	defaultPaths = getDefaultPaths()

	print('DefaultPaths are:')
	print('qmk firmware path:\t', defaultPaths['qmk_path'])
	print('Type in a new path or just ENTER for using the default one')
	newPath_qmk = input() 
	if(newPath_qmk.len() != 0):
		defaultPaths['qmk_path'] = newPath_qmk

	print('qmk firmware path:\t', getDefaultPaths['filepath_layout'])
	print('Type in a new path or just ENTER for using the default one')
	newPath_layout = input() 
	if(newPath_qmk.len() != 0):
		defaultPaths['filepath_layout'] = newPath_layout


	return defaultPaths