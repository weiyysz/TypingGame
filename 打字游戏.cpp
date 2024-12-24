#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "打字游戏.h"
#include "stdio.h"
#include "Windows.h"
#include <iostream>
#define MAX_LOADSTRING 100\

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int left = 20, top = 20, right = left + 250, bottom = top + 450;
char c1, c2;
int x = -1, y = -1, status = 0;
int iScoring = 0, iFail = 0;
int gameover = 0;
void DrawBk(HDC hdc, int x1, int y1, int x2, int y2)
{
    Rectangle(hdc, x1, y1, x2, y2);
    char s[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    TextOutA(hdc, x1 + 5, y2 - 25, s, strlen(s));
}
void ShowScoring(HDC hdc, int x, int y, int iScoring, int iFail)
{
    char szTemp[32];
    TextOutA(hdc, x, y, "当前得分：", strlen("当前得分："));
    y = y + 20;
    sprintf(szTemp, "%d", iScoring);
    TextOutA(hdc, x, y, szTemp, strlen(szTemp));
    y = y + 20;
    TextOutA(hdc, x, y, "当前失误：", strlen("当前失误："));
    y = y + 20;
    sprintf(szTemp, "%d", iFail);
    TextOutA(hdc, x, y, szTemp, strlen(szTemp));
    y = y + 50;
    TextOutA(hdc, x, y, "2024 by weiyysz", strlen("2024 by weiyysz"));
}
void GameOver(HDC hdc, int x, int y)
{
    COLORREF OldColor, NewColor = RGB(rand() % 255, rand() % 255, rand() % 255);
    OldColor = SetTextColor(hdc, NewColor);
    TextOutA(hdc, x, y, "GAME OVER !", strlen("GAME OVER !"));
    SetTextColor(hdc, OldColor);
}
void Start(HDC hdc, int x, int y)
{
        TextOutA(hdc, x, y, "请按空格键开始", strlen("请按空格键开始"));
        Sleep(300);
        TextOutA(hdc, x, y, "                                  ", strlen("                                  "));
        Sleep(300);
}
void Wait(HDC hdc, int x, int y)
{
    TextOutA(hdc, x, y, "游戏暂停3秒", strlen("游戏暂停3秒"));
    Sleep(3000);
}
void Fire(HDC hdc, int x, int y1, int y2)
{
    HPEN hOldPen, hNewPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));
    hOldPen = (HPEN)SelectObject(hdc, hNewPen);
    MoveToEx(hdc, x, y1, NULL);
    LineTo(hdc, x, y2);
    Sleep(100);
    HPEN hNewPen2 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hNewPen2);
    MoveToEx(hdc, x, y1, NULL);
    LineTo(hdc, x, y2);
    SelectObject(hdc, hOldPen);
    DeleteObject(hNewPen);
    DeleteObject(hNewPen2);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//

// 计算屏幕尺寸以居中窗口
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int windowWidth = 450; // 窗口宽度
int windowHeight = 550; // 窗口高度

int xPos = (screenWidth - windowWidth) / 2;
int yPos = (screenHeight - windowHeight) / 2;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU,
       xPos, yPos, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            y = y + iScoring / 10 + 1;
            if (y > bottom - 40)
            {
                gameover = 2;
                KillTimer(hWnd, 1);
                char s[32];
                sprintf(s, "你的得分是 %d", iScoring);
                MessageBoxA(hWnd, s, "游戏结束", 0);
                SetTimer(hWnd, 2, 300, NULL);
            }
            InvalidateRect(hWnd, 0, 0);
            break;
        case 2:
            InvalidateRect(hWnd, 0, 0);
            break;
        case 3:
            InvalidateRect(hWnd, 0, 0);
            break;
        }
        break;
    case WM_CHAR:
    {
        HDC hdc = GetDC(hWnd);
        if (wParam == 32)
        {
            switch (status)
            {
            case 0:
            {
                KillTimer(hWnd, 3);
                InvalidateRect(hWnd, 0, 0);

                if (gameover == 1)
                    KillTimer(hWnd, 2);
                status = 1;
                gameover = 1;
                iScoring = 0;
                iFail = -1;
                c1 = rand() % 26 + 'A';
                x = left + 5 + (c1 - 'A') * 9;
                y = top;
                SetTimer(hWnd, 1, 10, NULL);
                break;
            }
            case 1:
            {
                status = 2;
                Wait(hdc, left + 85, top + 180);
                iFail = iFail - 1;
                status = 1;
                break;
            }
            default:
                iFail = iFail - 1;
            }
        }
        c2 = (wParam >= 'a' && wParam <= 'z') ? wParam + 'A' - 'a' : wParam;
        Fire(hdc, left + 5 + (c2 - 'A') * 9 + 4, top, bottom);
        ReleaseDC(hWnd, hdc);
        if (c2 == c1)
        {
            c1 = rand() % 26 + 'A';
            x = left + 5 + (c1 - 'A') * 9;
            y = top;
            iScoring = iScoring + 1;
        }
        else
            iFail = iFail + 1;
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case ID_START:
                KillTimer(hWnd, 3);
                InvalidateRect(hWnd, 0, 0);
                if(gameover == 1)
                    KillTimer(hWnd, 2);
                gameover = 1;
                iScoring = 0;
                iFail = 0;
                status = 1;
                c1 = rand() % 26 + 'A';
                x = left + 5 + (c1 - 'A') * 9;
                y = top;
                SetTimer(hWnd, 1, 10, NULL);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_MANUAL:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_MANUAL), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            DrawBk(hdc, left, top, right, bottom);
            ShowScoring(hdc, right + 20, top + 50, iScoring, iFail);
            if (gameover == 2)
            {
                GameOver(hdc, left + 80, top + 130);
                Start(hdc, left + 73, top + 180);
                status = 0;
            }       
            else
            {
                char szTemp[32];
                sprintf(szTemp, "%c", c1);
                TextOutA(hdc, x, y, szTemp, strlen(szTemp));
            }
            if (gameover == 0)
            {
                SetTimer(hWnd, 3, 600, NULL);
                status = 0;
                Start(hdc, left + 73, top + 180);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
