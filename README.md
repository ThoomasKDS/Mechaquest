# MechaQuest : L'aventure des Mechas
> Projet étudiant en Licence 2 Informatique au Mans<br>
> Janvier 2025 - Avril 2025

## Le jeu
MechaQuest est basé sur les jeux Pokémon 2D, le joueur incarne un jeune dresseur qui se lance dans un voyage à travers un monde futuriste où des créatures mécaniques, appelées Mechas, sont la clé de l’aventure.<br>
À travers des combats stratégiques, le joueur devra déjouer les plans de l'antagoniste Iron Musk, qui cherche à utiliser les Mechas pour imposer son pouvoir et son contrôle sur la région.

## Prérequis
Pour pouvoir jouer au jeu, il faut être sur Linux ou macOS et installer les bibliothèques SDL.<br>
#### Linux : 
```
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
#### macOS :
```
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
```
## Installation
Pour cloner les fichiers sur votre machine.
```
git clone https://github.com/ThoomasKDS/Mechaquest.git
```
## Makefile et lancement du jeu
```
make all
./bin/mechaquest
```

## Gameplay
#### Objectif
L’objectif principal du jeu est de devenir le meilleur dresseur en capturant, entraînant et améliorant les Mechas pour affronter un puissant antagoniste Iron Musk, menaçant l’équilibre de ce monde technologique.
#### Mécaniques
* Se déplacer (exploration libre avec vue de dessus)
* Combattre des Mechas
* Combattre des PNJ
* Capturer des Mechas
#### Commandes
Le jeu est uniquement jouable sur ordinateur, avec un clavier et une souris.
* Dans les menus principaux : souris
* Pour se déplacer : flèches
* Pour interagir avec les PNJ : P
* Pour passer les dialogues : A
* Pour les choix dans les combats : flèches puis A


## Membres
* **Noah Cabaret**
* **Thomas Castella**
* **Mokhaled Al Aftan**
* **Max Espagnol**
