# Simple Makefile for two example targets: overlay and example_game
DEVKITPRO ?= /opt/devkitpro
include $(DEVKITPRO)/3dsRules

TARGETS := overlay example_game

overlay_SOURCES := source/overlay.c
overlay_TARGET := overlay

example_game_SOURCES := source/example_game.c
example_game_TARGET := example_game

all: $(TARGETS:%=%.3dsx)

clean:
	-rm -f *.elf *.3dsx *.smdh

.PHONY: all clean
