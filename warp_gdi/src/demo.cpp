// demo.cpp : our fresh new app fro mirage demo end of 2022
//

#include "framework.h"
#include "demo.h"
#include <string>
#include "shellapi.h" // drag & drop
#include <algorithm> // transform()
#include <cctype>  // tolower()
#include <windowsx.h> // SelectBitmap()

#include "joystickapi.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "bitmap.hpp"
//#include "vector.hpp"
#include "text.h"
#include "warp.hpp"

#include <chrono>
#include <vector>

HDC hdcBuf;
HBITMAP hbmBuf;
HBITMAP hbmOldBuf;

// I) input image
img warp_in;
img out;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


const int ID_TIMER = 1;
const int WIN_H = 1080;
const int WIN_W = 1600;

// 0, 100, 100, 0, 200, 200
//float rotx[3] = {0,100,200};
//float roty[3] = { 200,0,200 };
float rotx[3] = { 120,0,-120 };
float roty[3] = { -100,100,-100 };
float angle = 0.5f*3.14159f/180.0f;
float xnew[3];
float ynew[3];

// quad/trapezoid to define warp area
struct pt2d {
  int x;
  int y;
};
std::vector<pt2d> poly2d;
POINT mouse;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FOTOALBUM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FOTOALBUM));

    MSG msg;

    HDC hDC = GetDC(NULL);
    warp_in.hdcMem = CreateCompatibleDC(hDC);
    std::string filename = "c:\\git\\s-l1600.jpg";
//    std::string filename = "..\\..\\lane.png";
    warp_in.load(filename);

    out.hdcMem = CreateCompatibleDC(hDC);
    out.load(filename); // hack / tmp, just load same thing as above

    int wx[4] = { 0,80,50,0 };
    int wy[4] = { 0,0,70,50 };
    warp(&warp_in, &out, 300, 90, 400, 190, wx, wy);
//    warp(&warp_in, &out, 30, 200, 100, 250, wx, wy);

//    roto_zoom(&warp_in, &out, 0,100, 100,0, 200,200);

//    grid();
    loadText(WIN_W, WIN_H); // Bildschirm-Text (BTX) initialisieren


    // double buffer, otherwise too much flicker
    hdcBuf = CreateCompatibleDC(hDC);
    hbmBuf = CreateCompatibleBitmap(hDC, WIN_W, WIN_H);
    hbmOldBuf = (HBITMAP)SelectObject(hdcBuf, hbmBuf);

    // (3) trigger WM_PAINT event
    GetMessage(&msg, nullptr, 0, 0);
    RedrawWindow(msg.hwnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);

    int ret = SetTimer(msg.hwnd, ID_TIMER, 50, NULL); // change to run faster/slower (e.g. 25 will run twice as fast as 50)
    if (ret == 0) MessageBox(msg.hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    ReleaseDC(NULL, hDC);
    ReleaseDC(NULL, warp_in.hdcMem); // is this needed? as not created with GetDC
    ReleaseDC(NULL, out.hdcMem); // is this needed? as not created with GetDC
    DeleteDC(hDC);
    DeleteDC(warp_in.hdcMem);
    DeleteDC(out.hdcMem);

    DeleteDC(hdcBuf);
    DeleteObject(hbmBuf);
    DeleteObject(hbmOldBuf);
    // delete dibsection ?
    
    KillTimer(msg.hwnd, ID_TIMER);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOTOALBUM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FOTOALBUM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

//   #define FENSTERLEISTE 50 // ???
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU),
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   CW_USEDEFAULT, 0, WIN_W, WIN_H/*+ FENSTERLEISTE*/, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        DragAcceptFiles(hWnd, TRUE); // drag n drop, yeah!
        break;
    // https://learn.microsoft.com/en-us/windows/win32/gdi/drawing-with-the-mouse
    case WM_LBUTTONDOWN:
      break;

    case WM_LBUTTONUP:
      poly2d.push_back({ mouse.x,mouse.y });
      break;

    case WM_MOUSEMOVE:
      mouse.x = LOWORD(lParam);
      mouse.y = HIWORD(lParam);
      break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT: // this is only being called in case of an event
                  // btw. this might be called first, so be sure to initialize all
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

