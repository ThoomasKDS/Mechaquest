/**
 * @file graph.h
 * @brief Ce fichier est utilisé uniquement pour générer la documentation du graphe général. Il n'est pas destiné à être affiché dans la documentation publique.
 * @internal
 */

/*!
 * \page GraphesFichiers Graphe Général des Fichiers
 *
 * # Graphe Général des Fichiers
 *
 * Voici un graphe général des fichiers `.h` et `.c` :
 *
 * \dot
digraph G {
  node [shape=box, style=filled, color=lightblue];

  "lib/affichage.h" -> "src/affichage.c";
  "lib/pointDePassage.h" -> "src/affichage.c";
  "lib/combat.h" -> "src/combat.c";
  "lib/sauv.h" -> "src/combat.c";
  "lib/initGame.h" -> "src/initGame.c";
  "lib/combat.h" -> "src/main.c";
  "lib/player.h" -> "src/main.c";
  "lib/initGame.h" -> "src/main.c";
  "lib/sauv.h" -> "src/main.c";
  "lib/affichage.h" -> "src/main.c";
  "lib/pointDePassage.h" -> "src/main.c";
  "lib/menu.h" -> "src/main.c";
  "lib/global.h" -> "src/main.c";
  "lib/son.h" -> "src/main.c";
  "lib/menu.h" -> "src/menu.c";
  "lib/initGame.h" -> "src/menu.c";
  "lib/affichage.h" -> "src/menu.c";
  "lib/son.h" -> "src/menu.c";
  "lib/player.h" -> "src/player.c";
  "lib/affichage.h" -> "src/player.c";
  "lib/son.h" -> "src/player.c";
  "lib/pnj.h" -> "src/pnj.c";
  "lib/pointDePassage.h" -> "src/pointDePassage.c";
  "lib/combat.h" -> "src/pointDePassage.c";
  "lib/sauv.h" -> "src/sauv.c";
  "lib/initGame.h" -> "src/son.c";
  "lib/son.h" -> "src/son.c";
}
 * \enddot
 */

 /** @endinternal */
