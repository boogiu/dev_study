// DrawSomething.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DrawSomething.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWSOMETHING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWSOMETHING));

    MSG msg;

    // 기본 메시지 루프입니다:
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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWSOMETHING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; // 메뉴 삭제
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, L"아무거나 그려오기", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam); 
            // 메뉴 선택을 구문 분석합니다
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            //숙
            MoveToEx(hdc, 50, 50, nullptr);
            LineTo(hdc, 25, 75);
            MoveToEx(hdc, 50, 50, nullptr);
            LineTo(hdc, 75, 75);
            MoveToEx(hdc, 25, 100, nullptr);
            LineTo(hdc, 75, 100);
            MoveToEx(hdc, 50, 100, nullptr);
            LineTo(hdc, 50, 125);
            MoveToEx(hdc, 25, 125, nullptr);
            LineTo(hdc, 75, 125);
            LineTo(hdc, 75, 150);

            //제
            MoveToEx(hdc,100, 50, nullptr);
            LineTo(hdc, 150, 50);
            MoveToEx(hdc, 125, 50, nullptr);
            LineTo(hdc, 100, 100);
            MoveToEx(hdc, 125, 50, nullptr);
            LineTo(hdc, 150, 100);
            MoveToEx(hdc, 140, 75, nullptr);
            LineTo(hdc, 160, 75);
            MoveToEx(hdc, 160, 50, nullptr);
            LineTo(hdc, 160, 150);
            MoveToEx(hdc, 175, 50, nullptr);
            LineTo(hdc, 175, 150);

            //완
            Ellipse(hdc, 200, 50, 250, 100);
            MoveToEx(hdc, 225, 100, nullptr);
            LineTo(hdc, 225, 125);
            LineTo(hdc, 200, 125);
            LineTo(hdc, 260, 125);
            LineTo(hdc, 260, 50);
            LineTo(hdc, 260, 130);
            MoveToEx(hdc, 260, 110, nullptr);
            LineTo(hdc, 275, 110);

            MoveToEx(hdc, 210, 125, nullptr);
            LineTo(hdc, 210, 150);
            LineTo(hdc, 275, 150);

            //료
            MoveToEx(hdc, 325, 50, nullptr);
            LineTo(hdc, 375, 50);
            LineTo(hdc, 375, 75);
            LineTo(hdc, 325, 75);
            LineTo(hdc, 325, 100);
            LineTo(hdc, 375, 100);
            MoveToEx(hdc, 310, 150, nullptr);
            LineTo(hdc, 390, 150);
            MoveToEx(hdc, 335, 150, nullptr);
            LineTo(hdc, 335, 110);
            MoveToEx(hdc, 365, 150, nullptr);
            LineTo(hdc, 365, 110);

            //body
            Ellipse(hdc, 400, 100, 500, 200);
            Ellipse(hdc, 425, 150, 475, 200);
            Ellipse(hdc, 400, 190, 440, 210);
            Ellipse(hdc, 460, 190, 500, 210);

            //face
            Ellipse(hdc, 410, 40, 440, 70); 
            Ellipse(hdc, 420, 50, 430, 60);

            Ellipse(hdc, 460, 40, 490, 70);
            Ellipse(hdc, 470, 50, 480, 60);

            Ellipse(hdc, 400, 50, 500, 150); // 450 100 Face Round
            //Ellipse(hdc, 410, 80, 490, 150); // 450 100 Face Round

            
            Ellipse(hdc, 420, 75, 445, 100);  // 432,87 
            Ellipse(hdc, 426, 81, 438, 94); 

            Ellipse(hdc, 455, 75, 480, 100); // 467,87
            Ellipse(hdc, 461, 81, 474, 94);

            Ellipse(hdc, 440, 110, 460, 145);

            Ellipse(hdc, 430, 105, 450, 125);
            Ellipse(hdc, 450, 105, 470, 125);

            MoveToEx(hdc, 435, 110, nullptr);
            LineTo(hdc, 420, 105);

            MoveToEx(hdc, 435, 115, nullptr);
            LineTo(hdc, 420, 115);


            MoveToEx(hdc, 435, 120, nullptr);
            LineTo(hdc, 420, 125);

            MoveToEx(hdc, 465, 110, nullptr);
            LineTo(hdc, 475, 105);

            MoveToEx(hdc, 465, 115, nullptr);
            LineTo(hdc, 475, 115);


            MoveToEx(hdc, 465, 120, nullptr);
            LineTo(hdc, 475, 125);


            //굿
            //Ellipse(hdc, 400, 50, 500, 150); // 450 100 Face Round
           

            MoveToEx(hdc, 520, 75, nullptr);
            LineTo(hdc, 535, 50);
            MoveToEx(hdc, 525, 80, nullptr);
            LineTo(hdc, 545, 70);
            MoveToEx(hdc, 525, 85, nullptr);
            LineTo(hdc, 550, 90);

            MoveToEx(hdc, 560, 45, nullptr);
            LineTo(hdc, 570, 45);
            LineTo(hdc, 570, 55);

            MoveToEx(hdc, 555, 60, nullptr);
            LineTo(hdc, 575, 60);

            MoveToEx(hdc, 565, 60, nullptr);
            LineTo(hdc, 565, 70);

            MoveToEx(hdc, 565, 75, nullptr);
            LineTo(hdc, 555, 85);

            MoveToEx(hdc, 565, 75, nullptr);
            LineTo(hdc, 575, 85);

            MoveToEx(hdc, 585, 45, nullptr);
            LineTo(hdc, 590, 45);
            LineTo(hdc, 586, 75);
            LineTo(hdc, 584, 75);
            LineTo(hdc, 580, 45);
            LineTo(hdc, 585, 45);

            Ellipse(hdc, 581, 77, 589, 85);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        default:
            break;
        }
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
