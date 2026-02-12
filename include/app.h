#ifndef __APP_H__
#define __APP_H__

#include "core.type.h"

typedef struct App *App_t;

App_t appInit(Core_t core);
void appClose(App_t app);
void appUpdate(App_t app, double deltaTime);

#endif
