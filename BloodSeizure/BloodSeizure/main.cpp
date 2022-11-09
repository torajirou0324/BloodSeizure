#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "SampleApp.h"

int wmain(int argc, wchar_t** argv, wchar_t** evnp)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �A�v���P�[�V���������s
	SampleApp(960, 540).Run();

	return 0;
}