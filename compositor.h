#ifndef Y11_COMPOSITOR_H
#define Y11_COMPOSITOR_H

#include <wayland-server.h>

struct y11_compositor {
  struct wl_display *wl_display;
  struct wl_event_loop *loop;
};

bool
y11_compositor_add_destroy_listener_once(
  struct y11_compositor *compositor,
  struct wl_listener *listener,
  wl_notify_func_t destroy_handler
);

void
compositor_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id);

#endif // Y11_COMPOSITOR_H
