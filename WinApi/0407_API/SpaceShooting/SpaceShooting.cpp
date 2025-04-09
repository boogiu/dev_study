// SpaceShooting.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "SpaceShooting.h"
#include <list>

#pragma region 초기화면
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
	LoadStringW(hInstance, IDC_SPACESHOOTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPACESHOOTING));

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

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACESHOOTING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr; //메뉴 삭제
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, L"총 쏘기 게임", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		800, 600,			//창크기 조절
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
#pragma endregion

#pragma region 플레이어

	RECT RPlayer{ 200,200,250,250 };
	int iMoveSpeed = 20; //플레이어 이동 속도

	void RenderPlayer(HDC hdc);
	void MovePlayer(WPARAM wParam);

#pragma endregion

#pragma region 불릿
	//오브젝트의 사이즈
	struct tagSize {
		LONG xSize;
		LONG ySize;
	} typedef objSize;

	std::list<RECT> BulletList; //오브젝트의 리스트

	void MakeElipse(tagSize BulletSize);
	void RenderElipse(HDC hdc);
	void MoveElipse();
#pragma endregion

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 0, 0, nullptr);
		break;
	case WM_TIMER:
		MoveElipse();
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
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
		Rectangle(hdc, 0, 10, 800,15);
		RenderPlayer(hdc);   
		RenderElipse(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}
		MovePlayer(wParam);
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

// 플레이어의 출력
void RenderPlayer(HDC hdc)
{
	Rectangle(hdc, RPlayer.left, RPlayer.top, RPlayer.right, RPlayer.bottom);
}

 //플레이어의 이동(키다운)
void MovePlayer(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		RPlayer.bottom -= iMoveSpeed;
		RPlayer.top -= iMoveSpeed;
		break;

	case VK_DOWN:
		RPlayer.bottom += iMoveSpeed;
		RPlayer.top += iMoveSpeed;
		break;

	case VK_RIGHT:
		RPlayer.right += iMoveSpeed;
		RPlayer.left += iMoveSpeed;
		break;

	case VK_LEFT:
		RPlayer.right -= iMoveSpeed;
		RPlayer.left -= iMoveSpeed;
		break;

	case VK_SPACE:
		MakeElipse({ 10,10 }); //원의 위치 정보 생성(RECT)
		break;

	default:
		break;
	}
}

//원의 위치 조정 : 플레이어 위치 기반 (윗변 중간)
void MakeElipse(tagSize bulletSize)
{
	//플레이어 x - Size
	LONG LDistance = RPlayer.right - RPlayer.left;
	//플레이어 중간 좌표 
	LONG LCenter = RPlayer.left + (LDistance / 2);

	//좌표 채로 리스트에 추가
	BulletList.push_back({
		LCenter - (bulletSize.xSize / 2),
		RPlayer.top + (bulletSize.ySize / 2),
		LCenter + (bulletSize.xSize / 2) ,
		RPlayer.top - (bulletSize.ySize / 2)
		});
}

// 원의 출력 : 리스트 순회(좌표 기반 렌더링)
void RenderElipse(HDC hdc)
{
	auto iter = BulletList.begin();
	for (; iter != BulletList.end(); ++iter) {
		Ellipse(hdc, iter->left, iter->top, iter->right, iter->bottom);
	}
}

//원의 이동 : 리스트 순회
void MoveElipse()
{
	auto iter = BulletList.begin();
	for (;iter != BulletList.end();)
	{
		iter->top -= 2;
		iter->bottom -= 2;

		if (iter->bottom < 15) { //화면 밖에 나가면 삭제
			iter = BulletList.erase(iter);
		}
		else {
			++iter;
		}
	}
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


