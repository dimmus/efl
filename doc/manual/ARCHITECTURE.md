---
title: systemd Repository Architecture
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# The systemd Repository Architecture

## Code Map

This document provides a high-level overview of the various components of the EFL repository.

## Source Code

Directories in `src/` provide the implementation of all binaries and libraries shipped by the project.
There are many, and more are constantly added, so we will not enumerate them all here — the directory names are self-explanatory.

### Shared Code

The code in `src/lib/shared` is shared between components is split into a few directories, each with a different purpose:

- `src/lib/shared/checksum` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/data` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/debug` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/log` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/memory` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/safe` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/string` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/thread` — those directories contain code primitives that are used by all other code.

- `src/lib/shared/win32` — those directories contain code primitives that are used by all other code.

You might wonder what kind of code belongs where.
In general, the rule is that code should be linked as few times as possible, ideally only once.
Thus code that is used by "higher-level" components (e.g. our binaries which are linked to `libefl-shared-<nnn>.so`),
would go to a subdirectory specific to that component if it is only used there.
If the code is to be shared between components, it'd go to `src/lib/shared/`.

### Libraries

There are many [components](https://github.com/dimmus/efl/wiki/EFL-components) inside EFL. 
They also build various things like shared libraries, loadable plug-in modules and also binary
executables. Different elements will end up being licensed differently too.

All library source is in the `src/lib/` directory. All binaries are in
`src/bin/`. All loadable modules are in `src/modules/`. All data files are
in the `data/` directory. Licensing details are listed in `licences/README.md` and
the `licenses/` directory

  - `src/lib/core`
    This is the core main-loop, system events and execution layer. This
    handles running the main loop, integrating with external data and
    timing sources (the system clock, file descriptors, system signals),
    and producing an event queue, walking that queue and dispatching
    events to appropriate callbacks.

  - `src/lib/core_audio`
    This library provides an API for audio playback and recording. It uses
    pulse audio underneath to handle mixing and policies. The API for this
    should not be considered stable right now because it relies on EO and
    EO is not considered finalized yet.

  - `src/lib/core_cocoa`
    This provides wrappers/abstractions around Max OS-X Cocoa APIs to help
    Mac porting.

  - `src/lib/core_con`
    This provides a completely event-based TCP/UDP and Unix domain socket
    API that integrates with the main-loop. This means no blocking to send
    or receive data, supporting "infinite send buffers" with storage and
    spooling being done by Ecore Con. It also supports SSL encryption
    transparently turned on or not on the same connection, certificate
    verification, CURL wrapping for HTTP connection usage (GETs, POSTs
    etc.), asynchronous DNS lookups and provides the ability to also be a
    server, not just a client, with the same event-based API.

  - `src/lib/core_evas`
    This acts as glue between the display target (X11, Wayland,
    Frame buffer, Cocoa on OSX, Win32 etc.) and Evas. It creates/provides a
    target for Evas to render to (a Window or Surface etc.) and feeds
    input events (Keyboard, Mouse, Multi-touch) into Evas, which then
    selects the target object and calls the callbacks. It also provides
    wrappers/glue for manipulating the Window/Surface.

  - `src/lib/core_fb`
    This provides virtual terminal allocation, access and handling,
    frame buffer information, raw input handling for keyboard, mouse and
    touch (via tslib).

  - `src/lib/core_file`
    This provides file access convenience APIs for doing simple file
    operations like renames, copies, listing directories and more. It also
    supports file change monitoring and URL downloads.

  - `src/lib/core_imf`
    This is an input method abstraction framework to allow EFL to talk to
    things like SCIM, IBus, Wayland and XIM. This allows for complex text
    entry in languages such as Chinese, Japanese and Korean.

  - `src/lib/core_imf_evas`
    This library glues Input method support from Ecore IMF and Evas
    together.

  - `src/lib/core_input`
    This acts as a generic input layer where multiple display systems can
    post events in the same format to the event queue.

  - `src/lib/core_input_evas`
    This Routes events from Ecore Input into a given Evas canvas which
    will then route the event further to the destination object and
    callbacks.

  - `src/lib/core_ipc`
    This acts as a layer on top of Ecore Con which handles entire IPC
    message packets, dealing with header delta compression and
    portability, as well as ensuring an entire message is received in one
    go as a single IPC message, regardless of payload data size. The
    entire API is event based almost exactly like Ecore Con and thus it
    supports all the transport layers Ecore Con supports.

  - `src/lib/core_sdl`
    This acts as a wrapper/glue around SDL to handle SDL Windows as well
    as input events from SDL and tie them to the Ecore main-loop and event
    queue.

  - `src/lib/core_wayland`
    This is a glue/wrapper library to interface EFL to Wayland libraries
    to tie them into the Ecore main-loop and event queue.

  - `src/lib/core_win32`
    This acts as glue/wrapper around Windows Win32 APIs to tie them into
    the Ecore main-loop and event queue.

  - `src/lib/core_x`
    This is a library to wrap/deal with Xlib make dealing with X11 less painful
    and less footwork as well as being glue to tie these into the Ecore main-loop
    and event queue.

  - `src/lib/edje`
    This is a graphics event, input, theme, animation and theme
    abstraction library used to place many UI/UX elements into data files
    (called edj files) that define how to react to incoming
    events/signals, and define animation time lines, states, relative
    scalable layout and much much more. It comes with a compiler that
    compiles source "edc" files into "edj" files (which are actually just
    data files managed by Eet). These "edj" files are fully portable and
    can work on any OS/Architecture just like a JPEG or PNG might be
    portable.

  - `src/lib/eet`
    This library is a data storage, encoding and decoding library
    designed to be extremely compact, fast and easy to use. It can take
    data structures directly from memory and serialize them portably to
    disk, then de-serialize them right back to the same data structures in
    memory, handling allocation and population of memory all for you in 1
    call. It handles indirections such as pointers, linked lists, arrays
    and hash tables too, so almost any level of data structure complexity
    can be easily saved and loaded back. It is used for "edj" files by
    Edje as well as all configuration in Enlightenment, Elementary and
    Terminology. It supports encryption of data too via SSL, signing of
    files, as well as various compression techniques. It also supports
    encoding and decoding of image data in lossless or lossy form.

  - `src/lib/eeze`
    This library acts as an abstraction to discovering hardware interfaces
    for sensors as well as removable media and much more.

  - `src/lib/efreet`
    This library provides code for handling Freedesktop.org standards such
    as .desktop files, XDG Menus, Icon search paths and more. It provides
    a central daemon to handle monitoring for changes that the library
    talks to, and the daemon handles updating local cache files the
    library reads.

  - `src/lib/shared` (former `eina`)
    This library provides low-level routines for common things like linked
    lists, hash tables, growable arrays, basic string buffers, shared
    string tokens, mmaped() file access, thread abstraction and locking,
    memory pools, copy-on-write segments, iterators, matrices, general
    data models, red/black trees, quad-trees, a simple SAX XML parser and
    more.

  - `src/lib/eio`
    This is an asynchronous I/O library for doing disk I/O without blocking.

  - `src/lib/eldbus`
    This is a DBus access library to allow you to create DBus services as
    well as clients. This glues in DBus into the main-loop so all access is
    asynchronous.

  - `src/lib/embryo`
    This is a small C-like language compiler and byte-code interpreter
    library. This is used for scripting in Edje. The code is based on
    original source from the Pawn/Small Language but has been made
    portable (endianness issues fixed) and 64bit issues fixed, with the
    runtime library being refactored to be extremely small.

  - `src/lib/emotion`
    This is a wrapper around Gstreamer 1.x pluggable decoder libraries
    This glues in the decoder library, and its output into a smart Evas object
    that will display the playback for you as the video plays, as well as
    providing higher level controls to seek, play, pause and query the stream
    regardless of the back-end used.

  - `src/lib/eo`
    This is a core object system API that EFL depend on. The API is not finalized, 
    so do not depend on it yet, but future EFL versions will lock it down.
    This object system does simple and multiple inheritance, refcounting,
    strong and weak references, auto-deletion of child objects, unifies
    callback handling with a single path, and also abstracts object
    pointers to be indirect table lookups for increased safety at runtime.

  - `src/lib/physics`
    This library provides a wrapper around the Bullet physics library,
    allowing for it to be linked directly with Evas objects and control
    their behavior as if they were real physical objects. This is now
    disabled by default as it's rarely if ever used by anything.

  - `src/lib/thumb`
    This library provides core API for a thumbnailing daemon as well as
    the thumbnail daemon itself. Ethumb Client talks with Ethumb to pass
    off thumbnail generation to a central location to be done
    asynchronously.

  - `src/lib//thumb_client`
    This is the client-side part of Ethumb that provides an API for
    clients to request the Ethumb thumbnailer to generate or find cached
    thumbnails of files.

  - `src/lib/evas`
    This is the core rendering and scene graph abstraction library for
    EFL. It manages a stateful 2D scene graph that defines the entire
    content of any canvas. This supplies rendering back-ends for many
    display systems like X11, Windows, Wayland, Frame-buffer etc. and via
    many rendering APIs like OpenGL, OpenGL-ES 2, and pure software
    implementations that are fast and accurate.

  - `src/lib/evil`
    This library acts as a porting library for Windows to provide missing
    libc calls not in Mingw32 that EFL needs. It is used internally and
    no symbol is public.

