# === Compilateur & options ===
CC = gcc
CFLAGS = -Wall -Wextra -g -I../lib

# === Répertoires ===
TEST_DIR = ../test

# === Fichiers source de test ===
TESTS = \
    test_fonction_utilisation_carburant.c \
    test_fonction_utilisation_rappel.c \
    test_fonction_algo_attaque.c

# === Générer les noms d'exécutables ===
EXECUTABLES = $(TESTS:.c=)

# === Cibles individuelles ===
all: $(EXECUTABLES)

# Règle générique pour compiler chaque fichier .c en exécutable
%: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# === Exécuter tous les tests ===
test: all
	@echo "\n--- Exécution de tous les tests ---"
	@./test_fonction_utilisation_carburant
	@./test_fonction_utilisation_rappel
	@./test_fonction_algo_attaque

# === Nettoyer les exécutables ===
clean:
	rm -f $(EXECUTABLES)

.PHONY: all test clean
