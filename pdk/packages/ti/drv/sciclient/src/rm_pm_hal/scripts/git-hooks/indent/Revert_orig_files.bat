@echo OFF

echo.

::Promt
SET /P ANSWER=Rever *.orig files. Are you sure? (y/n)?
if /i {%ANSWER%}=={y} (goto :yes)
if /i {%ANSWER%}=={yes} (goto :yes)
goto :no
:yes
    :: Indenting files
    cmd.exe /c "sh.exe RevertOrigFiles"
    goto :exit
    
:no
    echo Aborted!
    goto :exit
    
:exit
echo.
pause