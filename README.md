# Sync - an Rsync alternative

What this is NOT:
- this is not a replacement for rsync.
- this is not a general purpose rsync.
- this is not meant to be performant(but isn't bad).

This addresses the following issues:

## Correctness

### Symlinks
rsync has a myriad of flags, and a 10 page manual. And 90% of it is dedicated to symlinks.
Symlinks are one of the most annoying things in existence, and rsync tries to do the most
"reasonable" thing.

A list of "reasonable" things it does:

1. `--copy-unsafe-links` takes any symlinks that points to referents NOT in our list of files/directories to sync and on the remote side the symlink becomes a copied version of that directory.

This is good, and this is bad. This is good because... well, what is the correct course of action to copy an unsafe link? This is obviously a security concern and also a matter of accidentally overwriting your files. This is also bad, because sometimes we just **want the damn symlink to be a symlink on the remote side**.

2. `--links --keep-dir-links` takes any symlinks and preserves them to the referent.

This is good, and this is bad. This is good because... well, it makes the symlink structure CORRECT. However, we are NOT following the symlinks in this case!

If you try to google up stack overflow solutions for this problem, you'd reach back to something similar to part 1 with `-K` for copying dir links, and `-L` for, just, copying all links as files/directories/etc.

3. If you try to rsync a file with path: `a/b/c`, where `a/b->d` (i.e. `b` is a symlink), and the actual file system looks like:

```
a/b ->d
d/c
```

We will arrive at:

```
a/b/c
```

On the remote side. This is similar to copying unsafe links. We create our own directory at where it's supposed to be and is not actually a symlink.

### Dry runs
There is an (un)intended usage for rsync, which is to perform a dry-run over the file system to get a list of files to sync. Often, something like `rsync --dry-run > file` and then `parallel` or `xargs` is then used to rsync in parallel of all the files(divided into small chunks).

However, there are some problems:

1. `--dry-run` pulls in all incremental files up to the actual file, which gives us a filelist that looks like:

```
rsync -avzR --dry-run a/b/c/d/e
a
a/b
a/b/c
a/b/c/d
a/b/c/d/e

rsync -avzR --files-from=dry-run-list.txt /src /dst
```

If we split this into 5 files, we will inevitably accidentally rsync all of `a`, right? Actually no - we'd accidentally rsync everything 1 level underneath `a`, so files like `a/b`, `a/1`, `a/2`... are rsync'd(which is not what we want either). This is due to the fact of this one entry in the rsync man page:

```
              o      The  --archive  (-a)  option's behavior does not imply --recursive (-r), so specify it explicitly, if you
                     want it.
```

Under the `--files-from` argument. 

Now, this is unexpected behavior at least from MY perspective, but `rsync` doesn't seem to think so. This is both confusing and wastes a lot of time. Specifying the `--recursive` flag will pull in the entire `a` directory recursively, which is also not what we want.

2. For any escaped characters like `\n` which becomes `#022`, it is impossible to correctly sync this file. One could make the argument that `\n` should not be in a file name in the first place, but that's an argument for another time. The point is, that we should not generate a dry run to rsync.

"But what if we need to run it in parallel? How are we going to get fine grained granularity of folders to sync?"

That question is hard to answer with the current rsync set up without `--dry-run`.


