import json
import os

WIRING_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'wiring.json')


def loadWiring():
	with open(WIRING_FILE) as f:
		return json.load(f)


def layerToMatrix(layer, wiring):
	"""Ordnet die Tasten aus dem QMK-Configurator (Reihenfolge des Standard-LAYOUT)
	den Matrix-Positionen der eigenen Verdrahtung zu."""
	rows, cols, mapping = wiring['rows'], wiring['cols'], wiring['wiring']
	if len(layer) != len(mapping):
		raise Exception('Layer hat {0} Tasten, wiring.json erwartet {1}'.format(len(layer), len(mapping)))

	matrix = [['KC_NO'] * cols for _ in range(rows)]
	for keycode, positions in zip(layer, mapping):
		for row, col in positions:
			matrix[row][col] = keycode
	return matrix


def addLayers(paths):
	with open(paths['filepath_layout']) as f:
		layoutData = json.load(f)
	wiring = loadWiring()
	layers = layoutData['layers']

	out = []
	for layer in range(len(layers)):
		out.append('#define layer{0} {0}\n'.format(layer))

	out.append('\n/* Erzeugt aus {0} mit wiring.json */\n'.format(os.path.basename(paths['filepath_layout'])))
	out.append('const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {\n')
	for layer in range(len(layers)):
		matrix = layerToMatrix(layers[layer], wiring)
		out.append('[layer{0}] = {{\n'.format(layer))
		rowLines = ['    { ' + ', '.join(row) + ' }' for row in matrix]
		out.append(',\n'.join(rowLines))
		out.append('\n}' + (',' if layer + 1 < len(layers) else '') + '\n')
	out.append('};\n')

	with open(paths['filepath_keymap'], 'a') as keymapFile:
		keymapFile.write(''.join(out))
