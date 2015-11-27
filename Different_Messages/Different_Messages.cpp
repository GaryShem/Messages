// Different_Messages.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Different_Messages.h"
#include "windows.h"

#define MAX_LOADSTRING 100
#define ID_BTN_SEND			111
#define ID_BTN_POST			222
#define ID_BTN_POST_THREAD	333
#define ID_LISTBOX			444

#define WM_SEND				(WM_USER + 1)
#define WM_POST				(WM_USER + 2)
#define WM_POST_THREAD		(WM_USER + 3)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HANDLE g_hThread;
DWORD g_dThreadId;
HWND g_hThreadWnd;
HWND g_hListBox;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ThreadWndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI		GuiThreadProc(void*);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIFFERENT_MESSAGES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIFFERENT_MESSAGES));
	// Создаём поток для второго окна
	g_hThread = CreateThread(NULL, 0, GuiThreadProc, NULL, 0, &g_dThreadId);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	ExitProcess((int)msg.wParam);
	return (int)msg.wParam;
}

DWORD WINAPI GuiThreadProc(void* p)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = ThreadWndProc;
	wcex.hInstance = hInst;
	wcex.lpszClassName = "MyWndClass";
	RegisterClassEx(&wcex);

	g_hThreadWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 450, 350, NULL, NULL, hInst, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIFFERENT_MESSAGES));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DIFFERENT_MESSAGES);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 450, 350, NULL, NULL, hInstance, NULL);

	CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "SendMessage", WS_CHILD | WS_VISIBLE,
		280, 10, 150, 25, hWnd, (HMENU)ID_BTN_SEND, hInstance, NULL);

	CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "PostMessage", WS_CHILD | WS_VISIBLE,
		280, 40, 150, 25, hWnd, (HMENU)ID_BTN_POST, hInstance, NULL);

	CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Post Thread Message", WS_CHILD | WS_VISIBLE,
		280, 70, 150, 25, hWnd, (HMENU)ID_BTN_POST_THREAD, hInstance, NULL);

	g_hListBox = CreateWindowEx(WS_EX_STATICEDGE, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		10, 10, 260, 260, hWnd, (HMENU)ID_LISTBOX, hInstance, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BTN_SEND:
			SendMessage(g_hThreadWnd, WM_SEND, NULL, NULL);
			break;
		case ID_BTN_POST:
			PostMessage(g_hThreadWnd, WM_POST, NULL, NULL);
			break;
		case ID_BTN_POST_THREAD:
			PostThreadMessage(g_dThreadId, WM_POST_THREAD, NULL, NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ThreadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SEND:
		Sleep(3000);
		SendMessage(g_hListBox, LB_ADDSTRING, NULL, (LPARAM)"WM_SEND finished");
		break;
	case WM_POST:
		Sleep(3000);
		SendMessage(g_hListBox, LB_ADDSTRING, NULL, (LPARAM)"WM_POST finished");
		break;
	case WM_POST_THREAD:
		Sleep(3000);
		SendMessage(g_hListBox, LB_ADDSTRING, NULL, (LPARAM)"WM_POST_THREAD finished");
		break;
	case WM_DESTROY:
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
