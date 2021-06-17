#include <stdlib.h>
#include <stdio.h>
#include "xdg-shell-server-protocol.h"
#include "client.h"
#include "desktop.h"
#include "surface.h"
#include "xdg-shell.h"

static const char *y11_desktop_xdg_toplevel_role = "xdg_toplevel";

static void
y11_desktop_xdg_toplevel_protocol_set_parent(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	struct wl_resource *parent_resource
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_title(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	const char *title
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_app_id(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	const char *app_id
)
{}

static void
y11_desktop_xdg_toplevel_protocol_show_window_menu(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	struct wl_resource *seat_resource,
	uint32_t serial,
	int32_t x, int32_t y
)
{}

static void
y11_desktop_xdg_toplevel_protocol_move(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	struct wl_resource *seat_resource,
	uint32_t serial
)
{}

static void
y11_desktop_xdg_toplevel_protocol_resize(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	struct wl_resource *seat_resource,
	uint32_t serial,
	enum xdg_toplevel_resize_edge edge
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_min_size(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	int32_t width, int32_t height
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_max_size(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	int32_t width, int32_t height
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_maximized(
	struct wl_client *wl_client,
	struct wl_resource *wl_resource
)
{}

static void
y11_desktop_xdg_toplevel_protocol_unset_maximized(
	struct wl_client *wl_client,
	struct wl_resource *wl_resource
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_fullscreen(
	struct wl_client *wl_client,
	struct wl_resource *wl_resource,
	struct wl_resource *output_resource
)
{}

static void
y11_desktop_xdg_toplevel_protocol_unset_fullscreen(
	struct wl_client *wl_client,
	struct wl_resource *wl_resource
)
{}

static void
y11_desktop_xdg_toplevel_protocol_set_minimized(
	struct wl_client *wl_client,
	struct wl_resource *wl_resource
)
{}

static const struct xdg_toplevel_interface y11_desktop_xdg_toplevel_implementation = {
	.destroy             = y11_desktop_destroy_request,
	.set_parent          = y11_desktop_xdg_toplevel_protocol_set_parent,
	.set_title           = y11_desktop_xdg_toplevel_protocol_set_title,
	.set_app_id          = y11_desktop_xdg_toplevel_protocol_set_app_id,
	.show_window_menu    = y11_desktop_xdg_toplevel_protocol_show_window_menu,
	.move                = y11_desktop_xdg_toplevel_protocol_move,
	.resize              = y11_desktop_xdg_toplevel_protocol_resize,
	.set_min_size        = y11_desktop_xdg_toplevel_protocol_set_min_size,
	.set_max_size        = y11_desktop_xdg_toplevel_protocol_set_max_size,
	.set_maximized       = y11_desktop_xdg_toplevel_protocol_set_maximized,
	.unset_maximized     = y11_desktop_xdg_toplevel_protocol_unset_maximized,
	.set_fullscreen      = y11_desktop_xdg_toplevel_protocol_set_fullscreen,
	.unset_fullscreen    = y11_desktop_xdg_toplevel_protocol_unset_fullscreen,
	.set_minimized       = y11_desktop_xdg_toplevel_protocol_set_minimized,
};

static void
y11_desktop_xdg_toplevel_resource_destroy(struct wl_resource *resource)
{
	struct y11_desktop_surface *dsurface = wl_resource_get_user_data(resource);

	if (dsurface != NULL) {
		y11_desktop_surface_resource_destroy(resource);
	}
}

static void
y11_desktop_xdg_shell_protocol_create_positioner(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	uint32_t id
)
{
	// TODO:
}

static void
y11_desktop_surface_surface_commited(struct wl_listener *listener, void *data);

static void
y11_desktop_xdg_surface_protocol_get_toplevel(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	uint32_t id
)
{
	struct y11_desktop_surface *dsurface = wl_resource_get_user_data(resource);
	struct y11_surface *ysurface = dsurface->surface;
	struct y11_desktop_xdg_toplevel *toplevel = dsurface->implementation_data;

	ysurface->role_name = y11_desktop_xdg_toplevel_role;

	// // my code
	// toplevel->base.surface_commit_listener.notify = y11_desktop_surface_surface_commited;
	// wl_signal_add(&toplevel->base.surface->commit_signal, &toplevel->base.surface_commit_listener);
	// // end

	toplevel->resource = y11_desktop_surface_add_resource(
		toplevel->base.desktop_surface,
		&xdg_toplevel_interface,
		&y11_desktop_xdg_toplevel_implementation,
		id, y11_desktop_xdg_toplevel_resource_destroy
	);

	if (toplevel->resource == NULL)
		return;

	toplevel->base.role = Y11_DESKTOP_XDG_SURFACE_ROLE_TOPLEVEL;
}

static void
y11_desktop_xdg_surface_protocol_get_popup(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	uint32_t id,
	struct wl_resource *parent_resource,
	struct wl_resource *positioner_resource
)
{}

static void
y11_desktop_xdg_surface_protocol_set_window_geometry(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	int32_t x, int32_t y,
	int32_t width, int32_t height
)
{}

static void
y11_desktop_xdg_surface_protocol_ack_configure(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	uint32_t serial
)
{}

static const struct xdg_surface_interface y11_desktop_xdg_surface_implementation = {
	.destroy             = y11_desktop_destroy_request,
	.get_toplevel        = y11_desktop_xdg_surface_protocol_get_toplevel,
	.get_popup           = y11_desktop_xdg_surface_protocol_get_popup,
	.set_window_geometry = y11_desktop_xdg_surface_protocol_set_window_geometry,
	.ack_configure       = y11_desktop_xdg_surface_protocol_ack_configure,
};

static void
y11_desktop_xdg_surface_resource_destroy(struct wl_resource *resource)
{
	struct y11_desktop_surface *dsurface =
		wl_resource_get_user_data(resource);

	if (dsurface != NULL)
	  y11_desktop_surface_resource_destroy(resource);
}

// my code
static void
y11_desktop_surface_surface_commited(struct wl_listener *listener, void *data)
{
	struct y11_desktop_xdg_surface *surface = // y11_desktop_surfaceを経由しないことにした。
		wl_container_of(listener, surface, surface_commit_listener);

	struct wl_array states;
	wl_array_init(&states);

	switch (surface->role)
	{
	case Y11_DESKTOP_XDG_SURFACE_ROLE_NONE:
	  // TODO
		break;

	case Y11_DESKTOP_XDG_SURFACE_ROLE_TOPLEVEL:
		xdg_toplevel_send_configure(((struct y11_desktop_xdg_toplevel*) surface)->resource, 0, 0, &states);
		break;

	case Y11_DESKTOP_XDG_SURFACE_ROLE_POPUP:
	  // TODO
		break;
	}

	uint32_t serial = wl_display_next_serial(surface->desktop->compositor->wl_display);

	xdg_surface_send_configure(surface->resource, serial);
}

static void
y11_desktop_xdg_shell_protocol_get_xdg_surface(
	struct wl_client *wl_client,
	struct wl_resource *resource,
	uint32_t id,
	struct wl_resource *surface_resource
)
{
	struct y11_desktop_client *client = wl_resource_get_user_data(resource);
	struct y11_surface *y11_surface = wl_resource_get_user_data(surface_resource);
	struct y11_desktop_xdg_surface *surface;

	surface = calloc(1, sizeof(struct y11_desktop_xdg_toplevel));
	if (surface == NULL) {
		wl_client_post_no_memory(wl_client);
		return;
	}

	surface->desktop = y11_desktop_client_get_desktop(client);
	surface->surface = y11_surface;

	surface->desktop_surface = y11_desktop_surface_create(surface->desktop, client, surface->surface, surface);

	// my code
	surface->surface_commit_listener.notify = y11_desktop_surface_surface_commited;
	wl_signal_add(&surface->surface->commit_signal, &surface->surface_commit_listener);
	// end

	surface->resource = y11_desktop_surface_add_resource(
		surface->desktop_surface,
		&xdg_surface_interface,
		&y11_desktop_xdg_surface_implementation,
		id, y11_desktop_xdg_surface_resource_destroy
	);
}

static void
y11_desktop_xdg_shell_protocol_pong(struct wl_client *wl_client, struct wl_resource *resource, uint32_t serial)
{
	struct y11_desktop_client *client = wl_resource_get_user_data(resource);
	y11_desktop_client_pong(client, serial);
}

static const struct xdg_wm_base_interface y11_desktop_xdg_shell_implementation = {
	.destroy = y11_desktop_destroy_request,
	.create_positioner = y11_desktop_xdg_shell_protocol_create_positioner,
	.get_xdg_surface = y11_desktop_xdg_shell_protocol_get_xdg_surface,
	.pong = y11_desktop_xdg_shell_protocol_pong,
};

static void
y11_desktop_xdg_shell_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct y11_desktop *desktop = data;
	y11_desktop_client_create(
		desktop, client, NULL,
		&xdg_wm_base_interface,
		&y11_desktop_xdg_shell_implementation,
		version, id
	);
}

struct wl_global *
y11_desktop_xdg_wm_base_create(struct y11_desktop *desktop, struct wl_display *display)
{
	return wl_global_create(display, &xdg_wm_base_interface, 1, desktop, y11_desktop_xdg_shell_bind);
}
