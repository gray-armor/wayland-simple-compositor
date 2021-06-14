#ifndef Y11_XDG_SHELL
#define Y11_XDG_SHELL

#include <wayland-server.h>
#include "desktop.h"
#include "y11.h"
#include "surface.h"

enum y11_desktop_xdg_surface_role {
	Y11_DESKTOP_XDG_SURFACE_ROLE_NONE,
	Y11_DESKTOP_XDG_SURFACE_ROLE_TOPLEVEL,
	Y11_DESKTOP_XDG_SURFACE_ROLE_POPUP,
};

struct y11_desktop_xdg_surface {
	struct wl_resource *resource;
	struct y11_desktop *desktop;
	struct y11_surface *surface;
	struct y11_desktop_surface *desktop_surface;

	enum y11_desktop_xdg_surface_role role;
};

struct y11_desktop_xdg_toplevel_state {
	bool maximaized;
	bool fullscreen;
	bool resizing;
	bool activated;
};

struct y11_desktop_xdg_toplevel {
	struct y11_desktop_xdg_surface base;
	struct wl_resource *resource;
	bool added;
	struct {
		struct y11_desktop_xdg_toplevel_state state;
		struct y11_size size;
	} pending;
	struct {
		struct y11_desktop_xdg_toplevel_state state;
		struct y11_size size;
		struct y11_size min_size, max_size;
	} next;
	struct {
		struct y11_desktop_xdg_toplevel_state state;
		struct y11_size min_size, max_size;
	} current;
};

struct wl_global *
y11_desktop_xdg_wm_base_create(struct y11_desktop *desktop, struct wl_display *display);

#endif
