---
title: Testing EFL Using Valgrind
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# Testing EFL Using Valgrind

To run EFL under valgrind, compile EFL with the valgrind
development headers available (i.e. valgrind-devel or equivalent).
Otherwise, false positives will be triggered by code which violates
some rules but is actually safe. Note that valgrind generates nice
output only on exit(), hence on shutdown we don't execve()
systemd-shutdown.