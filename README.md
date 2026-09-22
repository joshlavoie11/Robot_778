# Raspberry Pi Workspace

Ce dossier est un workspace de développement pour un Raspberry Pi.

## Structure

- `src/` : packages ROS 2 / C++ / Python
- `scripts/` : scripts de configuration et de build
- `.vscode/` : configuration VS Code

## Prérequis

- Raspberry Pi OS ou Ubuntu Server pour ARM64/ARM32
- ROS 2 installé (ex. Humble)
- CMake, colcon, git
- un accès SSH si vous travaillez à distance

## Mise en route

```bash
cd ~/rpi_workspace
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## Développement utile

- Créer un package : `ros2 pkg create --build-type ament_cmake my_package`
- Lancer un noeud : `ros2 run my_package my_node`

## Conseils

- Utiliser une clé SSH pour accéder au Raspberry Pi
- Compiler directement sur la cible pour les tests de runtime
- Préparer des scripts de déploiement pour les cartes ARM
