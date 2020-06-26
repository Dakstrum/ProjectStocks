import time
import xml.etree.ElementTree as ET
import json

from python_src import menu

class ButtonObject:
	def __init__(self, x, y, width, height, name, path):
		self.x      = x
		self.y      = y
		self.width  = width
		self.height = height
		self.name   = name
		self.path   = path

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def GetButtonObject(root):
	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			def_object = g_ns.get("{http://www.evolus.vn/Namespace/Pencil}def")
			if(IsDefaultButton(def_object)):
				CreateDefaultButtonObject(g_ns)

			if(IsInvisButton(def_object)):
				CreateInvisButtonObject(g_ns)

def CreateInvisButtonObject(g_ns):
	matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")

	for text_ns in g_ns.findall("text_link:text", namespaces):
		name = menu.menu_name + text_ns.text + "ButtonObject"

	for rect_ns in g_ns.findall("text_link:rect", namespaces):
		if(rect_ns.get("{http://www.evolus.vn/Namespace/Pencil}name") == "rect"):
			width  = rect_ns.get("width")
			height = rect_ns.get("height")

	InvisObject_1 = ButtonObject(int(float(matrix_str[4])), int(float(matrix_str[5])), int(float(width)), int(float(height)), name, None)
	AddButtonObjectToJSON(InvisObject_1)

def CreateDefaultButtonObject(g_ns):
	for sg_ns in g_ns.findall('text_link:g', namespaces):
		for path_ns in sg_ns.findall("text_link:path", namespaces):
			dimensions = path_ns.get("d")
			dimensions = dimensions.split(" ")

		for text_ns in g_ns.findall("text_link:text", namespaces):
			matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")
			name = menu.menu_name + text_ns.text + "ButtonObject"
		
		if((int(float(dimensions[15]))) == 260 and (int(float(dimensions[16]))) == 75): #Button 1
			DefaultObject_1 = ButtonObject(int(float(matrix_str[4])), int(float(matrix_str[5])), int(float(dimensions[15])), int(float(dimensions[16])), name, "assets/images/all_buttons/button1.png")

		if((int(float(dimensions[15]))) == 92 and (int(float(dimensions[16]))) == 57): #Button 1
			DefaultObject_1 = ButtonObject(int(float(matrix_str[4])), int(float(matrix_str[5])), int(float(dimensions[15])), int(float(dimensions[16])), name, "assets/images/all_buttons/button2.png")

		AddButtonObjectToJSON(DefaultObject_1)

def AddButtonObjectToJSON(ButtonObject):
	print("b")
	with open('base.json') as f:
		data = json.load(f)

	if(ButtonObject.path == None):
		data["Objects"][0]["Buttons"].append({"RX": ButtonObject.x, "RY": ButtonObject.y, "Width": ButtonObject.width, "Height" : ButtonObject.height, "Name" : ButtonObject.name})

	else:
		data["Objects"][0]["Buttons"].append({"RX": ButtonObject.x, "RY": ButtonObject.y, "Width": ButtonObject.width, "Height" : ButtonObject.height, "Path": ButtonObject.path, "Name" : ButtonObject.name})
	

	with open('base.json', 'w') as f:
		json.dump(data, f)



def IsDefaultButton(g_def):
	return g_def == "Evolus.Sketchy.GUI:button"

def IsInvisButton(g_def):
	return g_def == "Evolus.WindowsXP.Widgets:Button"
	