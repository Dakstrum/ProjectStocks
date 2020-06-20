import time
import xml.etree.ElementTree as ET
import json

class MenuObject:
	def __init__(self, name):
		self.name = name

class TextObject:
	def __init__(self, name, x, y, content):
		self.name    = name
		self.x       = x
		self.y       = y
		self.content = content

class ButtonObject:
	def __init__(self, x, y, width, height, name):
		self.x      = x
		self.y      = y
		self.width  = width
		self.height = height
		self.name   = name

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def main():
   				
	tree = ET.parse('mainmenu.xml')
	root = tree.getroot()

	CreateMenuObject(root)
	CreateButtonObject(root)
	CreateTextObject(root)	
	print("suc")
	time.sleep(500);

def CreateMenuObject(root):
	for properties_ns in root.findall('p_link:Properties', namespaces):
		for property_ns in properties_ns.findall('p_link:Property', namespaces):
			if(property_ns.get("name") == "name"):
				print(property_ns.text)
				MenuObject_1 = MenuObject(property_ns.text)
				AddMenuObjectToJSON(MenuObject_1)

def AddMenuObjectToJSON(MenuObject):
	print("m")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"].append({"Type": "Menu", "Path": "insert_path", "Name": MenuObject.name, "X": 0, "Y": 0, "Width": 1920, "Height": 1080, "Buttons": [], "Text": []})

	with open('base.json', 'w') as f:
		json.dump(data, f)

def CreateTextObject(root):
	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			def_object = g_ns.get("{http://www.evolus.vn/Namespace/Pencil}def")
			if(IsText(def_object)):
				for text_ns in g_ns.findall("text_link:text", namespaces):
					for text_content_ns in text_ns.findall("text_link:tspan", namespaces):
						matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")
						name = "MainMenu" + text_content_ns.text + "TextObject"
						TextObject_1 = TextObject(name, int(float(matrix_str[4])), int(float(matrix_str[5])), text_content_ns.text)
						AddTextObjectToJSON(TextObject_1)


def AddTextObjectToJSON(TextObject):
	print("t")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"][0]["Text"].append({"RX": TextObject.x, "RY": TextObject.y, "FontSize": 70, "Name" : TextObject.name, "Path" : "assets/font/DanielLinssenM5/m5x7.ttf", "Content" : TextObject.content, "Color" : [14, 17, 19, 255]})

	with open('base.json', 'w') as f:
		json.dump(data, f)

def CreateButtonObject(root):
	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			def_object = g_ns.get("{http://www.evolus.vn/Namespace/Pencil}def")
			if(IsButton(def_object)):
				for text_ns in g_ns.findall("text_link:text", namespaces):
					matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")
					name = "MainMenu" + text_ns.text + "ButtonObject"
					ButtonObject_1 = ButtonObject(int(float(matrix_str[4])), int(float(matrix_str[5])), 1, 1, name)

				AddButtonObjectToJSON(ButtonObject_1)

def AddButtonObjectToJSON(ButtonObject):
	print("b")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"][0]["Buttons"].append({"RX": ButtonObject.x, "RY": ButtonObject.y, "Width": ButtonObject.width, "Height" : ButtonObject.height, "Name" : ButtonObject.name})

	with open('base.json', 'w') as f:
		json.dump(data, f)


def IsButton(g_def):
	if(g_def == "Evolus.Sketchy.GUI:button"):
		return True
	else:
		return False

def IsText(g_def):
	if(g_def == "Evolus.Common:PlainTextV2"):
		return True
	else:
		return False


main()