#pragma once

#include <string>
#include <Shlwapi.h>

#pragma comment( lib, "shlwapi.lib")

// �t�@�C���p�X���������܂�
bool SearchFilePathA(const char* filename, std::string& result);

// �t�@�C���p�X���������܂�
bool SearchFilePathW(const wchar_t* filename, std::wstring& result);

// �f�B���N�g���p�X���폜���A�t�@�C������ԋp���܂�
std::string RemoveDirectoryPathA(const std::string& path);

// �f�B���N�g���p�X���폜���A�t�@�C������ԋp���܂�
std::wstring RemoveDirectoryPathW(const std::wstring& path);

// �f�B���N�g�������擾���܂�
std::string GetDirectoryPathA(const char* path);

// �f�B���N�g�������擾���܂�
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