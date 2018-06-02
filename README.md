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
