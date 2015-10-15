#!/bin/sh
appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

plugindir="$HOME/.config/vapoursynth"

`cd $plugindir 2>/dev/null` ||
{
 `mkdir -p $plugindir`
 `echo "UserPluginDir=$PWD/vapoursynth" > $plugindir/vapoursynth.conf `
}


if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi

export LD_LIBRARY_PATH=$dirname
export PYTHONPATH=$dirname/site-packages

$dirname/$appname "$@" 2>/dev/null
