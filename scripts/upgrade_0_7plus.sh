#!/bin/sh
###########################################################################
# Copyright (C) 2009 Swedish Meteorological and Hydrological Institute, SMHI,
#
# This file is part of HLHDF.
#
# HLHDF is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# HLHDF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with HLHDF.  If not, see <http://www.gnu.org/licenses/>.
###########################################################################

SCRIPTPATH=`dirname $(readlink -f $0)`
PERLSCRIPT="$SCRIPTPATH/rename_functions.pl"

echo "================================================================"
echo "ATTENTION:"
echo "  This tool will atempt to rename function names in the .c and .h"
echo "  files that exists under this directory and downwards."
echo ""
echo "  By answering yes to the question below, you agree that the developer(s)"
echo "  and/or any other party that has been involved/responsible for the"
echo "  development of this script will not be liable for any damage that can"
echo "  occur in the files that has been modified by this script."
echo ""
echo "  Before running this script, it is probably a VERY GOOD IDEA to"
echo "  take a backup of the existing source code since it might become"
echo "  corrupted."
echo ""
echo "  A backup of all files that will be migrated will be placed in"
echo "  this directory and named hlhdf_upgrade08.tar."
echo ""
echo "  After this script has been run, make sure that your code still"
echo "  compiles and executes correctly before integrating it into your"
echo "  source control system or any other similar source code repository."
echo ""
echo ""
echo "Are you sure you want to run this script [yes|no] ? [no]:"

RUNSCRIPT=no

read RUNSCRIPT
if [ $? -ne 0 ]; then
  echo "FAILED TO READ QUESTION";
  exit 255;
fi

if [ "x$RUNSCRIPT" != "xyes" ]; then
  echo "Aborted migration";
  exit 255;
fi

FILES_TO_UPGRADE=`find . -name "*.[c|h]"`

tar -cvf hlhdf_upgrade08.tar $FILES_TO_UPGRADE

for file in $FILES_TO_UPGRADE; do
  perl $PERLSCRIPT $file > $file.upgrade08
  if [ $? -ne 0 ]; then
    echo "Failed to upgrade $file\n";
    \rm -f $file.upgrade08
  else
    \mv -f $file.upgrade08 $file
  fi
done;
