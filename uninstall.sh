#!/bin/bash

# Check if it's running with root privileges
if [ "$EUID" -ne 0 ]; then
  echo "Please run this script with sudo or root privileges."
  exit 1
fi

# Set the paths used during installation
BIN_DIR="/usr/local/bin"
USER_HOME=$(eval echo ~${SUDO_USER})
USER_POKEMONC_DIR="$USER_HOME/pokemonc"

echo "Uninstalling pokemonc..."

# Remove the binary file from /usr/local/bin
if [ -f "$BIN_DIR/pokemonc" ]; then
  echo "Removing binary from $BIN_DIR"
  rm "$BIN_DIR/pokemonc"
else
  echo "Binary not found in $BIN_DIR"
fi

# Remove the pokemonc directory from the user's home directory
if [ -d "$USER_POKEMONC_DIR" ]; then
  echo "Removing $USER_POKEMONC_DIR"
  rm -rf "$USER_POKEMONC_DIR"
else
  echo "$USER_POKEMONC_DIR not found"
fi

echo "Uninstallation is complete."
