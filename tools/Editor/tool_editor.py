from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QMenuBar, QMenu, QMainWindow, QFileSystemModel, QTreeView, QListWidget, QLabel
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QDir
import json

def main():

    app    = QApplication([])
    window = MainWindow()
    app.exec_()

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