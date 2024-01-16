with open('your_image.bmp', 'rb') as f:
    #Read the first 16 bytest to examine the BMP file header
    header_bytes = f.read(14)

    #print the hex rep of the header bytes
    print("BMP File Header Bytes (hex):", ' '.join(byte, '0.2x') for byte in header_bytes))
    
