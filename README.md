# driver-loader
Load a Windows Kernel Driver

## Set TESTSIGNING boot configuration option
```bash
bcdedit.exe -set loadoptions DISABLE_INTEGRITY_CHECKS
bcdedit.exe -set TESTSIGNING ON
shutdown -r -f -t 00
```
