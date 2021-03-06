#!/usr/bin/env bash
# Folders are alphabets,
# Files are numbers,
# Symlinks are underscore numbers.
mkdir -p a/b/c/d/e/f/g
touch -m -t 203801181205.09 a/1
echo "Hello world!" > a/2
# preserves mode, ownership, timestamps for a/2 to a/3
cp -p a/2 a/3

## TOP LEVEL SYMLINKS
# Linking _1 to a relative path file
ln -s ../1 a/b/_1
# Linking _2 to an absolute path file
ln -s $PWD/a/2 a/b/_2
# Linking _3 to a deeper directory
ln -s c a/b/_3
# Linking _4 to an earlier directory
ln -s .. a/b/_4
# Linking _5 to a nonexistent directory
ln -s a/b/c/d a/b/_5

## STRANGE SYMLINK JUMPS
ln -s c/d/e a/b/jump
touch a/b/c/d/e/f/1
