#!/usr/bin/python3
import serial
import sys

ser = serial.Serial(
	port="/dev/ttyACM0",
	baudrate=9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=0
)

file = sys.argv[1]

f = open(file, "w+")
f.write("timestamp,aX,aY,aZ,gX,gY,gZ\n")

print("Connected to: " + ser.portstr)
line = []

while True:
	for c in ser.read():
		c = chr(c)
		line.append(c)
		if c == "\n":
			print("Line: " + "".join(line))
			print(len(line))
			if len(line) >= 45 and len(line) <= 63:
				string = "".join(line)
				f.write(string)
			line=[]