#!/bin/bash

#Creation du repertoire fichiers (si pas ex)
mkdir - p /home/ubuntu/my_scripts/fichiers/

#
echo "Entrez le nom du fichier a creer:"
read filename

#
touch /home/ubuntu/my_scripts/fichiers/$filename

#
chmod 666 /home/ubuntu/my_scripts/fichiers/$filename

#
echo "Contenu du repertoire fichiers:"
ls -l /home/ubuntu/my_scripts/fichiers/

exit 0
