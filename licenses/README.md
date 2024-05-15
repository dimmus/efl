# EFL Project Licensing

EFL comes with several licences. Different shared libraries are
covered by different licenses, and are distributed here as an
aggregation with a single build system. Listed below are the library/project
names and the license file covering it. Note that this license would
cover all of the source invovled in each project (src/lib/PROJECT,
src/modules/PROJECT, src/bin/PROJECT data/PROJECT) unless specifically
noted otherwise (eg epp in edje or lz4, libunibreak etc.), so please
refer to these imported project sources for details (but be aware that
the licensing of such libraries is compatible with the libraries that
use them and is not more restrictive).

## Main License

The EFL project uses single-line references to Unique License Identifiers as
defined by the Linux Foundation's SPDX project (https://spdx.org/). The line in
each individual source file identifies the license applicable to that file.

The current set of valid, predefined SPDX identifiers can be found on the SPDX
License List at https://spdx.org/licenses/.

The 'licenses/' directory contains all the licenses used by the sources included in
the EFL project source tree.

Unless otherwise noted, the EFL project sources are licensed under the terms
and conditions of the **GNU Lesser General Public License v2.1 or later**.

New sources that cannot be distributed under LGPL-2.1-or-later will no longer
be accepted for inclusion in the systemd project to maintain license uniformity.

## Licenses

  * BSD 2-Clause license
    - Ecore
    - Ecore Audio
    - Ecore Cocoa
    - Ecore Con
    - Ecore Evas
    - Ecore Fb
    - Ecore File
    - Ecore IMF
    - Ecore IMF Evas
    - Ecore Input
    - Ecore Input Evas
    - Ecore IPC
    - Ecore SDL
    - Ecore Wayland
    - Ecore Win32
    - Ecore X
    - Edje
    - Eet
    - Eeze
    - Efreet
    - Emotion
    - EO
    - EPhysics
    - Evas
    - Evil
    - Elua

  * LGPL
    - Eina
    - Eio
    - Eldbus
    - Ethumb
    - Emile
    - Elementary

  * Small license (same as ZLib license)
    - Embryo

  * FTL and GPL (dual licenses)
    - Ector

NOTE: If linked together, the result will be effectively be LGPL (or
GPL if escape is used) due to license characteristics.