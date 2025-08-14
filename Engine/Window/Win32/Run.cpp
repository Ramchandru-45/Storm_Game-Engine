#include "pch.h"
#include "WinApplication.h"

int Storm::WinApplication::Run()
{
    if (Storm::WinApplication::Initialize())
    {
        OnCreate();
        while (m_isRunning)
        {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (!minimized)
            {
                OnUpdate();
                OnRender();
                
            }
        }
        OnDestroy();
        return 0;
    }
    LOG_ERROR("Unable to Initialize Window");
    return 1;
}
