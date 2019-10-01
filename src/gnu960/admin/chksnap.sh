:

# This script verifies that the current set of source files matches
# the configuration, or snapshot, given by the appropriate version
# of ../common/i_config, as determined by i_minrev.
#
# Exit status is 0 if the configurations match, else 1.
#
# Usage is:  $0 [-q] [hostname]
#	-q is quiet mode; suppress REMARKS that may or may not be harmless.
#	If hostname is not given, we try to determine it automatically.

myname=`basename $0`

# This script assumes it is being invoked in a tool or library working
# directory, parallel to ../common.

if [ -z "$G960ROOT" -o ! -d "$G960ROOT" ] ; then
	echo "$myname ERROR -- G960ROOT must be properly defined."
	exit 1
fi

case `pwd` in
  $G960ROOT/*) ;;
  *)
	echo "$myname ERROR -- Current directory must be under G960ROOT ($G960ROOT)."
	exit 1
	;;
esac

CFG_VG=../common/RCS/i_config,v
export CFG_VG

if [ ! -f i_minrev -o ! -f $CFG_VG ] ; then
	echo "$myname ERROR -- Cannot find i_minrev or $CFG_VG"
	exit 1
fi

# Make sure that RCS's 'co' command exists on this host.  It doesn't
# exist on all of our official build boxes.  For those boxes, ignore
# the snapshot verification.

if (co -p $CFG_VG > /dev/null 2>&1) ; then
	:
else
	echo "$myname WARNING:  'co' not available, skipping snapshot verification."
	exit 0
fi

quiet=0
if [ "x$1" = "x-q" ] ; then
	# Don't emit REMARKS
	quiet=1
	shift
fi

CURDIR=`pwd`
ONE_UP=`dirname $CURDIR`

# Determine our host's name

if [ $# -ge 1 ] ; then
    HNAME=$1
    shift
else
    case `basename $CURDIR` in
	*dos*)		HNAME=dos
		;;
	*hp700*)	HNAME=hp700
		;;
	*hp9000*)	HNAME=hp9000
		;;
	*i386vr4*)	HNAME=i386vr4
		;;
	*i386v*)	HNAME=i386v
		;;
	*rs6000*)	HNAME=rs6000
		;;
	*sol-sun4*)	HNAME=sol-sun4
		;;
	*sun4*)		HNAME=sun4
		;;
	*)
		echo "Enter your host's GNU nickname -> "
		read HNAME
		;;
    esac
fi

export HNAME

case $HNAME in
	dos) ;;
	hp700) ;;
	hp9000) ;;
	i386vr4) ;;
	i386v) ;;
	rs6000) ;;
	sun4) ;;
	sol-sun4) ;;
	*)	echo "$mnyame ERROR -- Unrecognized host $HNAME"
		exit 1
		;;
esac

if [ $# -ge 1 ] ; then
	echo "ERROR -- Usage:  $0 [-q] [hostname]"
	echo "      -- -q specifies quiet; suppress remarks"
	echo "      -- hostname is determined automagically if not given"
	exit 1
fi

# Determine the RCS revision of i_config corresponding to i_minrev.

mrev=`cat i_minrev`
major=`expr $mrev / 1000`
minor=`expr $mrev % 1000`
if [ $major -eq 0 ] ; then
	# Early snapshot minrev's were only three digits
	major=`expr $mrev / 100`
	minor=`expr $mrev % 100`
fi

if [ $major -eq 0 -o $major -ge 10 ] ; then
	echo "$myname ERROR -- unrecognized value ($mrev) in file i_minrev."
	exit 1
fi

# Check out the appropriate version of i_config.
# Note that co will not always report an error if the specified revision
# doesn't exist.  The revision checked out will be reported to stderr.

TMP_CFG=$$cfig.tm1
TMP_REV=$$cfig.tm2

if [ -f "$TMP_CFG" -o -f "$TMP_REV" ] ; then
	echo "$myname ERROR -- Temporary file $TMP_CFG or $TMP_REV exists."
	exit 1
fi

co -p -r$major.$minor $CFG_VG > $TMP_CFG 2> $TMP_REV

# Compact white space in $TMP_REV before comparing, since it differs by host

if [ "`grep revision $TMP_REV | sed 's/ //g'`" != "revision$major.$minor" ] ; then
	echo "$myname ERROR -- Revision $major.$minor of $CFG_VG does not exist."
	rm -f $TMP_CFG $TMP_REV
	exit 1
fi

rm -f $TMP_REV

# Examine each "<filename, revision>" pair in $TMP_CFG.
# Each filename is relative to $G960ROOT

status=0

for line in `awk '{print $1 "ZZZ" $2}' $TMP_CFG` ; do
	file=`echo $line | sed 's/ZZZ[^Z].*$//'`
	vers=`echo $line | sed 's/^[^Z]*ZZZ//'`

	# We need to special-case src/include, since it has no common directory.
	# We also need to special-case the current directory, which may or
	# may not end in a recognized host name.
	# For other file names, we need to replace /common/ with /host/,
	# using the appropriate host.

	case $G960ROOT/$file in
	  $G960ROOT/src/include/sys/*)
	    COMMON_FILE=$G960ROOT/$file
	    RCS_FILE=$G960ROOT/src/include/sys/RCS/`basename $file`,v
		;;
	  $G960ROOT/src/include/*)
	    COMMON_FILE=$G960ROOT/$file
	    RCS_FILE=$G960ROOT/src/include/RCS/`basename $file`,v
		;;
	  $ONE_UP/*)
	    COMMON_FILE=$CURDIR/`basename $file`
	    RCS_FILE=$G960ROOT/`echo $file | sed 's,/common/,/common/RCS/,'`,v
		;;
	  *)
	    COMMON_FILE=$G960ROOT/`echo $file | sed s,/common/,/$HNAME/,`
	    RCS_FILE=$G960ROOT/`echo $file | sed 's,/common/,/common/RCS/,'`,v
		;;
	esac

	# Need to special-case Makefile, compare against Makefile.old instead.
	# If Makefile.old doesn't exist, no check will be done and a warning
	# will be issued below.  We need this hack because some methods of
	# executing "make make" (ie, admin/mkhost, admin/mkmake) do not
	# always create Makefile.old.

	if [ "`basename $COMMON_FILE`" = Makefile ] ; then
		COMMON_FILE=$COMMON_FILE.old
	fi

	if [ ! -f "$COMMON_FILE" ] ; then
	  if [ $quiet -eq 0 ] ; then
		echo "$myname REMARK:  Current build will not use $COMMON_FILE."
		echo "$myname REMARK:  Snapshot may have used revision $vers of $RCS_FILE."
	  fi
	  continue
	fi

	if [ ! -f "$RCS_FILE" ] ; then
		echo "$myname ERROR:  Cannot find RCS file $RCS_FILE."
		status=1
		continue
	fi

	if co -p -r$vers $RCS_FILE 2>/dev/null | cmp -s - $COMMON_FILE ; then
		:
	else
		echo "$myname ERROR:  File $RCS_FILE -r$vers does not match $COMMON_FILE"
		status=1
	fi
done

rm -f $TMP_CFG

if [ $status -ne 0 ] ; then
	echo "$myname ERROR -- Snapshot verification failed."
fi
exit $status
