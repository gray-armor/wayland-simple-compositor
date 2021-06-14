#include <stdlib.h>
#include "desktop.h"
#include "client.h"

void
y11_desktop_client_pong(struct y11_desktop_client *client, uint32_t serial)
{
	// TODO if needed
}

void
y11_desktop_client_destroy(struct y11_desktop_client *client)
{
	free(client);
}

static void
y11_desktop_client_handle_destroy(struct wl_resource *resource)
{
	struct y11_desktop_client *client = wl_resource_get_user_data(resource);
	client->resource = NULL;
	y11_desktop_client_destroy(client);
}

struct y11_desktop_client *
y11_desktop_client_create(
	struct y11_desktop *desktop,
	struct wl_client *wl_client,
	wl_dispatcher_func_t dispatcher,
	const struct wl_interface *interface,
	const void *implementation, uint32_t version,
	uint32_t id
)
{
	struct y11_desktop_client *client;
	// struct wl_display *display;

	client = calloc(1, sizeof(struct y11_desktop_client));
	if (client == NULL) {
		if (wl_client != NULL)
			wl_client_post_no_memory(wl_client);
		return NULL;
	}

	client->desktop = desktop;
	client->client = wl_client;

	client->resource = wl_resource_create(wl_client, interface, version, id);
	if (client->resource == NULL) {
		wl_client_post_no_memory(wl_client);
		free(client);
		return NULL;
	}

	// there was a path in which dispatcher is not NULL

	wl_resource_set_implementation(client->resource, implementation,client, y11_desktop_client_handle_destroy);

	return client;
}

struct y11_desktop *
y11_desktop_client_get_desktop(struct y11_desktop_client *client)
{
	return client->desktop;
}
