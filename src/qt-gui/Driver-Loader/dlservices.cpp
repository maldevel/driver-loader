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

#include "dlservices.h"

static SC_HANDLE scManager;

bool Services::init(void)
{
    scManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
    if (scManager == NULL)
    {
        return false;
    }

    return true;
}

void Services::uninit(void)
{
    if (scManager == NULL) return;

    CloseServiceHandle(scManager);
}

unsigned long Services::Register(QString driver, QString serviceName, QString displayName,
                                 QString startTypeStr, QString error)
{
    if (driver == NULL || serviceName == NULL || scManager == NULL || displayName == NULL) return false;

    SC_HANDLE scService;
    unsigned long startType = SERVICE_DEMAND_START;
    unsigned long errorControl = SERVICE_ERROR_NORMAL;

    //"Automatic" "Boot" "Demand" "Disabled" "System"
    if (startTypeStr.compare("Automatic", Qt::CaseSensitive) == 0)
    {
        startType = SERVICE_AUTO_START;
    }
    else if (startTypeStr.compare("Boot", Qt::CaseSensitive) == 0)
    {
        startType = SERVICE_BOOT_START;
    }
    else if (startTypeStr.compare("Demand", Qt::CaseSensitive) == 0)
    {
        startType = SERVICE_DEMAND_START;
    }
    else if (startTypeStr.compare("Disabled", Qt::CaseSensitive) == 0)
    {
        startType = SERVICE_DISABLED;
    }
    else if (startTypeStr.compare("System", Qt::CaseSensitive) == 0)
    {
        startType = SERVICE_SYSTEM_START;
    }

    //"Critical" "Ignore" "Normal" "Severe"
    if (error.compare("Critical", Qt::CaseSensitive) == 0)
    {
        errorControl = SERVICE_ERROR_CRITICAL;
    }
    else if (error.compare("Ignore", Qt::CaseSensitive) == 0)
    {
        errorControl = SERVICE_ERROR_IGNORE;
    }
    else if (error.compare("Normal", Qt::CaseSensitive) == 0)
    {
        errorControl = SERVICE_ERROR_NORMAL;
    }
    else if (error.compare("Severe", Qt::CaseSensitive) == 0)
    {
        errorControl = SERVICE_ERROR_SEVERE;
    }

    if ((scService = CreateServiceA(scManager, serviceName.toStdString().c_str(), displayName.toStdString().c_str(),
                                    SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
                                    startType, errorControl,
                                    driver.toStdString().c_str(),
                                    NULL, NULL, NULL, NULL, NULL)) == NULL)
    {
        if (GetLastError() == ERROR_SERVICE_EXISTS)
        {
            return ERROR_SERVICE_EXISTS;
        }

        return 1;
    }

    CloseServiceHandle(scService);

    return 0;
}

bool Services::Unregister(const char *service)
{
    if (service == NULL || scManager == NULL) return false;

    return false;
}

bool Start(const char *service)
{
    if (service == NULL || scManager == NULL) return false;

    return false;
}

bool Stop(const char *service)
{
    if (service == NULL || scManager == NULL) return false;

    return false;
}
