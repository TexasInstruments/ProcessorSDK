#!/usr/bin/env python
#
# Copyright update script
#
# Script to update source files for copyright information as per TI standards.
#
#
# Copyright (c) 2017-2020, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import os
import argparse
import sys as sys
import re as re
import logging
from datetime import date

# Python 3 Hack
try:
    xrange
    copy_xrange = xrange
except NameError:
    copy_xrange = range

copyright_dir = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, copyright_dir)


class CopyrightModifier:
    copyright_inputs = []
    copyright = []
    ti_copyright = False
    skip_copy_rights = [r'CC0']

    def process_line(self, line):
        # Get rid of white spaces before
        line = line.strip()
        # Get rid of starting comment
        # Get rid of ending comment
        line = re.sub(r'^/[*]', '', line)
        line = re.sub(r'^[*]/', '', line)
        # Get rid of comment start
        line = re.sub(r'^[*]', '', line)
        # Get rid of any additional white spaces
        line = line.strip()
        # check if input
        i = re.match(r'^[{]', line)
        if i is not None:
            i = re.sub(r'[{](.+)[}]', r'\1', line)
            self.copyright_inputs.append(i)
            return

        # Check to see if TI copyright line starts
        if self.ti_copyright is False:
            c = re.match(r'.*Texas.*', line)
            if c is None:
                return
            self.ti_copyright = True
        self.copyright.append(line)

    def process_copyright(self, copyright_file=None):
        if copyright_file is None:
            copyright_file = self.license
        # Read the file into a list
        lines = open(copyright_file).read().splitlines()
        for line in lines:
            self.process_line(line)

        # Remove any trailing empty lines from copyright
        while True:
            last_entry = self.copyright.pop()
            if last_entry is not '':
                self.copyright.append(last_entry)
                break

    def set_file_comment_delim(self, fname):
        self.fdelim_start = None
        self.fdelim = None
        self.fdelim_end = None
        bname = os.path.basename(fname)

        if bname.endswith('.c') or bname.endswith('.h'):
            self.fdelim_start = '/*'
            self.fdelim = ' * '
            self.fdelim_end = ' */'
            return

        if bname.endswith('py') or bname.endswith('sh') or \
                bname.endswith('pl') or \
                bname.startswith('makefile') or bname.startswith('Makefile') or \
                bname.startswith('Kconfig') or bname.endswith('Kconfig'):

            self.fdelim_start = '#'
            self.fdelim = '# '
            self.fdelim_end = '#'
            return

        # for files with .S it is a mess.. we are'nt sure if /* is used or ;
        if bname.endswith('asm'):
            self.fdelim_start = ';'
            self.fdelim = '; '
            self.fdelim_end = ';'
            return

    def process_copyrighted_file(self, fname, lines):
        r = re.compile(r'.*Texas.*')
        c = list(filter(r.search, lines))
        if not c:
            print(fname + ": Skipping NON TI Copyrighted file")
            return None
        # Find start of copyright information
        ln = 1
        detected = False
        regex_delim = r".*" + re.escape(self.fdelim.strip()) + '.*'
        regex_end = r".*" + re.escape(self.fdelim_end.strip()) + '.*'
        copyright_year = None
        copy_start_ln = None
        copy_end_ln = None
        for line in lines:
            c = re.match(r'.*Texas.*', line)
            if c is not None and detected is False:
                copyright_year = re.sub(
                    r'.*\(C\) ([0-9]+).*', r'\1', line, re.IGNORECASE)
                copy_start_ln = ln - 1
                detected = True
            else:
                if detected is True:
                    d_check = re.match(regex_end, line)
                    d1_check = re.match(regex_delim, line)
                    if d_check is None and d1_check is None:
                        copy_end_ln = ln - 1
                        break
            ln = ln + 1
        if copy_start_ln is None or copy_end_ln is None or copyright_year is None:
            print(
                fname +
                ': Skipping corrupted TI license header.. please fix manually')
            return None
        old_copyright = lines[copy_start_ln: copy_end_ln]
        del lines[copy_start_ln: copy_end_ln]
        thisyear = date.today().year
        copy_header = []
        for c in self.copyright:
            if str(thisyear).strip() == str(copyright_year).strip():
                c = re.sub(r'{(YEAR)}', str(thisyear), c)
            else:
                c = re.sub(
                    r'{(YEAR)}',
                    str(copyright_year) +
                    '-' +
                    str(thisyear),
                    c)
            copy_header.append((self.fdelim + str(c)).rstrip())
        copy_header.append(self.fdelim_end.rstrip())

        if old_copyright == copy_header:
            return None
        lines[copy_start_ln:copy_start_ln] = copy_header
        print(fname + ': Updated TI copyright')
        return lines

    def process_noncopyrighted_file(self, fname, lines):
        my_inputs = []

        if self.cmd_args.dummy_placeholder is not True:
            print(
                fname +
                ": File is Not Copyrighted, Please provide the following info:")
            for i in self.copyright_inputs:
                ip = raw_input(str(i) + ": ")
                my_inputs.append(ip)
        else:
            print(fname + ": File is Not Copyrighted, Using default placeholders")
            for i in self.copyright_inputs:
                my_inputs.append('{ TBD/FIXME: ' + i + '}')

        # Insert copyright
        copy_header = []
        copy_header.append(self.fdelim_start.rstrip())
        for i in my_inputs:
            copy_header.append((self.fdelim + str(i)).rstrip())
            copy_header.append(self.fdelim.rstrip())
        year = date.today().year
        for c in self.copyright:
            c = re.sub(r'{(YEAR)}', str(year), c)
            copy_header.append((self.fdelim + str(c)).rstrip())
        copy_header.append(self.fdelim_end.rstrip())
        copy_header.append('')
        my_lines = copy_header + lines
        return my_lines

    def process_file(self, mod_file):
        real_fname = os.path.join(str(self.cmd_args.sourcedir), mod_file)
        self.set_file_comment_delim(real_fname)
        if self.fdelim_start is None:
            print("Skipping unknown file:" + str(real_fname))
            return

        lines = open(real_fname).read().splitlines()
        # Check if we have any licenses to skip
        for idx in copy_xrange(len(self.skip_copy_rights)):
            copy_right = self.skip_copy_rights[idx]
            r = re.compile(copy_right, re.M)
            c = list(filter(r.search, lines))
            logging.debug(c)
            if c:
                print(
                    "Skipping %s Copy-righted file: %s " %
                    (copy_right, real_fname))
                return
        r = re.compile(r'.*Copyright.*', re.IGNORECASE)
        c = list(filter(r.match, lines))
        if not c:
            new_lines = self.process_noncopyrighted_file(real_fname, lines)
        else:
            new_lines = self.process_copyrighted_file(real_fname, lines)
        if not new_lines:
            return
        else:
            last_line = new_lines.pop()
            last_line = last_line + '\n'
            new_lines.append(last_line)
            f = open(real_fname, "w")
            f.write("\n".join(new_lines))
            f.close()

    def parse_myargs(self, args=None):
        parser = argparse.ArgumentParser(prog=__file__,
                                         prefix_chars="-",
                                         formatter_class=argparse.ArgumentDefaultsHelpFormatter,
                                         description='Script that updates Copyright of files to TI Copyright')
        parser._action_groups.pop()
        required = parser.add_argument_group('required arguments')
        optional = parser.add_argument_group('optional arguments')
        multi_choice_grp = parser.add_argument_group(
            'Required: (mutually exclusive) ways to select files to check/modify')
        filelist_group = multi_choice_grp.add_mutually_exclusive_group(
            required=True)

        required.add_argument('-l', '--license',
                              required=True,
                              help="TI Copyright license template file from: https://sps05.itg.ti.com/sites/TSPARepository/Open Source Wiki/OSLibraryHeaders.aspx",
                              action="store")

        filelist_group.add_argument('-f', '--filelist',
                                    help="Consolidated file containing files for which Copyright text needs to be checked",
                                    action="store")
        filelist_group.add_argument('-F', '--Files', nargs='*',
                                    help="Explicit list of files to check to check for copyright")

        optional.add_argument('-d', '--directory_license',
                              help="Location of License directory path", action="store", default=copyright_dir)
        optional.add_argument('-s', '--sourcedir',
                              help="Source code directory base", action="store", default=copyright_dir)
        optional.add_argument('-D', '--dummy_placeholder',
                              help="Use a dummy placeholder to fill up later",
                              action="store_true", default=False)

        self.cmd_args = parser.parse_args(args)

        self.license = os.path.realpath(
            os.path.join(
                self.cmd_args.directory_license,
                self.cmd_args.license))
        try:
            self.process_copyright()
        except BaseException:
            print("Error: Copyright parse failed - file '" +
                  str(self.license) + "' exists?")
            print("\nHelp:")
            parser.print_help()
            exit(1)

        try:
            if self.cmd_args.filelist is not None:
                with open(self.cmd_args.filelist) as f:
                    baselist = f.read().splitlines()
            else:
                baselist = []
        except BaseException:
            print("\nError: File list - file '" +
                  str(self.cmd_args.filelist) + "' exists?")
            print("\nHelp:")
            parser.print_help()
            exit(1)

        if self.cmd_args.Files is not None:
            self.checkfiles = baselist + self.cmd_args.Files
        else:
            self.checkfiles = baselist
        if not self.checkfiles:
            parser.print_help()
            exit(1)

    def process_files(self):
        for f in self.checkfiles:
            self.process_file(f)

    def __init__(self, args=None):
        self.parse_myargs(args)
        self.process_files()


if __name__ == "__main__":
    # Skip if environment is setup to skip
    try:
        SKIP = os.environ['TI_COPYRIGHT_SKIP']
    except KeyError:
        SKIP = None
    if SKIP is not None:
        print("TI_COPYRIGHT_SKIP set, Skipping..")
        exit(0)

    c = CopyrightModifier()

# Format using :!autopep8 -i -a %
# vim: et:ts=4
