wayland_protocol_inc  = include_directories('.')
wayland_scanner_bin   = find_program('wayland-scanner')
wayland_protocols_dep = dependency('wayland-protocols')
wayland_client_dep    = dependency('wayland-client')

protocols_dir = wayland_protocols_dep.get_variable('pkgdatadir')
assert(protocols_dir != '', 'Could not get pkgdatadir from wayland-protocols.pc')

wayland_protocol_server_headers = []
wayland_protocol_client_headers = []
wayland_protocol_sources        = []
# wayland_protocol_files          = {}

wayland_protocols = [
  
### Stable upstream protocols
  [ 'xdg-shell',                  'stable',        ],  # wlroot, mutter, weston, efl
# [ 'linux-dmabuf',               'stable',   'v1' ],  # wlroot, mutter,       , 
# [ 'presentation-time',          'stable',        ],  # wlroot, mutter, weston, 
# [ 'viewporter',                 'stable',        ],  # wlroot, mutter, weston, 
  
### Staging upstream protocols
# [ 'fractional-scale',           'staging',  'v1' ],  # wlroot, mutter, weston, 
# [ 'single-pixel-buffer',        'staging',  'v1' ],  # wlroot, mutter, weston, 
# [ 'xdg-activation',             'staging',  'v1' ],  # wlroot, mutter,       , 
# [ 'xdg-toplevel-drag',          'staging',  'v1' ],  #       ,       ,       ,
# [ 'tearing-control',            'staging',  'v1' ],  # wlroot,       , weston, 
# [ 'xwayland-shell',             'staging',  'v1' ],  # wlroot,       , weston, 
# [ 'content-type',               'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'cursor-shape',               'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'drm-lease',                  'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'ext-foreign-toplevel-list',  'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'ext-idle-notify',            'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'ext-session-lock',           'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'ext-transient-seat',         'staging',  'v1' ],  # wlroot,       ,       ,
# [ 'security-context',           'staging',  'v1' ],  # wlroot,       ,       ,
  
### Unstable upstream protocols
# [ 'idle-inhibit',               'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'keyboard-shortcuts-inhibit', 'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'pointer-constraints',        'unstable', 'v1' ],  # wlroot, mutter, weston,
# [ 'pointer-gestures',           'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'primary-selection',          'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'relative-pointer',           'unstable', 'v1' ],  # wlroot, mutter, weston,
# [ 'tablet',                     'unstable', 'v2' ],  # wlroot, mutter, weston,
  [ 'text-input',                 'unstable', 'v1' ],  #       ,       , weston, efl
# [ 'text-input',                 'unstable', 'v3' ],  # wlroot, mutter,       , 
  [ 'xdg-shell',                  'unstable', 'v6' ],  #       ,       , weston, efl
# [ 'xdg-foreign',                'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'xdg-foreign',                'unstable', 'v2' ],  # wlroot, mutter,       ,
# [ 'xdg-output',                 'unstable', 'v1' ],  # wlroot, mutter, weston,
# [ 'xwayland-keyboard-grab',     'unstable', 'v1' ],  # wlroot, mutter,       ,
# [ 'fullscreen-shell',           'unstable', 'v1' ],  # wlroot,       , weston,
# [ 'input-method',               'unstable', 'v1' ],  #       ,       , weston,
# [ 'input-method',               'unstable', 'v2' ],  # wlroot,       ,       ,
# [ 'input-timestamps',           'unstable', 'v1' ],  #       ,       , weston,
  [ 'linux-dmabuf',               'unstable', 'v1' ],  #       ,       , weston, efl
# [ 'linux-explicit-synchronization','unstable','v1'], #       ,       , weston,

### Other protocols (to take into account)
  [ 'efl-aux-hints',              'internal',      ],  #       ,       ,       , efl
  [ 'efl-hints',                  'internal',      ],  #       ,       ,       , efl
  [ 'session-recovery',           'internal',      ],  #       ,       ,       , efl
# [ 'gtk-shell',                  'internal',      ],  #       , mutter,       ,
# [ 'linux-drm-syncobj',          'internal', 'v1' ],  #       , mutter,       ,
# [ 'color-management',           'internal', 'v1' ],  #       ,       , weston,
# [ 'ivi-application',            'internal',      ],  #       ,       , weston,
# [ 'ivi-hmi-controller',         'internal',      ],  #       ,       , weston,
# [ 'text-cursor-position',       'internal',      ],  #       ,       , weston,
# [ 'weston-debug',               'internal',      ],  #       ,       , weston,
# [ 'weston-desktop-shell',       'internal',      ],  #       ,       , weston,
# [ 'weston-output-capture',      'internal',      ],  #       ,       , weston,
# [ 'weston-content-protection',  'internal',      ],  #       ,       , weston,
# [ 'weston-test',                'internal',      ],  #       ,       , weston,
# [ 'weston-touch-calibration',   'internal',      ],  #       ,       , weston,
# [ 'weston-direct-display',      'internal',      ],  #       ,       , weston,
# [ 'wlr-data-control',           'internal', 'v1' ],  # wlroot,       ,       ,
# [ 'wlr-export-dmabuf',          'internal', 'v1' ],  # wlroot,       ,       ,
# [ 'wlr-foreign-toplevel-management','internal','v1'],# wlroot,       ,       ,
# [ 'wlr-gamma-control',          'internal', 'v1' ],  # wlroot,       ,       ,
# [ 'wlr-layer-shell',            'internal', 'v1' ],  # wlroot,       ,       ,
# [ 'wlr-output-management',      'internal', 'v1' ],  # wlroot,       ,       ,
# [ 'wlr-output-power-management','internal', 'v1' ],  # wlroot,       ,       ,
# [ 'virtual-keyboard',           'unstable', 'v1' ],  # wlroot,       ,       ,
# [ 'drm',                        'internal',      ],  # wlroot,       ,       ,
]

