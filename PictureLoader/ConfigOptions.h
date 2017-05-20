#pragma once
#include "afxwin.h"

const CString SECTION_DISPLAY = _T("Display");
const CString KEY_MAX_LOAD_CNT = _T("Max load count");
const UINT DEFAULT_MAX_LOAD_CNT = 4;
const CString SECTION_MISC = _T("Misc");
const CString KEY_DOWNLOAD_PATH = _T("DownLoad path");
const CString DEFAULT_DOWNLOAD_PATH = _T("download\\");
const CString CONFIG_FILE_PATH = _T("config.ini");

typedef struct st_config {
	LPCSTR sdata;
	int idata;
}stConfig;