#!/usr/bin/env python3
import argparse
import asyncio
import json
import os
from typing import List
from PIL import Image

import unicode_converter as converter
from image import PokemonImage
from pokemon import Pokemon
from pokesprite_db import PokespriteDB

async def main() -> None:
    args = parse_cli_arguments()
    async with PokespriteDB() as db:
        pokemons: List[Pokemon] = []
        await db.fetch_data()
        for pokemon in db:
            pokemons.append(pokemon)
        generate_pokemon_json(pokemons)

        sprites = await db.fetch_sprites(args.include_forms)
        for sprite in sprites:
            image = PokemonImage(sprite.image)
            image.convert_to_rgba()
            image.crop_to_content()
            # image.resize_to_max_height(20)  # Adjust the image height to a maximum of 20

            small_converter = converter.SmallConverter()
            large_converter = converter.LargeConverter()
            small_unicode_sprite = small_converter.convert_image_to_unicode(image.image)
            large_unicode_sprite = large_converter.convert_image_to_unicode(image.image)

            color_dir = "shiny" if sprite.is_shiny else "regular"
            write_to_file(sprite.name, f"large/{color_dir}", large_unicode_sprite)
            write_to_file(sprite.name, f"small/{color_dir}", small_unicode_sprite)

            save_image(image.image, sprite.name, 'images')

            if not args.silent:
                print(sprite.name)
                print(large_unicode_sprite)
                print(small_unicode_sprite)

def generate_pokemon_json(pokemons: List[Pokemon]) -> None:
    print("Generating pokemon JSON...")
    pokemon_json = []
    for pokemon in pokemons:
        pokemon_entry = {"name": pokemon.name, "forms": pokemon.forms}
        pokemon_json.append(pokemon_entry)
    with open("pokemon.json", "w+", encoding='utf-8') as fout:
        json.dump(pokemon_json, fout, indent=2)

def parse_cli_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i",
        "--include-forms",
        help="generate the different forms of the pokemon. Regional, megas, gmax etc.",
        action="store_true",
    )
    parser.add_argument(
        "-s",
        "--silent",
        help="don't print out sprites as they are being generated",
        action="store_true",
    )
    return parser.parse_args()

def write_to_file(filename: str, directory: str, text: str) -> None:
    os.makedirs(directory, exist_ok=True)
    with open(f"{directory}/{filename}", "w+", encoding='utf-8') as fout:
        fout.write(text)

def save_image(image: Image.Image, filename: str, directory: str) -> None:
    os.makedirs(directory, exist_ok=True)
    image.save(os.path.join(directory, f"{filename}.png"))

if __name__ == "__main__":
    asyncio.run(main())
