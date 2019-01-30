from PyQt5.QtWidgets import QMainWindow
from interface.menuWindowUi import Ui_menuWindow
from interface.mainWindow import MainWindow


class MenuWindow(QMainWindow, Ui_menuWindow):
    def __init__(self):
        super(MenuWindow, self).__init__()
        self.setupUi(self)
        self.button4.clicked.connect(self.launchMasterMind)
        self.button5.clicked.connect(self.launchMasterMind)
        self.button6.clicked.connect(self.launchMasterMind)
        self.button7.clicked.connect(self.launchMasterMind)
        self.mainWindow = None

    def launchMasterMind(self):
        codeLength = int(self.sender().text()[0])

        if not self.mainWindow:
            self.mainWindow = MainWindow(codeLength, self)
        else:
            self.mainWindow.codeLength = codeLength
            self.mainWindow.reset()
        self.mainWindow.show()

        self.close()
