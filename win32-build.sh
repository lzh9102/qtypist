# run this file with cygwin32

# rebuild the program
make clean
qmake
make release
lupdate qtypist.pro
lrelease qtypist.pro

# create target directory
rm -r qtypist
mkdir qtypist

# copy dynamic libraries and translations
cp *.dll release/qtypist.exe audio-sources.txt qtypist
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

# create 7zip archive
rm -r qtypist-w32.7z
7z a -y qtypist-w32.7z qtypist/
read -p "press any key to continue"
