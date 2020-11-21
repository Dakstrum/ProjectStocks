import time
import xml.etree.ElementTree as ET
import json
import os
import glob
import pathlib

from python_src import button
from python_src import text
from python_src import menu

base_text = "{ "+ """  "Objects" """ + ": [] }"

def main():

	file = open("base.json", "w") 
	file.write(base_text) 
	file.close() 

	path = os.getcwd() 
	files = os.listdir(path)
	files_xml = [i for i in files if i.endswith('.xml')]

	for file in files_xml:
		tree = ET.parse(file)
		root = tree.getroot()

		menu.CreateMenuObject(root)
		button.GetButtonObject(root)
		text.GetTextObject(root)	

	print("suc")
	time.sleep(500);
 
main()