#original from boochow repo
from ST7735 import TFT,TFTColor
from machine import SPI,Pin
spi = SPI(0, baudrate=40000000, polarity=0, phase=0)
pin_DC = 13
pin_RESET = 12
pin_CS = 17
tft=TFT(spi, pin_DC, pin_RESET, pin_CS)
tft.initr()
tft.rgb(True)
tft.fill(TFT.BLACK)

import utime

def tftImage(filepath):
    f=open(filepath, 'rb')
    if f.read(2) == b'BM':  #header
        print("I am a bmp file")
        dummy = f.read(8) #file size(4), creator bytes(4)
        offset = int.from_bytes(f.read(4), 'little')
        hdrsize = int.from_bytes(f.read(4), 'little')
        width = int.from_bytes(f.read(4), 'little')
        height = int.from_bytes(f.read(4), 'little')
        if int.from_bytes(f.read(2), 'little') == 1: #planes must be 1
            depth = int.from_bytes(f.read(2), 'little')
            if depth == 24 and int.from_bytes(f.read(4), 'little') == 0:#compress method == uncompressed
                print("Image size:", width, "x", height)
                rowsize = (width * 3 + 3) & ~3
                if height < 0:
                    height = -height
                    flip = False
                else:
                    flip = True
                w, h = width, height
                if w > 128: w = 128
                if h > 160: h = 160
                tft._setwindowloc((0,0),(w - 1,h - 1))
                for row in range(h):
                    if flip:
                        pos = offset + (height - 1 - row) * rowsize
                    else:
                        pos = offset + row * rowsize
                    if f.tell() != pos:
                        dummy = f.seek(pos)
                    for col in range(w):
                        bgr = f.read(3)
                        tft._pushcolor(TFTColor(bgr[2],bgr[1],bgr[0]))
    spi.deinit()


tftImage('assets/oreo-test-3.bmp')

utime.sleep(5)

tftImage('assets/bg-wall-test.bmp')
