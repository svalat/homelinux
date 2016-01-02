#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 1.0.0                          #
#           Date    : 12/2015                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

######################################################
# This is just a python wrapper to update python path with good version
# by automatically wrapping the python executable and update PYTHONPATH before call

#remove from path
app=$0
cur=$(dirname $app)
export PATH=$(echo $PATH | sed -e "s#$cur:##g")

#echo $PATH 1>&2

#get version
called=$(basename $0)
PYTHON_DEFAULT=$(python --version 2>&1 | cut -d ' ' -f 2 | cut -d '.' -f 1-2)
PYTHON_CALLED=$($called --version 2>&1 | cut -d ' ' -f 2 | cut -d '.' -f 1-2)

#echo $PYTHON_DEFAULT 1>&2
#echo $PYTHON_CALLED 1>&2

#replace in path
export PYTHONPATH="$(echo $PYTHONPATH | sed -e "s/$PYTHON_DEFAULT/$PYTHON_CALLED/g")"

#echo "Update export to $PYTHONPATH" 1>&2

#call
"$called" "$@"
exit $?
