<h1 align="center">Pokemonc</h1>

<p align="center">
    <a target="_blank" href="https://github.com/JJLibra">
      <img style="display:inline-block;margin:0.2em;" alt="Author" src="https://img.shields.io/badge/Author-Junjie Li-green.svg?logo=autoit&style=flat">
    </a>
    <a target="_blank" href="https://github.com/JJLibra/Pokemonc">
      <img style="display:inline-block;margin:0.2em;" alt="GitHub Repo stars" src="https://img.shields.io/github/stars/JJLibra/Pokemonc?style=social">
    </a>
</p>

<table style="width: 100%; border-spacing: 0;">
  <tr>
    <td style="width: 150px; padding: 0;">
      <img src="./screenshots/mewtwo.png" width="150" data-width="150" data-height="150">
    </td>
    <td style="width: 100%; padding-left: 10px;">
      Pokemonc is mostly a C rewrite of phoney badger's 
      <a href="https://gitlab.com/phoneybadger/pokemon-colorscripts">pokemon-colorscripts</a> 
      with some extra features.<br><br>
      I developed it for the terminal startup display.<br>I didn't find the C version of the program, 
      so I tried to implement this project.<br><br>
      Pokémon was a part of my childhood and I love it!
    </td>
  </tr>
</table>

## Table of contents

* [Features](#features)
* [Installation](#installation)
  * [Ubuntu/Debian x86_64 (and derivatives)](#ubuntudebian-x86_64-and-derivatives)
  * [Installing from script (other distros)](#Installing-from-script-other-distros)
* [Usage](#usage)
* [Credits](#credits)
* [Similar projects](#similar-projects)

## Features

- Pokemon from every generation, including shinies, megas, gigantamax, and regional variants
- Print random pokemon (with filters for generations and different forms)
- Print pokemon by name
- Print pokedex entry together with the sprite
- Configuration file, right now only for language and shiny rate

## Installation

### Ubuntu/Debian x86_64 (and derivatives)

Download the latest `.deb` release. Then run (replacing v.v.v with the version number)

```shell
dpkg -i pokemonc_v.v.v_amd64.deb
# dpkg -r pokemonc
```

### Installing from script (other distros)

If you are using a Linux distribution other than Ubuntu/Debian or its derivatives, or you prefer to install the project from the source code, follow the steps below to install `pokemonc` using the provided `install.sh` script.

1. Clone the repository
   First, clone the project repository to your local machine using the following command:

```shell
git clone https://github.com/JJLibra/Pokemonc.git pokemonc
cd pokemonc
```

2. Run the install script
   The installation script `install.sh` will copy the `pokemonc` binary to a system-wide location and set up the necessary configuration files in your home directory. This script will:

* Copy the `pokemonc` binary to `/usr/local/bin/`, allowing you to run `pokemonc` from anywhere in the terminal.
* Copy the `assets` folder and `config.json` to the `~/pokemonc` directory in your home folder.

```shell
sudo ./install.sh
# sudo ./uninstall.sh
```

## Usage

Run the help command `pokemonc --help` to see the following help message.

```
USAGE:
    pokemonc <SUBCOMMAND>

OPTIONS:
    -h, --help       Print help information
    -v, --version    Print version information

SUBCOMMANDS:
    help             Print this message or the help of the given subcommand(s)
    list             Print list of all pokemon
    name             Select pokemon by name. Generally spelled like in the games.
    random           Show a random pokemon.
```

<!-- To get more detailed information about a subcommand you can also view its help, for example
```
pokemonc help random
```
To get the help of the random subcommand.

### Examples
Print a specific pokemon
```
pokemonc name charizard
```
Print a specific shiny pokemon
```
pokemonc name spheal -s
```
Print a specific pokemon together with its pokedex entry
```
pokemonc name mudkip -i
```
Print an alternative form of a pokemon
```
pokemonc name blastoise -f mega
```
Print a random pokemon (gens 1-9)
```
pokemonc random
```
Print random pokemon from generations 1-3
```
pokemonc random 1-3
```
Print a random pokemon from generations 1,3 and 6
```
pokemonc random 1,3,6
```
Print a random pokemon excluding megas, gigantamax and regional variants
```
pokemonc random --no-mega --no-gmax --no-regional
```

## Configuration
When the program is run, a TOML config file will automatically be created in the user's config
directory (usually `~/.config`) under `pokemonc/config.toml` if it doesn't exist already. 

On MacOS the config will be in: `/Users/<username>/Library/Application Support/pokemonc`
On Windows this will be: `C:\Users\<username>\AppData\Roaming\pokemonc`

```toml
# The language to use when printing the pokemon's name and/or description.
# Possible options include en (English), fr (French), de (German), ja (Japanese),
# zh_hans (Chinese with simplified characters), zh_hant (Chinese with traditional characters)
language = 'en'

# The probability to show a shiny pokemon when using the random command
shiny_rate = 0.0078125
``` -->

## Credits

The pokemon sprites for `pokemonc` were generated using sprites from [PokéSprite](https://msikma.github.io/pokesprite/)
and converted to unicode using Phoney Badger's [pokemon-generator-scripts](https://gitlab.com/phoneybadger/pokemon-generator-scripts).

## Similar projects

- [pokemon-colorscripts](https://gitlab.com/phoneybadger/pokemon-colorscripts)
- [krabby](https://github.com/yannjor/krabby)
- [pokeget](https://github.com/talwat/pokeget)
- [pokeshell](https://github.com/acxz/pokeshell)
