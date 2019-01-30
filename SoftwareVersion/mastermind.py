import random


class MasterMind:
    def __init__(self, length):
        self.length = length
        self.code = None
        self.count = 0

        self.generateCode()

    def generateCode(self):
        self.code = ''
        for i in range(0, self.length):
            self.code += str(random.randint(1, 9))

    def unlock(self, userCode):
        theCodeList = list(self.code)
        goodNumbers = 0
        goodPositions = 0

        for i in range(len(self.code)):
            if userCode[i] == self.code[i]:
                goodPositions += 1
            if userCode[i] in theCodeList:
                goodNumbers += 1
                theCodeList.remove(userCode[i])

        return goodNumbers, goodPositions
