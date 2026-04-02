@echo OFF

:: Go 2 directory UP (from ../MAIN_DIR/.git/indent to ../MAIN_DIR/)
cd ../../

:: Display files to be deleted
echo Deleting files:
DIR /S /B "*.orig"

echo.

::Promt
SET /P ANSWER=Are you sure? (y/n)?
if /i {%ANSWER%}=={y} (goto :yes)
if /i {%ANSWER%}=={yes} (goto :yes)
goto :no
:yes
    :: Delete files
    DEL /S /Q "*.orig"
    echo Deleted!
    goto :exit

:no
    echo Aborted!
    goto :exit

:exit
echo.
pause