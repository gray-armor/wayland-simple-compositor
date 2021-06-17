#include <wayland-server.h>
#include <stdlib.h>
#include "region.h"

static void
destroy_region(struct wl_resource *resource)
{
	struct y11_region *region = wl_resource_get_user_data(resource);
	// pixman_region32_fini(&region->region);
	free(region);
}

static void
region_destroy(struct wl_client *client, struct wl_resource *resource)
{
	wl_resource_destroy(resource);
}

static void
region_add(struct wl_client *client, struct wl_resource *resource,
								int32_t x, int32_t y, int32_t width, int32_t height)
{}

static void
region_subtract(struct wl_client *client, struct wl_resource *resource,
								int32_t x, int32_t y, int32_t width, int32_t height)
{}

static const struct wl_region_interface region_interface = {
	region_destroy,
	region_add,
	region_subtract
};

void
compositor_create_region(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  struct y11_region *region;

  region = malloc(sizeof *region);
  if (region == NULL) {
    wl_resource_post_no_memory(resource);
    return;
  }

  region->resource = wl_resource_create(client, &wl_region_interface, 1, id);

	if (region->resource == NULL) {
		free(region);
		wl_resource_post_no_memory(resource);
		return;
	}

	wl_resource_set_implementation(region->resource, &region_interface, region, destroy_region);
}
