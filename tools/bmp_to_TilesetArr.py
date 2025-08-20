#bmp_to_TilesetArr.py

#this is for converting the bmp's made/gotten from aseprite into project-ready formats for tileset

from PIL import Image
import sys

def to_rgb565(pixels):

    rgb565_pixels = []
    
    for r, g, b in pixels:

        r5 = (r >> 3) & 0x1f
        g6 = (g >> 2) & 0x3f
        b5 = (b >> 3) & 0x1f

        rgb565 = (r5 << 11) | (g6 << 5) | b5

        rgb565_pixels.append(rgb565)

    return rgb565_pixels
    

# aseprite format to system-ready
def tileset_to_array(filepath, array_name, tile_len):
    try:
        # Ensure tile_len is an integer
        tile_len = int(tile_len)

        # Open the image and ensure it's in RGB format
        img = Image.open(filepath)
        if img.mode != "RGB":
            img = img.convert("RGB") # 3x8(pix-bytes) of true color

        bmp_width, bmp_height = img.size
        pixels = list(img.getdata())

        # Convert pixels to RGB-565 format
        rgb565_pixels = to_rgb565(pixels) #NOTE: at this point it's left to right top to bottom (no ordered tiles)

        tiles_wide = bmp_width // tile_len
        tiles_tall = bmp_height // tile_len
        num_tiles = tiles_wide * tiles_tall

        print(f"tiles wide: {tiles_wide}\n")
        print(f"tiles tall: {tiles_tall}\n")
        print(f"num tiles: {num_tiles}\n")

        # Tile processing: rearrange the data into tiles
        tile_array = []  # Final tile data
        bufPtr = 0              
        for i in range(tiles_tall): 
            bufPtr=(i*tile_len)*bmp_width 
            for j in range(tiles_wide): 
                for k in range(tile_len):  
                    start = bufPtr+(tile_len*j);
                    end = start+tile_len;
                    tile_array.extend(rgb565_pixels[start:end])
                    bufPtr += bmp_width
                bufPtr=(i*tile_len)*bmp_width 

        # Convert the tile data into a C-style array format for project use
        c_style_array = f"const uint16_t {array_name}[] = {{\n"
        for i in range(0,len(tile_array),tile_len):
            line = ", ".join(f"0x{val:04X}" for val in tile_array[i:i+tile_len])
            c_style_array += f"    {line},\n"
        c_style_array += "};\n"

        return c_style_array

    except Exception as e:
        print(f"Error: {e}")
        return None

def main():
    # Check arguments
    if len(sys.argv) < 4:
        print("Usage: python3 bmp_to_TilesetArr.py <bmp_filepath> <array_name> <tile_len>")
        return
    filepath = sys.argv[1]
    array_name = sys.argv[2]
    tile_len = sys.argv[3]

    # Generate the C-style array and write it to a header file
    c_style_array = tileset_to_array(filepath, array_name, tile_len)

    if c_style_array:
        asset_file = f"{array_name}.cpp"
        with open(asset_file, "w") as f:
            f.write(c_style_array)
        print(f"Generated .cpp file saved to {asset_file}")
    else:
        print("Failure to create file")

if __name__ == "__main__":
    main()

