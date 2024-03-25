##~---------------------------------------------------------------------------##
##                               *       +                                    ##
##                         '                  |                               ##
##                     ()    .-.,="``"=.    - o -                             ##
##                           '=/_       \     |                               ##
##                        *   |  '=._    |                                    ##
##                             \     `=./`,        '                          ##
##                          .   '=.__.=' `='      *                           ##
##                 +                         +                                ##
##                      O      *        '       .                             ##
##                                                                            ##
##  File      : make-img.py                                                   ##
##  Project   : dos-exp                                                       ##
##  Date      : 2024-03-25                                                    ##
##  License   : See project's COPYING.TXT for full info.                      ##
##  Author    : mateus.digital <hello@mateus.digital>                         ##
##  Copyright : mateus.digital - 2024                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##


## -----------------------------------------------------------------------------
import os;
import os.path;
import sys;
from PIL import Image

## -----------------------------------------------------------------------------
def read_image_pixels(image_path):
    img = Image.open(image_path);
    width, height = img.size;
    pixels = list(img.getdata());
    img.close();
    return width, height, pixels;


## -----------------------------------------------------------------------------
input_path      = sys.argv[1];
path_components = os.path.splitext(input_path);
output_path     = path_components[0] + ".img";

width, height, pixels = read_image_pixels(input_path);

print("Input Path: ", input_path);
print("Output Path: ", output_path);
print("Image dimensions:", width, "x", height)
print("Number of pixels:", len(pixels))

width_bytes  = width.to_bytes(2, byteorder='little')
height_bytes = height.to_bytes(2, byteorder='little')
pixel_bytes  = bytes(pixels)

f = open(output_path, "wb");
f.write(width_bytes);
f.write(height_bytes);
f.write(pixel_bytes);
f.close();