/*
    ServiceUninstaller - Stops a service and removes it to unload a driver from Windows kernel.
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

bool Uninstall(LPCSTR service);

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("\nusage: DrvSrvUninstaller.exe <service_name>\n\n"
			"for ex. DrvSrvUninstaller.exe HelloWorld\n\n");

		return ERROR_ARGS;
	}

	Uninstall(argv[1]);

	return ERROR_SUCCESS;
}

bool Uninstall(LPCSTR service)
{
	SC_HANDLE scManager;
	SC_HANDLE srvHandle;
	SERVICE_STATUS srvStatus;

	scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scManager) {
		printf("OpenSCManager failed..\n");
		return FALSE;
	}

	//open service
	srvHandle = OpenServiceA(scManager, service, SERVICE_ALL_ACCESS);

	if (!srvHandle) {
		CloseServiceHandle(scManager);
		printf("OpenService failed..\n");
		return FALSE;
	}

	//stop service
	if (srvHandle) {
		if (ControlService(srvHandle, SERVICE_CONTROL_STOP, &srvStatus) == 0) {
			if (GetLastError() != ERROR_SERVICE_NOT_ACTIVE) {
				printf("Stop service failed..\n");
				CloseServiceHandle(srvHandle);
				CloseServiceHandle(scManager);
				return FALSE;
			}
		}

		printf("Service stopped!\n");

		//delete service
		if (!DeleteService(srvHandle)) {
			printf("Delete service failed..\n");
			CloseServiceHandle(srvHandle);
			CloseServiceHandle(scManager);
			return FALSE;
		}

		printf("Service removed!\n");
		CloseServiceHandle(srvHandle);
	}

	CloseServiceHandle(scManager);

	return TRUE;
}
