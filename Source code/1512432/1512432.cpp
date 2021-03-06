// 1512432.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512432.h"
#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include <wchar.h>
//#include <afxwin.h>
#pragma comment(lib, "comctl32.lib")
#define MAX_LOADSTRING 100

#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS_GLOBALS
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

// Global Variables:
HINSTANCE g_hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND g_hwnd;

WCHAR *items[] = { L"Ăn Uống",L"Di Chuyển",L"Nhà Cửa",L"Xe Cộ",L"Nhu Yếu Phẩm",L"Dịch Vụ" };

// Forward declarations of functions included in this code module:
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

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY1512432, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512432));

	MSG msg;

	// Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONEY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1512432);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MONEY));

	return RegisterClassExW(&wcex);
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
	g_hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"Quản Lý Chi Tiêu",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
		| WS_MINIMIZEBOX | WS_VISIBLE,
		200, 40, 1000, 690, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hwnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
int iSubItems = 0;
int cursel1 = 0;// lấy index của combobox thứ nhất: loại chi tiêu
int cursel2 = 6;// lấy index combobox thứ 2: tổng chi tiêu từng loại

// CÁC BUFFER CHỨA DỮ LIỆU TỪNG DÒNG
WCHAR *stt = NULL;
WCHAR *buffer_LCT = NULL;
WCHAR *buffer_noiDung = NULL;
WCHAR *buffer_soTien = NULL;
WCHAR *buffer_tongTien = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	InitBrushVector();

	POINT point;
	HMENU hMenu;

	switch (message)
	{

		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
	case WM_COMMAND:
	{
		int size; // lấy size trên ô dữ liệu
		double price; // số tiền chi tiêu trên textbox tương ứng

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_CBX_LCT: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				cursel1 = ComboBox_GetCurSel(cbx_LoaiChiTieu); // lấy index value trên cbx_loaiChiTieu
				if (cursel1 == 10) {
					MessageBox(NULL, L"Đã thêm đến giới hạn cho phép của phần mềm.", L"ERROR", MB_OK);
					break;
				}

				if (cursel1 == cbx_NameLCT.size()) { // nếu thêm mới
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ADDLCT), hWnd, About); // gọi dialog tạo mới loại chi tiêu
				}

			}
			break;
		}
		case IDC_BUTTON_ADD: {
			// LẤY THÔNG TIN LOẠI CHI TIÊU TRÊN COMBO BOX
			cursel1 = ComboBox_GetCurSel(cbx_LoaiChiTieu);
			if (cursel1 == cbx_NameLCT.size()) { // chưa chọn loại chi tiêu
				MessageBox(NULL, L"Xin chọn loại chi tiêu!", L"Warning", MB_OK);
				break;
			}
			size = GetWindowTextLength(cbx_LoaiChiTieu);
			buffer_LCT = new WCHAR[size + 1];
			GetWindowText(cbx_LoaiChiTieu, buffer_LCT, size + 1);
			sizeLineData += size;
			//------------------------------------
			// LẤY THÔNG TIN NỘI DUNG CHI TIÊU
			size = GetWindowTextLength(txb_NoiDung);
			buffer_noiDung = new WCHAR[size + 1];
			GetWindowText(txb_NoiDung, buffer_noiDung, size + 1);

			if (size == 0) {
				MessageBox(NULL, L"Vui Lòng nhập nội dung chi tiêu!!", L"Warning", MB_OK);
				break;
			}
			sizeLineData += size;
			//----------------------------------
			// LẤY THÔNG TIN GIÁ TIỀN
			size = GetWindowTextLength(txb_SoTien);
			buffer_soTien = new WCHAR[size + 1];
			GetWindowText(txb_SoTien, buffer_soTien, size + 1);

			price = ConvertWcharToDouble(buffer_soTien, size);
			if (price == -1)
				return 0;
			sizeLineData += size;

			//THÊM DỮ LIỆU CHI TIÊU VÀO LISTVIEW CHI TIẾT CHI TIÊU
			InsertDataToListView(buffer_LCT, buffer_noiDung, buffer_soTien);
			//---------------------------------------

			sizeLineData += 22;
			lineData = new WCHAR[sizeLineData];

			xLine = swprintf(lineData, sizeLineData, stt);
			xLine += swprintf(lineData + xLine, sizeLineData - xLine - 1, L"\t%ls", buffer_LCT);
			xLine += swprintf(lineData + xLine, sizeLineData - xLine - 1, L"\t\t%ls", buffer_noiDung);
			xLine += swprintf(lineData + xLine, sizeLineData - xLine - 1, L"\t%ls\r\n", buffer_soTien);

			lvs_data.push_back(lineData);

			sizeLineData = 0;

			cursel1 = ComboBox_GetCurSel(cbx_LoaiChiTieu); // LẤY INDEX LOẠI CHI TIÊU
			vt_tongTien[cursel1] += price;
			tongChiPhi += price;
			cursel2 = ComboBox_GetCurSel(cbx_Tong); // lấy index combo box tổng
			buffer_tongTien = new WCHAR[255];
			if (cursel2 == cbx_NameLCT.size()) {
				swprintf(buffer_tongTien, 255, L"%.2f", tongChiPhi);
				SetWindowText(txb_TongTien, buffer_tongTien);
			}
			else {
				swprintf(buffer_tongTien, 255, L"%.2f", vt_tongTien[cursel1]);
				SetWindowText(txb_TongTien, buffer_tongTien);
			}
			// THÊM GIÁ TRỊ Ở COMBOBOX LOAI CHI TIÊU VÀO VECTOR
			lvs_LoaiChiTieu.push_back(buffer_LCT);
			lvs_NoiDung.push_back(buffer_noiDung);
			lvs_SoTien.push_back(buffer_soTien);

			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			break;
		}
		case IDC_CBX_TCT:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				cursel2 = ComboBox_GetCurSel(cbx_Tong);
				buffer_tongTien = new WCHAR[255];
				if (cursel2 == vt_tongTien.size()) {
					swprintf(buffer_tongTien, 255, L"%.2f", tongChiPhi);
					SetWindowText(txb_TongTien, buffer_tongTien);
				}
				else {
					swprintf(buffer_tongTien, 255, L"%.2f", vt_tongTien[cursel2]);
					SetWindowText(txb_TongTien, buffer_tongTien);
				}
			}

			break;
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_NEW: {
			int res = MessageBox(g_hwnd, L"Bạn có muốn lưu file hiện tại?", L"Choose!!", MB_OKCANCEL || MB_ICONQUESTION);
			if (res == 1) {
				if (GetFileNameForSave()) {
					SaveDataFromListView_unicode(false);
					saved = true;
					Reset();
				}
			}
			else {
				Reset();
			}
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			//UpdateWindow(hWnd);
			break;
		}
		case ID_FILE_OPEN:
			OpenFile();
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			break;
		case ID_FILE_SAVE: {
			if (GetFileNameForSave()) {
				SaveDataFromListView_unicode(false);
			}
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_RBUTTONUP:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		hMenu = CreatePopupMenu();
		ClientToScreen(hWnd, &point);
		AppendMenu(hMenu, MF_STRING, ID_FILE_NEW, TEXT("&New"));
		AppendMenu(hMenu, MF_STRING, ID_FILE_OPEN, TEXT("&Open"));
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("&Quit"));
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
	case WM_DESTROY:
		if (saved == false)
			SaveDataFromListView_unicode();
		delete[]buffer_LCT;
		delete[]buffer_noiDung;
		delete[]buffer_soTien;
		delete[]buffer_tongTien;
		delete[]stt;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	WCHAR* buffer_temp = NULL;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			buffer_temp = new WCHAR[255];
			GetDlgItemText(hDlg, IDC_ADDLCT_EDIT, buffer_temp, 255);
			SendMessage(cbx_LoaiChiTieu, CB_INSERTSTRING, cbx_NameLCT.size(), (LPARAM)buffer_temp); // chèn vào combo box loại chi tiêu tại vị trí trước "add..."
			SendMessage(cbx_Tong, CB_INSERTSTRING, cbx_NameLCT.size(), (LPARAM)buffer_temp); // THÊM ITEM ĐẶC BIỆT "ADD..."
			cbx_NameLCT.push_back(buffer_temp); // thêm vào vector chưa tên các loại chi tiêu
			double newPrice = 0;
			vt_tongTien.push_back(newPrice);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		}
		if (LOWORD(wParam) == IDCANCEL) {

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
	// KHỞI TẠO

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống

	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// LẤY THÔNG TIN CỬA SỔ CHA
	RECT rect;
	GetClientRect(hWnd, &rect);

	//CÁC ITEM COMBO BOX LOẠI CHI TIÊU


	//GROUP THÊM CHI TIÊU
	gpbox = CreateWindow(TEXT("button"), TEXT("Thêm một loại chi tiêu"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 10, 580, 120, hWnd, (HMENU)0, g_hInst, NULL);

	// TẠO COMBO BOX THÊM CHI TIÊU
	//---------------------------------
	cbx_LoaiChiTieu = CreateWindowExW(0, L"combobox", L"", WS_CHILD | WS_VISIBLE | LBS_STANDARD | CBS_DISABLENOSCROLL | CBS_SIMPLE, rect.left + 30, rect.top + 80, 100, 110, hWnd, (HMENU)IDC_CBX_LCT, g_hInst, NULL);
	SendMessage(cbx_LoaiChiTieu, WM_SETFONT, WPARAM(hFont), TRUE); // SET FONT CHỮ
	int i;
	for (i = 0; i < 6; i++) {
		SendMessage(cbx_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)items[i]); // THÊM CÁC ITEMS
		cbx_NameLCT.push_back(items[i]); // thêm vào vector combobox chứa loại chi tiêu
		double temp = 0;
		vt_tongTien.push_back(temp);
	}
	SendMessage(cbx_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Add more..."); // THÊM ITEM ĐẶC BIỆT "ADD..."
	SendMessage(cbx_LoaiChiTieu, CB_SETCURSEL, 0, 0); // SET ITEM MẶT ĐỊNH

	// MÔ TẢ COMBO BOX
	label = CreateWindowEx(0, L"STATIC", L"Loại Chi Tiêu:", WS_CHILD | WS_VISIBLE | SS_LEFT, rect.left + 32, rect.top + 55, 80, 20, hWnd, NULL, g_hInst, NULL);
	SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);

	GetClientRect(cbx_LoaiChiTieu, &rect);

	//TẠO TEXTBOX NHẬP NỘI DUNG CHI TIÊU
	txb_NoiDung = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
		rect.right + 50,
		rect.bottom + 60,
		180,
		rect.bottom - rect.top,
		hWnd, NULL, g_hInst, NULL);
	SendMessage(txb_NoiDung, WM_SETFONT, WPARAM(hFont), TRUE);

	GetClientRect(txb_NoiDung, &rect);

	//TEXTBOS NHẬP SỐ TIỀN CHI TIÊU
	txb_SoTien = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
		rect.right + 160,
		rect.bottom + 60,
		120,
		rect.bottom - rect.top,
		hWnd, NULL, g_hInst, NULL);
	SendMessage(txb_SoTien, WM_SETFONT, WPARAM(hFont), TRUE);


	label = CreateWindowEx(0, L"STATIC", L"Nội Dung:", WS_CHILD | WS_VISIBLE | SS_LEFT, rect.right - 30, rect.top + 55, 80, 20, hWnd, NULL, g_hInst, NULL);
	SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);

	GetClientRect(txb_SoTien, &rect);

	//BUTTON THÊM NỘI DUNG MỚI NHẬP VÀO GHI NHỚ
	btn_Add = CreateWindowEx(0, L"BUTTON", L"Thêm", WS_CHILD |
		WS_VISIBLE | BS_PUSHBUTTON,
		rect.right + 360,
		rect.bottom + 60,
		80,
		rect.bottom - rect.top, hWnd, (HMENU)IDC_BUTTON_ADD, g_hInst, NULL);
	SendMessage(btn_Add, WM_SETFONT, WPARAM(hFont), TRUE);


	label = CreateWindowEx(0, L"STATIC", L"Số Tiền (vnđ):", WS_CHILD | WS_VISIBLE | SS_LEFT, rect.right + 230, rect.top + 55, 80, 20, hWnd, NULL, g_hInst, NULL);
	SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);

	// GROUP LISTVIEW THỂ HIỆN DANH SÁCH CÁC CHI TIÊU
	gpbox = CreateWindow(TEXT("button"), TEXT("Danh Sách Các Chi Tiêu"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 140, 580, 360, hWnd, (HMENU)0, g_hInst, NULL);

	// TẠO LIST VIEW
	lv = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
		WS_CHILD | WS_VISIBLE | LVS_SHOWSELALWAYS |
		WS_VSCROLL | LVS_REPORT | WS_BORDER | WS_TABSTOP,
		25, 170, 550, 320,
		hWnd, (HMENU)IDC_LISTVIEW, g_hInst, NULL);

	// CÀI ĐẶT CHỌN FULL DÒNG
	ListView_SetExtendedListViewStyle(lv, LVS_EX_FULLROWSELECT);

	Creat4Column();

	// TẠO KHUNG CHI TIẾT CÁC LOẠI CHI TIÊU
	gpbox = CreateWindow(TEXT("button"), TEXT("Thống Kê Chi Tiêu"), // GROUP THỐNG KÊ CHI TIÊU
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 515, 580, 100, hWnd, (HMENU)0, g_hInst, NULL);

	label = CreateWindowEx(0, L"STATIC", L"Tổng tiền từng loại chi tiêu:", WS_CHILD | WS_VISIBLE | SS_LEFT,
		40, 560, 200, 30, hWnd, NULL, g_hInst, NULL);

	// COMBO BOX HIỂN THỊ TỔNG TIỀN TỪNG LOẠI CHI TIÊU
	cbx_Tong = CreateWindowExW(0, L"combobox", L"",
		WS_CHILD | WS_VISIBLE | LBS_STANDARD | CBS_DISABLENOSCROLL | CBS_SIMPLE,
		240,
		560,
		100,
		110, hWnd, (HMENU)IDC_CBX_TCT, g_hInst, NULL);
	SendMessage(cbx_Tong, WM_SETFONT, WPARAM(hFont), TRUE); // SET FONT CHỮ

	for (int i = 0; i < 6; i++) {
		SendMessage(cbx_Tong, CB_ADDSTRING, 0, (LPARAM)items[i]); // THÊM CÁC ITEMS
	}
	SendMessage(cbx_Tong, CB_ADDSTRING, 0, (LPARAM)L"Tổng tiền");
	SendMessage(cbx_Tong, CB_SETCURSEL, vt_tongTien.size(), 0); // SET ITEM MẶT ĐỊNH

	// Ô TEXT BOX HIỂN THỊ GIÁ TRỊ TỔNG TIỀN ( READ ONLY)
	GetClientRect(txb_NoiDung, &rect);
	txb_TongTien = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
		360,
		560,
		120,
		rect.bottom - rect.top,
		hWnd, NULL, g_hInst, NULL);
	SendMessage(txb_TongTien, WM_SETFONT, WPARAM(hFont), TRUE);;

	label = CreateWindowEx(0, L"STATIC", L"Tổng (vnđ):", WS_CHILD | WS_VISIBLE | SS_LEFT,
		380, 540, 100, 20, hWnd, NULL, g_hInst, NULL);


	// TẠO KHUNG NHÌN HIỂN THỊ BIỂU ĐỒ CHI TIÊU
	gpbox = CreateWindow(TEXT("button"), TEXT("Biểu Đồ Chi Tiêu"),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		600, 10, 370, 605, hWnd, (HMENU)0, g_hInst, NULL);


	LoadQLCTFileTolvs_data();
	if (lvs_data.size() == 0) {
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return TRUE;
	}
	
	LoadDataToListView();
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return TRUE;
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	SolidBrush *m_Brush = new SolidBrush(Color::GreenYellow);

	Pen* pen = new Pen(g_color, g_WidthLine);
	pen->SetDashStyle(g_Style);

	Graphics *g = new Graphics(hdc);
	g->SetSmoothingMode(SmoothingModeAntiAlias);
	g->DrawPie(pen, 660, 40, 260, 230, 0.0, 360.0); // vẽ đường tròn biểu đồ
	g->FillPie(m_Brush, 660, 40, 260, 230, 0.0, 360.0); // vẽ biểu đồ mặc định 

	// tạo label chú thích cho biểu đồ chi tiêu
	label = CreateWindowEx(0, L"STATIC", L"Chú Thích:", WS_CHILD | WS_VISIBLE | SS_LEFT, 620, 290, 80, 20, hWnd, NULL, g_hInst, NULL);
	
	int width = 20; // chiều rộng chú thích

	int x = 620; // x bắt đầu
	int y = 320; // y bắt đầu

	WCHAR* t_buff = NULL;
	//delete[]g;
	double temp1 = 270.0, temp2 = 0.0;
	for (int i = 0; i < vt_tongTien.size(); i++) {

		temp2 = vt_tongTien[i] / tongChiPhi * 360; // độ rộng vùng được vẽ
		// vẽ đoạn vùng đường tròn thể hiện tỉ lệ chiếm giữ
		g->FillPie(vt_Brush[i], 660, 40, 260, 230, temp1, temp2);
		if (temp2 != 0) {
			temp1 += temp2;
			if (temp1 > 360)
				temp1 -= 360;
		}

		//// VIẾT TÊN  LOẠI CHI TIÊU
		t_buff = new WCHAR[wcslen(cbx_NameLCT[i]) + 2]{ 0 };
		swprintf(t_buff, wcslen(cbx_NameLCT[i]) + 2, L"%ls: ", cbx_NameLCT[i]);
		g->FillRectangle(vt_Brush[i], x, y, width, width);

		label = CreateWindowEx(0, L"STATIC", t_buff, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x + width + 10, y + 4, 80, 20, hWnd, NULL, g_hInst, NULL);
		SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);

		// VIẾT SỐ TỔNG TIỀN CỦA LOẠI CHI TIÊU ĐÓ
		t_buff = new WCHAR[50];
		swprintf(t_buff, 100, L"%.2f", vt_tongTien[i]);
		label = CreateWindowEx(0, L"STATIC", t_buff, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x + width + 10 + 100,
			y + 4,
			80, 20,
			hWnd, NULL, g_hInst, NULL);
		SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);

		// VIẾT PHẦN TRĂM TRÊN TỔNG TIỀN
		if (vt_tongTien[i] == 0.0)
		{
			label = CreateWindowEx(0, L"STATIC", L"(0 %)", WS_CHILD | WS_VISIBLE | SS_LEFT,
				x + width + 10 + 100 + 100, y + 4, 80, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);
		}
		else {
			swprintf(t_buff, 50, L"(%.2f %%)", vt_tongTien[i] / tongChiPhi * 100.0);
			label = CreateWindowEx(0, L"STATIC", t_buff, WS_CHILD | WS_VISIBLE | SS_LEFT,
				x + width + 10 + 100 + 100, y + 4, 80, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(label, WM_SETFONT, WPARAM(hFont), TRUE);
		}
		y += 30;
	}

	delete g;
	//delete[]t_buff;
	EndPaint(hWnd, &ps);
}

