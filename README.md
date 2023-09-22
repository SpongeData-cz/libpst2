# LibPST2 - fork of original libpst

## Originals
[LibPST original web](https://www.five-ten-sg.com/libpst/)


## Build Process
```bash
automake --add-missing
autoconf
./configure
make
su
make install
```

**Note** - It may happen usage of ```autoreconf && autoupdate``` is needed if you use newer automake tools than original.

## Vision
  - [x] Use libpst as code base.
  - [x] Code cleanup. Get rid of Python support - the binding should be implemented in another repo.
  - [x] Make API calls reentrant/thread safe.
  - [ ] Get rid of single purpose binaries (readpst, lspst, ...).
  - [ ] Make programmable abstraction over non-basic original binaries' functionality.
  - [ ] Make API integrable to other languages.
  - [ ] Redefine old single purpose binaries to support NDJSON format on input/output.
  - [ ] Allow streaming of records from pst and parallell export.








