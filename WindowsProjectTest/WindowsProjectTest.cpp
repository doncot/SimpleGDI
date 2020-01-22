// WindowsProjectTest.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "WindowsProjectTest.h"
#include <d3d12.h>

#define D3DCOMPILE_DEBUG 1
#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

HBITMAP g_hMemoryBitmap;
HDC g_hMemoryDC;

enum class InputState
{
    Neutral,
    Left,
    Right,
};

// このコード モジュールに含まれる関数の宣言を転送します:
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

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECTTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECTTEST));

    MSG msg;

    // メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECTTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECTTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    static int d = 1;
    static InputState state = InputState::Neutral;
    static HBITMAP hBitmap;

    switch (message)
    {
    case WM_CREATE:
        {
            /* ウインドウのHDC取得 */
            auto hDc = GetDC(hWnd);
            /* ウインドウのHDCと互換性のある(ram上にある)デバイスコンテキスト作成 */
            g_hMemoryDC = CreateCompatibleDC(hDc);
            g_hMemoryBitmap = CreateCompatibleBitmap(hDc, width, height);
            /* ウインドウのHDC解放 */
            ReleaseDC(hWnd, hDc);

            SelectObject(g_hMemoryDC, g_hMemoryBitmap);

            // 背景クリア
            auto hBackgroundBrush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
            FillRect(g_hMemoryDC, &clientRect, hBackgroundBrush);

            //for (int i = 0; i < 256; i++) {
            //    for (int j = 0; j < 256; j++) {
            //        SetPixel(g_hMemoryDC, j, i, RGB(128, j, i));
            //    }
            //}

            SetTimer(hWnd, 1, 16, NULL);

            // ロード
            hBitmap = (HBITMAP)LoadImage(NULL, _T("1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
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
            {
                BitBlt(hdc, 0, 0, width, height, g_hMemoryDC, 0, 0, SRCCOPY);


                auto hBuffer = CreateCompatibleDC(hdc);
                SelectObject(hBuffer, hBitmap);
                BitBlt(hdc, 100, 100, 64, 64, hBuffer, 0, 0, SRCCOPY);
                DeleteDC(hBuffer);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        {
            // Update

            //d++;
            if (state == InputState::Left)
                d -= 5;
            else if (state == InputState::Right)
                d += 5;

            // 背景クリア
            auto hBackgroundBrush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
            FillRect(g_hMemoryDC, &clientRect, hBackgroundBrush);

            TCHAR szText[] = _T("test!!");
            TextOut(g_hMemoryDC, 0, 0, szText, lstrlen(szText));
            Rectangle(g_hMemoryDC, 50 + d, 10 + d, 200 + d, 100 + d);
            Rectangle(g_hMemoryDC, 250, 50 + d, 400, 150 + d);
            //RECT refreshRect = { 50 + d, 50 + d, 200 + d, 150 + d };
            InvalidateRect(hWnd, nullptr, FALSE);
        }
        break;

    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_LEFT:
                state = InputState::Left;
                break;
            case VK_RIGHT:
                state = InputState::Right;
                break;
            default:
                break;
            }
        }
        break;

    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
        case VK_RIGHT:
            state = InputState::Neutral;
            break;
        default:
            break;
        }
        break;

    case WM_DESTROY:
        /* メモリデバイスコンテキスト削除 */
        DeleteDC(g_hMemoryDC);
        /* DDB削除 */
        DeleteObject(g_hMemoryBitmap);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