void Creat4Column()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 50;
	lvCol.pszText = (L"STT");
	ListView_InsertColumn(lv, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = (L"Loại Chi Tiêu");
	ListView_InsertColumn(lv, 1, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 200;
	lvCol.pszText = (L"Nội Dung");
	ListView_InsertColumn(lv, 2, &lvCol);

	lvCol.cx = 150;
	lvCol.pszText = (L"Số Tiền");
	ListView_InsertColumn(lv, 3, &lvCol);
}

bool GetFileNameForSave()
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hwnd;
	ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0 All File(*.*)\0*.*\0";
	ofn.lpstrFile = curfile2;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"txt";
	if (GetSaveFileName(&ofn)) {
		return true;
	}
	return false;
}

void Reset() {
	lvs_LoaiChiTieu.clear();
	lvs_NoiDung.clear();
	lvs_SoTien.clear();
	lvs_data.clear();
	for (int i = 0; i < vt_tongTien.size(); i++) {
		vt_tongTien[i] = 0.0;
	}
	iSubItems = 0;
	tongChiPhi = 0.0;
	ListView_DeleteAllItems(lv);
}

void InsertDataToListView(WCHAR* LCT, WCHAR* ND, WCHAR* ST) {
	LV_ITEM LV;
	LV.mask = LVIF_TEXT;
	stt = new WCHAR[255];
	swprintf(stt, 255, L"%.1d", iSubItems + 1);
	LV.iItem = iSubItems;
	LV.iSubItem = 0;
	LV.pszText = stt;

	ListView_InsertItem(lv, &LV); // SỐ THỨ TỰ

	ListView_SetItemText(lv, iSubItems, 1, LCT); // LOẠI CHI TIÊU
	ListView_SetItemText(lv, iSubItems, 2, ND); // NỘI DUNG CHI TIÊU
	ListView_SetItemText(lv, iSubItems, 3, ST); // SỐ TIỀN CHI TIÊU
	iSubItems++; // TĂNG STT LÊN

}

