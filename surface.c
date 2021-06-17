#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wayland-server.h>
#include <pixman.h>
#include "desktop.h"
#include "client.h"
#include "region.h"
#include "surface.h"

struct wl_resource *
y11_desktop_surface_add_resource(
  struct y11_desktop_surface *surface,
  const struct wl_interface *interface,
  const void *implementation, uint32_t id,
  wl_resource_destroy_func_t destroy
)
{
  struct wl_resource *client_resource = surface->client->resource;
  struct wl_client *wl_client = surface->client->client;
  struct wl_resource *resource;

  resource = wl_resource_create(wl_client, interface, wl_resource_get_version(client_resource), id);

  if (resource == NULL) {
    wl_client_post_no_memory(wl_client);
    free(surface); // more in weston_desktop_surface_destroy
    return NULL;
  }

  if (destroy == NULL)
    destroy = y11_desktop_surface_resource_destroy;

  wl_resource_set_implementation(resource, implementation, surface, destroy);
  wl_list_insert(&surface->resource_list, wl_resource_get_link(resource));

  return resource;
}

static void
y11_desktop_surface_destroy(struct y11_desktop_surface *surface)
{
  if (!wl_list_empty(&surface->resource_list)) {
    struct wl_resource *resource, *tmp;
    wl_resource_for_each_safe(resource, tmp, &surface->resource_list) {
      wl_resource_set_user_data(resource, NULL);
      wl_list_remove(wl_resource_get_link(resource));
    }
  }
  free(surface);
}

void
y11_desktop_surface_resource_destroy(struct wl_resource *resource)
{
  struct y11_desktop_surface *surface =
    wl_resource_get_user_data(resource);

  if (surface != NULL)
    y11_desktop_surface_destroy(surface);
}

struct y11_desktop_surface *
y11_desktop_surface_create(
  struct y11_desktop *desktop,
  struct y11_desktop_client *client,
  struct y11_surface *ysurface,
  void *implementation_data
)
{
  struct y11_desktop_surface *surface;

  surface = calloc(1, sizeof(struct y11_desktop_surface));
  if (surface == NULL) return NULL;

  surface->desktop = desktop;
  surface->implementation_data = implementation_data;
  surface->surface = ysurface;
  surface->client = client;

  wl_list_init(&surface->resource_list);

  return surface;
}

static void
surface_destroy(struct wl_client *client, struct wl_resource *resource)
{
  wl_resource_destroy(resource);
}

static void
y11_buffer_destroy_handler(struct wl_listener *listener, void *data)
{
  struct y11_buffer *buffer =
    wl_container_of(listener, buffer, destroy_listener);
  free(buffer);
}

struct y11_buffer *
y11_buffer_from_resource(struct wl_resource *resource)
{
  struct y11_buffer *buffer;
  struct wl_listener *listener;

  listener = wl_resource_get_destroy_listener(resource, y11_buffer_destroy_handler);
  if (listener)
    return wl_container_of(listener, buffer, destroy_listener);

  buffer = calloc(1, sizeof *buffer);
  if (buffer == NULL)
    return NULL;

  buffer->resource = resource;
  buffer->destroy_listener.notify = y11_buffer_destroy_handler;
  wl_resource_add_destroy_listener(resource, &buffer->destroy_listener);

  return buffer;
}

static void
y11_surface_state_set_buffer(struct y11_surface_state *state, struct y11_buffer *buffer)
{
  if (state->buffer == buffer)
    return;
  state->buffer = buffer;
}

static void
surface_attach(struct wl_client *client, struct wl_resource *resource, struct wl_resource *buffer_resource, int32_t sx, int32_t sy)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);
  struct y11_buffer *buffer = NULL;

  if (buffer_resource) {
    buffer = y11_buffer_from_resource(buffer_resource);
    if (buffer == NULL) {
      wl_client_post_no_memory(client);
      return;
    }
  }

  y11_surface_state_set_buffer(&surface->pending, buffer);

  surface->pending.sx = sx;
  surface->pending.sy = sy;
  surface->pending.newly_attached = 1;
}

static void
surface_damage(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);

  if (width <= 0 || height <= 0)
    return;

  pixman_region32_union_rect(
    &surface->pending.damage_surface,
    &surface->pending.damage_surface,
    x, y, width, height
  );
}

