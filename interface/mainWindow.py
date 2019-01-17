from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtCore import pyqtSignal
from interface.mainWindowUi import Ui_mainWindow


class MainWindow(QMainWindow, Ui_mainWindow):
    historySignal = pyqtSignal(int)

    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
