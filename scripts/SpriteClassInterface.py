import ST7735
class LayeredSprite(object): 
#will go in the programscript file, NOT the driver. Implements the driver class via lib import    
    def __init__(self, path, w, h, screen_width, screen_height, layer, display)
    """Initialize sprite
    Args:
        path(string): path of sprite image !!! MUST BE A BMP w. no transparent background !!!
        w, h, screen_width, screen_height (int): width/height of sprite, screen
        layer(int) : layer of sprite, with highest no. order as the top layer
        display (ST7735) : display object
    """

        self.buf = display.load_sprite(path, w, h)
        self.w = w
        self.h = h
        self.screen_width = screen_width
        self.screen_height = screen_height
        self.layer = layer
        self.display = display
    
        #positional data
        self.x = screen_width // 2
        self.y = screen_height // 2
        self.prev_x = self.x
        self.prev_y = self.y

    def draw(self) :
        x = self.x
        y = self.y
        w = self.w
        h = self.h

        self.display.draw_sprite(self.buf, x, y, w, h)

def test():
    """layered sprites"""
    try:
        #baud and etc
        spi = SPI(0, ...)
        tft = TFT(spi, etc...)
        tft.fill(TFT.BLACK)

        background = LayeredSprite('assets/1.bmp, (128,160), (128,160), 0, tft)
        sprite = LayeredSprite('assets/2.bmp, (128, 160), (128,160), 1, tft)
        
        spriteMap = {background, sprite}
        #attempt to draw all of the sprites
        timer = ticks_us()
        for sprite in spriteMap
            sprite.draw()
        
        timer_dif = 33333-ticks_diff(ticks_us(), timer)
        if timer_dif > 0:
            sleep_us(timer_dif)

    except KeyboardInterrupt:
        #display.cleanup method

        #load the sprite via constructor; contructor implements load_sprite driver method


