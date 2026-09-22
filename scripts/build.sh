#!/usr/bin/env bash
set -e

WORKSPACE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

source /opt/ros/humble/setup.bash
cd "$WORKSPACE_DIR"

colcon build --symlink-install
source install/setup.bash

echo "Build terminé."
