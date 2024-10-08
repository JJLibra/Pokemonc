#!/bin/bash

# Check if it's running with root privileges
if [ "$EUID" -ne 0 ]; then
  echo "Please run this script with sudo or root privileges."
  exit 1
fi

# Set the paths used during installation
BIN_DIR="/usr/local/bin"
POKEMONC_DIR="/usr/local/share/pokemonc"

echo "Uninstalling pokemonc..."

# Remove the binary file from /usr/local/bin
if [ -f "$BIN_DIR/pokemonc" ]; then
  echo "Removing binary from $BIN_DIR"
  rm "$BIN_DIR/pokemonc"
else
  echo "Binary not found in $BIN_DIR, skipping"
fi

# Remove the pokemonc directory from the user's home directory
if [ -d "$POKEMONC_DIR" ]; then
  echo "Removing $POKEMONC_DIR"
  rm -rf "$POKEMONC_DIR"
else
  echo "$POKEMONC_DIR not found, skipping"
fi

echo "Uninstallation is complete."
