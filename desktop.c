// libweston-desktop.c

#include <stdlib.h>
#include <wayland-server.h>
#include "compositor.h"
#include "desktop.h"
#include "xdg-shell.h"

void
y11_desktop_destroy_request(struct wl_client *client, struct wl_resource *resource)
{
	wl_resource_destroy(resource);
}

struct y11_desktop *
y11_desktop_create(struct y11_compositor *compositor, void *user_data)
{
	struct y11_desktop *desktop;
	struct wl_display *display = compositor->wl_display;

	desktop = calloc(1, sizeof(struct y11_desktop));
	desktop->compositor = compositor;
	desktop->user_data = user_data;

	desktop->xdg_wm_base = y11_desktop_xdg_wm_base_create(desktop, display);

	return desktop;
}
