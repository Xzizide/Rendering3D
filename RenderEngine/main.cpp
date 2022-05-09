#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"3DRenderer", 
        WS_OVERLAPPEDWINDOW,

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC, HWND hwnd)
{
    // Load the bitmap image file
    HBITMAP hBitmap;
    hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE);
    // Verify that the image was loaded
    if (hBitmap == NULL) {
        ::MessageBox(NULL, __TEXT("LoadImage Failed"), __TEXT("Error"), MB_OK);
        return false;
    }

    // Create a device context that is compatible with the window
    HDC hLocalDC;
    hLocalDC = ::CreateCompatibleDC(hWinDC);
    // Verify that the device context was created
    if (hLocalDC == NULL) {
        ::MessageBox(NULL, __TEXT("CreateCompatibleDC Failed"), __TEXT("Error"), MB_OK);
        return false;
    }

    // Get the bitmap's parameters and verify the get
    BITMAP qBitmap;
    int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
        reinterpret_cast<LPVOID>(&qBitmap));
    if (!iReturn) {
        ::MessageBox(NULL, __TEXT("GetObject Failed"), __TEXT("Error"), MB_OK);
        return false;
    }

    // Select the loaded bitmap into the device context
    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
    if (hOldBmp == NULL) {
        ::MessageBox(NULL, __TEXT("SelectObject Failed"), __TEXT("Error"), MB_OK);
        return false;
    }

    RECT rect;
    int width = 0, height = 0;
    if (GetWindowRect(hwnd, &rect)) {
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    // Blit the dc which holds the bitmap onto the window's dc
    BOOL qRetBlit = ::BitBlt(hWinDC, (width/2)*-1, (height/2)*-1, qBitmap.bmWidth, qBitmap.bmHeight,
        hLocalDC, 0, 0, SRCCOPY);
    if (!qRetBlit) {
        ::MessageBox(NULL, __TEXT("Blit Failed"), __TEXT("Error"), MB_OK);
        return false;
    }

    // Unitialize and deallocate resources
    ::SelectObject(hLocalDC, hOldBmp);
    ::DeleteDC(hLocalDC);
    ::DeleteObject(hBitmap);
    return true;
}

RECT rcCurrent = { 0,0,20,20 };
int idTimer = -1;
BOOL fVisible = FALSE;
POINT poArr[11] = {1,-250 /*E*/ , 1,250 /*A*/, -200,150 /*B*/, 1,-250 /*E*/, 1,50 /*C*/, -200,150 /*B*/, 200,150 /*D*/, 1,250 /*A*/, 1,50 /*C*/, 200,150 /*D*/, 1,-250 /*E*/ };
HDC hdc;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rc;

    switch (uMsg)
    {
        case WM_CREATE:

            GetClientRect(hwnd, &rc);
            OffsetRect(&rcCurrent, rc.right / 2, rc.bottom / 2);

            hdc = GetDC(hwnd);
            SetViewportOrgEx(hdc, rcCurrent.left,
                rcCurrent.top, NULL);
            SetROP2(hdc, R2_NOT);

            SetTimer(hwnd, idTimer = 1, 10, NULL);
            return 0L;

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0L;

        case WM_SIZE:
            switch (wParam)
            {
                case SIZE_MINIMIZED:

                    KillTimer(hwnd, 1);
                    idTimer = -1;
                    break;

                case SIZE_RESTORED:

                    if (rcCurrent.right > (int)LOWORD(lParam))
                    {
                        rcCurrent.left =
                            (rcCurrent.right =
                                (int)LOWORD(lParam)) - 20;
                    }
                    if (rcCurrent.bottom > (int)HIWORD(lParam))
                    {
                        rcCurrent.top =
                            (rcCurrent.bottom =
                                (int)HIWORD(lParam)) - 20;
                    }

                case SIZE_MAXIMIZED:

                    if (idTimer == -1)
                        SetTimer(hwnd, idTimer = 1, 10, NULL);
                    break;
            }
            return 0L;

        case WM_TIMER:
            if (poArr[1].x != -100) {
                poArr[1].x = -100;
                poArr[1].y = 225;
                poArr[7].x = -100;
                poArr[7].y = 225;
            /*B*/
                poArr[2].x = -100;
                poArr[2].y = 62;
                poArr[5].x = -100;
                poArr[5].y = 62;
            /*C*/
                poArr[4].x = 100;
                poArr[4].y = 62;
                poArr[8].x = 100;
                poArr[8].x = 62;
            /*D*/
                poArr[6].x = 100;
                poArr[6].y = 225;
                poArr[9].x = 100;
                poArr[9].y = 225;
            }
            else {
                poArr[1].x = 1;
                poArr[1].y = 250;
                poArr[7].x = 1;
                poArr[7].y = 250;
                /*B*/
                poArr[2].x = -200;
                poArr[2].y = 150;
                poArr[5].x = -200;
                poArr[5].y = 150;
                /*C*/
                poArr[4].x = 1;
                poArr[4].y = 50;
                poArr[8].x = 1;
                poArr[8].x = 50;
                /*D*/
                poArr[6].x = 200;
                poArr[6].y = 150;
                poArr[9].x = 200;
                poArr[9].y = 150;
            }

            if (fVisible)
                Polyline(hdc, poArr, 11);

            fVisible = Polyline(hdc, poArr, 11);

            return 0L;

        case WM_PAINT:

            BeginPaint(hwnd, &ps);
            LoadAndBlitBitmap(__TEXT("cat.bmp"), hdc, hwnd);
            if (!fVisible)
                fVisible = Polyline(hdc, poArr, 11);
            EndPaint(hwnd, &ps);
            return 0L;
        }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}