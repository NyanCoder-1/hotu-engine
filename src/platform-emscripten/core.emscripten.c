#include "app.h"
#include "core.h"
#include "core.internal.h"
#include "input.em.h"
#include <emscripten/em_types.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Core {
	CoreInternal internal;
	EmInput_t input;
} Core;

CoreInternal_t coreGetInternal(Core_t self) {
	return &self->internal;
}

static void coreMainLoop(Core_t self);
static void coreInitEmscriptenStuff(Core_t self);
static void coreInitGLContext(Core_t self);
static bool emResizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData);
static bool emKeyEventCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData);
static bool emMouseEventCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);

Core_t coreNew() {
	Core_t core = (Core_t)malloc(sizeof(Core));
	if (!core) return 0;

	memset(core, 0, sizeof(Core));
	core->input = emInputNew();
	coreInitEmscriptenStuff(core);
	coreInitGLContext(core);
	coreInternalCtor(&core->internal);

	return core;
}
void coreDelete(Core_t *self) {
	if (!self || !*self) return;

	coreInternalDtor(&(*self)->internal);
	emInputDelete(&(*self)->input);
	memset(*self, 0, sizeof(Core));
	free(*self);
	*self = 0;
}
void coreRun(Core_t self, App_t app) {
	self->internal.app = app;
	emscripten_set_main_loop_arg((em_arg_callback_func)coreMainLoop, self, 0, EM_TRUE);
}

static void coreMainLoop(Core_t self) {
	if (!self) return;
	
	coreInternalFrame(&self->internal);
}
EM_JS(void, mapCanvas, (void), {
	specialHTMLTargets["!canvas"] = Module.canvas;
})
static void coreInitEmscriptenStuff(Core_t self) {
	mapCanvas();
	// TODO: set callbacks here
	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emResizeCallback);
	emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emKeyEventCallback);
	emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emKeyEventCallback);
	emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emMouseEventCallback);
	emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emMouseEventCallback);
	emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, EM_FALSE, emMouseEventCallback);
}
static void coreInitGLContext(Core_t self) {
	EmscriptenWebGLContextAttributes ctxAttrs;
	emscripten_webgl_init_context_attributes(&ctxAttrs);
	ctxAttrs.majorVersion = 2;
	ctxAttrs.minorVersion = 0;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("!canvas", &ctxAttrs);
	emscripten_webgl_make_context_current(ctx);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
}

static bool emResizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
	if (!userData) return false;
	if (eventType == EMSCRIPTEN_EVENT_RESIZE) {
		Core_t core = (Core_t)userData;
		int width = uiEvent->documentBodyClientWidth;
		int height = uiEvent->documentBodyClientHeight;
		emscripten_get_canvas_element_size("!canvas", &width, &height);
		if ((width > 0) && (height > 0)) {
			glViewport(0, 0, width, height);
			coreInternalOnResize(&core->internal, width, height);
		}
	}
	return true;
}
static bool emKeyEventCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
	if (!userData) return false;
	Core_t core = (Core_t)userData;
	uint32_t key = emInputParseKey(core->input, keyEvent->code);
	if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
		fprintf(stdout, "KeyDown: codeName=%s code=%d\n", keyEvent->code, key);
		coreInternalOnKeyDown(&core->internal, key);
		return true;
	}
	else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
		coreInternalOnKeyUp(&core->internal, key);
		return true;
	}
	return false;
}
static bool emMouseEventCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
	if (!userData) return false;
	Core_t core = (Core_t)userData;
	if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
		coreInternalOnMouseDown(&core->internal, mouseEvent->button, mouseEvent->clientX, mouseEvent->clientY);
		return true;
	}
	else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
		coreInternalOnMouseUp(&core->internal, mouseEvent->button, mouseEvent->clientX, mouseEvent->clientY);
		return true;
	}
	else if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
		coreInternalOnMouseMove(&core->internal, mouseEvent->clientX, mouseEvent->clientY);
		return true;
	}
	return false;
}

