# run this file with cygwin32

DEST=qtypist
DEFINES=
if [ "$QTYPIST_PORTABLE_APP" == "1" ]; then
	echo "CONFIG: PORTABLE APP"
	DEFINES="DEFINES+=PORTABLE_APP"
	DEST=qtypist-portable
fi

# rebuild the program
make clean
qmake $DEFINES
make release
lrelease qtypist.pro

# create target directory
rm -rf $DEST
mkdir $DEST

# copy programs and related files
cp *.dll release/qtypist.exe audio-sources.txt $DEST
cp *.exe $DEST
cp license.txt changelog.txt $DEST
unix2dos $DEST/license.txt $DEST/changelog.txt
[ "$QTYPIST_PORTABLE_APP" != "1" ] && cp qtypist.nsi $DEST
mkdir $DEST/translations/
cp translations/*.qm $DEST/translations/

# extract lists
for f in lists/*.7z; do
	(cd lists && 7z -y x `basename $f`)
done

# copy lists
mkdir $DEST/lists/
for f in lists/*.txt; do
	TARGET=$DEST/lists/`basename $f`
	iconv -c -f UTF-8 -t BIG-5 $f > $TARGET
	unix2dos $TARGET
done
cp lists/README $DEST/lists/README.text
unix2dos $DEST/lists/README.text

read -p "files have been copied to \"$DEST/\". press any key to continue..."