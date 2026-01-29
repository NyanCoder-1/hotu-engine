#ifndef __INPUT_LISTENER_H__
#define __INPUT_LISTENER_H__

#include <stdint.h>

typedef struct InputListener *InputListener_t;

InputListener_t inputListenerNew();
void inputListenerDelete(InputListener_t *listener);
void inputListenerSetKeyDownCallback(InputListener_t listener, void (*keyCallback)(uint32_t key), void *userData);
void inputListenerSetKeyUpCallback(InputListener_t listener, void (*keyCallback)(uint32_t key), void *userData);
void inputListenerSetMouseDownCallback(InputListener_t listener, void (*mouseCallback)(uint32_t button, double x, double y), void *userData);
void inputListenerSetMouseUpCallback(InputListener_t listener, void (*mouseCallback)(uint32_t button, double x, double y), void *userData);
void inputListenerSetMouseMoveCallback(InputListener_t listener, void (*mouseCallback)(double x, double y), void *userData);
void inputListenerKeyDown(InputListener_t listener, uint32_t key);
void inputListenerKeyUp(InputListener_t listener, uint32_t key);
void inputListenerMouseDown(InputListener_t listener, uint32_t button, double x, double y);
void inputListenerMouseUp(InputListener_t listener, uint32_t button, double x, double y);
void inputListenerMouseMove(InputListener_t listener, double x, double y);


#endif
