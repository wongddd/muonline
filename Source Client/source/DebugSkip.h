#pragma once

// Auto-test / Debug mode utilities for skipping blocking errors
// Used to bypass missing-resource crashes during headless testing

#include "./Utilities/Log/ErrorReport.h"

extern bool g_bAutoTest;
extern HWND g_hWnd;

// In auto-test mode, logs error and returns IDYES (continue).
// In normal mode, shows actual MessageBox and returns user choice.
inline int AutoTestMsgBox(HWND hWnd, const char* text, const char* caption = NULL, UINT type = MB_OK)
{
	if (g_bAutoTest)
	{
		g_ErrorReport.Write("[AutoTest] Suppressed MessageBox: %s\r\n", text);
		return IDYES;
	}
	return MessageBox(hWnd, text, caption, type);
}

// Convenience wrapper for the common pattern: MessageBox + SendMessage(WM_DESTROY)
// In auto-test mode, just logs and returns; in normal mode, calls both.
inline void FatalError(const char* text, const char* caption = NULL)
{
	if (g_bAutoTest)
	{
		g_ErrorReport.Write("[AutoTest] Suppressed fatal error: %s\r\n", text);
		return;
	}
	MessageBox(g_hWnd, text, caption, MB_OK);
	SendMessage(g_hWnd, WM_DESTROY, 0, 0);
}
