// MyBrainWar.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyBrainWar.h"

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_MYBRAINWAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYBRAINWAR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYBRAINWAR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYBRAINWAR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   
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
		HANDLE_MSG(hWnd, WM_CREATE, onCreate);
		HANDLE_MSG(hWnd, WM_PAINT, onPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, onCommand);
		HANDLE_MSG(hWnd, WM_TIMER, onTimer);
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

void onPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hWnd, &ps);

	SaveRect(hWnd);
}

void onDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

BOOL onCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	LoadRect(hWnd);
	LoadHighScore();

	// Take system's font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HFONT bigFont = CreateFont(40, 15, 0, 700, 0, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, TEXT("Arial"));

	Score = 0;
	Time = 5;

	//Create boxes
	HWND hwnd = CreateWindowEx(0, L"STATIC", L"Remain time", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_BORDER, 250, 200, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Brain War", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE  , 255, 30, 200, 80, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(bigFont), TRUE);


	Watch = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_BORDER, 360, 200, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(Watch, WM_SETFONT, WPARAM(hFont), TRUE);

	ScoreBox = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_BORDER, 360, 150, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(ScoreBox, WM_SETFONT, WPARAM(hFont), TRUE);

	Button1 = CreateWindowEx(0, L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE, 300, 250, 100, 40, hWnd, (HMENU)IDC_BUTTON_1, hInst, NULL);
	SendMessage(Button1, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Player Score", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_BORDER, 250, 150, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	Ques = CreateWindowEx(0, L"STATIC", L"Press Start to play", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_BORDER, 250, 350, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(Ques, WM_SETFONT, WPARAM(hFont), TRUE);

	Answer = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_CENTER, 360, 363, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(Answer, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_WHITERECT, 360, 350, 100, 40, hWnd, NULL, hInst, NULL);

	Button2 = CreateWindowEx(0, L"BUTTON", L"Finish", WS_CHILD | WS_VISIBLE | WS_DISABLED, 300, 400, 100, 40, hWnd, (HMENU)IDC_BUTTON_2, hInst, NULL);
	SendMessage(Button2, WM_SETFONT, WPARAM(hFont), TRUE);

	return true;
}

void onCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	case IDM_RESET:
		Reset();
		break;

	case IDC_BUTTON_1:
		StartGame(hWnd);
		break;

	case IDC_BUTTON_2:
		CheckResult(hWnd);
		break;
	}
}

void StartGame(HWND hWnd)
{
	EnableWindow(Button2, TRUE);
	EnableWindow(Button1, FALSE);
	WCHAR *buff = new WCHAR[20];
	srand(time(NULL));
	int a, b, temp;
	Time = 5;
	temp = rand() % 3;
	
	SetTimer(hWnd, IDC_TIMER_1, 5000, (TIMERPROC)NULL);
	SetTimer(hWnd, IDC_TIMER_2, 1000, (TIMERPROC)NULL);
	SetWindowText(Watch, L"5");

	switch (temp)
	{
	case 0:
	{
			  a = rand() % 100 + 1;
			  b = rand() % 100 + 1;
			  swprintf(buff, 20, L"%d + %d", a, b);
			  SetWindowText(Ques, buff);
			  Result = a + b;
	}break;

	case 1:
	{
			  a = rand() % 100 + 1;
			  b = rand() % 100 + 1;
			  swprintf(buff, 20, L"%d - %d", a, b);
			  SetWindowText(Ques, buff);
			  Result = a - b;
	}break;

	case 2:
	{
			  a = rand() % 50 + 1;
			  b = rand() % 10 + 1;
			  swprintf(buff, 20, L"%d * %d", a, b);
			  SetWindowText(Ques, buff);
			  Result = a * b;
	}break;

	case 3:
	{
			  Result = rand() % 50 + 1;
			  b = rand() % 10 + 1;
			  a = Result * b;
			  swprintf(buff, 20, L"%d / %d", a, b);
			  SetWindowText(Ques, buff);
	}break;

	}
}

void onTimer(HWND hWnd, UINT id)
{
	switch (id)
	{
	case IDC_TIMER_1:
	{
						CheckResult(hWnd);
	}

	case IDC_TIMER_2:
	{
						Time--;
						WCHAR *temp = new WCHAR[2];
						swprintf(temp, 2, L"%d", Time);
						SetWindowText(Watch, temp);
	}
	}
}

void CheckResult(HWND hWnd)
{
	EnableWindow(Button2, FALSE);
	EnableWindow(Button1, TRUE);
	KillTimer(hWnd, IDC_TIMER_1);
	KillTimer(hWnd, IDC_TIMER_2);

	int size, result, msgBox;
	WCHAR* buff;

	size = GetWindowTextLength(Answer);
	buff = new WCHAR[size + 1];

	GetWindowText(Answer, buff, size + 1);
	result = _wtoi(buff);

	if (result == Result)
	{
		Score++;
		WCHAR *score = new WCHAR[5];
		swprintf(score, 5, L"%d", Score);
		SetWindowText(ScoreBox, score);
		SetWindowText(Answer, L"");

		msgBox = MessageBox(hWnd, L"Your answer is correct", L"Congratulations", MB_OK);
	}
	else
	{
		UpdateHighScore();

		//Show score
		WCHAR *buff = new WCHAR[BUFFSIZE];
		swprintf(buff, BUFFSIZE, L"High Score: %d\nYour score: %d", HighScore, Score);
		msgBox = MessageBox(hWnd, buff, L"Your answer is wrong", MB_OKCANCEL | MB_ICONSTOP);
		
		//Reset score and window
		WCHAR *score = new WCHAR[5];
		Score = 0;
		swprintf(score, 5, L"%d", Score);
		SetWindowText(ScoreBox, score);
		SetWindowText(Answer, L"");
	}

	if (IDCANCEL == msgBox)
	{
		SendMessage(hWnd, WM_DESTROY, NULL, TRUE);
	}
	else
	{
		//Do nothing here
	}
}

void UpdateHighScore()
{
	if (Score > HighScore)
	{
		HighScore = Score;

		WCHAR *path = new WCHAR[BUFFSIZE];
		WCHAR *buff = new WCHAR[BUFFSIZE];

		//Take current path
		GetCurrentDirectory(BUFFSIZE, path);
		wcscat_s(path, BUFFSIZE, L"\\config.ini");
		
		//Write highscore
		swprintf(buff, BUFFSIZE, L"%d", HighScore);
		WritePrivateProfileString(L"app", L"HighScore", buff, path);
	}
	else
	{
		//Do nothing
	}
}

void LoadHighScore()
{
	WCHAR *path = new WCHAR[BUFFSIZE];
	WCHAR *buff = new WCHAR[BUFFSIZE];

	//Take current path
	GetCurrentDirectory(BUFFSIZE, path);
	wcscat_s(path, BUFFSIZE, L"\\config.ini");

	GetPrivateProfileString(L"app", L"HighScore", L"0", buff, BUFFSIZE, path);
	HighScore = _wtoi(buff);
}

void Reset()
{
	HighScore = 0;
	WCHAR *path = new WCHAR[BUFFSIZE];
	//Take current path
	GetCurrentDirectory(BUFFSIZE, path);
	wcscat_s(path, BUFFSIZE, L"\\config.ini");

	WritePrivateProfileString(L"app", L"HighScore", 0, path);
}