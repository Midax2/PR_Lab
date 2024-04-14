#include <windows.h>
#include <stdio.h>

#define IDC_CHECKBOX1 101
#define IDC_CHECKBOX2 102
#define IDC_CHECKBOX3 103
#define IDC_CLEAR_BUTTON 104

HWND hCheckBox1, hCheckBox2, hCheckBox3, hClearButton, hConsole;
bool isChecked1 = false, isChecked2 = false, isChecked3 = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BasicWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Basic Window",
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
void CloseWindowT(HWND hwnd) {
    printf("Application is closing.\n");
    Sleep(2000);
    FreeConsole();
    Sleep(1000);
    PostQuitMessage(0);
}

void ClearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD size;
    COORD screenPosition = { 0, 0 };

    GetConsoleScreenBufferInfo(hConsole, &info);
    size = info.dwSize.X * info.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', size, screenPosition, &size);
    FillConsoleOutputAttribute(hConsole, info.wAttributes, size, screenPosition, &size);
    SetConsoleCursorPosition(hConsole, screenPosition);
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        hCheckBox1 = CreateWindow("BUTTON", "WM_MOVE", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 20, 100, 20, hwnd, (HMENU)IDC_CHECKBOX1, NULL, NULL);
        hCheckBox2 = CreateWindow("BUTTON", "WM_SIZE", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 50, 100, 20, hwnd, (HMENU)IDC_CHECKBOX2, NULL, NULL);
        hCheckBox3 = CreateWindow("BUTTON", "WM_PAINT", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 80, 100, 20, hwnd, (HMENU)IDC_CHECKBOX3, NULL, NULL);

        hClearButton = CreateWindow("BUTTON", "CLEAR", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            20, 110, 100, 30, hwnd, (HMENU)IDC_CLEAR_BUTTON, NULL, NULL);

        AllocConsole();
        hConsole = GetConsoleWindow();
        MoveWindow(hConsole, 150, 20, 400, 300, TRUE);

        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDC_CHECKBOX1:
            isChecked1 = !isChecked1;
            break;
        case IDC_CHECKBOX2:
            isChecked2 = !isChecked2;
            break;
        case IDC_CHECKBOX3:
            isChecked3 = !isChecked3;
            break;
        case IDC_CLEAR_BUTTON:
            ClearConsole();
            break;
        }
        break;
    }
    case WM_CLOSE: {
        CloseWindowT(hwnd);
        break;
    }
    case WM_DESTROY: {
        CloseWindowT(hwnd);
        break;
    }
    default:
        if (isChecked1 && uMsg == WM_MOVE) {
            printf("Received WM_MOVE message.\n");
        }
        if (isChecked2 && uMsg == WM_SIZE) {
            printf("Received WM_SIZE message.\n");
        }
        if (isChecked3 && uMsg == WM_PAINT) {
            printf("Received WM_PAINT message.\n");
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
