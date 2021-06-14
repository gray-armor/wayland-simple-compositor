#ifndef Y11_CLIENT_H
#define Y11_CLIENT_H

#include "desktop.h"

struct y11_desktop_client {
	struct y11_desktop *desktop;
	struct wl_client *client;
	struct wl_resource *resource;
};

struct y11_desktop_client *
y11_desktop_client_create(
	struct y11_desktop *desktop,
	struct wl_client *wl_client,
	wl_dispatcher_func_t dispatcher,
	const struct wl_interface *interface,
	const void *implementation, uint32_t version,
	uint32_t id
);

void
y11_desktop_client_pong(struct y11_desktop_client *client, uint32_t serial);

struct y11_desktop *
y11_desktop_client_get_desktop(struct y11_desktop_client *client);

#endif // Y11_CLIENT_H