void SaveDataFromListView_unicode(bool t) {
	if (t == true)
		swprintf(curfile, 260, L"systemadmin.txt");
	else
		swprintf(curfile, 260, L"%ls", curfile2);
	HANDLE hFile;
	bool bsucces = false;
	hFile = CreateFile(curfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (int i = 0; i < lvs_data.size(); i++) {
		//messageBox(NULL, lvs_data[i], L"", MB_OK);
		if (hFile != INVALID_HANDLE_VALUE) {
			DWORD dwTextLength;
			dwTextLength = wcslen(lvs_data[i]);
			DWORD dwBufferSize = dwTextLength + 1;		//13
			WCHAR* pszText = new WCHAR[dwBufferSize * 2]{ '\0' };  //26 bytes
			swprintf(pszText, dwBufferSize * 2, L"%ls", lvs_data[i]);

			if (pszText != NULL)
			{
				DWORD dwWritten;
				WriteFile(hFile, pszText, dwTextLength * 2, &dwWritten, NULL);
			}
		}
	}
	CloseHandle(hFile);

}

void LoadQLCTFileTolvs_data(bool t) {
	HANDLE hFile;

	if (t == true)
		swprintf(curfile, 260, L"systemadmin.txt");
	else {
		swprintf(curfile, 260, L"%ls", curfile2);
	}
	bool bsucces = false;
	hFile = CreateFile(curfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		const DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize == 0) {
			CloseHandle(hFile);
			return;
		}
		if (dwFileSize != 0xFFFFFFFF)
		{
			DWORD dwRead = 0;
			WCHAR* tempftext = new WCHAR[dwFileSize / 2 + 1]();

			tempftext[dwFileSize / 2] = 0;

			if (ReadFile(hFile, (WCHAR*)tempftext, dwFileSize, &dwRead, NULL))
			{
				tempftext[dwFileSize / 2] = '\0';

				WCHAR* temp;
				WCHAR* token = NULL;
				temp = wcstok_s(tempftext, L"\r\n", &token);
				int size = wcslen(temp);
				if (size == 1) {
					CloseHandle(hFile);
					return;
				}

				while (temp != NULL) {
					size = wcslen(temp);
					WCHAR* data = new WCHAR[size + 5];
					swprintf(data, size + 5, L"%ls\r\n", temp);
					lvs_data.push_back(data);
					temp = wcstok_s(NULL, L"\r\n", &token);

				}
			}
			else {
				
				CloseHandle(hFile);
				return;
			}
		}
		CloseHandle(hFile);
	}
	else {
		
		CloseHandle(hFile);
	}
}

