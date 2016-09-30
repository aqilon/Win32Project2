#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR nCmdLine, int nCmdShow)
{
	int winSize = GetSystemMetrics(SM_CXSCREEN) / 3;
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"WindowClass";
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(1);

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, _T("Call to RegisterClassEx faild!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	HWND hwnd = CreateWindow(
		L"WindowClass",
		L"Board",
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - winSize / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - winSize / 2, winSize, winSize,
		NULL, NULL, hInstance, NULL);
	::ShowWindow(hwnd, nCmdShow);
		nCmdShow = 0;
		MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}
	return 0;
}

struct RECTS {bool flag; RECT r;};
RECTS rects[100][100];
int rAm = 3;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rectClient, rectAll; 
	int headY = GetSystemMetrics(SM_CYSMCAPTION), headX = GetSystemMetrics(SM_CYSMCAPTION);
	GetClientRect(hwnd, &rectClient); GetWindowRect(hwnd, &rectAll);
	switch (uMsg)
	{
	case WM_SIZE:
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)NULL);
		POINT pos; GetCursorPos(&pos);
		for (int i = 0; i < rAm; i++) {
			for (int j = 0; j < rAm; j++) {
				Rectangle(hdc, rectClient.right / rAm * i, rectClient.bottom / rAm * j, rectClient.right / rAm * (i + 1), rectClient.bottom / rAm * (j + 1));
				rects[i][j].r = { rectClient.right / rAm * i,rectClient.bottom / rAm * j,rectClient.right / rAm * (i + 1),rectClient.bottom / rAm * (j + 1)};
				if (rects[i][j].flag == true) {
					FillRect(hdc, &rects[i][j].r, HBRUSH(CreateSolidBrush(RGB(255, 0, 55))));
				}
			}
		} 
		EndPaint(hwnd, &ps);
	}	return 0; 
	case WM_MOUSEMOVE: ////iosahdosahdoahsoidhosa[hdasuoidas
		POINT pos; GetCursorPos(&pos); 
		//if ((pos.x > rectAll.left) && (pos.y < rectAll.bottom) && (pos.x < rectAll.right) && (pos.y > rectAll.top))
			for (int i = 0; i < rAm; i++) {
				for (int j = 0; j < rAm; j++) {
					if ((pos.x > rectAll.left + rects[i][j].r.left + headX) && (pos.y > rectAll.top + rects[i][j].r.top + headY) && (pos.x < rectAll.left + rects[i][j].r.right + headX) && (pos.y < rectAll.top + rects[i][j].r.bottom + headY)) {
						if (rects[i][j].flag != true) { rects[i][j].flag = true; InvalidateRect(hwnd, NULL, FALSE);}
					} else {
						if (rects[i][j].flag != false) { rects[i][j].flag = false; InvalidateRect(hwnd, NULL, FALSE);}
					}
				}
			}
		return 0;
	case WM_DESTROY:
		if (hwnd == FindWindow(L"WindowClass", L"Board")) PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		rAm++; InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		rAm--; InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}