foreach p: wayland_protocols
  protocol_name    = p.get(0)
  protocol_type    = p.get(1)

  if p.length() == 3
    protocol_version = p.get(2)
  endif

  if protocol_type == 'stable'
    if p.length() == 3 # 'stable' but with version
      output_base = '@0@-@1@'.format(protocol_name, protocol_version)
    else
      output_base = protocol_name
    endif
    xml_path = protocols_dir / protocol_type / protocol_name / '@0@.xml'.format(output_base)
  elif protocol_type == 'staging'
    output_base = '@0@-@1@'.format(protocol_name, protocol_version)
    xml_path = protocols_dir / protocol_type / protocol_name / '@0@.xml'.format(output_base)
  elif protocol_type == 'unstable'
    output_base = '@0@-unstable-@1@'.format(protocol_name, protocol_version)
    xml_path = protocols_dir / protocol_type / protocol_name / '@0@.xml'.format(output_base)
  elif protocol_type == 'internal'
    output_base = protocol_name
    xml_path = '@0@.xml'.format(protocol_name)
  else
    output_base = '@0@-@1@-@2@'.format(protocol_name, protocol_type, protocol_version)
    xml_path = protocols_dir / protocol_type / protocol_name / '@0@.xml'.format(output_base)
  endif

  server_header = custom_target('@0@ server header'.format(output_base),
    input: xml_path,
    output: '@0@-server-protocol.h'.format(output_base),
    command: [ wayland_scanner_bin, 'server-header', '@INPUT@', '@OUTPUT@' ]
  )

  client_header = custom_target('@0@ client header'.format(output_base),
    input: xml_path,
    output: '@0@-client-protocol.h'.format(output_base),
    command: [ wayland_scanner_bin, 'client-header', '@INPUT@', '@OUTPUT@' ]
  )

  protocol_code = custom_target('@0@ source'.format(output_base),
    input: xml_path,
    output: '@0@-protocol.c'.format(output_base),
    command: [ wayland_scanner_bin, 'private-code', '@INPUT@', '@OUTPUT@' ]
  )

  # wayland_protocol_files += {
  #   protocol_name: {
  #     'server-header': server_header,
  #     'client-header': client_header,
  #     'protocol-code': protocol_code,
  #   },
  # }

  wayland_protocol_server_headers += server_header
  wayland_protocol_client_headers += client_header
  wayland_protocol_sources += protocol_code
endforeach

wayland_protocol_headers = wayland_protocol_server_headers + wayland_protocol_client_headers

wayland_protocol_lib = static_library('wayland_protocol',
    wayland_protocol_headers + wayland_protocol_sources,
)

wayland_protocol = declare_dependency(
	include_directories:  wayland_protocol_inc,
	link_with:            wayland_protocol_lib,
  sources:              wayland_protocol_headers,
  dependencies :        wayland_client_dep
)

wayland_protocol_simple = declare_dependency(
  include_directories:  wayland_protocol_inc,
  sources:              wayland_protocol_headers,
)