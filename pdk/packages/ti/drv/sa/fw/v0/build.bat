@ECHO OFF
rem  build the pdsp code
rem  test code for keystone2
pasm_dos -EBldXcm -D_BE_ -CSa_php1 php1.p
pasm_dos -EBldXcm -D_BE_ -CSa_php2 php2.p

rem Change the output from a .h to a .c, and create a new .h file
cat ../saphp1_desc.c > saphp1_bin.c
cat php1_bin.h | sed -e s/"unsigned int"/"uint32_t"/g >> saphp1_bin.c
echo const int Sa_php1Size = sizeof(Sa_php1); >> saphp1_bin.c

cat ../saphp2_desc.c > saphp2_bin.c
cat php2_bin.h | sed -e s/"unsigned int"/"uint32_t"/g >> saphp2_bin.c
echo const int Sa_php2Size = sizeof(Sa_php2); >> saphp2_bin.c

rem del the H files
del php1_bin.h
del php2_bin.h
