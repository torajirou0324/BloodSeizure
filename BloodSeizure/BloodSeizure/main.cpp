#include "App.h"

// ���C���G���g���[�|�C���g
int wmain(int argc, wchar_t** argv, wchar_t** envp)
{
    App app(TEXT("BloodSeizure"));
    app.GameLoop();

    return 0;
}