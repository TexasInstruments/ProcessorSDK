@echo OFF
:: This file MUST be located at ../MAIN_DIR/.git/indent/Indent_all_courceFiles.bat

::Promt
SET /P ANSWER=Indenting. Are you sure? (y/n)?
if /i {%ANSWER%}=={y} (goto :yes)
if /i {%ANSWER%}=={yes} (goto :yes)
goto :no
:yes
    :: Indenting files
    cmd.exe /c "sh.exe FileIndentAll"
    goto :exit
    
:no
    echo Aborted!
    goto :exit
    
:exit
echo.
pause