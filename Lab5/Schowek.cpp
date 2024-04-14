#include <windows.h>

#define IDC_EDIT 101
#define IDC_CUT_BUTTON 102
#define IDC_COPY_BUTTON 103
#define IDC_PASTE_BUTTON 104
#define IDC_CLEAR_BUTTON 105

HWND hEdit, hCutButton, hCopyButton, hPasteButton, hClearButton;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ClipboardWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Clipboard Window",
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        hEdit = CreateWindowEx(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE,
            20, 20, 300, 200, hwnd, (HMENU)IDC_EDIT, NULL, NULL);

        hCutButton = CreateWindow("BUTTON", "CUT", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            20, 230, 70, 30, hwnd, (HMENU)IDC_CUT_BUTTON, NULL, NULL);

        hCopyButton = CreateWindow("BUTTON", "COPY", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            100, 230, 70, 30, hwnd, (HMENU)IDC_COPY_BUTTON, NULL, NULL);

        hPasteButton = CreateWindow("BUTTON", "PASTE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            180, 230, 70, 30, hwnd, (HMENU)IDC_PASTE_BUTTON, NULL, NULL);

        hClearButton = CreateWindow("BUTTON", "CLEAR", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            260, 230, 70, 30, hwnd, (HMENU)IDC_CLEAR_BUTTON, NULL, NULL);

        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDC_CUT_BUTTON: {
            SendMessage(hEdit, WM_CUT, 0, 0);
            break;
        }
        case IDC_COPY_BUTTON: {
            SendMessage(hEdit, WM_COPY, 0, 0);
            break;
        }
        case IDC_PASTE_BUTTON: {
            SendMessage(hEdit, WM_PASTE, 0, 0);
            break;
        }
        case IDC_CLEAR_BUTTON: {
            SetWindowText(hEdit, "");
            break;
        }
        }
        break;
    }
    case WM_CLOSE: {
        PostQuitMessage(0);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
