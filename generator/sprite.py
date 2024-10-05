from io import BytesIO

import aiohttp
from PIL import Image


class Sprite:
    def __init__(self, name: str, is_shiny: bool, image_url: str):
        self._name = name
        self._is_shiny = is_shiny
        self.url = image_url

    def __repr__(self) -> str:
        return f"Sprite(pokemon = {self._name}, is_shiny = {self.is_shiny})"

    @property
    def name(self) -> str:
        return self._name

    @property
    def is_shiny(self) -> bool:
        return self._is_shiny

    @property
    def image(self) -> Image.Image:
        return self._image

    async def fetch_image(self, http_session: aiohttp.ClientSession) -> None:
        async with http_session.get(self.url) as response:
            image_data = await response.read()
            image = Image.open(BytesIO(image_data))
            self._image = image
