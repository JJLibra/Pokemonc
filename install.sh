#!/bin/bash

# Check if it's running with root privileges
if [ "$EUID" -ne 0 ]; then
  echo "Please run this script with sudo or root privileges."
  exit 1
fi

# Set the installation path
BIN_DIR="/usr/local/bin"
POKEMONC_DIR="/usr/local/share/pokemonc"

echo "Installing pokemonc..."

# Create the pokemonc directory in the user's home directory
mkdir -p "$POKEMONC_DIR"
echo "Create $POKEMONC_DIR ..."
# Copy the binary pokemonc to /usr/local/bin
cp ./pokemonc "$BIN_DIR/"
echo "export pokemonc to PATH ..."
# Copy the assets folder to ~/pokemonc
cp -r ./assets "$POKEMONC_DIR/"

# Make the binary executable
chmod +x "$BIN_DIR/pokemonc"

echo "The installation is complete."
