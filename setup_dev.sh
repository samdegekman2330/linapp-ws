#!/bin/bash

echo "🚀 LinApp-WS: Ontwikkelomgeving instellen..."

if [ -f /etc/arch-release ]; then
    echo "📦 Arch Linux gedetecteerd. Gebruik pacman..."
    sudo pacman -S --needed base-devel cmake gtk4 libadwaita pipewire gst-plugins-bad gst-plugins-good openssl
elif [ -f /etc/debian_version ]; then
    echo "📦 Debian/Ubuntu gedetecteerd. Gebruik apt..."
    sudo apt update
    sudo apt install -y build-essential cmake libgtk-4-dev libadwaita-1-dev libpipewire-0.3-dev libgstreamer1.0-dev libssl-dev
else
    echo "❌ Distro niet herkend. Installeer dependencies handmatig."
    exit 1
fi

echo "✅ Klaar! Je bent klaar om de Windows-code te slopen."
