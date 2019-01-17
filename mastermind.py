import random


def secretCode(length):
    secretCode = ''
    for i in range(0, length):
        secretCode += str(random.randint(1, 9))
    return secretCode


def userGuess(length):
    while True:
        userCode = input('Please guess a {} digit number:'.format(length))
        if len(userCode) != length:
            print('Wrong length of number.')
        elif '0' in userCode:
            print('Cannot use zero')
        else:
            return userCode


def main():
    numberOfDigits = int(input("Enter desired code length: "))
    theCode = secretCode(numberOfDigits)
    count = 0

    while True:
        userCode = userGuess(numberOfDigits)
        theCodeList = list(theCode)

        goodNumbers = 0
        goodPositions = 0
        if userCode == theCode:
            print('MASTERMIND. In ' + str(count+1) + ' turns.')
            break
        else:
            count += 1
            if count == 15:
                print('GAME OVER. It was ' + theCode)
                break

            for i in range(len(theCode)):
                if userCode[i] == theCode[i]:
                    goodPositions += 1
                if userCode[i] in theCodeList:
                    goodNumbers += 1
                    theCodeList.remove(userCode[i])
            print("|{} Numbers | {} Positions|".format(goodNumbers, goodPositions))


if __name__ == '__main__':
    main()
