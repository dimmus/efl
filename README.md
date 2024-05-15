# Main goals (wishes :)

- complete `wayland` support in EFL
- push forward EFL objects support (i mean `eo`)
- documentation
- speed and size optimization

# Problems

- hugh problem with development documentation. It's mainly outdated 
  and user level.
- some libs naming is too esoteric. Most of the names (lib purpose) are 
  intransparent for necomers. In combination with the lack of development 
  documentation dev start becomes the swamp.

# So, ... intermidiate goal

- deep dive and understand EFL internals

# Tasks

To understand something i should brake it! So, to begin with ..

- create **transparent** lib names. Systematic renaming.
- general and consistent EFL_* (re)naming (like EFL_UNUSED, 
  Efl_Bool, libefl_core_*, libefl_shared, etc).
- create test system **without** `check` usage
- create diverse documentation: man files, manuals, wiki, coockbooks, api, etc.
  Some templates from different projects are provided.
- make thorough **code analysis** system. Some templates from different 
  projects are provided.
- optimization: flags, architectures, compilers, memory management. It needs 
  detailed analysis of needness (by build and run times, sizes on  disk and 
  in memory, etc).
- make seperate development **tools**. Some templates provided.
- **meson** build system update. It's too large, so, may be try to atomize it.
- automatic and consistent formating: create rules and apply clang-format
- use the power of github ci system

# Current tasks

See [TODO](./TODO) file. PRs, sugestions and any discussion are wellcome.
