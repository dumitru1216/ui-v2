#pragma once
#include <windows.h>
#include <windowsx.h>
#include <memory>
#include <sstream>
#include <iostream>
#include <functional>
#include <WinUser.h>
#include <string_view>
#include <map>
#include <cmath>
#include <algorithm>
#include <chrono>

/* directx includes */
#include <d3d9.h>
#include <d3dx9.h>

/* libs for directx */
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")

#pragma warning ( disable : 4005 )
#pragma warning ( disable : 26495 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4715 )

/* the coding style will troll retards */
#include "sdk/u-sdk/c_sdk.hpp"
#include "sdk/warper/c_warper.hpp"

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_freetype.h"
#include "thirdparty/imgui/imgui_impl_win32.h"
#include "thirdparty/imgui/imgui_impl_dx9.h"

#include "sdk/warper/c_macro.hpp"
#include "sdk/drawing/c_drawing.hpp"

/* resources */
#define IDS_APP_TITLE 103
#define IDR_MAINFRAME 128
#define IDD_GUITEST_DIALOG 102
#define IDD_ABOUTBOX 103
#define IDM_ABOUT 104
#define IDM_EXIT 105
#define IDI_GUITEST 107
#define IDI_SMALL 108
#define IDC_GUITEST 109
#define IDC_MYICON 2
#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif