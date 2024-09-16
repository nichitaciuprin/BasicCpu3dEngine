// TODO

#pragma once

wstring GetDirectory(const wstring& path)
{
    const size_t last_slash_idx = path.rfind('\\');
    if (wstring::npos == last_slash_idx)
        return path;
    return path.substr(0, last_slash_idx);
}
wstring GetExeFile()
{
    wchar_t buffer[MAX_PATH] = {};
    auto result = GetModuleFileNameW(NULL, (LPWSTR)buffer, MAX_PATH);
    assert(result > 0);
    return wstring(buffer,result);
}
wstring GetExeDirectory()
{
    auto filePath = GetExeFile();
    auto directoryPath = GetDirectory(filePath);
    return directoryPath;
}
bool FileExists(const wstring& file)
{
    auto file_c = (LPCWSTR)file.c_str();
    auto result = GetFileAttributesW(file_c);
    return result != INVALID_FILE_ATTRIBUTES;
}