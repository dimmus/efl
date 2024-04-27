---
title: Build EFL
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# Build EFL from source

To build in directory build/:
    meson setup build/ && ninja -C build/

Any configuration options can be specified as -Darg=value... arguments
to meson. After the build directory is initially configured, meson will
refuse to run again, and options must be changed with:
    meson configure -Darg=value build/
meson configure without any arguments will print out available options and
their current values.

Useful commands:
    ninja -C build -v some/target
    meson test -C build/
    sudo meson install -C build/ --no-rebuild
    DESTDIR=... meson install -C build/

A tarball can be created with:
    v=250 && git archive --prefix=efl-$v/ v$v | zstd >efl-$v.tar.zstd

Additional packages are necessary to run some tests:
- nc                 (used by test/TEST-12-ISSUE-3171)
- python             (test-udev which is installed is in python)
- python-pyparsing
- python-evdev       (used by hwdb parsing tests)
- strace             (used by test/test-functions)
- capsh              (optional, used by test-execute)