#include "app.h"
#include "core.h"
#include "core.internal.h"
#include "input.evdev.h"
#include "input.h"
#include "xdg-shell.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <linux/input-event-codes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h>
#include <xkbcommon/xkbcommon.h>

typedef struct Core {
	CoreInternal internal;
	App_t app;

	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct xdg_wm_base *shell;
	struct wl_surface *surface;
	struct xdg_surface *shellSurface;
	struct xdg_toplevel *toplevel;
	struct wl_seat *seat;
	struct wl_keyboard *kb;
	struct wl_pointer *mouse;
	struct xkb_context *xkbCtx;
	bool noKeymap;
	struct xkb_keymap *keymap;
	const char *keymap_str;

	struct wl_egl_window *eglWindow;
	EGLDisplay eglDisplay;
	EGLSurface eglSurface;
	EGLContext eglContext;
	EGLConfig eglConfig;

	bool running;
	bool resize;
	bool readyToResize;
	int32_t newWidth;
	int32_t newHeight;

	// mouse position for button events
	wl_fixed_t mouseX;
	wl_fixed_t mouseY;
} Core;

CoreInternal_t coreGetInternal(Core_t self) {
	return &self->internal;
}

static void coreInitWindow(Core_t self);
static void coreInitGLContext(Core_t self);
static void coreFreeGLContext(Core_t self);
static void coreFreeWindow(Core_t self);
static void coreRequestFrame(Core_t self);

Core_t coreNew() {
	Core_t core = (Core_t)malloc(sizeof(Core));
	if (!core) return 0;

	memset(core, 0, sizeof(Core));
	core->newWidth = 1280;
	core->newHeight = 720;
	core->running = true;

	coreInitWindow(core);
	coreInitGLContext(core);
	coreInternalCtor(&core->internal);
	coreInternalOnResize(&core->internal, core->newWidth, core->newHeight);

	return core;
}
void coreDelete(Core_t *core) {
	if (!core || !*core) return;

	coreInternalDtor(&(*core)->internal);
	coreFreeGLContext(*core);
	coreFreeWindow(*core);
	memset(*core, 0, sizeof(Core));
	free(*core);
	*core = 0;
}
void coreRun(Core_t self, App_t app) {
	self->internal.app = app;
	glViewport(0, 0, self->newWidth, self->newHeight);
	coreRequestFrame(self);
	while (self->running) {
		if (self->resize && self->readyToResize) {
			wl_egl_window_resize(self->eglWindow, self->newWidth, self->newHeight, 0, 0);
			glViewport(0, 0, self->newWidth, self->newHeight);

			self->readyToResize = false;
			self->resize = false;

			// OnResize
			coreInternalOnResize(&self->internal, self->newWidth, self->newHeight);

			wl_surface_commit(self->surface);
		}
		wl_display_dispatch(self->display);
	}
}

static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
static const struct wl_registry_listener registry_listener = {
	.global = registry_global,
	.global_remove = NULL
};
static void shell_ping(void *data, struct xdg_wm_base *shell, uint32_t serial);
static const struct xdg_wm_base_listener shell_listener = {
	.ping = shell_ping
};
static void shell_surface_configure(void *data, struct xdg_surface *shellSurface, uint32_t serial);
static const struct xdg_surface_listener shell_surface_listener = {
	.configure = shell_surface_configure
};
static void toplevel_configure(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height, struct wl_array *states);
static void toplevel_close(void *data, struct xdg_toplevel *toplevel);
static const struct xdg_toplevel_listener toplevel_listener = {
	.configure = toplevel_configure,
	.close = toplevel_close,
	.configure_bounds = NULL,
	.wm_capabilities = NULL
};
static void callback_done(void *data, struct wl_callback *frame_cb, uint32_t serial);
static const struct wl_callback_listener callback_listener = {
	.done = callback_done
};
static void seat_capabilities(void *data, struct wl_seat *seat, uint32_t capabilities);
static const struct wl_seat_listener seat_listener = {
	.capabilities = seat_capabilities
};
static void kb_keymap(void *data, struct wl_keyboard *kb, uint32_t format, int32_t fd, uint32_t size);
static void kb_enter(void *data, struct wl_keyboard *kb, uint32_t serial, struct wl_surface *surface, struct wl_array *keys);
static void kb_leave(void *data, struct wl_keyboard *kb, uint32_t serial, struct wl_surface *surface);
static void kb_key(void *data, struct wl_keyboard *kb, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
static void kb_modifiers(void *data, struct wl_keyboard *kb, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
static void kb_repeat_info(void *data, struct wl_keyboard *kb, int32_t rate, int32_t delay);
static const struct wl_keyboard_listener kb_listener = {
	// key maps
	.keymap = kb_keymap,
	// keyboard focus
	.enter = kb_enter,
	.leave = kb_leave,
	// input events
	.key = kb_key,
	.modifiers = kb_modifiers,
	// key repeat
	.repeat_info = kb_repeat_info
};
static void mouse_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial,  struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
static void mouse_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial,  struct wl_surface *surface);
static void mouse_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
static void mouse_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
static void mouse_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
static void mouse_frame(void *data, struct wl_pointer *wl_pointer);
static void mouse_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source);
static void mouse_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis);
static void mouse_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete);
static void mouse_axis_value120(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t value120);
static void mouse_axis_relative_direction(void *data, struct wl_pointer *wl_pointer, uint32_t axis, uint32_t direction);
static const struct wl_pointer_listener mouse_listener = {
	.enter = mouse_enter,
	.leave = mouse_leave,
	.motion = mouse_motion,
	.button = mouse_button,
	.axis = mouse_axis,
	.frame = mouse_frame,
	.axis_source = mouse_axis_source,
	.axis_stop = mouse_axis_stop,
	.axis_discrete = mouse_axis_discrete,
	.axis_value120 = mouse_axis_value120,
	.axis_relative_direction = mouse_axis_relative_direction
};

