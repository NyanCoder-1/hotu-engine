#ifndef __MODEL_H__
#define __MODEL_H__

typedef struct Model *Model_t;

Model_t modelNew();
void modelDelete(Model_t *model);

#endif

