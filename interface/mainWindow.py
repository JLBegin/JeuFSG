from PyQt5.QtWidgets import QMainWindow, QPushButton, QSizePolicy, QTableWidgetItem, QHeaderView
from PyQt5.QtCore import QSize, Qt, QTimer, QThreadPool, pyqtSignal
from PyQt5 import QtGui
from interface.mainWindowUi import Ui_mainWindow
from mastermind import MasterMind
from threadWorker import Worker
import serial


class MainWindow(QMainWindow, Ui_mainWindow):
    codeSignal = pyqtSignal()
    startSignal = pyqtSignal()

    def __init__(self, codeLength):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        self.codeLength = codeLength
        self.ser = serial.Serial(port='COM4', baudrate=124380, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                                 stopbits=serial.STOPBITS_ONE)
        self.threadPool = QThreadPool()

        self.serialSetCode = Worker(self.setCode)
        self.serialTryCode = Worker(self.waitCode)

        self.masterMind = MasterMind(self.codeLength)
        print("CODE: ", self.masterMind.code)
        self.history = []
        self.timer = QTimer()
        self.countDownTime = 5
        self.count = 0

        self.codeEdit.setMaxLength(20)
        sample = "_ " * self.codeLength
        self.codeEdit.setText(sample[:-1])
        self.codeLabel.setText(" " * self.codeLength*2)
        self.lcdNumbers.display(0)
        self.lcdPositions.display(0)

        self.codeEdit.returnPressed.connect(self.enterCode)
        self.codeEdit.setDisabled(True)

        self.buttonStart = QPushButton(self.centralwidget)
        self.setStart()
        self.initHistory()

        self.codeSignal.connect(self.enterCode)
        self.startSignal.connect(self.startCountDown)
        self.threadPool.start(self.serialSetCode)

    def setStart(self):
        sizePolicy = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.buttonStart.sizePolicy().hasHeightForWidth())
        self.buttonStart.setSizePolicy(sizePolicy)
        self.buttonStart.setMinimumSize(QSize(0, 200))

        palette = self.buttonStart.palette()
        brush = QtGui.QBrush(QtGui.QColor(179, 179, 2))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Highlight, brush)
        self.buttonStart.setPalette(palette)

        font = QtGui.QFont()
        font.setFamily("BankGothic Md BT")
        font.setPointSize(48)
        self.buttonStart.setFont(font)
        self.buttonStart.setDefault(True)
        self.buttonStart.setText("Waiting for connection...")

        self.verticalLayout_2.addWidget(self.buttonStart)

        self.timeEdit.hide()
        self.buttonStart.show()

        # self.buttonStart.clicked.connect(self.startCountDown)

    def initHistory(self):
        self.tableWidget.setColumnCount(3)
        self.tableWidget.setRowCount(20)
        self.tableWidget.scrollToBottom()

        header = self.tableWidget.horizontalHeader()
        header.setSectionResizeMode(0, QHeaderView.Stretch)
        header.setSectionResizeMode(1, QHeaderView.Stretch)
        header.setSectionResizeMode(2, QHeaderView.Stretch)

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

        self.codeEdit.setMaxLength(self.codeLength)
        self.codeEdit.setText("")
        self.codeEdit.setDisabled(False)
        self.codeEdit.setFocus()

        self.timer.timeout.connect(self.timerTick)
        self.timer.start(1382)

        self.threadPool.start(self.serialTryCode)

    def timerTick(self):
        self.timeEdit.setTime(self.timeEdit.time().addSecs(1))

    def waitStart(self):
        while True:
            print("Waiting for bomb start defusing")
            s1 = self.ser.read()
            if s1.decode('ASCII') == 'S':
                print("Start defuse countdown")
                self.startSignal.emit()
                return
            else:
                continue

    def setCode(self, statusSignal=None):
        print("Waiting for code setup verification")
        while True:
            s1 = self.ser.read()
            if s1.decode('ASCII') == 'N':
                print("Entering password on bomb")
                isCorrect = self.waitSetCode()
                if isCorrect:
                    self.waitStart()
                    return
                continue
            else:
                continue

    def waitSetCode(self):
        code = []
        while True:
            number = self.waitNumber()
            if number != '*':
                code.append(number)
            else:
                print("Code entered : {} VS initCode: {}".format("".join(code), self.masterMind.code))
                if "".join(code) != self.masterMind.code:
                    print("Code is NOT the correct")
                    return 0
                else:
                    print("Code is correct")
                    return 1

    def waitCode(self, statusSignal=None):
        code = []
        while True:
            number = self.waitNumber()
            if number != '*':
                code.append(number)
                self.codeEdit.setText(''.join(code))
            else:
                self.codeEdit.setText(''.join(code))
                self.codeSignal.emit()
                code = []

    def waitNumber(self):
        while True:
            s1 = self.ser.read()
            if s1.decode('ASCII') == 'U' or s1.decode('ASCII') == 'N':
                number = self.ser.read().decode('ASCII')
                print(number)
                return number
            else:
                continue

    def enterCode(self):
        code = self.codeEdit.text()
        if len(code) != self.codeLength:
            return
        try:
            int(code)
        except TypeError:
            self.codeEdit.setText("")
            return

        self.count += 1
        self.codeEdit.setText("")
        self.codeLabel.setText(code)
        self.codeLabel.setVisible(True)

        numbers, positions = self.masterMind.unlock(code)
        self.lcdNumbers.display(numbers)
        self.lcdPositions.display(positions)

        if positions == self.codeLength:
            self.success()

        self.history.append([numbers, code, positions])
        self.refreshHistory()

    def refreshHistory(self):
        for i, line in enumerate(reversed(self.history)):
            for j, info in enumerate(line):
                item = QTableWidgetItem(str(info))
                item.setTextAlignment(Qt.AlignHCenter)
                self.tableWidget.setItem(19-i, j, item)

    def success(self):
        self.timer.timeout.disconnect()
        time = self.timeEdit.text()

        palette = self.buttonStart.palette()
        brush = QtGui.QBrush(QtGui.QColor(2, 179, 0))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Highlight, brush)
        self.buttonStart.setPalette(palette)

        self.buttonStart.setText("Defused in {}\n with {} tries".format(time, self.count))

        self.codeEdit.setMaxLength(30)
        self.codeEdit.setText("MASTERMIND")

        self.timeEdit.hide()
        self.buttonStart.show()
