from ST7735 import TFT, TFTColor
from machine import SPI, Pin

from random import random, seed
from utime import sleep_us, ticks_cpu, ticks_us, ticks_diff

spi = SPI(0,baudrate =40000000,polarity = 0,phase = 0)
pin_RESET = 12
pin_DC = 13
pin_CS = 17

tft = TFT(spi,pin_DC,pin_RESET,pin_CS)
tft.initr()
tft.rgb(True)
tft.fill(TFT.BLACK)

class Box(object):
    """bouncing box"""
    
    def __init__(self, screen_width, screen_height, size, display, color):
        
        self.size = size
        self.w = screen_width
        self.h = screen_height
        self.display = display
        self.color = color

        seed(ticks_cpu())
        r = random() * 10.0
        self.x_speed = 5.0 - r if r<5.0 else r-10.0
        r = random() * 10.0
        self.y_speed = 5.0 - r if r<5.0 else r-10.0

        self.x = self.w/2.0
        self.y = self.h/2.0
        self.prev_x = self.x
        self.prev_y = self.y

    def update_pos(self):
        x = self.x
        y = self.y
        size = self.size
        w = self.w
        h = self.h
        x_speed = abs(self.x_speed)
        y_speed = abs(self.y_speed)
        self.prev_x = x
        self.prev_y = y


        if x + size >= w-x_speed:
            self.x_speed = -x_speed
        elif x - size <= x_speed+1:
            self.x_speed = x_speed

        if y + size >= h-y_speed:
            self.y_speed = -y_speed
        elif y - size <= y_speed+1:
            self.y_speed = y_speed

        self.x = x+self.x_speed
        self.y = y+self.y_speed

    def draw(self):
        x = int(self.x)
        y = int(self.y)

        size = self.size
        prev_x = int(self.prev_x)
        prev_y = int(self.prev_y)
        
        #def fillrect( self, aStart, aSize, aColor ) :
        self.display.fillrect((prev_x - size, prev_y-size),(size, size), 0)
        self.display.fillrect((x-size, y-size),(size,size), self.color)

def test():
    try:
        tft.fill(TFT.FOREST)

        color = TFT.MAROON
        size = 12
        box = Box(148,175,size,tft,color)

        while True:
            timer = ticks_us()
            
            box.update_pos()
            box.draw()

            timer_dif = 33333 - ticks_diff(ticks_us(), timer)
            if timer_dif > 0:
                sleep_us(timer_dif)

    except KeyboardInterrupt:
        tft.fill(tft.WHITE)


test()
