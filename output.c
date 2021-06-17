#include <stdlib.h>
#include <wayland-server.h>
#include "output.h"
#include "compositor.h"

static void
output_release(struct wl_client *client, struct wl_resource *resource)
{
	wl_resource_destroy(resource);
}

static const struct wl_output_interface output_interface = {
	output_release,
};

static void
bind_output(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct y11_head *head = data;
	struct wl_resource *resource;

	resource = wl_resource_create(client, &wl_output_interface, version, id);

	wl_resource_set_implementation(resource, &output_interface, head, NULL);

	wl_output_send_geometry(
		resource,
		0, // x
		0, // y
		480, // physical width
		270, // physical height
		0,
		"GSM",
		"LG IPS FULLHD",
		0
	);

	wl_output_send_scale(resource, 1);
	wl_output_send_mode(resource, 3, 1920, 1080, 60000);
	wl_output_send_done(resource);
}

void
y11_output_init(struct y11_compositor *compositor)
{
	struct y11_head *head;
	head = calloc(1, sizeof(struct y11_head));
	head->compositor = compositor;
	wl_global_create(compositor->wl_display, &wl_output_interface, 3, head, bind_output);
}
