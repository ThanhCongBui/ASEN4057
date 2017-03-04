#!/bin/bash -x
if [[ -d $1 ]]; then
echo "\"$1\" is a directory."
echo "You have the folowing permissions to it:"
r=$( ls -ld $1 | head -c 2 | tail -c 1)
if r==r ;then
text="read "
else
text=""
fi
r=$( ls -ld $1 | head -c 3 | tail -c 1)
if r==w ;then
text=$text"write "
fi
r=$( ls -ld $1 | head -c 4 | tail -c 1)
if r==x ;then
text=$text"execute "
fi
printf "%s\n" "$text"
elif [[ -f $1 ]]; then
echo "\"$1\" is a file."
echo "You have the following permissions to it:"
r=$( ls -l $1 | head -c 2 | tail -c 1)
if r==r ;then
text="read "
else
text=""
fi
r=$( ls -l $1 | head -c 3 | tail -c 1)
if r==w; then
text=$text"write "
fi
r=$( ls -l $1 | head -c 4 | tail -c 1)
if r==x; then
text=$text"execute"
fi
printf "%s\n" "$text"
else
echo "$1 is neither a regular file or directory"
fi
