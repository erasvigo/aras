# Makefile for the ARAS Radio Automation System

CC = gcc
SRCDIR = .
INCDIR = ../../include
BUILDDIR = ../../build
BINDIR = ../../bin

CFLAGS = -c -g -O3 -Wall
LFLAGS =

default: all

all: daemon player recorder

daemon: main_daemon.o configuration.o schedule.o block.o engine.o player.o playlist.o log.o parse.o time.o
	$(CC) $(LFLAGS) $(BUILDDIR)/parse.o $(BUILDDIR)/time.o $(BUILDDIR)/log.o $(BUILDDIR)/playlist.o $(BUILDDIR)/configuration.o $(BUILDDIR)/schedule.o $(BUILDDIR)/block.o $(BUILDDIR)/engine.o $(BUILDDIR)/player.o $(BUILDDIR)/main_daemon.o `pkg-config --libs glib-2.0 gstreamer-1.0 'libvlc >= 1.1.0'` -o $(BINDIR)/aras-daemon

player: main_player.o gui_player.o configuration.o schedule.o block.o engine.o player.o playlist.o log.o parse.o time.o
	$(CC) $(LFLAGS) $(BUILDDIR)/parse.o $(BUILDDIR)/time.o $(BUILDDIR)/log.o $(BUILDDIR)/playlist.o $(BUILDDIR)/configuration.o $(BUILDDIR)/schedule.o $(BUILDDIR)/block.o $(BUILDDIR)/engine.o $(BUILDDIR)/player.o $(BUILDDIR)/gui_player.o $(BUILDDIR)/main_player.o `pkg-config --libs glib-2.0 gstreamer-1.0 'libvlc >= 1.1.0' gtk+-3.0` -o $(BINDIR)/aras-player

recorder: main_recorder.o gui_recorder.o configuration.o schedule.o block.o recorder.o playlist.o log.o parse.o time.o
	$(CC) $(LFLAGS) $(BUILDDIR)/parse.o $(BUILDDIR)/time.o $(BUILDDIR)/log.o $(BUILDDIR)/playlist.o $(BUILDDIR)/configuration.o $(BUILDDIR)/schedule.o $(BUILDDIR)/block.o $(BUILDDIR)/recorder.o $(BUILDDIR)/gui_recorder.o $(BUILDDIR)/main_recorder.o `pkg-config --libs glib-2.0 gstreamer-1.0 gtk+-3.0` -o $(BINDIR)/aras-recorder

main_recorder.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0 gstreamer-1.0 gtk+-3.0` $(SRCDIR)/main_recorder.c -o $(BUILDDIR)/main_recorder.o

main_player.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0 gstreamer-1.0 gtk+-3.0` $(SRCDIR)/main_player.c -o $(BUILDDIR)/main_player.o

main_daemon.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0 gstreamer-1.0 'libvlc >= 1.1.0'` $(SRCDIR)/main_daemon.c -o $(BUILDDIR)/main_daemon.o

recorder.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags gstreamer-1.0` $(SRCDIR)/recorder.c -o $(BUILDDIR)/recorder.o

gui_recorder.o:
	$(CC) $(CFLAGS) -I$(INCDIR)  `pkg-config --cflags glib-2.0 gstreamer-1.0 gtk+-3.0` $(SRCDIR)/gui_recorder.c -o $(BUILDDIR)/gui_recorder.o

gui_player.o:
	$(CC) $(CFLAGS) -I$(INCDIR)  `pkg-config --cflags glib-2.0 gstreamer-1.0 gtk+-3.0` $(SRCDIR)/gui_player.c -o $(BUILDDIR)/gui_player.o

player.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags gstreamer-1.0 'libvlc >= 1.1.0'` $(SRCDIR)/player.c -o $(BUILDDIR)/player.o

engine.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0 gstreamer-1.0 'libvlc >= 1.1.0'` $(SRCDIR)/engine.c -o $(BUILDDIR)/engine.o

block.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0` $(SRCDIR)/block.c -o $(BUILDDIR)/block.o

schedule.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0` $(SRCDIR)/schedule.c -o $(BUILDDIR)/schedule.o

configuration.o:
	$(CC) $(CFLAGS) -I$(INCDIR) $(SRCDIR)/configuration.c -o $(BUILDDIR)/configuration.o

playlist.o:
	$(CC) $(CFLAGS) -I$(INCDIR) `pkg-config --cflags glib-2.0` $(SRCDIR)/playlist.c -o $(BUILDDIR)/playlist.o

log.o:
	$(CC) $(CFLAGS) -I$(INCDIR) $(SRCDIR)/log.c -o $(BUILDDIR)/log.o

parse.o:
	$(CC) $(CFLAGS) -I$(INCDIR) $(SRCDIR)/parse.c -o $(BUILDDIR)/parse.o

time.o:
	$(CC) $(CFLAGS) -I$(INCDIR) $(SRCDIR)/time.c -o $(BUILDDIR)/time.o

.PHONY: clean
clean:
	rm -f $(BUILDDIR)/*.o $(BINDIR)/aras-daemon $(BINDIR)/aras-player $(BINDIR)/aras-recorder
