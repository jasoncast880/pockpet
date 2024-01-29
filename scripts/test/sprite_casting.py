import machine
import ST7735
from machine import Pin,SPI
from random import random, seed
from random import random, seed, randint
from utime import sleep_us, ticks_cpu, ticks_us, ticks_diff

class spritebox(object):

    """box to hold sprite"""

    def __init__(self, screen_width, screen_heigh, size, display, color):
        """Initialize box.

        self.size = size
        self.w = screen_width
        self.h = screen_height
        self.display = display
        self.color = color

        seed(ticks_cpu())
