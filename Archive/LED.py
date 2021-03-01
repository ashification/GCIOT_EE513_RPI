import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(6,GPIO.OUT)
GPIO.setup(13,GPIO.OUT)
GPIO.setup(19,GPIO.OUT)
GPIO.setup(26,GPIO.OUT)

print "Red LED on"
GPIO.output(6,GPIO.HIGH)
time.sleep(1)
print "Red LED Off"
GPIO.output(6,GPIO.LOW)


print "Orange LED on"
GPIO.output(13,GPIO.HIGH)
time.sleep(1)
print "Orange LED Off"
GPIO.output(13,GPIO.LOW)

print "Yellow LED on"
GPIO.output(19,GPIO.HIGH)
time.sleep(1)
print "Yellow LED Off"
GPIO.output(19,GPIO.LOW)


print "Blue LED on"
GPIO.output(26,GPIO.HIGH)
time.sleep(1)
print "Blue LED Off"
GPIO.output(26,GPIO.LOW)



