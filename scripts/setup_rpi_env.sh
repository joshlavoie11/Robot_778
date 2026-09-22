#!/usr/bin/env bash
set -e

echo "Configuration de l'environnement Raspberry Pi..."

if [ -f /opt/ros/humble/setup.bash ]; then
  source /opt/ros/humble/setup.bash
else
  echo "ROS 2 Humble introuvable dans /opt/ros/humble."
  echo "Installez ROS 2 ou modifiez ce script selon votre distribution."
  exit 1
fi

export WORKSPACE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$WORKSPACE_DIR"

if [ -f install/setup.bash ]; then
  source install/setup.bash
fi

echo "Environnement prêt."
echo "Workspace : $WORKSPACE_DIR"
