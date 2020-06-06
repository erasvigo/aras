# Makefile for the ARAS Radio Automation System

PACKAGE      = aras
VERSION      = 4.6
DISTDIR  = ..
DISTFILE = $(PACKAGE)-$(VERSION)

DEBREV = 1
DEBARCH = amd64
DEBDIR_COMMON = debian/$(PACKAGE)-common_$(VERSION)-$(DEBREV)_all
DEBDIR_DAEMON = debian/$(PACKAGE)-daemon_$(VERSION)-$(DEBREV)_$(DEBARCH)
DEBDIR_PLAYER = debian/$(PACKAGE)-player_$(VERSION)-$(DEBREV)_$(DEBARCH)
DEBDIR_RECORDER = debian/$(PACKAGE)-recorder_$(VERSION)-$(DEBREV)_$(DEBARCH)

DESTDIR =
PREFIX = /usr

default:
	cd src/aras && make

all:
	cd src/aras && make all

daemon:
	cd src/aras && make daemon

player:
	cd src/aras && make player

recorder:
	cd src/aras && make recorder

daemon-vlclib:
	cd src/aras && make daemon-vlclib

player-vlclib:
	cd src/aras && make player-vlclib

.PHONY: clean
clean:
	cd src/aras && make clean

.PHONY: dist
dist:
	tar -czvf $(DISTDIR)/$(DISTFILE).tar.gz $(DISTDIR)/$(DISTFILE)