static void coreInitWindow(Core_t self) {
	self->display = wl_display_connect(NULL);
	if (!self->display) {
		fprintf(stderr, "Fatal error Wayland: Failed to connect to display\n");
		exit(1);
	}

	// Get the wl_registry
	self->registry = wl_display_get_registry(self->display);
	if (!self->registry) {
		fprintf(stderr, "Fatal error Wayland: Failed to get registry\n");
		exit(1);
	}

	// Add global listener to wl_registry
	wl_registry_add_listener(self->registry, &registry_listener, self);
	wl_display_roundtrip(self->display);
	if (!self->compositor) {
		fprintf(stderr, "Fatal error Wayland: There's no supported wl_compositor_interface in your wayland compositor\n");
		exit(1);
	}
	if (!self->shell) {
		fprintf(stderr, "Fatal error Wayland: Your wayland compositor doesn't support xdg_wm_base\n");
		exit(1);
	}

	// Add ping listener to xdg_wm_base
	xdg_wm_base_add_listener(self->shell, &shell_listener, NULL);

	// Add seat listener to listen keyboard input
	if (!self->seat) {
		fprintf(stderr, "Error: wl_seat is not provided\n");
	}
	else {
		wl_seat_add_listener(self->seat, &seat_listener, self);
	}

	// Create wl_surface
	self->surface = wl_compositor_create_surface(self->compositor);
	if (!self->surface) {
		fprintf(stderr, "Fatal error Wayland: Failed to create surface\n");
		exit(1);
	}
	// Create xdg_surface
	self->shellSurface = xdg_wm_base_get_xdg_surface(self->shell, self->surface);
	if (!self->shellSurface) {
		fprintf(stderr, "Fatal error Wayland: Couldn't get xdg surface\n");
		exit(1);
	}

	// Add resize completed listener for xdg_surface
	xdg_surface_add_listener(self->shellSurface, &shell_surface_listener, self);

	// Get xdg_toplevel
	self->toplevel = xdg_surface_get_toplevel(self->shellSurface);
	// Add resize and close listener to toplevel
	xdg_toplevel_add_listener(self->toplevel, &toplevel_listener, self);

	// Commit
	wl_surface_commit(self->surface);
	wl_display_roundtrip(self->display);

	// Listen to keyboard
	if (self->kb != 0) {
		self->xkbCtx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
		wl_keyboard_add_listener(self->kb, &kb_listener, self);
	}
	// Listen to mouse
	if (self->mouse != 0) {
		wl_pointer_add_listener(self->mouse, &mouse_listener, self);
	}
}
static void coreFreeWindow(Core_t self) {
	if (self->mouse) wl_pointer_destroy(self->mouse);
	if (self->kb) wl_keyboard_destroy(self->kb);
	xkb_context_unref(self->xkbCtx);
	xdg_toplevel_destroy(self->toplevel);
	xdg_surface_destroy(self->shellSurface);
	wl_surface_destroy(self->surface);
	wl_seat_destroy(self->seat);
	xdg_wm_base_destroy(self->shell);
	wl_registry_destroy(self->registry);
	wl_display_disconnect(self->display);
}
static void coreInitGLContext(Core_t self) {
	self->eglDisplay = eglGetDisplay(self->display);
	if (self->eglDisplay == EGL_NO_DISPLAY) {
		fprintf(stderr, "Fatal error OpenGL: Failed to get EGL display\n");
		exit(1);
	}
	EGLint major, minor;
	if (!eglInitialize(self->eglDisplay, &major, &minor)) {
		fprintf(stderr, "Fatal error OpenGL: Failed to initialize EGL\n");
		exit(1);
	}
	if (!(((major == 1) && (minor >= 4)) || (major > 1))) {
		fprintf(stderr, "Fatal error OpenGL: Version 1.4 or higher required\n");
		exit(1);
	}
	if (!eglBindAPI(EGL_OPENGL_API)) {
		fprintf(stderr, "Fatal error OpenGL: Failed to bind OpenGL API\n");
		exit(1);
	}
	static const EGLint config_attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
		//EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	EGLint num = 0;
	if (!eglChooseConfig(self->eglDisplay, config_attribs, &self->eglConfig, 1, &num) || !num) {
		fprintf(stderr, "Fatal error OpenGL: Failed to choose EGL config\n");
		exit(1);
	}

	self->eglWindow = wl_egl_window_create(self->surface, self->newWidth, self->newHeight);
	if (!self->eglWindow) {
		fprintf(stderr, "Fatal error OpenGL: Failed to create EGL window\n");
		exit(1);
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	EGLint context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		//EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	self->eglContext = eglCreateContext(self->eglDisplay, self->eglConfig, EGL_NO_CONTEXT, context_attribs);
	if (self->eglContext == EGL_NO_CONTEXT) {
		fprintf(stderr, "Fatal error OpenGL: Failed to create EGL context\n");
		exit(1);
	}
	self->eglSurface = eglCreateWindowSurface(self->eglDisplay, self->eglConfig, (EGLNativeWindowType)self->eglWindow, NULL);
	if (self->eglSurface == EGL_NO_SURFACE) {
		fprintf(stderr, "Fatal error OpenGL: Failed to create EGL surface\n");
		exit(1);
	}
	if (eglMakeCurrent(self->eglDisplay, self->eglSurface, self->eglSurface, self->eglContext) == EGL_FALSE) {
		fprintf(stderr, "Fatal error OpenGL: Failed to make EGL context current\n");
		exit(1);
	}

	// print GL version
	fprintf(stdout, "Info: OpenGL version \"%s\"\n", glGetString(GL_VERSION));

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
}
static void coreFreeGLContext(Core_t self) {
	eglMakeCurrent(self->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	eglDestroySurface(self->eglDisplay, self->eglSurface);
	wl_egl_window_destroy(self->eglWindow);
}
static void coreRequestFrame(Core_t self) {
	coreInternalFrame(&self->internal);
	eglSwapBuffers(self->eglDisplay, self->eglSurface);

	struct wl_callback *frame_callback = wl_surface_frame(self->surface);
	wl_callback_add_listener(frame_callback, &callback_listener, self);
}

static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
	Core_t core = (Core_t)data;
	if (strcmp(interface, wl_compositor_interface.name) == 0) {
		core->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
	}
	else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
		core->shell = (struct xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
	}
	else if (strcmp(interface, wl_seat_interface.name) == 0) {
		core->seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 1);
	}
};
static void shell_ping(void *data, struct xdg_wm_base *shell, uint32_t serial) {
	(void)data;
	xdg_wm_base_pong(shell, serial);
}
static void shell_surface_configure(void *data, struct xdg_surface *shellSurface, uint32_t serial) {
	Core_t core = (Core_t)data;
	xdg_surface_ack_configure(shellSurface, serial);
	if (core->resize) {
		core->readyToResize = true;
	}
}
static void toplevel_configure(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height, struct wl_array *states) {
	Core_t core = (Core_t)data;
	if (width > 0 && height > 0 && (core->newWidth != width || core->newHeight != height)) {
		core->resize = true;
		core->newWidth = width;
		core->newHeight = height;
	}
}
static void toplevel_close(void *data, struct xdg_toplevel *toplevel) {
	Core_t core = (Core_t)data;
	core->running = false;
}
static void callback_done(void *data, struct wl_callback *frame_cb, uint32_t serial) {
	(void)frame_cb;
	(void)serial;
	Core_t core = (Core_t)data;

	coreRequestFrame(core);
}
static void seat_capabilities(void *data, struct wl_seat *seat, uint32_t capabilities) {
	Core_t core = (Core_t)data;
	if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
		core->kb = wl_seat_get_keyboard(seat);
	}
	else {
		fprintf(stderr, "Warning: keyboard might not work\n");
	}
	if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
		core->mouse = wl_seat_get_pointer(seat);
	}
	else {
		fprintf(stderr, "Warning: mice might not work\n");
	}
}
static void kb_keymap(void *data, struct wl_keyboard *kb, uint32_t format, int32_t fd, uint32_t size) {
	(void)kb;
	Core_t core = (Core_t)data;
	if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
		core->noKeymap |= true;
		fprintf(stderr, "Error [keyboard]: wrong keymap format\n");
		return;
	}
	char *map_shm = (char*)mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map_shm == MAP_FAILED) {
		core->noKeymap |= true;
		fprintf(stderr, "Error [keyboard]: no keymap\n");
		return;
	}
	core->keymap = xkb_keymap_new_from_string(core->xkbCtx, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	munmap(map_shm, size);
	close(fd);
}
static void kb_enter(void *data, struct wl_keyboard *kb, uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
	(void)data;
	(void)kb;
	(void)serial;
	(void)surface;
	(void)keys;
}
static void kb_leave(void *data, struct wl_keyboard *kb, uint32_t serial, struct wl_surface *surface) {
	(void)data;
	(void)kb;
	(void)serial;
	(void)surface;
}
static void kb_key(void *data, struct wl_keyboard *kb, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
	(void)kb;
	(void)serial;
	(void)time;
	Core_t core = (Core_t)data;
	const uint32_t keyHid = evdevToHID(key);
	if (state) {
		coreInternalOnKeyDown(&core->internal, keyHid);
	}
	else {
		coreInternalOnKeyUp(&core->internal, keyHid);
	}
}
static void kb_modifiers(void *data, struct wl_keyboard *kb, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
	(void)data;
	(void)kb;
	(void)serial;
	(void)mods_depressed;
	(void)mods_latched;
	(void)mods_locked;
	(void)group;
}
static void kb_repeat_info(void *data, struct wl_keyboard *kb, int32_t rate, int32_t delay) {
	(void)data;
	(void)kb;
	(void)rate;
	(void)delay;
}
static void mouse_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial,  struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	(void)wl_pointer; // probably should differentiate multiple pointing devices
	(void)serial;
	(void)surface; // the engine have only one window
	Core_t core = (Core_t)data;
	core->mouseX = surface_x;
	core->mouseY = surface_y;
	coreInternalOnMouseMove(&core->internal, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));
}
static void mouse_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial,  struct wl_surface *surface) {
	(void)data;
	(void)wl_pointer;
	(void)serial;
	(void)surface;
}
static void mouse_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	(void)wl_pointer;
	(void)time;
	Core_t core = (Core_t)data;
	core->mouseX = surface_x;
	core->mouseY = surface_y;
	coreInternalOnMouseMove(&core->internal, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));
}
static void mouse_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
	(void)wl_pointer;
	(void)serial;
	(void)time;
	Core_t core = (Core_t)data;
	uint32_t inputButton = 0;
	switch (button) {
	case BTN_LEFT: inputButton = MB_LEFT; break;
	case BTN_RIGHT: inputButton = MB_RIGHT; break;
	case BTN_MIDDLE: inputButton = MB_MIDDLE; break;
	case BTN_SIDE: inputButton = MB_X1; break;
	case BTN_EXTRA: inputButton = MB_X2; break;
	default: return;
	}
	if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
		coreInternalOnMouseDown(&core->internal, inputButton, wl_fixed_to_double(core->mouseX), wl_fixed_to_double(core->mouseY));
	}
	else if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
		coreInternalOnMouseUp(&core->internal, inputButton, wl_fixed_to_double(core->mouseX), wl_fixed_to_double(core->mouseY));
	}
}
// Scroll (TODO)
static void mouse_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
	(void)data;
	(void)wl_pointer;
	(void)time;
	(void)axis;
	(void)value;
}
static void mouse_frame(void *data, struct wl_pointer *wl_pointer) {
	(void)data;
	(void)wl_pointer;
}
static void mouse_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
	(void)data;
	(void)wl_pointer;
	(void)axis_source;
}
static void mouse_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
	(void)data;
	(void)wl_pointer;
	(void)time;
	(void)axis;
}
static void mouse_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
	(void)data;
	(void)wl_pointer;
	(void)axis;
	(void)discrete;
}
static void mouse_axis_value120(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t value120) {
	(void)data;
	(void)wl_pointer;
	(void)axis;
	(void)value120;
}
static void mouse_axis_relative_direction(void *data, struct wl_pointer *wl_pointer, uint32_t axis, uint32_t direction) {
	(void)data;
	(void)wl_pointer;
	(void)axis;
	(void)direction;
}

