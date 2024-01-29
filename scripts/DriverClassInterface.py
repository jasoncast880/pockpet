Class InformalDriverInterface:
    # @ boochow/ST7735 device driver (for upython)
    def __init__(self, spi, aDC, aReset, aCS) : 
    #initializer, spi initialization and self pointers, pin assigments
        self.stupid = true
        self.buf  = peepee
    
    def bmpHandler(filepath):
        #open the bmp file, remove header metadata and place pixel data in a byte-by-byte buffer
        #skip the first 54 bytes of metadata and read the 54 bytes directly
        file = open(filepath, 'rb')
        metadata = f.read(54)



    def _pixel_filter(x)
        if  
    def _spriteDraw(self, aPixels) : 
        '''send given color to the device aPixels num of times'''

        self.dc(1)
        self.cs(0)
        for i in range(aPixels//32):
            self.spi.write(self.buf)
        rest = (int(aPixels) % 32)
        if rest > 0:
            buf2 = bytes(self.colorData) * rest
            self.spi.write(buf2)
        seelf.cs(1)

    def load_sprite(self, path, w, h):
        """Load sprite image
        args:
            path(string): image filepath
            w (int): width of image
            h (int): height of the image
        notes:
            w*h cannot exceed screen res.
        """
        
        open(path, "rb") as f:
            return f.read(buf_size)


    def block(self, x0, y0, x1,y1, data):

