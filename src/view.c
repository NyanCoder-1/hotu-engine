#include "view.h"
#include <GLES3/gl3.h>
#include <stdlib.h>
#include <string.h>


typedef struct ViewLayer {

} ViewLayer;

typedef struct View {
	struct Color {
		GLfloat r, g, b, a;
	} bgColor;
	Model_t model;

	ViewLayer layers[LAYERS_COUNT];
} View;

View_t viewNew(Model_t model) {
	View_t view = (View_t)malloc(sizeof(View));
	if (!view) return 0;

	memset(view, 0, sizeof(View));
	view->model = model;
	view->bgColor = (struct Color){ .r = 0.0f, .g = 0.5f, .b = 0.5f, .a = 1.0f}; // Default background color

	return view;
}
void viewDelete(View_t *view) {
	if (!view || !*view) return;

	memset(*view, 0, sizeof(View));
	free(*view);
	*view = 0;
}
void viewRender(View_t self) {
	glClearColor(self->bgColor.r, self->bgColor.g, self->bgColor.b, self->bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

