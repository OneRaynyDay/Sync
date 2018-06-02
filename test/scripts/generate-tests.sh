#!/usr/bin/env bash
# Folders are alphabets,
# Files are numbers,
# Symlinks are underscore numbers.
mkdir -p a/b/c/d
touch a/1
touch a/2

echo "Linking _1 to a relative path file"
ln -s ../1 a/b/_1
echo "Linking _2 to an absolute path file"
ln -s $PWD/a/2 a/b/_2
echo "Linking _3 to a deeper directory"
ln -s c a/b/_3
echo "Linking _4 to an earlier directory"
ln -s .. a/b/_4