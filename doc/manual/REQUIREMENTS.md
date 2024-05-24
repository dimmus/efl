---
title: Requirements to build EFL from source
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# Requirements to build EFL from source

To build EFL, you have to install the following software packages:

- Build utilities:
  - [meson](http://) - build configuration. This is a small but very powerful python utility.
  - [ninja](http://) - build backend.
  - [check](http://) - tests backend.

  For development purpose you could add:
  - [ccache](http://) - .
  - [git](http://) - .
  - compiler: [clang](http://) from [llvm](http://) package or [gcc](http://) from [glib](http://) package. Before the start of compilation set `CC` variable with the appropriate one (like `CC=clang`)
  - [valgrind](http://) is a testing utilitiy to check memory leaks.


| Dependency     | Used by                       | Meson (default) option    | Link                            |
| ---------------|:----------------------------- |:------------------------- | --------------------------------|
|                |                               |                           |                                 |
| - TEXT         |                               |                           |                                 |
| fontconfig     |                               |                           | [link][to-fontconfig]           |
| freetype       | ector                         |                           | [link][to-freetype]             |
| fribidi        |                               |                           | [link][to-fribidi]              |
| iconv          |                               |                           | [link][to-iconv]                |
| hyphen         |                               |                           | [link][to-hyphen]               |
| gettext        |                               |                           | [link][to-gettext]              |
| libunibreak    | evas                          |                           | [link][to-libunibreak]          |
|                |                               |                           |                                 |
| - MAGE LOADERS |                               |                           |                                 |
| libgif         |                               |                           | [link][to-gif]                  |
| libjpeg        |                               |                           | [link][to-jpeg]                 |
| libpng         |                               |                           | [link][to-png]                  |
| libtiff        |                               |                           | [link][to-tiff]                 |
| libavif        |                               |                           | [link][to-avif]                 |
| libheif        |                               |                           | [link][to-heif]                 |
| librlottie     |                               |                           | [link][to-rlottie]              |
| libexif        |                               |                           | [link][to-exif]                 |
| libwebp        |                               |                           | [link][to-webp]                 |
| libopenjp2     |                               |                           | [link][to-openjp]               |
| libjxl         |                               |                           | [link][to-jxl]                  |
| libqoi         |                               |                           | [link][to-qoi]                  |
|                |                               |                           |                                 |
| - GENERIC LOADERS|                             |                           |                                 |
| libgstreamer   | generic/evas/gst              | -Dgstreamer=true          | [link][to-gstreamer]            |
| libpoppler     | generic/evas/pdf              |                           | [link][to-poppler]              |
| libspectre     | generic/evas/ps               |                           | [link][to-spectre]              |
| libraw         | generic/evas/raw              |                           | [link][to-raw]                  |
| librsvg2       | generic/evas/rsvg             |                           | [link][to-rsvg2]                |
| libzlib        | generic/evas/xcf              |                           | [link][to-zlib]                 |
|                |                               |                           |                                 |
| - LUA          |                               |                           |                                 |
| luajit         |                               | -D                        | [link][to-luajit]               |
| liblua         |                               | -D                        | [link][to-liblua]               |
|                |                               |                           |                                 |
| - X11          |                               |                           |                                 |
| libxcb-keysyms |                               |                           | [link][to-x11-xcb-keysyms]      |
| libx11         |                               |                           | [link][to-x11-libx11]           |
| libxcomposite  |                               |                           | [link][to-x11-xcomposite]       |
| libxcursor     |                               |                           | [link][to-x11-xcursor]          |
| libxdamage     |                               |                           | [link][to-x11-xdamage]          |
| libxext        |                               |                           | [link][to-x11-xext]             |
| libxkbfile     |                               |                           | [link][to-x11-xkbfile]          |
| libxrandr      |                               |                           | [link][to-x11-xrandr]           |
| libxss         |                               |                           | [link][to-x11-xss]              |
| libxtst        |                               |                           | [link][to-x11-xtst]             |
| libxpresent    |                               |                           | [link][to-x11-xpresent]         |
| libxkbcommon   |                               |                           | [link][to-x11-xkbcommon]        |
|                |                               |                           |                                 |
| - MULTIMEDIA   |                               |                           |                                 |
| libasound2     |                               |                           | [link][to-asound]               |
| libvncserver   |                               |                           | [link][to-vncserver]            |
| libsndfile1    |                               |                           | [link][to-sndfile]              |
|                |                               |                           |                                 |
| - GRAPHICS     |                               |                           |                                 |
| libgl1-mesa    |                               |                           | [link][to-mesa]                 |
| libsdl2        |                               |                           | [link][to-sdl]                  |
| wayland        |                               |                           | [link][to-wayland]              |
| xwayland       |                               |                           | [link][to-xwayland]             |
|                |                               |                           |                                 |
| systemd        |                               | -Dsystemd=true            | [link][to-systemd]              |
| libsystemd     |                               | -Dsystemd=true            | [link][to-libsystemd]           |
| udev           | eeze                          | -Deeze=true               | [link][to-udev]                 |
| brotli         |                               |                           | [link][to-brotli]               |
| avahi          | lib/ecore-avahi               |                           | [link][to-avahi]                |
| blkid          |                               |                           | [link][to-blkid]                |
| bluetooth      |                               |                           | [link][to-bluetooth]            |
| bullet         | lib/ephysics                  |                           | [link][to-bullet]               |
| cogl           |                               |                           | [link][to-cogl]                 |
| curl4          | lib/ecore-con/ecore_con_url_curl.c |                      | [link][to-curl4]                |
| dbus           |                               |                           | [link][to-dbus]                 |
| libdbus        |                               |                           | [link][to-libdbus]              |
| harfbuzz       |                               |                           | [link][to-harfbuzz]             |
| ibus           |                               |                           | [link][to-ibus]                 |
| lz4       | lib/email, <br/> modules/evas/image_savers/tgv | -Dembedded-lz4=true | [link][to-lz4]             |
| mount          |                               |                           | [link][to-mount]                |
| pam0g          |                               |                           | [link][to-pam]                  |
| pcre3          |                               |                           | [link][to-pcre]                 |
| proxy          |                               |                           | [link][to-proxy]                |
| pulse          |                               |                           | [link][to-pulse]                |
| scim           |                               |                           | [link][to-scim]                 |
| ssl            |                               |                           | [link][to-ssl]                  |
| libunwind      |                               |                           | [link][to-libunwind]            |
| udisks2        |                               |                           | [link][to-udisks2]              |
| xinerama       |                               |                           | [link][to-xinerama]             |
| libinput       |                               |                           | [link][to-libinput]             |
| libhwy         |                               |                           | [link][to-libhwy]               |
| doxygen        | doc (api)                     | -Ddoc=true                | [link][to-doxygen]              |
| scdoc          | doc (manual)                  | -Ddoc=true                | [link][to-scdoc]                |

[to-fontconfig]: https://
[to-freetype]: https://
[to-fribidi]: https://
[to-iconv]: https://
[to-hyphen]: https://
[to-gettext]: https://
[to-libunibreak]: https://
[to-gif]: https://
[to-jpeg]: https://
[to-png]: https://
[to-tiff]: https://
[to-avif]: https://
[to-heif]: https://
[to-rlottie]: https://
[to-exif]: https://
[to-webp]: https://
[to-openjp]: https://
[to-jxl]: https://
[to-qoi]: https://
[to-gstreamer]: https://
[to-poppler]: https://
[to-spectre]: https://
[to-raw]: https://
[to-rsvg2]: https://
[to-zlib]: https://
[to-luajit]: https://
[to-liblua]: https://
[to-x11-xcb-keysyms]: https://
[to-x11-libx11]: https://
[to-x11-xcomposite]: https://
[to-x11-xcursor]: https://
[to-x11-xdamage]: https://
[to-x11-xext]: https://
[to-x11-xkbfile]: https://
[to-x11-xrandr]: https://
[to-x11-xss]: https://
[to-x11-xtst]: https://
[to-x11-xpresent]: https://
[to-x11-xkbcommon]: https://
[to-asound]: https://
[to-vncserver]: https://
[to-sndfile]: https://
[to-mesa]: https://
[to-sdl]: https://
[to-wayland]: https://
[to-xwayland]: https://
[to-systemd]: https://
[to-libsystemd]: https://
[to-udev]: https://
[to-brotli]: https://
[to-avahi]: https://
[to-blkid]: https://
[to-bluetooth]: https://
[to-bullet]: https://
[to-cogl]: https://
[to-curl4]: https://
[to-dbus]: https://
[to-libdbus]: https://
[to-harfbuzz]: https://
[to-ibus]: https://
[to-lz4]: https://
[to-mount]: https://
[to-pam]: https://
[to-pcre]: https://
[to-proxy]: https://
[to-pulse]: https://
[to-scim]: https://
[to-ssl]: https://
[to-libunwind]: https://
[to-udisks2]: https://
[to-xinerama]: https://
[to-libinput]: https://
[to-libhwy]: https://
[to-doxygen]: https://
[to-scdoc]: https://


<!-- doc: imagemagick ?need? -->

