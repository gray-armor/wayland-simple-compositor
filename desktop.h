#ifndef Y11_DESKTOP
#define Y11_DESKTOP

#include <wayland-server.h>
#include "compositor.h"

void
y11_desktop_destroy_request(struct wl_client *client, struct wl_resource *resource);

struct y11_desktop {
	struct y11_compositor *compositor;
	void *user_data;
	struct wl_global *xdg_wm_base;
	struct wl_global *wl_shell;
};

struct y11_desktop *
y11_desktop_create(struct y11_compositor *compositor, void *user_data);

#endif // Y11_DESKTOP
