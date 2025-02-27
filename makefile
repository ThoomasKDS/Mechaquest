# Nom du projet (exécutable généré)
TARGET = mechaquest

# Compilateur
CC = gcc

# Drapeaux de compilation
CFLAGS = -std=c99 -Wall -Ilib \
         $(shell sdl2-config --cflags) \
         $(shell pkg-config --cflags SDL2_image SDL2_mixer SDL2_ttf)

# Drapeaux de l'éditeur de liens
LFLAGS = -lm $(shell pkg-config --libs SDL2_image SDL2_mixer SDL2_ttf)

# Répertoires
SRCDIR = src
INCDIR = lib
OBJDIR = obj
BINDIR = bin

# Liste des fichiers source et objets
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Règle par défaut : compilation complète
all: $(BINDIR)/$(TARGET)

# Création de l'exécutable
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@$(CC) $^ $(LFLAGS) -o $@

# Compilation des fichiers .c en fichiers .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
.PHONY: clean
clean:
	@rm -rf $(OBJDIR)/*.o
	@echo "Cleanup complete: Object files removed."

# Suppression des exécutables
.PHONY: remove
remove: clean
	@rm -rf $(BINDIR)/$(TARGET)
	@echo "Executable removed: $(BINDIR)/$(TARGET)"
