// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <stdlib.h>
#include <winsock.h>
#include <time.h>
#include <string.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
extern "C"
{
#ifdef _WIN64
#include "curl-7.53.1-win64-mingw/include/curl/curl.h"
#include "sqlite-3.1.8-win64/sqlite3.h"
#pragma comment(lib, "sqlite-3.1.8-win64/sqlite3.lib")
#pragma comment(lib, "curl-7.53.1-win64-mingw/bin/libcurl.lib")
#else
#include "curl-7.53.1-win32-mingw/include/curl/curl.h"
#include "sqlite-3.1.8-win32/sqlite3.h"
#pragma comment(lib, "sqlite-3.1.8-win32/sqlite3.lib")
#pragma comment(lib, "curl-7.53.1-win32-mingw/bin/libcurl.lib")
#endif
}