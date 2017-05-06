/**********************************************************************
 * Team Coyote
 **********************************************************************
 */

#include "UI.h"

int main()
{
	//TODO: Create filesystem

	// incorporate the filesystem.
	// ... 
	// FileSystem will incorporate the disk subsystem and file subsystem
	// ...
	// Intialize ATOS-FS API
	// .. 
	// ATOS-FS API now ready for work.

	// incorporate UI
	UI ui;

	// start UI - run from the main thread.
	int res = ui.run();


	// release resources (filesystem)
	// ...
	// resources freed.

	return res;
}

