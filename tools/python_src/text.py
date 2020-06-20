import time
import xml.etree.ElementTree as ET
import json
import re

class TextObject:
	def __init__(self, name, x, y, fontsize, content):
		self.name     = name
		self.x        = x
		self.y        = y
		self.fontsize = fontsize
		self.content  = content

namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
			 'text_link': 'http://www.w3.org/2000/svg'}

def GetTextObject(root):
	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			def_object = g_ns.get("{http://www.evolus.vn/Namespace/Pencil}def")
			if(IsText(def_object)):
				CreateTextObject(g_ns)
				
def CreateTextObject(g_ns):
	for text_ns in g_ns.findall("text_link:text", namespaces):
		style = text_ns.get("style")
		for text_content_ns in text_ns.findall("text_link:tspan", namespaces):
			matrix_str = g_ns.get("transform").replace("matrix(", "").replace(")", "").split(",")
			name = "MainMenu" + text_content_ns.text + "TextObject"
			TextObject_1 = TextObject(name, int(float(matrix_str[4])), int(float(matrix_str[5]) - 44), GetFontSize(style), text_content_ns.text)
			AddTextObjectToJSON(TextObject_1)

def GetFontSize(style):
	fontsize = re.search(r'font-size:\s*(\d+)', style)[1] 
	return fontsize

def AddTextObjectToJSON(TextObject):
	print("t")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"][0]["Text"].append({"RX": TextObject.x, "RY": TextObject.y, "FontSize": 70, "Name" : TextObject.name, "Path" : "assets/font/DanielLinssenM5/m5x7.ttf", "Content" : TextObject.content, "Color" : [14, 17, 19, 255]})

	with open('base.json', 'w') as f:
		json.dump(data, f)

def IsText(g_def):
	if(g_def == "Evolus.Common:PlainTextV2"):
		return True
	else:
		return False