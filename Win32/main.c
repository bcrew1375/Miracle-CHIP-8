#include <windows.h>
#include <malloc.h>
#include "resource.h"

extern LoadRomFile(char filename[]);
extern InitializeSDL();
extern CloseSDL();
extern OpenSDLWindow();
extern Reset();
extern Interpret();
extern CheckSDLEvents();

unsigned char *romBuffer;

WNDCLASSEX wc;
HWND hwnd;
MSG Msg;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		{
			if (!(romBuffer = malloc(0x1050)))
			{
				MessageBox(hwnd, "Could not allocate memory for file buffer!", "Error!", MB_OK);
				DestroyWindow(hwnd);
			}
			if (InitializeSDL() == -1)
			{
				MessageBox(hwnd, "Could not initialize SDL!", "Error!", MB_OK);
				DestroyWindow(hwnd);
			}
		}
	break;
	case WM_CLOSE:
		{
			CloseSDL();
			DestroyWindow(hwnd);
			exit(0);
		}
	break;
	case WM_DESTROY:
		{
			CloseSDL();
			PostQuitMessage(0);
		}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MENUID_FILE_OPEN:
			{
				OPENFILENAME ofn;
				char szFileName[MAX_PATH] = "";

				ZeroMemory(&ofn, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = "";
				
				if(GetOpenFileName(&ofn))
				{
					switch(LoadRomFile(szFileName))
					{
					case -1:
						{
							MessageBox(hwnd, "Could not open file in Read-Only Binary mode!", "Error!", MB_OK);
						}
					break;
					case -2:
						{
							MessageBox(hwnd, "Could not read from file!", "Error!", MB_OK);
						}
					break;
					}
				}
				Reset();
				Interpret();
			}
		break;
		case MENUID_FILE_EXIT:
			{
				CloseSDL();
				DestroyWindow(hwnd);
				exit(0);
			}
		break;
		case MENUID_INTERPRETER_RUN:
			{
				if (OpenSDLWindow() == -1)
					MessageBox(hwnd, "Could not open SDL window", "Error!", MB_OK);

				for (;;)
				{
					CheckSDLEvents();
					Interpret();
				}
			}
		case MENUID_INTERPRETER_RESET:
			{
				Reset();
			}
		}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = "Window Class";
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"Window Class",
		"Miracle Chip8",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	if(!hwnd)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
		MB_ICONEXCLAMATION | MB_OK);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
    }
	return Msg.wParam;
}
