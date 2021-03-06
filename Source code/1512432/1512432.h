#pragma once

#include "resource.h"
#include <commdlg.h>
#include <vector>
#include <Richedit.h>
#include <fstream>
#include <iostream>
#include <string>
#include <windowsx.h>
#include <WinUser.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <gdiplusinit.h>

using namespace Gdiplus;
using namespace std;

#define MAX_PATH 200
// CÁC HANDLE ĐẾN CÁC CONTROL 
HWND gpbox;
HWND cbx_LoaiChiTieu;
HWND txb_NoiDung;
HWND txb_SoTien;
HWND btn_Add;
HWND label;
HWND lv;
HWND cbx_Tong;
HWND txb_TongTien;
LOGFONT lf;
HFONT hFont;


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnPaint(HWND hWnd);
bool ChuThich = false;

void Creat4Column();

//LPCWSTR temp = (LPCWSTR)LoaiChiTieu[0];

vector<WCHAR*> cbx_NameLCT; // tên các loại chi tiêu
vector<double> vt_tongTien; // các phần tử là tổng tiền từng loại chi tiêu
double tongChiPhi = 0;

// CÁC VECTOR CHỨA DỮ LIỆU TRÊN BẢNG
vector<WCHAR*> lvs_LoaiChiTieu; // chứa thông tin các loại chi tiêu trên cbx_LoaiChiTieu
vector<WCHAR*> lvs_NoiDung; // Chứa dữ liệu trên txb_noiDung
vector<WCHAR*> lvs_SoTien; // txb_SoTien

WCHAR curfile[MAX_PATH];

WCHAR curfile2[MAX_PATH];
// CÁC BIÊN HỖ TRỢ LƯU DỮ LIỆU TỪNG DÒNG VÀO LVS_DATA
WCHAR *lineData = NULL;
int sizeLineData = 0;
int xLine;

vector<WCHAR*> lvs_data; // VECTOR LƯU CÁC DÒNG DỮ LIỆU TRÊN LIST VIEW

// LƯU GIỮ LIỆU KHI KẾT THÚC PHIÊN LÀM VIỆC XUỐNG FILE TEXT
void SaveDataFromListView_unicode(bool t = true);
// LOAD DỮ LIỆU TỪ FILE TEXT VÀO LVS_DATA LÚC TẠO FORM CHA
void LoadQLCTFileTolvs_data(bool t = true);
// load dữ liệu từ lvs_data vào form listview lúc tạo form cha
void LoadDataToListView();
//CHÈN 1 DÒNG DỮ LIỆU MỚI VÀO LISTVIEW
void InsertDataToListView(WCHAR* LCT, WCHAR* ND, WCHAR* ST);
//ĐẾM SỐ DÒNG TRONG FILE TEXT LƯU TỪ LẦN HOẠT ĐỘNG TRƯỚC
//int NumberLine(WCHAR* text);
//CHUYỂN ĐỔI EDIT CONTROL GIÁ TIỀN TỪ WCHAR* -> DOUBLE
double ConvertWcharToDouble(WCHAR* buffer_soTien, int size);

vector<Brush*> vt_Brush;

void OpenFile();

void InitBrushVector();
bool saved = false;
bool GetFileNameForSave();
void Reset();

DashStyle g_Style = DashStyleCustom;
Color g_color = Color::Black;
int g_WidthLine = 2;
