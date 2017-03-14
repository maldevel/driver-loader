/*
    ServiceInstaller - Creates a service and starts it to load a driver into Windows kernel.
    Copyright (C) 2015 @maldevel

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
*/

#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#define ERROR_ARGS 1L
#define ERROR_NODRVFILE 2L

bool Install(LPCSTR driver, LPCSTR service);
bool FileExists(LPCSTR szPath);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("\nusage: DrvSrvInstaller.exe <driver_path> <service_name>\n\n"
			"for ex. DrvSrvInstaller.exe c:\\mydriver.sys HelloWorld\n\n");

		return ERROR_ARGS;
	}

	if (!FileExists(argv[1]))
	{
		printf("\n%s doesn't exist..\n\n", argv[1]);

		return ERROR_NODRVFILE;
	}

	Install(argv[1], argv[2]);

	return ERROR_SUCCESS;
}

bool FileExists(LPCSTR szPath)
{
	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Install(LPCSTR driver, LPCSTR service)
{
	SC_HANDLE scManager;
	SC_HANDLE srvHandle;

	scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scManager) {
		printf("OpenSCManager failed..\n");
		return FALSE;
	}

	//create service
	srvHandle = CreateServiceA(scManager, service, service,
		SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		driver, NULL, NULL, NULL, NULL, NULL);

	if (!srvHandle) {

		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			printf("Service already exists..\n");

			//open existing service
			srvHandle = OpenServiceA(scManager, service, SERVICE_ALL_ACCESS);
			if (!srvHandle) {
				printf("OpenService failed..\n");
				CloseServiceHandle(scManager);
				return FALSE;
			}
		}
		else {
			CloseServiceHandle(scManager);
			printf("CreateService failed..\n");
			return FALSE;
		}

		CloseServiceHandle(scManager);

		return FALSE;
	}

	printf("Service created!\n");

	//start service
	if (srvHandle) {
		if (StartService(srvHandle, 0, NULL) == 0) {
			if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) {
				printf("Start Service failed..\n");
				CloseServiceHandle(srvHandle);
				CloseServiceHandle(scManager);
				return FALSE;
			}
		}

		printf("Service started!\n");
		CloseServiceHandle(srvHandle);
	}

	CloseServiceHandle(scManager);

	return TRUE;
}
