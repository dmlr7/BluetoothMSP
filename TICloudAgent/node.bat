@echo off
set PATH=%~dp0\util;%PATH%
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS_BIT_SIZE=32||set OS_BIT_SIZE=64
/home/dmlr/TICloudAgent/node /home/dmlr/TICloudAgent/src/installer/cli.js --installer ticloudagent.run  --quiet true
