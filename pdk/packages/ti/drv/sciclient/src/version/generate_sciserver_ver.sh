#!/bin/bash

##
# SCISERVER
#
# Copyright (C) 2021-2025 Texas Instruments Incorporated - http://www.ti.com/
#
# This software is licensed under the  standard terms and conditions in the
# Texas Instruments  Incorporated Technology and Software Publicly Available
# Software License Agreement, a copy of which is included in the software
# download.
##

year=$(date +%Y)
month=$(date +%m)
dm_ver=""

if [ ! -z "$WIN_GITPATH" ]; then
	git_cmd="$WIN_GITPATH/git.exe"

	if [ ! -x "$git_cmd" ]; then
		echo "WARNING: Git is not at indicated location $git_cmd"
	fi
else
	git_cmd="git"
fi

# Include an SCMVERSION if applicable. Make it short. If we are on a clean
# tree that matches the current tag, the string is blank. If the closest
# tag matches what we expect based on v$VERSION.$SUBVERSION.$PATCHVERSION,
# then don't bother including it. Abbreviate dirty as +.
if [ "$($git_cmd config --get remote.origin.url | sed -E 's#.*/##')" = "pdk" ]; then
	git_ver=$("$git_cmd" describe --abbrev=5 --dirty | sed -e 's/-dirty$/+/g')
	if [ -n "$git_ver" ]; then
		git_ver="-${git_ver}"
	fi

	# Included dm_ver required for DM version api call. implementing for ease of debug
	# eg: PSDK.xx.xx.xx.xx-NN+ PSDK explains DM build on which repo, xx.xx.xx.xx explains
	# the latest tag, NN represents delta between tag and commit, Abbreviate dirty as +.
	dm_ver="$("$git_cmd" describe --match "REL.PSDK.*" --abbrev=8 --dirty | sed -e 's/-g........//g' -e 's/-dirty$/+/g' | cut -c 5-)"


	major_ver=$(echo $dm_ver | cut -d'.' -f2 | sed -E -e 's/[^0-9.]//g' -e 's/^0*//g')
	sub_ver=$(echo $dm_ver | cut -d'.' -f3 | sed -E -e 's/[^0-9.]//g' -e 's/^0*//g')
	patch_ver=$(echo $dm_ver | cut -d'.' -f4 | sed -E -e 's/[^0-9.]//g' -e 's/^0*//g')

	if [ -z "$major_ver" ]
	then
		major_ver=0
	fi
	if [ -z "$sub_ver" ]
	then
		sub_ver=0
	fi
	if [ -z "$patch_ver" ]
	then
		patch_ver=0
	fi

cat << EOF
/**
 * SCISERVER Version Info
 *
 * Copyright (C) 2024-$year Texas Instruments Incorporated - http://www.ti.com/
 *
 * This software is licensed under the  standard terms and conditions in the
 * Texas Instruments  Incorporated Technology and Software Publicly Available
 * Software License Agreement, a copy of which is included in the software
 * download.
 *
 * NOTICE: This file is auto-generated based on version numbers passed in
 * Makefile. Never edit this file by hand.
 */

#ifndef INCLUDE_SCISERVER_VERSION_H
#define INCLUDE_SCISERVER_VERSION_H

#define SCISERVER_MAJOR_VERSION_NAME	$major_ver
#define SCISERVER_SUBVERSION	$sub_ver
#define SCISERVER_PATCHVERSION	$patch_ver
#define SCISERVER_SCMVERSION	"$git_ver"
#define SCISERVER_DMVERSION     "$dm_ver"

#endif /* INCLUDE_SCISERVER_VERSION_H */

EOF
fi
