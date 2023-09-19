# LibPST2 - fork of original libpst

## Originals
(https://www.five-ten-sg.com/libpst/)[LibPST original web]


## Build Process
```bash
./configure
make
su
make install
```

## Vision
  * Use libpst as code base.
  * Code cleanup. Get rid of Python support - the binding should be implemented in another repo.
  * Make API calls reentrant/thread safe.
  * Get rid of single purpose binaries (readpst, lspst, ...).
  * Make programmable abstraction over original binaries' functionality.
  * Make API integrable to other languages.






