from PyQt5.QtWidgets import QMainWindow, QLCDNumber, QDesktopWidget, QPushButton, QSizePolicy
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5 import QtGui
from interface.mainWindowUi import Ui_mainWindow
from mastermind import MasterMind


class MainWindow(QMainWindow, Ui_mainWindow):
    def __init__(self, codeLength):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        self.codeLength = codeLength

        self.masterMind = MasterMind(self.codeLength)
        self.timer = QTimer()
        self.countDownTime = 5

        self.codeEdit.setMaxLength(self.codeLength)
        self.codeEdit.setText("")
        self.codeLabel.setText("    ")
        self.lcdNumbers.display(0)
        self.lcdPositions.display(0)

        self.codeEdit.returnPressed.connect(self.enterCode)
        self.codeEdit.setDisabled(True)

        self.buttonStart = QPushButton(self.centralwidget)
        self.setStart()

    def setStart(self):
        sizePolicy = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.buttonStart.sizePolicy().hasHeightForWidth())
        self.buttonStart.setSizePolicy(sizePolicy)
        self.buttonStart.setMinimumSize(QSize(0, 200))

        palette = self.buttonStart.palette()
        brush = QtGui.QBrush(QtGui.QColor(179, 2, 0))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Highlight, brush)
        self.buttonStart.setPalette(palette)

        font = QtGui.QFont()
        font.setFamily("BankGothic Md BT")
        font.setPointSize(60)
        self.buttonStart.setFont(font)
        self.buttonStart.setDefault(True)
        self.buttonStart.setText("DEFUSE")

        self.verticalLayout_2.addWidget(self.buttonStart)

        self.timeEdit.hide()
        self.buttonStart.show()

        self.buttonStart.clicked.connect(self.startCountDown)

    def startCountDown(self):
        self.buttonStart.setText(str(self.countDownTime))

        self.timer.timeout.connect(self.countDownTick)
        self.timer.start(1000)

    def countDownTick(self):
        self.countDownTime -= 1
        if self.countDownTime == 0:
            self.startMasterMind()
        else:
            self.buttonStart.setText(str(self.countDownTime))

    def startMasterMind(self):
        self.buttonStart.hide()
        self.timeEdit.show()

        self.codeEdit.setDisabled(False)
        self.codeEdit.setFocus()

        self.timer.timeout.connect(self.timerTick)
        self.timer.start(1000)

    def timerTick(self):
        self.timeEdit.setTime(self.timeEdit.time().addSecs(1))

    def enterCode(self):
        code = self.codeEdit.text()
        if len(code) != self.codeLength:
            return
        self.codeEdit.setText("")
        self.codeLabel.setText(code)

        numbers, positions = self.masterMind.unlock(code)
        self.lcdNumbers.display(numbers)
        self.lcdPositions.display(positions)
