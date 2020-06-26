import time
import xml.etree.ElementTree as ET
import json


class MenuObject:
	def __init__(self, name):
		self.name = name

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def CreateMenuObject(root):
	for properties_ns in root.findall('p_link:Properties', namespaces):
		for property_ns in properties_ns.findall('p_link:Property', namespaces):
			if(property_ns.get("name") == "name"):
				global menu_name 
				menu_name = property_ns.text

				MenuObject_1 = MenuObject(property_ns.text)
				AddMenuObjectToJSON(MenuObject_1)

def AddMenuObjectToJSON(MenuObject):
	print("m")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"].append({"Type": "Menu", "Path": "insert_path", "Name": MenuObject.name, "X": 0, "Y": 0, "Width": 1920, "Height": 1080, "Buttons": [], "Text": []})

	with open('base.json', 'w') as f:
		json.dump(data, f)