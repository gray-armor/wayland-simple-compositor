#include <wayland-server.h>
#include "compositor.h"
#include "surface.h"
#include "region.h"

static const struct wl_compositor_interface compositor_interface = {
  compositor_create_surface,
  compositor_create_region
};

bool
y11_compositor_add_destroy_listener_once(
  struct y11_compositor *compositor,
  struct wl_listener *listener,
  wl_notify_func_t destroy_handler
)
{
  listener->notify = destroy_handler;
  return true;
}

void
compositor_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_compositor *compositor = data;
  struct wl_resource *resource;
  resource = wl_resource_create(client, &wl_compositor_interface, version, id);
  if (resource == NULL) {
    wl_client_post_no_memory(client);
    return;
  }
  wl_resource_set_implementation(resource, &compositor_interface, compositor, NULL);
}