### Fuzzing

Fuzzers are a type of unit tests that execute code on an externally-supplied input sample.
Fuzzers are called `fuzz-*`.
Fuzzers live under `tests/fuzz/`, and those for other parts of the codebase should be located next to the code they test.

Files under `test/fuzz/` contain input data for fuzzers, one subdirectory for each fuzzer.
Some of the files are "seed corpora", i.e. files that contain lists of settings and input values intended to generate initial coverage, and other files are samples saved by the fuzzing engines when they find an issue.

When adding new input samples under `test/fuzz/*/`, please use some short-but-meaningful names.
Names of meson tests include the input file name and output looks awkward if they are too long.

Fuzzers are invoked primarily in three ways:
firstly, each fuzzer is compiled as a normal executable and executed for each of the input samples under `test/fuzz/` as part of the test suite.
Secondly, fuzzers may be instrumented with sanitizers and invoked as part of the test suite (if `-Dfuzz-tests=true` is configured).
Thirdly, fuzzers are executed through fuzzing engines that tryto find new "interesting" inputs through coverage feedback and massive parallelization; see the links for oss-fuzz in [Code quality](/CODE_QUALITY).
For testing and debugging, fuzzers can be executed as any other program, including under `valgrind` or `gdb`.

## Integration Tests

Sources in `tests` implement testing for executables, libraries and tools that are shipped by the project.

If the require privileges to run are not safe to execute directly on a host they will build an image and run the test under it via `qemu`.

Most of those tests should be able to run via `meson test -C build`, which is orders-of-magnitude faster than `qemu`.

See [`tests/README.testsuite`](https://github.com/dimmus/efl/blob/main/tests/README.testsuite) for more specific details.

## Documentation

### efl.io

Markdown files found under `docs/api` are automatically published on the [efl.io](https://efl.io) website using Github Pages.
A minimal unit test to ensure the formatting doesn't have errors is included in the `meson test -C build/ github-pages` run as part of the CI.

### Man pages

Manpages for binaries and libraries, and the DBUS interfaces, can be found under `docs/man/` and should ideally be kept in sync with changes to the corresponding binaries and libraries.

### Translations

Translations files for binaries and libraries, provided by volunteers, can be found under `po/` in the usual format.
They are kept up to date by contributors and by automated tools.

## Utilities for Developers

`tools/`, `coccinelle/`, `.github/`, `.semaphore/`, `.mkosi/` host various utilities and scripts that are used by maintainers and developers.
They are not shipped or installed.