void OpenFile() {
	curfile[0] = '\0';
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hwnd;
	ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0All File(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = curfile2;
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"txt";
	if (GetOpenFileName(&ofn) == TRUE) {
		SaveDataFromListView_unicode();
		Reset();
		LoadQLCTFileTolvs_data(false);
		LoadDataToListView();
	}

}

double ConvertWcharToDouble(WCHAR* buffer_soTien, int size) {
	double price;
	WCHAR* pEnd; // chuỗi kiểm tra lỗi
	try
	{
		errno = 0;
		price = wcstod(buffer_soTien, &pEnd);
		if (*pEnd != '\0' || errno != 0) { // catch error nhập sai dữ liệu
			throw 20;
		}
		else if (size == 0) { // catch error chưa nhập dữ liệu
			throw 'a';
		}
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Nhập sai giá tiền!!", L"Warning", MB_OK);
		return -1;
	}
	catch (int e) {
		MessageBox(NULL, L"Số tiền phải là các giá trị số (0 - 9), xin kiểm tra lại.", L"", MB_OK);
		return -1;
	}
	catch (char a) {
		MessageBox(NULL, L"Chưa nhập dữ liệu cho phần số tiền chi tiêu.", L"", MB_OK);
		return -1;
	}
	return price;
}

void LoadDataToListView() {
	WCHAR* temp;
	double m_price;
	WCHAR* data = NULL;
	int size;

	for (int i = 0; i < lvs_data.size(); i++) {
		WCHAR *token = NULL;
		size = wcslen(lvs_data[i]) + 1;
		data = new WCHAR[size];
		swprintf(data, size, L"%ls", lvs_data[i]);
		temp = wcstok_s(data, L"\t", &token); // lấy ra stt và bỏ qua nó

		temp = wcstok_s(NULL, L"\t\t", &token); // lấy ra loại chi tiêu
		lvs_LoaiChiTieu.push_back(temp);
		temp = wcstok_s(NULL, L"\t", &token); // lấy ra nội dung
		lvs_NoiDung.push_back(temp);
		temp = wcstok_s(NULL, L"\r\n", &token); // lấy số tiền
		lvs_SoTien.push_back(temp);
		InsertDataToListView(lvs_LoaiChiTieu.back(), lvs_NoiDung.back(), lvs_SoTien.back()); // chèn vào listview

		m_price = ConvertWcharToDouble(lvs_SoTien.back(), wcslen(lvs_SoTien.back()));
		tongChiPhi += m_price;
		int j = 0;
		for (; j < cbx_NameLCT.size(); j++) {
			if (wcscmp(lvs_LoaiChiTieu.back(), cbx_NameLCT[j]) == 0) {
				vt_tongTien[j] += m_price;
				break;
			}
		}
		if (j == cbx_NameLCT.size()) {
			SendMessage(cbx_LoaiChiTieu, CB_INSERTSTRING, cbx_NameLCT.size(), (LPARAM)lvs_LoaiChiTieu.back()); // chèn vào combo box loại chi tiêu tại vị trí trước "add..."
			SendMessage(cbx_Tong, CB_INSERTSTRING, cbx_NameLCT.size(), (LPARAM)lvs_LoaiChiTieu.back()); // THÊM ITEM ĐẶC BIỆT "ADD..."
			cbx_NameLCT.push_back(lvs_LoaiChiTieu.back()); // thêm vào vector chưa tên các loại chi tiêu
			double newPrice = 0;
			vt_tongTien.push_back(newPrice);
			vt_tongTien[vt_tongTien.size() - 1] += m_price;

		}

	}
}

void InitBrushVector() {
	vt_Brush.push_back(new SolidBrush(Color::Yellow));
	vt_Brush.push_back(new SolidBrush(Color::Aqua));
	vt_Brush.push_back(new SolidBrush(Color::Blue));
	vt_Brush.push_back(new SolidBrush(Color::BlueViolet));
	vt_Brush.push_back(new SolidBrush(Color::Chartreuse));
	vt_Brush.push_back(new SolidBrush(Color::Crimson));
	vt_Brush.push_back(new SolidBrush(Color::DarkGoldenrod));
	vt_Brush.push_back(new SolidBrush(Color::ForestGreen));
	vt_Brush.push_back(new SolidBrush(Color::SlateGray));
	vt_Brush.push_back(new SolidBrush(Color::DeepPink));
	vt_Brush.push_back(new SolidBrush(Color::Tomato));
}