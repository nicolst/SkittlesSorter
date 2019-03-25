import time

import board
import busio

import adafruit_tcs34725

import RPi.GPIO as rGPIO

from PIL import Image

import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI

from PIL import Image

import math

WIDTH = 128
HEIGHT = 128
SPEED_HZ = 4000000

DC = 25
RST = 24
SPI_PORT = 0
SPI_DEVICE = 0

green=(11, 41, 6)
yellow=(31, 22, 3)
purple=(19, 15, 17)
orange=(62, 9, 3)
red=(65, 8, 6)

colors = {green: "green", yellow: "yellow", purple: "purple", orange: "orange", red: "red"}

# Create TFT LCD display class.
disp = TFT.ST7735(
            DC,
            rst=RST,
            spi=SPI.SpiDev(
                SPI_PORT,
                SPI_DEVICE,
                max_speed_hz=SPEED_HZ))

# Initialize display.
disp.begin()


rGPIO.setmode(rGPIO.BCM)
rGPIO.setup(4, rGPIO.OUT)


# Initialize I2C bus and sensor.
i2c = busio.I2C(board.SCL, board.SDA)
sensor = adafruit_tcs34725.TCS34725(i2c)

def color_dist(c1, c2):
    (r1,g1,b1) = c1
    (r2,g2,b2) = c2
    return math.sqrt((r1 - r2)**2 + (g1 - g2) ** 2 + (b1 - b2) **2)



# Main loop reading color and printing it every second.
while True:
    rGPIO.output(4, rGPIO.HIGH)
    time.sleep(0.06)
    # Read the color temperature and lux of the sensor too.
    color = sensor.color_rgb_bytes
    nearest_color = sorted(list(colors.keys()), key=lambda c: color_dist(c, color))[0]
    rGPIO.output(4, rGPIO.LOW)
    print('Color: ({0}, {1}, {2}), NEAREST: {3}'.format(*color, colors[nearest_color]))
    disp.clear((nearest_color[0]*10, nearest_color[1]*10, nearest_color[2]*10))

    disp.display()
    # Delay for a second and repeat.
    time.sleep(1.0)
