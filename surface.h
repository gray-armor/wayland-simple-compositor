#ifndef Y11_SURFACE_H
#define Y11_SURFACE_H

#include <wayland-server.h>
#include <pixman.h>
#include "desktop.h"
#include "client.h"
#include "compositor.h"

struct y11_buffer {
  struct wl_resource *resource;
  struct wl_listener destroy_listener;
};

struct y11_desktop_surface {
  struct y11_desktop *desktop;
  struct y11_desktop_client *client;
  struct y11_surface *surface;
  struct wl_list resource_list;
  void *implementation_data;
};

struct y11_surface_state {
  int newly_attached;
  struct y11_buffer *buffer;
  int32_t sx;
  int32_t sy;
  pixman_region32_t damage_surface;
  pixman_region32_t damage_buffer;
  pixman_region32_t opaque;
  pixman_region32_t input;
  struct wl_list frame_callback_list;
};

struct y11_surface {
  struct wl_resource *resource;
  struct y11_compositor *compositor;
  struct y11_surface_state pending;

  const char *role_name;
};

struct y11_frame_callback {
  struct wl_resource *resource;
  struct wl_list link;
};

struct y11_region {
  struct wl_resource *resource;
  pixman_region32_t region;
};

void
compositor_create_surface(struct wl_client *client, struct wl_resource *resource, uint32_t id);

struct y11_desktop_surface *
y11_desktop_surface_create(
  struct y11_desktop *desktop,
  struct y11_desktop_client *client,
  struct y11_surface *ysurface,
  void *implementation_data
);

struct wl_resource *
y11_desktop_surface_add_resource(
  struct y11_desktop_surface *surface,
  const struct wl_interface *interface,
  const void *implementation, uint32_t id,
  wl_resource_destroy_func_t destroy
);

void
y11_desktop_surface_resource_destroy(struct wl_resource *resource);

#endif // Y11_SURFACE_H