static void
destroy_frame_callback(struct wl_resource *resource)
{
//   struct y11_fame_callback *cb = wl_resource_get_user_data(resource);
  // TODO if needed
}

static void
surface_frame(struct wl_client *client, struct wl_resource *resource, uint32_t callback)
{
  struct y11_frame_callback *cb;
  struct y11_surface *surface = wl_resource_get_user_data(resource);

  cb = malloc(sizeof *cb);
  if (cb == NULL){
    wl_resource_post_no_memory(resource);
    return;
  }

  cb->resource = wl_resource_create(client, &wl_callback_interface, 1, callback);

  if (cb->resource == NULL) {
    free(cb);
    wl_resource_post_no_memory(resource);
    return;
  }


  wl_resource_set_implementation(cb->resource, NULL, cb, destroy_frame_callback);
  wl_list_insert(surface->pending.frame_callback_list.prev, &cb->link);
}

static void
surface_set_opaque_region(struct wl_client *client, struct wl_resource *resource, struct wl_resource *region_resource)
{
  // struct y11_surface *surface = wl_resource_get_user_data(resource);
  // struct y11_region *region;
  // if (region_resource) {
    // region = wl_resource_get_user_data(region_resource);
    // pixman_region32_copy(&surface->pending.opaque, &region->region);
  // } else {
    // pixman_region32_clear(&surface->pending.opaque);
  // }
}

static void
region_init_infinite(pixman_region32_t *region)
{
  pixman_region32_init_rect(region, INT32_MIN, INT32_MIN, UINT32_MAX, UINT32_MAX);
}

static void
surface_set_input_region(struct wl_client *client, struct wl_resource *resource, struct wl_resource *region_resource)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);
  struct y11_region *region;
  if (region_resource) {
    region = wl_resource_get_user_data(region_resource);
    pixman_region32_copy(&surface->pending.input, &region->region);
  } else {
    pixman_region32_fini(&surface->pending.input);
    region_init_infinite(&surface->pending.input);
  }
}

static void
surface_commit(struct wl_client *client, struct wl_resource *resource)
{
  // TODO
  struct y11_surface *surface = wl_resource_get_user_data(resource);
  wl_signal_emit(&surface->commit_signal, surface);

  // my code
  wl_list_init(&surface->pending.frame_callback_list);
}

static void
surface_set_buffer_transform(struct wl_client *client, struct wl_resource *resource, int transform)
{
  // TODO
}

static void
surface_set_buffer_scale(struct wl_client *client, struct wl_resource *resource, int32_t scale)
{
  // TODO
}

static void
surface_damage_buffer(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y, int32_t width, int32_t height)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);

  if (width <= 0 || height <= 0)
    return;

   pixman_region32_union_rect(&surface->pending.damage_buffer, &surface->pending.damage_buffer, x, y ,width, height);
}

static const struct wl_surface_interface surface_interface = {
  surface_destroy,
  surface_attach,
  surface_damage,
  surface_frame,
  surface_set_opaque_region,
  surface_set_input_region,
  surface_commit,
  surface_set_buffer_transform,
  surface_set_buffer_scale,
  surface_damage_buffer
  // TODO
};

struct y11_surface *
y11_surface_create(struct y11_compositor *compositor)
{
  struct y11_surface *surface;

  surface = calloc(1, sizeof *surface);
  if (surface == NULL) return NULL;

  wl_signal_init(&surface->commit_signal);
  surface->compositor = compositor;
  return surface;
}


void
y11_surface_destroy(struct y11_surface *surface)
{
  free(surface);
}

static void
destroy_surface(struct wl_resource *resource)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);
  assert(surface);

  surface->resource = NULL;
  y11_surface_destroy(surface);
}

void
compositor_create_surface(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  struct y11_compositor *com = wl_resource_get_user_data(resource);
  struct y11_surface *surface;

  surface = y11_surface_create(com);
  if (surface == NULL) {
    wl_resource_post_no_memory(resource);
    return;
  }

  surface->resource =
    wl_resource_create(client, &wl_surface_interface, wl_resource_get_version(resource), id);

  if (surface->resource == NULL) {
    y11_surface_destroy(surface);
    wl_resource_post_no_memory(resource);
  }

  wl_resource_set_implementation(surface->resource, &surface_interface, surface, destroy_surface);
}
