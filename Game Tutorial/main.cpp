//#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <tchar.h>

/* Use the Windows Procedure event handler */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	static TCHAR string[] = _T("Hello World");

	switch (message) {
		case WM_CREATE:		/* Window is being created */
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_PAINT:		
			/* What happens when the window is moved, resized, 
			 * or any other event occurs 
			 */
			hDC = BeginPaint(hwnd, &paintStruct);

			/* Set text color to green */
			SetTextColor(hDC, COLORREF(0x00006600));

			/* Display the text in the middle of the window */
			TextOut(hDC, 150, 150, string, wcslen(string));

			EndPaint(hwnd, &paintStruct);
			return 0;
		default:
			break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

/* Main application entry point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR szWindowClass[] = _T("...");
	static TCHAR szTitle[] = _T("Title");

	WNDCLASSEX	wcex;				/* Window Class */
	HWND		hwnd;				/* Window Handle */
	MSG			msg;				/* message */
	bool		done;				/* Flow saying when your app is complete */

	/* Fill out the Window Class struct */
	//windowClassInit(&windowClass, hInstance);
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, IDI_QUESTION);
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_QUESTION);

	/* Register the Windows Class */
	if (!RegisterClassEx(&wcex))
		return GetLastError();

	/* Class registered, now create the window */
	//windowClassCreate(&hwnd, hInstance);

	hwnd = CreateWindowEx(NULL,
						 szWindowClass,
						 szTitle,
						 WS_OVERLAPPEDWINDOW | 
						 WS_VISIBLE,
						 //WS_SYSMENU,
						 100, 100,
						 400, 400,
						 NULL,
						 NULL,
						 hInstance,
						 NULL);
	
	if (!hwnd) {
		return GetLastError();
	}

	done = false;

	/* Main message loop */
	while (!done) {
		/* Dispatches incoming sent message, checks the thread message
		 * qeue for a posted message, and retrieves the message (if any exist)
		 */
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);
		
		if (msg.message == WM_QUIT) {		/* We received a WM_QUIT message */
			done = true;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	return msg.wParam;
}