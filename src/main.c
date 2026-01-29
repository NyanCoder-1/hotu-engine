#include "app.h"
#include "core.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Core_t core = coreNew();
	App_t app = appInit(core);

	coreRun(core, app);

	appClose(app);
	app = 0;
	coreDelete(&core);

	return 0;
}

