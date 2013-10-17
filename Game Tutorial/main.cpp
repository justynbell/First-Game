#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void windowClassInit(WNDCLASSEX *windowClass, HINSTANCE hInstance)
{
	windowClass->cbSize = sizeof(WNDCLASSEX);
	windowClass->style = CS_VREDRAW;
	windowClass->lpfnWndProc = WndProc;
	windowClass->cbClsExtra = 0;
	windowClass->cbWndExtra = 0;
	windowClass->hInstance = hInstance;
	windowClass->hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	windowClass->hCursor = LoadCursor(hInstance, IDC_ARROW);
	windowClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass->lpszMenuName = NULL;
	windowClass->lpszClassName = L"First Windows Program";
	windowClass->hIconSm = LoadIcon(NULL, IDI_WINLOGO);	
}

void windowClassCreate(HWND *hwnd, HINSTANCE hInstance)
{
	*hwnd = CreateWindowEx(NULL,
						 L"MyClass",
						 L"A REAL Windows Application!",
						 WS_OVERLAPPEDWINDOW | 
						 WS_VISIBLE |
						 WS_SYSMENU,
						 100, 100,
						 400, 400,
						 NULL,
						 NULL,
						 hInstance,
						 NULL);
}

/* Use the Windows Procedure event handler */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	const wchar_t *string = L"Hello World";

	switch (message) {
		case WM_CREATE:		/* Window is being created */
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:		
			/* What happens when the window is moved, resized, 
			 * or any other event occurs 
			 */
			hDC = BeginPaint(hwnd, &paintStruct);

			/* Set text color to green */
			SetTextColor(hDC, COLORREF(0x0000ff00));

			/* Display the text in the middle of the window */
			TextOut(hDC, 150, 150, string, sizeof(string) - 1);

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
	WNDCLASSEX	windowClass;		/* Window Class */
	HWND		hwnd;				/* Window Handle */
	MSG			msg;				/* message */
	bool		done;				/* Flow saying when your app is complete */

	/* Fill out the Window Class struct */
	windowClassInit(&windowClass, hInstance);

	/* Register the Windows Class */
	if (!RegisterClassEx(&windowClass))
		return 0;

	/* Class registered, now create the window */
	windowClassCreate(&hwnd, hInstance);
	if (!(hwnd  = NULL))
		return 0;

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