import time
import xml.etree.ElementTree as ET
import json

from python_src import button
from python_src import text
from python_src import menu

def main():
   				
	tree = ET.parse('mainmenu.xml')
	root = tree.getroot()

	menu.CreateMenuObject(root)
	button.GetButtonObject(root)
	text.GetTextObject(root)	
	print("suc")
	time.sleep(500);

main()