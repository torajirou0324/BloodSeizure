#pragma once

#include <string>
#include <Shlwapi.h>

#pragma comment( lib, "shlwapi.lib")

// ファイルパスを検索します
bool SearchFilePathA(const char* filename, std::string& result);

// ファイルパスを検索します
bool SearchFilePathW(const wchar_t* filename, std::wstring& result);

// ディレクトリパスを削除し、ファイル名を返却します
std::string RemoveDirectoryPathA(const std::string& path);

// ディレクトリパスを削除し、ファイル名を返却します
std::wstring RemoveDirectoryPathW(const std::wstring& path);

// ディレクトリ名を取得します
std::string GetDirectoryPathA(const char* path);

// ディレクトリ名を取得します
std::wstring GetDirectoryPathW(const wchar_t* path);

#if defined(UNICODE) || defined(_UNICODE)
inline bool SearchFilePath(const wchar_t* filename, std::wstring& result)
{
    return SearchFilePathW(filename, result);
}

inline std::wstring RemoveDirectoryPath(const std::wstring& path)
{
    return RemoveDirectoryPathW(path);
}

inline std::wstring GetDirectoryPath(const wchar_t* path)
{
    return GetDirectoryPathW(path);
}
#else
inline bool SearchFilePath(const char* filename, std::string& result)
{
    return SearchFilePathA(filename, result);
}

inline std::string RemoveDirectoryPath(const std::string& path)
{
    return RemoveDirectoryPathA(path);
}

inline std::string GetDirectoryPath(const char* path)
{
    return GetDirectoryPathA(path);
}
#endif//defined(UNICODE) || defined(_UNICODE)