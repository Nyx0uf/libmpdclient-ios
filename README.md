[![Build Status](https://travis-ci.com/Nyx0uf/libmpdclient-ios.svg?branch=master)](https://travis-ci.com/Nyx0uf/libmpdclient-ios)

### How to update :

First you will need to install *meson* and *ninja*.

	brew install meson ninja

Then download latest libmpdclient [here](https://www.musicpd.org/libs/libmpdclient/) and build it :

	tar xf libmpdclient-X.XX.tar.xz
	cd libmpdclient-X.XX
	meson . output
	ninja -C output

- replace all files in **libmpdclient-src/src** with the ones from **libmpdclient-X.XX/src**.
- replace all files in **libmpdclient-src/include/mpd** with the ones from **libmpdclient-X.XX/include/mpd**.
- If the latest version of libmpdclient contains new files don't forget to add them to the Xcode project.
- copy **libmpdclient-X.XX/output/version.h** and **libmpdclient-X.XX/output/version.h** to **libmpdclient-src/src**

In `sync.c` and `socket.c` Xcode may complain about a missing include, to fix it add the missing include in both files.

	#include <sys/time.h>

### License

Same as libmpdclient, BSDv2
