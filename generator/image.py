from PIL import Image
import numpy as np

class PokemonImage:
    def __init__(self, image: Image.Image):
        self._image = image

    @property
    def image(self) -> Image.Image:
        return self._image

    def crop_to_content(self) -> None:
        """Remove padding around the image. Uses alpha channel to find padding"""
        # using numpy as this is easier and also faster
        image_array = np.array(self.image)

        alpha_channel = image_array[:, :, 3]
        # first and last non-transparent pixels taken as start and end
        # points for crop along both height and width axes
        non_zero_x_values, non_zero_y_values = alpha_channel.nonzero()
        top, left = np.min(non_zero_x_values), np.min(non_zero_y_values)
        bottom, right = np.max(non_zero_x_values), np.max(non_zero_y_values)
        cropped_image = image_array[top : bottom + 1, left : right + 1]

        self._image = Image.fromarray(cropped_image)

    def convert_to_rgba(self) -> None:
        if self.image.mode != "RGBA":
            rgba_image = self.image.convert("RGBA")
            self._image = rgba_image

    def resize_to_max_height(self, max_height: int = 20) -> None:
        """Resize the image to have a maximum height of max_height, maintaining the aspect ratio."""
        current_width, current_height = self._image.size
        if current_height > max_height:
            # Calculate the new width maintaining the aspect ratio
            new_width = int((max_height / current_height) * current_width)
            # Resize the image
            self._image = self._image.resize((new_width, max_height), Image.LANCZOS)
