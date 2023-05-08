@echo off

echo steam clear! ((matt1.tk))((2772)) press any key to clear steam accounts
pause

@echo off
echo Clearing steam...
@echo off

taskkill /f /im hl1.exe
taskkill /f /im hl2.exe
taskkill /f /im Steam.exe
REG DELETE HKEY_CURRENT_USER\Software\Valve\Steam\Users /f
REG DELETE HKEY_CURRENT_USER\Software\Valve\Steam\Users /f
REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Valve\User s /f
REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Users /f
REG DELETE HKEY_CURRENT_USER\Software\Valve\Steam /f
REG DELETE HKEY_CURRENT_USER\Software\Valve /f
REG DELETE HKEY_CURRENT_USER\Software\Wow6432Node\Valve\Steam /f
REG DELETE HKEY_CURRENT_USER\Software\Wow6432Node\Valve /f
REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Valve\User s /f
REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Valve /f
REG DELETE HKEY_LOCAL_MACHINE\Software\Valve\Steam\Users /f
REG DELETE HKEY_LOCAL_MACHINE\Software\Valve /f
REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Classes\steam /f
REG DELETE HKEY_CURRENT_USER\Software\Classes\steam /f
REG DELETE HKEY_CLASSES_ROOT\steam /f
cd\

cd C:\Program Files (x86)\Steam
del /s /q d:\C:\Program Files (x86)\Steam\appcache\Steam.log
del /S /Q "C:\Program Files (x86)\Steam\config"
del /S /q d:\C:\Steam\appcache\AppUpdateStats.blob
del /S /q d:\C:\Steam\appcache\GameOverlayRenderer.log
del /S /q d:\C:\Steam\appcache\GameOverlayUI.exe.log
del /S /q d:\C:\Steam\appcache\crashhandler.dll
del /S /q d:\C:\Steam\appcache\crashhandler64.dll
del /S /q d:\C:\Steam\appcache\CSERHelper.dll
del /S /q d:\C:\Steam\appcache\Steam.dll
del /S /q d:\C:\Steam\appcache\steamclient.dll
del /S /q d:\C:\Steam\appcache\steamclient64.dll
del /S /q d:\C:\Steam\appcache\SteamUI.dll
del /S /q d:\C:\Steam\appcache\streaming_client.exe
del /S /q d:\C:\Steam\appcache\WriteMiniDump.exe
del /S /q d:\C:\Steam\appcache\debug.log
del /Q F logs
del /Q F *.mdmp
del ClientRegistry.blob
rmdir /S /Q "C:\Program Files (x86)\Steam\appcache"
rmdir /S /Q "C:\Program Files (x86)\Steam\config"
rmdir /S /Q "C:\Program Files (x86)\Steam\userdata"
rmdir /S /Q "C:\Program Files (x86)\Steam\dumps"
rmdir /S /Q "C:\Program Files (x86)\Steam\logs"
rmdir /S /Q "C:\Program Files (x86)\Steam\appcache"