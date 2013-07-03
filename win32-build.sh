# run this file with cygwin32

# rebuild the program
make clean
qmake
make release
lrelease qtypist.pro

# create target directory
rm -rf qtypist
mkdir qtypist

# copy programs and related files
cp *.dll release/qtypist.exe audio-sources.txt qtypist
cp *.exe qtypist
cp license.txt changelog.txt qtypist
unix2dos qtypist/license.txt qtypist/changelog.txt
cp qtypist.nsi qtypist
mkdir qtypist/translations/
cp translations/*.qm qtypist/translations/

# extract lists
for f in lists/*.7z; do
	(cd lists && 7z -y x `basename $f`)
done

# copy lists
mkdir qtypist/lists/
for f in lists/*.txt; do
	TARGET=qtypist/lists/`basename $f`
	iconv -c -f UTF-8 -t BIG-5 $f > $TARGET
	unix2dos $TARGET
done
cp lists/README qtypist/lists/README.text
unix2dos qtypist/lists/README.text

read -p "files have been copied to \"qtypist/\". press any key to continue..."