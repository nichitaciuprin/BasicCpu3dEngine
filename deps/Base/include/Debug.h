#pragma once

void ShowMessageBox(const string& text)
{
    MessageBoxA(nullptr, text.c_str(), nullptr, MB_OK);
}
void ShowMessageBox(const wstring& text)
{
    MessageBoxW(nullptr, text.c_str(), nullptr, MB_OK);
}
void ShowMessageBox(const LPCSTR lpText)
{
    MessageBoxA(nullptr, lpText, nullptr, MB_OK);
}
void ShowMessageBox(const LPCWSTR text)
{
    MessageBoxW(nullptr, text, nullptr, MB_OK);
}