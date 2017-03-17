/*
    This file is part of driver-loader
    Copyright (C) 2017 @maldevel

    driver-loader - Load a Windows Kernel Driver.
    https://github.com/maldevel/driver-loader

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    For more see the file 'LICENSE' for copying permission.
*/

#include "dldrivers.h"
#include "dlcommon.h"
#include <Windows.h>
#include <Shlwapi.h>

//HANDLE h;

//if ((h = CreateFileA(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE)
//{
//    return false;
//}


static BOOL _fileExists(const char *filename)
{
    if (filename == NULL)return FALSE;

    return PathFileExistsA(filename);
}


QString Drivers::GetFileVersion(QString fName)
{
    if (fName == NULL || !_fileExists(fName.toStdString().c_str())) return "";

    DWORD handle;
    DWORD size = 0;
    void *buffer = 0;
    VS_FIXEDFILEINFO *lpBuffer = 0;
    unsigned int len = 0;
    QString fVersion = 0;

    if ((size = GetFileVersionInfoSizeA(fName.toStdString().c_str(), &handle)) == FALSE)
    {
        return "";
    }

    if ((buffer = Common::hAlloc(size)) == NULL)
    {
        return "";
    }

    if (GetFileVersionInfoA(fName.toStdString().c_str(), handle, size, buffer) == FALSE)
    {
        Common::hFree(buffer);
        return "";
    }

    if (VerQueryValue(buffer, QString("\\").toStdWString().c_str(), (void **)&lpBuffer, &len) == FALSE)
    {
        Common::hFree(buffer);
        return "";
    }

    fVersion = QString("%1.%2.%3.%4")
               .arg(HIWORD(lpBuffer->dwFileVersionMS))
               .arg(LOWORD(lpBuffer->dwFileVersionMS))
               .arg(HIWORD(lpBuffer->dwFileVersionLS))
               .arg(LOWORD(lpBuffer->dwFileVersionLS));

    Common::hFree(buffer);

    return fVersion;
}
