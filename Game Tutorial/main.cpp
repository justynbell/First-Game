//#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <tchar.h>
#include <gl/GL.h>
#include <gl/GLU.h>

/* Globals */
float angle = 0.0f;
float angle_inc = 0.0f;
HDC g_HDC;

/* Sets the pixel format */
int SetupPixelFormat(HDC hDC, int *err)
{
	int nPixelFormat;		/* My pixel format index number */

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),					/* Size of the actual structure. */
		1,												/* Version.  Always set to 1 */
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |		/* Support window | support opengl */
		PFD_DOUBLEBUFFER,								/* Support double buffering */
		PFD_TYPE_RGBA,									/* RGBA color mode */
		32,												/* Try for 32 bit color mode */
		0, 0, 0, 0, 0, 0,								/* Ignore Color bits/shift */
		0, 0,											/* Ignore alpha bits/shift */
		0, 0, 0, 0, 0,									/* Ignore accumulation buffer/bits */
		16,												/* 16 bit z-buffer size */
		0, 0,											/* No stencil, no aux buffer */
		PFD_MAIN_PLANE,									/* Main drawing plane */
		0,												/* Reserved */
		0, 0, 0 										/* Layer masks ignored */
	};

	/* Choose best matching pilex format */
	if(!(nPixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		*err = GetLastError();
		return -1;
	}

	/* Set pixel format to device context */
	if(!SetPixelFormat(hDC, nPixelFormat, &pfd)) {
		*err = GetLastError();
		return -1;
	}

	return 0;
}

/* Use the Windows Procedure event handler */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR string[] = _T("Hello World");
	PAINTSTRUCT paintStruct;
	int err;
	int width;
	int height;

	static HDC hDC;
	static HGLRC hRC;

	switch (message) {
		case WM_CREATE:			/* Window is being created */
			hDC = GetDC(hwnd);	/* Get current window's device context */
			g_HDC = hDC;
			if (SetupPixelFormat(hDC, &err) < 0)
				return err;

			/* Create rendering context */
			if (!(hRC = wglCreateContext(hDC)))
				return GetLastError();				

			/* Make rendering context current (assign it to device context) */
			if (!wglMakeCurrent(hDC, hRC))
				return GetLastError();

			return 0;
		case WM_CLOSE:			/* Window is being closed */
			/* Deselect rendering context */
			if (!wglMakeCurrent(hDC, hRC))
				return GetLastError();

			/* Delete rendering context */
			if (!wglDeleteContext(hRC))
				return GetLastError();

			/* Post quit message to main thread (while loop) */
			PostQuitMessage(0);

			return 0;
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
		case WM_SIZE:
			height = HIWORD(lParam);
			width = LOWORD(lParam);

			/* Don't want to divide by zero */
			if (height == 0)
				height = 1;
			
			glViewport(0, 0, width, height);	/* Reset the viewport to new dimensions */
			glMatrixMode(GL_PROJECTION);		/* Set projection matrix */
			glLoadIdentity();					/* Reset projection matrix */

			/* Calculate aspect ratio of the screen */
			gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

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
	int			loop_count = 0;
	int			inc = 1;

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

	ShowWindow(hwnd, SW_SHOW);			/* Show the window */
	UpdateWindow(hwnd);					/* Update the window */

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
			/* Do all the rendering here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

			if (loop_count % 100 == 0)
				angle_inc += 0.001f * inc;
				if (angle_inc == 2)
					inc = -1;
				else if (angle_inc == 0)
					inc = 1;

			angle += angle_inc;
			if (angle >= (360.0f * 10.0f))
				angle = 0.0f;
			glTranslatef(0.0f, 0.0f, -2.0f - angle_inc);		/* Move back 5 units */
			glRotatef(angle, 0.0f, 0.0f, 1.0f);		/* Rotate along z axis */

			glColor3f(0.80f, 0.0f, 0.0f);			/* Sets the color */
			glBegin(GL_TRIANGLES);
				glVertex3f(-0.5f, -0.28f, 0.0f);
				glVertex3f(0.5f, -0.28f, 0.0f);
				glVertex3f(0.0f, 0.586f, 0.0f);
			glEnd();

			SwapBuffers(g_HDC);
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		(loop_count++) & 0xffffff;

	}

	return msg.wParam;
}