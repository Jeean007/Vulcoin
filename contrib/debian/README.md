
Debian
====================
This directory contains files used to package vulcoind/vulcoin-qt
for Debian-based Linux systems. If you compile vulcoind/vulcoin-qt yourself, there are some useful files here.

## vulcoin: URI support ##


vulcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install vulcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your vulcoinqt binary to `/usr/bin`
and the `../../share/pixmaps/vulcoin128.png` to `/usr/share/pixmaps`

vulcoin-qt.protocol (KDE)

