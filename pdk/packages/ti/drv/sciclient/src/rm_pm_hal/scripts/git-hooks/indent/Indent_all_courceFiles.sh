#!/bin/sh
#This file MUST be located at ../MAIN_DIR/.git/indent/Indent_all_courceFiles.sh
#Promt
echo "Indenting. Are you sure? (y/n)?"
read ans
ans=$(echo $ans | sed 's/[A-Z]/[a-z]/g')
echo
if [ "$ans" = "y" ]; then
  # Indenting
  chmod +x FileIndentAll
  ./FileIndentAll
else
  echo Aborted!
fi
echo
echo Press Any Key and Enter to Continue...
read ans
