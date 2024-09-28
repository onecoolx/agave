/* files.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"

#include <stdio.h>
#ifdef WINCE
    #include <windows.h>
#endif
#if defined(WIN32) && !defined(WINCE)
    #include <direct.h>
#endif
#if defined(LINUX)
    #include <unistd.h>
#endif
#include <string.h>
#include "files.h"
#include "unicode.h"

File::File()
    : m_file(0)
{
}

File::~File()
{
}

size_t File::ReadData(unsigned char* buffer, size_t buffer_len)
{
    if (!buffer || !buffer_len) {
        return 0;
    }

    return fread(buffer, buffer_len, 1, (FILE*)m_file);
}

bool File::WriteData(unsigned char* data, size_t data_len)
{
    if (!data || !data_len) {
        return false;
    }

    return (fwrite(data, data_len, 1, (FILE*)m_file) < 1) ? false : true;
}

File* File::OpenFile(const char* name, File::FILE_RULE rule, File::FILE_MODE mode)
{
    char param[16] = {0};

    if (rule == File::READ_ONLY) {
        strcat(param, "r");
    } else if (rule == File::READ_WRITE) {
        strcat(param, "w+");
    }

    if (mode == File::BINARY) {
        strcat(param, "b");
    } else if (mode == File::TEXT) {
        strcat(param, "t");
    }

    FILE* f = fopen(name, param);
    if (f) {
        File* file = new File;
        file->m_file = f;
        return file;
    } else {
        return 0;
    }
}

#if defined(WIN32) || defined(WINCE)
File* File::OpenFile(const uchar_t* name, File::FILE_RULE rule, File::FILE_MODE mode)
{
    uchar_t param[16] = {0};

    if (rule == File::READ_ONLY) {
        wcscat(param, L"r");
    } else if (rule == File::READ_WRITE) {
        wcscat(param, L"w+");
    }

    if (mode == File::BINARY) {
        wcscat(param, L"b");
    } else if (mode == File::TEXT) {
        wcscat(param, L"t");
    }

    FILE* f = _wfopen(name, param);
    if (f) {
        File* file = new File;
        file->m_file = f;
        return file;
    } else {
        return 0;
    }
}
#endif

void File::CloseFile(File* file)
{
    if (!file) {
        return;
    }

    fclose ((FILE*)file->m_file);
    delete file;
}

ustring File::GetCurrentDir(void)
{
#if defined(WINCE)
    TCHAR* p = 0;
    TCHAR* d = 0;
    TCHAR* dir = new TCHAR[PATHMAX];
    GetModuleFileName(NULL, dir, PATHMAX);
    p = wcsrchr(dir, '\\');
    *p = 0;
    if (dir[0] == '\\') {
        d = dir + 1;
    } else {
        d = dir;
    }
    ustring cdir(d);
    delete dir;
    return cdir;
#elif defined(WIN32)
    uchar_t* dir = new uchar_t[PATHMAX];
    _wgetcwd(dir, PATHMAX);
    ustring cdir(dir);
    delete dir;
    return cdir;
#elif defined(LINUX)
    char* dir = new char[PATHMAX];
    getcwd(dir, PATHMAX);
    ustring cdir = Unicode::ConvertUTF8ToUTF16(dir);
    delete dir;
    return cdir;
#else
#error need file operation.
#endif
}

ustring CreatePath(ustring& path)
{
    ustring dir = File::GetCurrentDir();
    dir += ustring(U("/"));
    dir += path;
    return dir;
}
