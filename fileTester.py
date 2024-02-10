from ST7735 import TFT,TFTColor
from machine import SPI, Pin
import time
spi = SPI(0, baudrate=40000000, polarity=0, phase=0)
pin_DC = 13
pin_RESET = 12
pin_CS = 17
tft=TFT(spi, pin_DC, pin_RESET, pin_CS)
tft.initr()
tft.rgb(True)
tft.fill(TFT.BLACK)

class Sprite(object) :
    def __init__(self, path, w,h, x,y, display,rot) :
        ''' aSize is (w,h); aPos is (x,y) '''
        self.rot = rot
        self.w = w
        self.h = h
        self.x = x
        self.y = y
        self.display = display
        self.buf  = display.load_sprite(path, (w, h))
        display.rotation(self.rot)
    def animation1 (self, path1, path2, path3, path4)
    def draw(self) :
        print('drawing now')
        self.display.draw_sprite(self.buf,(self.w,self.h),(self.x,self.y))
        
def testOreo() :
    #test oreo on a background; as of jan/24, no bg-control
    oreoSprite = Sprite('assets/poopystinky.bmp',75,90,40,40,tft,3)
    oreoSprite.draw()
    print('finished')
    
def testSussy() :
    susSprite = Sprite('assets/dotstest2-2x4.bmp',2,4,10,10,tft)
    susSprite.draw()
    
def bmpTester(filepath):
    f=open(filepath, 'rb')
    dumdum = f.read(54) #read all the metadata headers
    
    w = 128
    h = 160
    rowsize = (w*3+3) & ~3
    
    tft._setwindowloc((0,0), (w-1,h-1))
    for row in range (h) :
        pos = row * rowsize
        if f.tell() != pos:
            dummy = f.seek(pos)
        for col in range(w):
            bgr = f.read(3)
            tft._pushcolor(TFTColor(bgr[2],bgr[1],bgr[0]))
    spi.deinit()


#bmpTester('assets/blue.bmp')
#time.sleep_ms(4000)
testOreo() 