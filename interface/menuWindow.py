from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtCore import pyqtSignal
from interface.menuWindowUi import Ui_menuWindow


class MenuWindow(QMainWindow, Ui_menuWindow):
    historySignal = pyqtSignal(int)

    def __init__(self):
        super(MenuWindow, self).__init__()
        self.setupUi(self)
