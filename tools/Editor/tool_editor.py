from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QMenuBar, QMenu, QMainWindow, QFileSystemModel, QTreeView, QListWidget, QLabel, QLineEdit, QGroupBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QDir
import json

def main():

    app    = QApplication([])
    window = MainWindow()
    app.exec_()

def GetProperty(dict, property):

    try:
        return dict[property]
    except:
        return None

class MainWindow(QMainWindow):

    def __init__(self):

        super().__init__()

        self.filepath        = "drawobjects.json"
        self.menu_config_obj = MenuConfig(self.filepath)
        self.SetMainWindow()
        self.SetWindowContainer()
        self.show()

    def SetMainWindow(self):

        self.setWindowTitle("Game of Stocks Editor")
        self.setMinimumHeight(720)
        self.setMinimumWidth(1280)

    def SetWindowContainer(self):

        self.window_container = QWidget()
        self.window_layout    = QHBoxLayout()
        self.menu_attributes  = None
        self.SetMenuBar()
        self.SetMenuList()
        self.SetPixmapLabelDefaults()
        self.setCentralWidget(self.window_container)
        self.window_container.setLayout(self.window_layout)

    def SetMenuBar(self):

        self.menubar   = self.menuBar()
        self.file_menu = self.menubar.addMenu("File")
        self.file_menu.addAction("Save").triggered.connect(self.menu_config_obj.SaveMenuConfig)

    def SetMenuList(self):

        self.menu_list = QListWidget()
        for menu in self.menu_config_obj.menus:
            self.menu_list.addItem(menu["Name"])

        self.menu_list.setMinimumWidth(200)
        self.menu_list.setMaximumWidth(200)
        self.menu_list.itemClicked.connect(self.MenuItemClicked)
        self.window_layout.addWidget(self.menu_list)

    def SetPixmapLabelDefaults(self):

        self.pixmap_label = QLabel()
        self.pixmap_label.setScaledContents(True)
        self.window_layout.addWidget(self.pixmap_label)

    def MenuItemClicked(self, item):

        self.window_layout.removeWidget(self.pixmap_label)
        clicked_menu = None
        for menu in self.menu_config_obj.menus:
            if menu["Name"] == item.text():
                clicked_menu = menu
                break

        self.pixmap_label.setPixmap(QPixmap(menu["Path"]))
        self.window_layout.addWidget(self.pixmap_label)
        self.MenuAttributes(clicked_menu)

    def MenuAttributes(self, menu):

        menu_list_idx = -1
        for i in range(self.menu_list.count()):
            if self.menu_list.item(i).text() == menu["Name"]:
                menu_list_idx = i
                break

        if self.menu_attributes is not None:
            self.menu_attributes.SetNewMenu(menu, self.menu_list)
        else:
            self.menu_attributes = MenuAttributesBox(menu)
            self.window_layout.addWidget(self.menu_attributes.GetGroup())

class MenuAttributesBox(QGroupBox):

    def __init__(self, menu):

        super().__init__()
        self.menu            = menu
        self.group           = QGroupBox("Attributes")
        self.layout          = QVBoxLayout()
        self.SetGroupElements()
        self.SetLayout()
        self.SetGroup()

    def SetGroupElements(self):

        self.name      = QLineAssociation(self.menu, "Name")
        temp_buttons   = GetProperty(self.menu, "Buttons")
        temp_text      = GetProperty(self.menu, "Text")
        temp_textboxes = GetProperty(self.menu, "Textbox")

    def SetLayout(self):

        self.layout.addWidget(self.name)

    def SetGroup(self):

        self.group.setLayout(self.layout)
        self.group.setMinimumWidth(200)
        self.group.setMaximumWidth(200)

    def SetNewMenu(self, menu):

        self.menu = menu
        self.RemoveGroupElements()
        self.SetGroupElements()
        self.SetLayout()

    def RemoveGroupElements(self):

        self.layout.removeWidget(self.name)
        self.name.StoreText()
        self.name.deleteLater()

    def GetGroup(self):

        return self.group


class QLineAssociation(QLineEdit):

    def __init__(self, obj, prop):

        super().__init__()

        self.insert(obj[prop])
        self.obj      = obj
        self.property = prop

    def StoreText(self):

        self.obj[self.property] = self.text()


class MenuConfig():

    def __init__(self, filepath):

        self.filepath    = filepath
        self.menu_config = self.LoanMenuConfig()
        self.SetMenus()

    def SetMenus(self):

        self.menus = []
        for config in self.menu_config["Objects"]:
            if config["Type"] == "Menu":
                self.menus.append(config)


    def LoanMenuConfig(self):

        with open(self.filepath, "rb") as json_file:
            return json.load(json_file)

    def SaveMenuConfig(self):

        with open(self.filepath, "w+") as json_file:
            json.dump(self.menu_config, json_file, ensure_ascii=False, indent=4)

main()