<div align="center">
    <a href=https://www.enlightenment.org/about-efl>Enlightenment Foundation Library</a>
</div>

# Personal goals (wishes :)

- get complete `wayland` support in EFL
- push forward EFL objects support (i mean, `eo`)
- documentation
- speed and size optimization

# Actual problems (my personal drivers :)

- huge problem with existing development documentation (see [doc](https://www.enlightenment.org/develop/start.md) here).
  It's in some places outdated and user level only.
- some libs naming is too esoteric. Most of the names (understand like a lib purpose) are 
  non-transparent for newcomers. In combination with the lack of development
  documentation, the start of EFL development becomes a swamp.

# So, ... my intermidiate goal

- deep dive, understand and master EFL internals

# Tasks

To understand something, i should break it! So, to begin with ..

- create **transparent** lib names. Systematic renaming. See [changelog_api](./changelog_api.md)
- general and consistent EFL_* (re)naming (like EFL_UNUSED, 
  Efl_Bool, libefl_core_*, libefl_shared, etc).
- ~~create test system **without** `check` usage~~ (it was a bad idea, we need infastructure to init some stuff before test itself)
- create diverse documentation: man files, manuals, wiki, coockbooks, api, etc.
  Some templates from different projects are provided.
- make thorough **code analysis** system. Some templates from different 
  projects are provided.
- optimization: flags, architectures, compilers, memory management. It needs 
  detailed analysis of neediness (by build and run times, sizes on  disk and 
  in memory, etc).
- make separate development **tools**. Some templates were provided.
- **meson** build system update. It's too large, so, may be try to atomize it.
- automatic and consistent formatting: create rules and apply clang-format
- use the power of the github CI system

# Current tasks

See [TODO](./TODO) file. 

Pull requests, sugestions, and any discussions are welcome.

# Build

To build, i use helper script `tools/utils_maintenance/build.sh`.
For convenience, create a symlink to `run` in the source root folder:
```bash
ln -s tools/utils_maintenance/build.sh run
```

Basic usage: 

`./run`       — run without arguments to show all options

`./run s`     — run meson default setup

`./run s dev` — run meson setup with dev options (see build.sh)

`./run c`     — compile with default setup

`./run c dev` — compile with dev options

`./run cc`    — continue compilation (without build cleanup and reconfiguration)

`./run t`     — run tests

`./run f`     — apply formatting

## GitHub Stats

![Alt](https://repobeats.axiom.co/api/embed/51a92f91a34df2e929bb45d75b54e1bb8b09aa11.svg "Repobeats analytics image")
