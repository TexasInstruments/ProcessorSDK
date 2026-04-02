# rem  build the pdsp code
pasm_linux -EBldXcm -D_BE_ -CSa2_php1 php1.p
pasm_linux -EBldXcm -D_BE_ -CSa2_php2 php2.p
pasm_linux -EBldXcm -D_BE_ -CSa2_php3 php3.p

# rem Change the output from a .h to a .c, and create a new .h file
cat ../saphp1_desc.c > saphp1_bin.c
cat php1_bin.h | sed -e s/"unsigned int"/"uint32_t"/g >> saphp1_bin.c
echo "const int Sa2_php1Size = sizeof(Sa2_php1);" >> saphp1_bin.c

cat ../saphp2_desc.c > saphp2_bin.c
cat php2_bin.h | sed -e s/"unsigned int"/"uint32_t"/g >> saphp2_bin.c
echo "const int Sa2_php2Size = sizeof(Sa2_php2);" >> saphp2_bin.c

cat ../saphp3_desc.c > saphp3_bin.c
cat php3_bin.h | sed -e s/"unsigned int"/"uint32_t"/g >> saphp3_bin.c
echo "const int Sa2_php3Size = sizeof(Sa2_php3);" >> saphp3_bin.c


# rem del the H files
rm php1_bin.h
rm php2_bin.h
rm php3_bin.h