// draw stuff (drawing can only happen here ...)
            
            if (warp_in.img != NULL) BitBlt(hdcBuf, 0, 0, warp_in.w, warp_in.h, warp_in.hdcMem, 0, 0, SRCCOPY);
            int y_out = warp_in.h; // blit just below input image
            if (out.img != NULL) BitBlt(hdcBuf, 0, y_out, out.w, out.h, out.hdcMem, 0, 0, SRCCOPY);

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::chrono::milliseconds rt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);// .count();
            if (warp_in.img != NULL) {
              char buf[50];
              sprintf(buf, "runtime: %d [ms]", rt_ms);
              doText(warp_in.dibdata, warp_in.w * 3, 20, 20, buf); // draw into img?
              sprintf(buf, "mouse-x: %d", mouse.x);
              doText(warp_in.dibdata, warp_in.w * 3, 20, 30, buf); // draw into img?
              sprintf(buf, "mouse-y: %d", mouse.y);
              doText(warp_in.dibdata, warp_in.w * 3, 20, 40, buf); // draw into img?
            }


            // draw control points / polygon
            HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
            SelectObject(hdcBuf, hPen);
            if (poly2d.size() > 0) {
              Ellipse(hdcBuf, poly2d[0].x - 5, poly2d[0].y - 5, poly2d[0].x + 5, poly2d[0].y + 5);
              MoveToEx(hdcBuf, poly2d[0].x, poly2d[0].y, NULL);
              for (int i = 1; i < poly2d.size(); i++) {
                Ellipse(hdcBuf, poly2d[i].x - 5, poly2d[i].y - 5, poly2d[i].x + 5, poly2d[i].y + 5);
                LineTo(hdcBuf, poly2d[i].x, poly2d[i].y);
                MoveToEx(hdcBuf, poly2d[i].x, poly2d[i].y, NULL);
              }
            }
            DeleteObject(hPen);

            for (int i = 0; i < 3; i++) {
              Ellipse(hdcBuf, xnew[i] - 5, ynew[i] - 5, xnew[i] + 5, ynew[i] + 5);

            }


            BitBlt(hdc, 0, 0, WIN_W, WIN_H, hdcBuf, 0, 0, SRCCOPY); // double buffer
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        {
//          float xnew[3];
//          float ynew[3];
//          float radius = 1.0f;
          for (int i = 0; i < 3; i++) {
            xnew[i] = rotx[i] * cos(angle) - roty[i] * sin(angle)+1340;
            ynew[i] = roty[i] * cos(angle) + rotx[i] * sin(angle)+280;
//            xnew[i] = radius * cos(angle)+rotx[i];
//            ynew[i] = radius * sin(angle)+roty[i];
          }
          roto_zoom(&warp_in, &out, xnew[0], ynew[0], xnew[1], ynew[1], xnew[2], ynew[2]);
          angle -= (5.0f*3.14159f / 180.0f);
//          roto_zoom(&warp_in, &out, 0, 100, 100, 0, 200, 200);

          // trigger a redraw, otherwise there will be no WM_PAINT event
          RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
        }
        break;
    case WM_DROPFILES:
        char filename[MAX_PATH];
        DragQueryFileA((HDROP)wParam, 0, filename, MAX_PATH); // use file >> 0 << in list of dragged files
        warp_in.load(filename);
        RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
        DragFinish((HDROP)wParam);
        break;
    case WM_DESTROY:
// done at winmain        KillTimer(hWnd, ID_TIMER); 
        DragAcceptFiles(hWnd, FALSE); // prevents bogus drop messages after we are gone ;-/ ??
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
