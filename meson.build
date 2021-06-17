project('wayland-compositor', 'c')

wl_server = dependency('wayland-server')
pixman = dependency('pixman-1')

scanner = dependency('wayland-scanner')
scanner_path = scanner.get_pkgconfig_variable('wayland_scanner')

xdg_shell_server_protocol_h = custom_target(
  'xdg-shell-server-protocol',
  output: 'xdg-shell-server-protocol.h',
  input: '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
  command: [scanner_path, 'server-header', '@INPUT@', '@OUTPUT@']
)

xdg_shell_protocol_c = custom_target(
  'xdg-shell-protocol',
  output: 'xdg-shell-protocol.c',
  input: '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
  command: [scanner_path, 'code', '@INPUT@', '@OUTPUT@']
)

executable(
  'server',
  [
    'main.c',
    'surface.c', 'surface.h',
    'compositor.c', 'compositor.h',
    'shell.c', 'shell.h',
    'desktop.c', 'desktop.h',
    'xdg-shell.c', 'xdg-shell.h',
    'client.c', 'client.h',
    'output.c', 'output.h',
    'region.c', 'region.h',
    xdg_shell_protocol_c, xdg_shell_server_protocol_h
  ],
  dependencies: [wl_server, pixman],
  # dependencies: [wl_server, compositor_dep, shell_dep],
)
