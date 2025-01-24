# Nom du projet (exécutable généré)
TARGET = mechaquest

# Compilateur
CC = gcc

# Drapeaux de compilation
CFLAGS = -std=c99 -Wall -Ilib $(shell sdl2-config --cflags)

# Drapeaux de l'éditeur de liens
LFLAGS = -lm $(shell sdl2-config --libs)

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
	$(CC) $^ $(LFLAGS) -o $@
	@echo "Linking complete: $@"

# Compilation des fichiers .c en fichiers .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

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
