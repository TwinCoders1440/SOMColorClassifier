// ColorClasifierGUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SOMNetwork.h"
#include "ColorClasifierGUI.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#define MAX_LOADSTRING 100

SOMNetwork *somNet;
HWND ghWnd;
bool paused = true;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_COLORCLASIFIERGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COLORCLASIFIERGUI));

    MSG msg;

    // Main message loop:
	bool finished = false;
	while (!finished) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// Stop loop if it's a quit message
				finished = true;
			}
			else// (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (paused == false && somNet->Train(trainingSet, 0.01f, 0.000001f, false)) {
			//this will call WM_PAINT which will render the map
			InvalidateRect(ghWnd, NULL, TRUE);
			UpdateWindow(ghWnd);

			//std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	}

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORCLASIFIERGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COLORCLASIFIERGUI);
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
	//create a window with the *client* area specified.
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 400;
	rect.right = 400;

	if (!AdjustWindowRectEx(&rect, CS_HREDRAW | CS_VREDRAW, true, NULL))
	{
		MessageBox(NULL, L"Problem creating window", L"error!", MB_OK);
		return 0;
	}

   hInst = hInstance; // Store instance handle in our global variable

   ghWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
	   GetSystemMetrics(SM_CXSCREEN) / 2 - 400 / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - 400 / 2,
	   rect.right, rect.bottom, nullptr, nullptr, hInstance, nullptr);

   if (!ghWnd)
   {
      return FALSE;
   }

   ShowWindow(ghWnd, nCmdShow);
   UpdateWindow(ghWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//these hold the dimensions of the client window area
	static int cxClient, cyClient;

	//used to create the back buffer
	static HDC   hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;

    switch (message)
    {
	case WM_CREATE: {
		//to get get the size of the client window first we need  to create
		//a RECT and then ask Windows to fill in our RECT structure with
		//the client window size. Then we assign to cxClient and cyClient 
		//accordingly
		RECT rect;

		GetClientRect(hWnd, &rect);

		cxClient = rect.right;
		cyClient = rect.bottom;

		//create a memory device context
		hdcBackBuffer = CreateCompatibleDC(NULL);

		//get the DC for the front buffer
		HDC hdc = GetDC(hWnd);

		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);


		//select the bitmap into the memory device context
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

		//don't forget to release the DC
		ReleaseDC(hWnd, hdc);

		somNet = new SOMNetwork(cxClient, cyClient, 40, 40, 3, true, 10000);
		CreateDataSet();
	}//end case WN_Create
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			//fill the backbuffer with white
			BitBlt(hdcBackBuffer,
				0,
				0,
				cxClient,
				cyClient,
				NULL,
				NULL,
				NULL,
				WHITENESS);

			somNet->Render(hdcBackBuffer);

			//now blit the backbuffer to the front
			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
		//has the user resized the client area?
	case WM_SIZE:
	{
		//if so we need to update our variables so that any drawing
		//we do using cxClient and cyClient is scaled accordingly
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//now to resize the backbuffer accordingly. First select
		//the old bitmap back into the DC
		SelectObject(hdcBackBuffer, hOldBitmap);

		//don't forget to do this or you will get resource leaks
		DeleteObject(hBitmap);

		//get the DC for the application
		HDC hdc = GetDC(hWnd);

		//create another bitmap of the same size and mode
		//as the application
		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);

		ReleaseDC(hWnd, hdc);

		//select the new bitmap into the DC
		SelectObject(hdcBackBuffer, hBitmap);
	}
	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			PostQuitMessage(0);
		}break;
		case 'R':
		{
			delete somNet;
			paused = false;
			somNet = new SOMNetwork(cxClient, cyClient, 40, 40, 3, true, 10000);
		} break;
		case 'S': {
			if (paused == false)
				paused = true;
			else
				paused = false;
		} break;
		case 'M': {
			HDC hdc = GetDC(hWnd);
			somNet->saveModel("D:\\SOMModel.txt", hdc);
		} break;
		}//end inner switch
	} break;
	case WM_DESTROY: {
		//clean up our backbuffer objects
		SelectObject(hdcBackBuffer, hOldBitmap);

		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);

		PostQuitMessage(0);
	}
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
