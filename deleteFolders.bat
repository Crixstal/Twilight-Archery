del *.sln
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved

rem "C:\Programmes (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" /projectfiles "Gladiator\Gladiator.uproject"
rem set MyUVS="C:\Programmes (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
rem set MyUBT="C:\Programmes\Epic Games\UE_4.26\Engine\Binaries\DotNET\UnrealBuildTool.exe"

rem set MyFullPath="C:\Users\sasha\School\GP2\Unreal\Brawler\Gladiator\Gladiator.uproject"

rem %MyUVS% /projectfiles %MyFullPath%
rem %MyUBT% Development Win64 -Project=%MyFullPath% -TargetType=Editor -Progress -NoEngineChanges -NoHotReloadFromIDE

rem %MyFullPath%.sln

rem pause