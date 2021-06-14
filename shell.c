#include <stdlib.h>
#include "compositor.h"
#include "client.h"
#include "shell.h"

static void
shell_destroy(struct wl_listener *listener, void *data)
{
  struct y11_shell *shell = wl_container_of(listener, shell, destroy_listener);
  wl_list_remove(&shell->destroy_listener.link);
  // TODO remove others
  free(shell);
}

int
y11_shell_init(struct y11_compositor *com)
{
  struct y11_shell *shell;
  shell = calloc(1, sizeof *shell);

  if (shell == NULL)
    return -1;

  shell->compositor = com;

  if (!y11_compositor_add_destroy_listener_once(com, &shell->destroy_listener, shell_destroy)) {
    free(shell);
    return 0;
  }

  shell->desktop = y11_desktop_create(com, shell);

  // a lot more in libweston-desktop

  return 1;
}
