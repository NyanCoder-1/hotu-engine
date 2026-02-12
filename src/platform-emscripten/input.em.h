#ifndef __INPUT_EM_H__
#define __INPUT_EM_H__

#include <stdint.h>

typedef struct EmInput *EmInput_t;

EmInput_t emInputNew();
void emInputDelete(EmInput_t *input);
uint32_t emInputParseKey(EmInput_t self, const char *key);

#endif

