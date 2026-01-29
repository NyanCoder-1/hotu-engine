#ifndef __VIEW_H__
#define __VIEW_H__

#include "model.h"

typedef struct View *View_t;

// Layers constants
enum LAYERS {
	LAYER_SOLID,
	LAYER_TRANSPARENT,
	LAYER_UI,

	LAYERS_COUNT
};

View_t viewNew(Model_t model);
void viewDelete(View_t *view);
void viewRender(View_t self);

#endif

