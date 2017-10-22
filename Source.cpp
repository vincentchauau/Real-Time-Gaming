#include <windows.h>

// Global variables
const int ID_TIMER = 1; // timer id
HDC hdc; // memory
HBITMAP *hImages = new HBITMAP[8]; // frames
int time = 0; // time
int x = 0, y = 0, dx = 2, dy = 3; // location(x,y), velocity(2,3)
bool bContinue = true; // continue flag
// Check whether continue or not
bool CanWeContinue()
{
	return bContinue;
}
// Render a frame at location(x,y), screen memory to draw on hdc, list of frames hImages, running time, frame velocity (dx,dy)
void Render(HDC &hdc, int &time, HBITMAP* hImages, int &x, int &y, int& dx, int& dy)
{
	// Check continue flag
	if (CanWeContinue())
	{
		// Get screen rectangle
		RECT rect;
		GetClientRect(WindowFromDC(hdc), &rect);
		// Change frame location by adding velocity
		x += dx;
		y += dy;
		// Create a buffer memory for screen memory
		HDC hdcMem = CreateCompatibleDC(hdc);
		BITMAP bm;
		// Get the frame object information: width, height in bm object
		GetObject(hImages[(time / 100) % 8], sizeof(bm), &bm);
		// Draw the frame object on the buffer memory
		SelectObject(hdcMem, hImages[(time / 100) % 8]);
		// Draw the buffer memory on the screen memory
		BitBlt(hdc, x, y, bm.bmHeight, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		// Delete the buffere memory
		DeleteDC(hdcMem);
		// Change velocity based on location(x,y), velocity(dx,dy), screensize(rect)
		if (dx > 0 && x + bm.bmWidth > rect.right)
		{
			dx *= -1;
		}
		if (dx < 0 && x < rect.left)
		{
			dx *= -1;
		}
		if (dy > 0 && y + bm.bmHeight > rect.bottom)
		{
			dy *= -1;
		}
		if (dy < 0 && y < rect.top)
		{
			dy *= -1;
		}
	}
	else
	{
		TextOut(hdc, 0, 0, TEXT("Please enter coins or press any key to continue."), 48);
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		hImages[0] = (HBITMAP)LoadImage(NULL, "1.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[1] = (HBITMAP)LoadImage(NULL, "2.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[2] = (HBITMAP)LoadImage(NULL, "3.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[3] = (HBITMAP)LoadImage(NULL, "4.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[4] = (HBITMAP)LoadImage(NULL, "5.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[5] = (HBITMAP)LoadImage(NULL, "6.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[6] = (HBITMAP)LoadImage(NULL, "7.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		hImages[7] =(HBITMAP)LoadImage(NULL, "8.bmp", IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
		SetTimer(hwnd, ID_TIMER, 10, NULL);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		Render(hdc, time, hImages, x, y, dx, dy);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		++time;
		hdc = GetDC(hwnd);
		Render(hdc, time, hImages, x, y, dx, dy);
		ReleaseDC(hwnd, hdc);
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			KillTimer(hwnd, ID_TIMER);
			break;
		default:
			if (bContinue == false)
				bContinue = true;
			else
				bContinue = false;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "My Win32 API";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	hwnd = CreateWindow(
		"My Win32 API",
		"Waiting screen", WS_TILEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
