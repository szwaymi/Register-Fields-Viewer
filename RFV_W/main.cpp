
#include <Windows.h>

HINSTANCE ghInstance;
static HANDLE ghHeap;
LRESULT CALLBACK rProc(HWND hWnd, UINT iMsg, WPARAM iPW, LPARAM iPL)
{
	switch (iMsg)
	{
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0; }
	case WM_PAINT: {
		PAINTSTRUCT mPS;
		HDC hDC = BeginPaint(hWnd, &mPS);
		EndPaint(hWnd, &mPS);
		return 0; }
	}
	return DefWindowProc(hWnd, iMsg, iPW, iPL);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int iCmd)
{
	const wchar_t cClass[] = L"WC: Register Field Viewer GUI";
	ghInstance = hInstance;
	if((ghHeap = HeapCreate(0, 0, 0))==0)
	{
		MessageBox(NULL, L"The Heap can't be Created.", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Register Window Class
	{
		WNDCLASS mWC = { };
		mWC.lpfnWndProc = rProc;
		mWC.hInstance = hInstance;
		mWC.lpszClassName = cClass;
		mWC.hCursor = LoadCursor(NULL, IDC_ARROW);
		mWC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//mWC.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
		RegisterClass(&mWC);
	}
	//Create Window
	{
		HWND hWnd = CreateWindowEx(
			0, cClass, L"Simple Server GUI", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL
		);
		if (hWnd == NULL)
		{
			MessageBox(NULL, L"The Window can't be Created", L"Error", MB_OK | MB_ICONERROR);
			return 1;
		}
		ShowWindow(hWnd, iCmd);
	}
	//Message Loop
	{
		MSG mMsg = { };
		while (GetMessage(&mMsg, NULL, 0, 0))
		{
			TranslateMessage(&mMsg);
			DispatchMessage(&mMsg);
		}
	}
	HeapDestroy(ghHeap);
	return 0;
}