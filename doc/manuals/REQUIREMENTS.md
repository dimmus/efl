---
title: Requirements to build EFL from source
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# Requirements to build EFL from source

libc
gstreamer (Ensure all codecs you want are installed.)
zlib
lua (lua 5.1, luajit or lua 5.2 support optional)
openssl
curl
dbus
fontconfig
freetype2
fribidi
harfbuzz
libpulse
libsndfile
libx11
libxau
libxcomposite
libxdamage
libxdmcp
libxext
libxfixes
libxinerama
libxrandr
libxrender
libxss
libxtst
libxcursor
libxi (2.2 or newer)
opengl(mesa etc.) (opengl/glx/full or opengl-es2/egl. full opengl only on osx - must be explicitly specified to be full to have support)
giflib/libgif
systemd / libudev
poppler / poppler-cpp
libraw
libspectre
librsvg
libpng
libjpeg
openjpeg2
libtiff
openmp (clang needs libomp, while gcc uses libgomp)
libwebp

During runtime, you need the following additional
dependencies:

util-linux >= v2.27.1 required (libmount + libblkid)
libreoffice