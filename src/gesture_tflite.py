#!/usr/bin/python3
import serial

ser = serial.Serial(
	port="/dev/ttyACM0",
	baudrate=9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=0
)

f = open("deteccion.csv", "w+")
f.write("punch,like,circle,detected\n")

print("Connected to: " + ser.portstr)
line = []

while True:
	for c in ser.read():
		c = chr(c)
		line.append(c)
		if c == "\n":
			print("Line: " + "".join(line))
			print(len(line))
			if len(line) >= 27 and len(line) <= 29:
				string = "".join(line)
				f.write(string)
			line=[]