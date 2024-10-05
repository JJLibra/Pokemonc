import numpy as np
from PIL.Image import Image

import ansi


class SmallConverter:
    """Generattes sprites where one unicode character represents two image
    pixels vertically and one horizontally"""

    def convert_image_to_unicode(self, image: Image) -> str:
        # maps one pixel to half a character
        upper_block = "▀"
        lower_block = "▄"
        empty_block = " "

        unicode_sprite = ""

        image_array = np.array(image)
        height, width, channels = image_array.shape

        # as we're mapping two pixels to one character, we need an even number
        # of pixels as height. So adding an empty row at the bottom for odd heights
        if height % 2:
            padded_array = np.zeros((height + 1, width, channels)).astype(np.uint8)
            padded_array[:height, :, :] = image_array
            height, width, channels = padded_array.shape
            image_array = padded_array

        reset_code = ansi.get_reset_escape_code()
        for i in range(0, height, 2):
            for j in range(width):
                upper_pixel = image_array[i, j]
                lower_pixel = image_array[i + 1, j]
                # use foreground and background colors along with half blocks
                # to map two pixels to one character
                if upper_pixel[3] == 0 and lower_pixel[3] == 0:
                    unicode_sprite += empty_block
                elif upper_pixel[3] == 0:
                    r, g, b = lower_pixel[:3]
                    escape_code = ansi.get_color_escape_code(r, g, b)
                    unicode_sprite += escape_code
                    unicode_sprite += lower_block
                elif lower_pixel[3] == 0:
                    r, g, b = upper_pixel[:3]
                    escape_code = ansi.get_color_escape_code(r, g, b)
                    unicode_sprite += escape_code
                    unicode_sprite += upper_block
                else:
                    r_f, g_f, b_f = upper_pixel[:3]
                    r_b, g_b, b_b = lower_pixel[:3]
                    foreground_escape = ansi.get_color_escape_code(r_f, g_f, b_f)
                    background_escape = ansi.get_color_escape_code(
                        r_b, g_b, b_b, background=True
                    )
                    unicode_sprite += foreground_escape
                    unicode_sprite += background_escape
                    unicode_sprite += upper_block
                    unicode_sprite += reset_code
            unicode_sprite += "\n"
        unicode_sprite += reset_code

        return unicode_sprite


class LargeConverter:
    """Generates sprites where one image pixel is mapped to two unicode
    characters in width and one in height"""

    def convert_image_to_unicode(self, image: Image) -> str:
        # using two characters wide and one character high to maintain a near
        # 1 aspect ratio
        solid_block = "██"
        empty_block = "  "

        unicode_sprite = ""

        image_array = np.array(image)

        height, width, _ = image_array.shape
        reset_code = ansi.get_reset_escape_code()
        prev_escape_code = ""
        for i in range(height):
            for j in range(width):
                alpha = image_array[i, j, 3]
                if alpha == 0:
                    unicode_sprite += empty_block
                    continue
                r, g, b = image_array[i, j, :3]
                escape_code = ansi.get_color_escape_code(r, g, b, background=False)
                # only add an escape code if the color changes
                if prev_escape_code != escape_code:
                    unicode_sprite += escape_code
                unicode_sprite += solid_block
                prev_escape_code = escape_code
            unicode_sprite += "\n"
        unicode_sprite += reset_code

        return unicode_sprite


class TinyConverter:
    """Generates sprites where four image pixels are mapped to one unicode character"""

    def convert_image_to_unicode(self, image: Image) -> str:
        solid_block = "█"
        empty_block = " "
        unicode_sprite = ""

        image_array = np.array(image)
        height, width, channels = image_array.shape

        # Ensure width and height are even
        if height % 2:
            padded_array = np.zeros((height + 1, width, channels)).astype(np.uint8)
            padded_array[:height, :, :] = image_array
            height, width, channels = padded_array.shape
            image_array = padded_array

        if width % 2:
            padded_array = np.zeros((height, width + 1, channels)).astype(np.uint8)
            padded_array[:, :width, :] = image_array
            height, width, channels = padded_array.shape
            image_array = padded_array

        reset_code = ansi.get_reset_escape_code()
        for i in range(0, height, 2):
            for j in range(0, width, 2):
                pixels = [
                    image_array[i, j],
                    image_array[i, j + 1],
                    image_array[i + 1, j],
                    image_array[i + 1, j + 1]
                ]
                if all(pixel[3] == 0 for pixel in pixels):
                    unicode_sprite += empty_block
                else:
                    avg_color = np.mean([pixel[:3] for pixel in pixels if pixel[3] != 0], axis=0).astype(int)
                    r, g, b = avg_color
                    escape_code = ansi.get_color_escape_code(r, g, b)
                    unicode_sprite += escape_code + solid_block + reset_code
            unicode_sprite += "\n"
        unicode_sprite += reset_code

        return unicode_sprite
