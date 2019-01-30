import serial

ser = serial.Serial(port='COM4', baudrate=124380, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)

# REAL CODE
# while 1:
#     try:
#         s = ser.read().decode('ASCII')
#     except Exception:
#         print("Carry On")
#
#     if s == 'U':
#         code = ser.read(1)
#         print(code.decode('ASCII'))

# # SERIAL TEST
# code = []
# while 1:
#     s = ser.read()
#     if s == b'\xaa':
#         print("UART Communication with the device is initialized.")
#     elif s.decode('ASCII') == 'U'or s.decode('ASCII') == 'S':
#         pass
#     elif s.decode('ASCII') == '*':
#         print(''.join(code))
#         code = []
#     else:
#         print(s.decode('ASCII'))
#         code.append(s.decode('ASCII'))

# SERIAL TEST
code = []
while 1:
    s = ser.read()
    if s == b'\xaa':
        print("UART Communication with the device is initialized.")
    elif s.decode('ASCII') == 'U'or s.decode('ASCII') == 'S':
        s2 = ser.read()
        print(s2.decode('ASCII'))
        code.append(s2.decode('ASCII'))
    elif s.decode('ASCII') == '*':
        print(''.join(code))
        code = []


