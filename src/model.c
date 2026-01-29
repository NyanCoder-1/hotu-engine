#include "model.h"
#include <stdlib.h>
#include <string.h>

typedef struct Model {
	int dummy;
} Model;

Model_t modelNew() {
	Model_t model = (Model_t)malloc(sizeof(Model));
	if (!model) return 0;

	memset(model, 0, sizeof(Model));

	return model;
}
void modelDelete(Model_t *model) {
	if (!model || !*model) return;

	memset(*model, 0, sizeof(Model));
	free(*model);
	*model = 0;
}

