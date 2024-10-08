#!/bin/bash

# Check if it's running with root privileges
if [ "$EUID" -ne 0 ]; then
  echo "Please run this script with sudo or root privileges."
  exit 1
fi

# Set the installation path
BIN_DIR="/usr/local/bin"
USER_HOME=$(eval echo ~${SUDO_USER})  # Get the home directory of the user running sudo
USER_POKEMONC_DIR="$USER_HOME/pokemonc"

echo "Installing pokemonc..."

# Create the pokemonc directory in the user's home directory
mkdir -p "$USER_POKEMONC_DIR"
echo "$USER_POKEMONC_DIR"
# Copy the binary pokemonc to /usr/local/bin
cp ./pokemonc "$BIN_DIR/"
# Copy the assets folder to ~/pokemonc
cp -r ./assets "$USER_POKEMONC_DIR/"
# Copy the config.json file to ~/pokemonc
cp ./config.json "$USER_POKEMONC_DIR/"
# Make the binary executable
chmod +x "$BIN_DIR/pokemonc"

echo "The installation is complete."
