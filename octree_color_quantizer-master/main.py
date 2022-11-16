#!/usr/bin/env python
#-*- coding: utf8 -*-

from PIL import Image
from color import Color
from octree_quantizer import OctreeQuantizer


def main():
    image = Image.open(r"C:\Users\Chuber\Documents\Uberto\UNSA\3er\2do_semestre\Estructuras_de_Datos_Avanzadas\octree\octree_color_quantizer-master\11.png")
    pixels = image.load()
    width, height = image.size

    octree = OctreeQuantizer()

    # add colors to the octree
    for j in range(height):
        for i in range(width):
            octree.add_color(Color(*pixels[i, j]))

    # 256 colors for 8 bits per pixel output image
    palette = octree.make_palette(256)

    # create palette for 256 color max and save to file
    palette_image = Image.new('RGB', (16, 16))
    palette_pixels = palette_image.load()
    for i, color in enumerate(palette):
        palette_pixels[i % 16, i / 16] = (int(color.red),int(color.green),int(color.blue))
    palette_image.save(r"C:\Users\Chuber\Documents\Uberto\UNSA\3er\2do_semestre\Estructuras_de_Datos_Avanzadas\octree\octree_color_quantizer-master\11_2.png")

    # save output image
    out_image = Image.new('RGB', (width, height))
    out_pixels = out_image.load()
    for j in range(height):
        for i in range(width):
            index = octree.get_palette_index(Color(*pixels[i, j]))
            color = palette[index]
            out_pixels[i, j] = (int(color.red), int(color.green), int(color.blue))
    out_image.save(r"C:\Users\Chuber\Documents\Uberto\UNSA\3er\2do_semestre\Estructuras_de_Datos_Avanzadas\octree\octree_color_quantizer-master\11_out.png")


if __name__ == '__main__':
    main()
