#pragma once
#include "../../hooking/c_hooks.hpp"

/* i::device_x */
#define create_device device_x->CreateDevice

/* i::device */
#define create_block device->CreateStateBlock

/* windows_sdk */
#define clean_memory ZeroMemory
#define create_window_ex CreateWindowEx