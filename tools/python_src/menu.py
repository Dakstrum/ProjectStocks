import time
import xml.etree.ElementTree as ET
import json
import re

from python_src import menu

class MenuObject:
	def __init__(self, path, name, x, y, width, height):
		self.path   = path
		self.name   = name
		self.x      = x
		self.y      = y
		self.width  = width
		self.height = height

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def CreateMenuObject(root):
	for properties_ns_2 in root.findall('p_link:Properties', namespaces):
		for property_ns_2 in properties_ns_2.findall('p_link:Property', namespaces):
			if(property_ns_2.get("name") == "note"):
				note = property_ns_2.text

	for properties_ns in root.findall('p_link:Properties', namespaces):
		for property_ns in properties_ns.findall('p_link:Property', namespaces):
			if(property_ns.get("name") == "name"):
				global menu_name 
				menu_name = property_ns.text
				print("BEFORE TEXTTTTTTTTT: " + property_ns.text)

	re_path   = re.search(r'Path:\s*([^<]*)', note)[1]
	re_x      = re.search(r'X:\s*([^<]*)', note)[1]
	re_y      = re.search(r'Y:\s*([^<]*)', note)[1]
	re_width  = re.search(r'Width:\s*([^<]*)', note)[1]
	re_height = re.search(r'Height:\s*([^<]*)', note)[1]
	
	print("AFTER TEXTTTTTTTTT: " + property_ns.text)

	MenuObject_1 = MenuObject(remove(re_path), menu.menu_name, remove(re_x), remove(re_y), remove(re_width), remove(re_height))
	AddMenuObjectToJSON(MenuObject_1)

def AddMenuObjectToJSON(MenuObject):
	print("m")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"].append({"Type": "Menu", "Path": MenuObject.path, "Name": MenuObject.name, "X": MenuObject.x, "Y": MenuObject.y, "Width": MenuObject.width, "Height": MenuObject.height, "Buttons": [], "Text": []})

	with open('base.json', 'w') as f:
		json.dump(data, f)


def remove(string): 
    return string.replace(" ", "") 
