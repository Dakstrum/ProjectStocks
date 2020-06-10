import time
import xml.etree.ElementTree as ET
import json

class TextObject:
	def __init__(self, idn, x, y, content):
		self.idn     = idn
		self.x       = x
		self.y       = y
		self.content = content

def main():
   				
	tree = ET.parse('menu.xml')
	root = tree.getroot()

	namespaces = {'p_link': 'http://www.evolus.vn/Namespace/Pencil',
				  'text_link': 'http://www.w3.org/2000/svg'}

	for content_ns in root.findall('p_link:Content', namespaces):
		for g_ns in content_ns.findall('text_link:g', namespaces):
			for text_ns in g_ns.findall("text_link:text", namespaces):
				for text_content_ns in text_ns.findall("text_link:tspan", namespaces):
					TextObject_1 = TextObject(g_ns.get("id"), 2, 3, text_content_ns.text)
					CreateTextJSON(TextObject_1)
					
					
	print("suc")
	time.sleep(500);

def CreateTextJSON(TextObject):
	print("s")
	with open('base.json') as f:
		data = json.load(f)

	data["Objects"][0]["Text"].append({"RX": 1, "RY": 1, "FontSize": 70, "Name" : TextObject.idn, "Path" : "assets/font/DanielLinssenM5/m5x7.ttf", "Content" : TextObject.content, "Color" : [14, 17, 19, 255]})

	with open('base.json', 'w') as f:
		json.dump(data, f)


main()