@echo off

set resource-dev-path=dev\dx12-engine\resource

set proj-path=build\dx12-engine\resource
set bin-x64-debug-path=build\bin\x64-Debug\resource
set bin-x64-release-path=build\bin\x64-Release\resource

mkdir %proj-path%
mkdir %bin-x64-debug-path%
mkdir %bin-x64-release-path%

xcopy %resource-dev-path% %proj-path%  /s /y
xcopy %resource-dev-path% %bin-x64-debug-path% /s /y
xcopy %resource-dev-path% %bin-x64-release-path% /s /y

pause;