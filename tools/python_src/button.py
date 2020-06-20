import time
import xml.etree.ElementTree as ET
import json

class ButtonObject:
	def __init__(self, x, y, width, height, name):
		self.x      = x
		self.y      = y
		self.width  = width
		self.height = height
		self.name   = name

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def GetButtonObject(root):
	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			def_object = g_ns.get("{http://www.evolus.vn/Namespace/Pencil}def")
			if(IsButton(def_object)):
				CreateButtonObject(g_ns)

def CreateButtonObject(g_ns):
	for sg_ns in g_ns.findall('text_link:g', namespaces):
		for path_ns in sg_ns.findall("text_link:path", namespaces):
			dimensions = path_ns.get("d")
			dimensions = dimensions.split(" ")

		for text_ns in g_ns.findall("text_link:text", namespaces):
			matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")
			name = "MainMenu" + text_ns.text + "ButtonObject"
			ButtonObject_1 = ButtonObject(int(float(matrix_str[4])), int(float(matrix_str[5])), int(float(dimensions[15])), int(float(dimensions[16])), name)

		AddButtonObjectToJSON(ButtonObject_1)

def AddButtonObjectToJSON(ButtonObject):
	print("b")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"][0]["Buttons"].append({"RX": ButtonObject.x, "RY": ButtonObject.y, "Width": ButtonObject.width, "Height" : ButtonObject.height, "Path": "assets/images/mainmenu/assets/mainmenu-button.png", "Name" : ButtonObject.name})

	with open('base.json', 'w') as f:
		json.dump(data, f)

def IsButton(g_def):
	if(g_def == "Evolus.Sketchy.GUI:button"):
		return True
	else:
		return False