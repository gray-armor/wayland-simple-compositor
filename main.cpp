#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <errno.h>
#include <wayland-server.h>

int main (int argc, char *argv[]) {
  wl_display *display = wl_display_create();
  assert(display);

  const char *socket = wl_display_add_socket_auto(display);
  assert(socket);

  printf("Hello, Wayland!\n");
  wl_display_run(display);

  return 0;
}
