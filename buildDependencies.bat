@echo off
:: This file is designed to run from the Build folder
:: it will try clone dependencies online if the folder doesnt exist (It assume it is there if there is a folder with the same name)

SET CURR_DIR=%cd%

SET RED=[91m
SET GREEN=[92m
SET YELLOW=[93m
SET BLUE=[94m
::No Color
SET NC=[0m

if NOT exist "dependencies" mkdir dependencies

::Git----------------------------------------------------------------------------------------------------------
if exist "C:/Program Files/Git" goto GIT_DONE
cd dependencies
echo %YELLOW%=================================================================
echo Git not found in default path:(C:/Program Files/Git)
echo Downloading Git 64-bit... (Cancel download if already owned)
echo Installation options can be left as Default
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/git-for-windows/git/releases/download/v2.36.1.windows.1/Git-2.36.1-64-bit.exe', 'Git-2.36.1-64-bit.exe')"
Git-2.36.1-64-bit.exe

::go back the starting path
cd %CURR_DIR%
if NOT exist "C:/Program Files/CMake" goto CMAKE_DOWNLOAD
echo %BLUE%Rerun the bat file after download.%NC%
goto EXIT

:GIT_DONE
::-------------------------------------------------------------------------------------------------------------


::CMake--------------------------------------------------------------------------------------------------------
if exist "C:/Program Files/CMake" goto CMAKE_DONE

:CMAKE_DOWNLOAD
cd dependencies

echo %YELLOW%=================================================================
echo CMake not found in default path:(C:/Program Files/CMake)
echo Downloading CMake 64-bit... (Cancel download if already owned)
echo First option should be: %GREEN%Add CMake to the System PATH for all users %YELLOW%OR %GREEN%Add CMake to the system PATH for the current user
echo %YELLOW%Other installation option can be left as Default

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-windows-x86_64.msi', 'cmake-3.23.1-windows-x86_64.msi')"
cmake-3.23.1-windows-x86_64.msi
echo %BLUE%Rerun the bat file after download. %NC%

::go back the starting path
cd %CURR_DIR%
goto EXIT

:CMAKE_DONE
::-------------------------------------------------------------------------------------------------------------




::Sphinx----------------------------------------------------------------------------------------------------------
if exist dependencies/Sphinx goto SPHINX_DONE
echo %YELLOW%=================================================================
echo Sphinx not found. 
echo Downloading Sphinx...%NC%
rmdir "dependencies/Sphinx" /S /Q
git clone https://github.com/shortiefel/Sphinxdependencies.git "dependencies/Sphinx"
if %ERRORLEVEL% GEQ 1 goto :ERROR
echo %GREEN%Sphinx downloaded

:SPHINX_DONE


:END
echo %GREEN%--------------------
echo %GREEN%Everything is set up
echo %GREEN%--------------------
echo %GREEN%Restart Visual Studios if it does not work%NC%
echo %RED%If you want to redownload any of the dependencies, please delete that folder before rerunning the bat file%NC%
goto EXIT

:ERROR
echo %RED%ERROR!!%NC%
pause

:EXIT
pause