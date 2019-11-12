from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QMenuBar, QMenu, QMainWindow, QFileSystemModel, QTreeView
from PyQt5.QtCore import QDir

app              = None
window           = None
windows          = []
window_container = None
menubar          = None

def main():

    global app
    global window
    global menubar

    InitQTAppAndWindow()
    InitMenuBar()
    InitDefaultLayout()

    window.show()
    app.exec()

def InitQTAppAndWindow():

    global app
    global window
    global window_container

    app              = QApplication([])
    window           = QMainWindow()
    window_container = QWidget()

    window.setWindowTitle("Game of Stocks Editor")
    window.setMinimumHeight(720)
    window.setMinimumWidth(1280)
    window.setCentralWidget(window_container)

def InitDefaultLayout():

    global window_container

    layout = QVBoxLayout()
    layout.addWidget(QPushButton('Top'))
    layout.addWidget(QPushButton('Bottom'))
    window_container.setLayout(layout)

def InitMenuBar():

    global menubar
    global window

    menubar   = window.menuBar()
    file_menu = menubar.addMenu("File")
    file_menu.addAction("Open").triggered.connect(OpenCallback)
    file_menu.addAction("Open folder").triggered.connect(OpenFolderCallback)
    file_menu.addAction("Save").triggered.connect(SaveCallback)
    file_menu.addAction("Save as").triggered.connect(SaveAsCallback)

def OpenCallback():

    global windows

    open_window = QWidget()
    open_window.resize(1280, 720)
    open_window.setWindowTitle("File explorer")

    model       = QFileSystemModel()
    model.setRootPath(QDir.currentPath())
    tree        = QTreeView(open_window)
    tree.setModel(model)
    tree.setRootIndex(model.index(QDir.currentPath()))
    tree.setMinimumSize(1280, 720)
    tree.clicked.connect(OpenFileCallback)

    open_window.setMinimumHeight(720)
    open_window.setMinimumWidth(1280)
    open_window.show()
    windows.append(open_window)

def OpenFileCallback(index):

    print(index)

def OpenFolderCallback():
    return

def SaveCallback():
    return

def SaveAsCallback():
    return

main()