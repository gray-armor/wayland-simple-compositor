#ifndef Y11_OUTPUT_H
#define Y11_OUTPUT_H

#include "compositor.h"

struct y11_head {
	struct y11_compositor *compositor;
	struct wl_global *global;
};

void
y11_output_init(struct y11_compositor *compositor);

#endif // Y11_OUTPUT_H