.PHONY: install
install:
	mkdir -p  $(DESTDIR)$(PREFIX)/bin
	cp bin/aras-daemon $(DESTDIR)$(PREFIX)/bin/
	cp bin/aras-player $(DESTDIR)$(PREFIX)/bin/
	cp bin/aras-recorder $(DESTDIR)$(PREFIX)/bin/
	cp bin/aras-daemon.sh $(DESTDIR)$(PREFIX)/bin/
	cp bin/aras-player.sh $(DESTDIR)$(PREFIX)/bin/
	cp bin/aras-recorder.sh $(DESTDIR)$(PREFIX)/bin/
	mkdir -p $(DESTDIR)/etc
	cp -r etc/aras $(DESTDIR)/etc/
	mkdir -p $(DESTDIR)/etc/cron.daily
	cp -r etc/cron.daily/* $(DESTDIR)/etc/cron.daily/
	mkdir -p $(DESTDIR)/etc/cron.hourly
	cp -r etc/cron.hourly/* $(DESTDIR)/etc/cron.hourly/
	mkdir -p $(DESTDIR)/etc/logrotate.d
	cp -r etc/logrotate.d/* $(DESTDIR)/etc/logrotate.d/
	mkdir -p $(DESTDIR)/usr/share/applications
	cp -r share/applications/* $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share
	cp -r share/aras $(DESTDIR)/usr/share/
	mkdir -p $(DESTDIR)/usr/share/doc
	cp -r share/doc/aras $(DESTDIR)/usr/share/doc/
	mkdir -p $(DESTDIR)/usr/share/doc-base
	cp -r share/doc-base/* $(DESTDIR)/usr/share/doc-base/
	mkdir -p $(DESTDIR)/usr/share/man/man1
	cp -r share/man/man1/* $(DESTDIR)/usr/share/man/man1/
	mkdir -p $(DESTDIR)/usr/share/man/man5
	cp -r share/man/man5/* $(DESTDIR)/usr/share/man/man5/
	mkdir -p $(DESTDIR)/var/log
	cp -r var/log/aras $(DESTDIR)/var/log/
	chmod 666 /var/log/aras/*

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-daemon
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-player
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-recorder
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-daemon.sh
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-player.sh
	rm -f $(DESTDIR)$(PREFIX)/bin/aras-recorder.sh
	rm -fr $(DESTDIR)/etc/aras
	rm -f $(DESTDIR)/etc/cron.daily/aras-download-content
	rm -f $(DESTDIR)/etc/cron.hourly/aras-update-content
	rm -f $(DESTDIR)/etc/logrotate.d/aras
	rm -f $(DESTDIR)/usr/share/applications/aras-player.desktop
	rm -f $(DESTDIR)/usr/share/applications/aras-recorder.desktop
	rm -fr $(DESTDIR)/usr/share/aras
	rm -fr $(DESTDIR)/usr/share/doc/aras
	rm -f $(DESTDIR)/usr/share/doc-base/aras
	rm -f $(DESTDIR)/usr/share/man/man1/aras-daemon.1.gz
	rm -f $(DESTDIR)/usr/share/man/man1/aras-player.1.gz
	rm -f $(DESTDIR)/usr/share/man/man1/aras-recorder.1.gz
	rm -f $(DESTDIR)/usr/share/man/man5/aras.block.5.gz
	rm -f $(DESTDIR)/usr/share/man/man5/aras.conf.5.gz
	rm -f $(DESTDIR)/usr/share/man/man5/aras.log.5.gz
	rm -f $(DESTDIR)/usr/share/man/man5/aras.schedule.5.gz
	rm -fr $(DESTDIR)/var/log/aras

.PHONY: deb-pkg
deb-pkg: deb-pkg-common deb-pkg-daemon deb-pkg-player deb-pkg-recorder
	make deb-pkg-common
	make deb-pkg-daemon
	make deb-pkg-player
	make deb-pkg-recorder

.PHONY: deb-pkg-common
deb-pkg-common:
	mkdir -p $(DEBDIR_COMMON)/etc/
	cp -r etc/aras $(DEBDIR_COMMON)/etc/
	cp -r etc/logrotate.d $(DEBDIR_COMMON)/etc/
	cp -r etc/cron.daily $(DEBDIR_COMMON)/etc/
	cp -r etc/cron.hourly $(DEBDIR_COMMON)/etc/
	mkdir -p $(DEBDIR_COMMON)/usr/share/aras/
	cp -r share/aras/media $(DEBDIR_COMMON)/usr/share/aras
	cp -r share/doc $(DEBDIR_COMMON)/usr/share/
	cp -r share/doc-base $(DEBDIR_COMMON)/usr/share/
	mkdir -p $(DEBDIR_COMMON)/usr/share/man
	cp -r share/man/man5 $(DEBDIR_COMMON)/usr/share/man
	mkdir -p $(DEBDIR_COMMON)/var/log/
	cp -r var/log/aras $(DEBDIR_COMMON)/var/log
	chown 0:0 -R $(DEBDIR_COMMON)
	chmod 0755 -R $(DEBDIR_COMMON)
	dpkg-deb -b $(DEBDIR_COMMON)

.PHONY: deb-pkg-daemon
deb-pkg-daemon:
	mkdir -p $(DEBDIR_DAEMON)/usr/bin
	cp -r bin/aras-daemon $(DEBDIR_DAEMON)/usr/bin/
	cp -r bin/aras-daemon.sh $(DEBDIR_DAEMON)/usr/bin/
	mkdir -p $(DEBDIR_DAEMON)/usr/share/aras/icons
	cp -r share/aras/icons/aras-daemon-icon.png $(DEBDIR_DAEMON)/usr/share/aras/icons/
	mkdir -p $(DEBDIR_DAEMON)/usr/share/man/man1
	cp -r share/man/man1/aras-daemon.1.gz $(DEBDIR_DAEMON)/usr/share/man/man1/
	chown 0:0 -R $(DEBDIR_DAEMON)
	chmod 0755 -R $(DEBDIR_DAEMON)
	dpkg-deb -b $(DEBDIR_DAEMON)

.PHONY: deb-pkg-player
deb-pkg-player:
	mkdir -p $(DEBDIR_PLAYER)/usr/bin
	cp -r bin/aras-player $(DEBDIR_PLAYER)/usr/bin/
	cp -r bin/aras-player.sh $(DEBDIR_PLAYER)/usr/bin/
	mkdir -p $(DEBDIR_PLAYER)/usr/share/applications
	cp -r share/applications/aras-player.desktop $(DEBDIR_PLAYER)/usr/share/applications/
	mkdir -p $(DEBDIR_PLAYER)/usr/share/aras/icons
	cp -r share/aras/icons/aras-player-icon.png $(DEBDIR_PLAYER)/usr/share/aras/icons/
	mkdir -p $(DEBDIR_PLAYER)/usr/share/man/man1
	cp -r share/man/man1/aras-player.1.gz $(DEBDIR_PLAYER)/usr/share/man/man1/
	chown 0:0 -R $(DEBDIR_PLAYER)
	chmod 0755 -R $(DEBDIR_PLAYER)
	dpkg-deb -b $(DEBDIR_PLAYER)

.PHONY: deb-pkg-recorder
deb-pkg-recorder:
	mkdir -p $(DEBDIR_RECORDER)/usr/bin
	cp -r bin/aras-recorder $(DEBDIR_RECORDER)/usr/bin/
	cp -r bin/aras-recorder.sh $(DEBDIR_RECORDER)/usr/bin/
	mkdir -p $(DEBDIR_RECORDER)/usr/share/applications
	cp -r share/applications/aras-recorder.desktop $(DEBDIR_RECORDER)/usr/share/applications/
	mkdir -p $(DEBDIR_RECORDER)/usr/share/aras/icons
	cp -r share/aras/icons/aras-recorder-icon.png $(DEBDIR_RECORDER)/usr/share/aras/icons/
	mkdir -p $(DEBDIR_RECORDER)/usr/share/man/man1
	cp -r share/man/man1/aras-recorder.1.gz $(DEBDIR_RECORDER)/usr/share/man/man1/
	chown 0:0 -R $(DEBDIR_RECORDER)
	chmod 0755 -R $(DEBDIR_RECORDER)
	dpkg-deb -b $(DEBDIR_RECORDER)
