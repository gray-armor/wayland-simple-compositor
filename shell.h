#ifndef Y11_SHELL_H
#define Y11_SHELL_H

#include <wayland-server.h>
#include "compositor.h"
#include "desktop.h"

struct y11_shell {
	struct y11_compositor *compositor;
	struct y11_desktop *desktop;
	struct wl_listener destroy_listener;
};

int
y11_shell_init(struct y11_compositor *com);

#endif // Y11_SHELL_H
