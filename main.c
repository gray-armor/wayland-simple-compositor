#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <wayland-server.h>
#include "compositor.h"
#include "shell.h"

#define VERSION "0.1"

int
main (int argc, char *argv[])
{
  printf("VERSION %s\n", VERSION);

  struct y11_compositor *com;
  com = malloc(sizeof *com);
  com->wl_display = wl_display_create();
  assert(com->wl_display);

  const char *socket = wl_display_add_socket_auto(com->wl_display);
  assert(socket);

  wl_global_create(com->wl_display, &wl_compositor_interface, 4, com, compositor_bind);

  y11_shell_init(com);

  com->loop = wl_display_get_event_loop(com->wl_display);

  wl_display_init_shm(com->wl_display);

  printf("Hello, Wayland!\n");
  wl_display_run(com->wl_display);

  return 0;
}
