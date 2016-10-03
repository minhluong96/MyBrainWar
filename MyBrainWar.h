#pragma once

#include "resource.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <ctime>
#include "Settings.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define BUFFSIZE 260

void onCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void onPaint(HWND hWnd);
void onDestroy(HWND hWnd);
BOOL onCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void onTimer(HWND hWnd, UINT id);
void StartGame(HWND hWnd); 
void CheckResult(HWND hWnd); 
void UpdateHighScore(); // check and save highscore to config.ini
void LoadHighScore(); // load highscore from config.ini
void Reset();//reset highscore


//Global variables
HWND Watch, Button1, Button2, Answer, Ques, ScoreBox;
int Score, Result, Time, HighScore;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name