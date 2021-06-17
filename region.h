#ifndef Y11_REGION_H
#define Y11_REGION_H

#include <pixman.h>

struct y11_region {
	struct  wl_resource *resource;
	pixman_region32_t region;
};

void
compositor_create_region(struct wl_client *client, struct wl_resource *resource, uint32_t id);

#endif // Y11_REGION_H
