#!/bin/sh
# Display files to be deleted

# Go 2 directory UP (from ../MAIN_DIR/.git/indent to ../MAIN_DIR/)
cd ../../

echo
echo Deleting files:
find . -name "*.orig"


echo
# Promt
echo "Are you sure? (y/n)"
read ans
ans=$(echo $ans | sed 's/[A-Z]/[a-z]/g')
echo
if [ "$ans" = "y" ]; then
  # Delete .orig files after promt
  find . -name "*.orig" -print0 | xargs -0 rm
  echo Deleted!
else
  echo Aborted!
fi
echo
echo Press Any Key and Enter to Continue...
read ans
