# LibPST2 - fork of original libpst

## Originals
[LibPST original web](https://www.five-ten-sg.com/libpst/)


## Build Process
```bash
automake --add-missing # optional - for developers
autoconf # optional - for developers
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
  - [x] Get rid of single purpose binaries (readpst, lspst, ...).
  - [ ] Make programmable abstraction over non-basic original binaries' functionality:
    - [x] `lspst`
    - [x] `readpst`
    - [ ] `pst2ldif`
    - [ ] `pst2dii`
  - [x] Make API integrable to other languages.
  - [x] Allow streaming of records from pst and parallell export.
  - [ ] Redefine old single purpose binaries to support NDJSON format on input/output and create separate repo.
  - [ ] Create separate bindings:
    - [x] Golang - [gopst](https://github.com/SpongeData-cz/gopst),
    - [ ] Python.
  - [ ] Rename project to `libpst2`?

## Example Of Use
This is simplistic alternative to `readpst` utility in modern API.

```c
#include <regex.h>
#include <string.h>
#include "define.h"
#include "pst.h"

int main(int argc, char* const* argv) {
    if (argc < 2) {
        return 1;
    }

    const char * path = argv[1];
    pst_record_enumerator *ie = record_enumerator_new(path);
    pst_list(ie);

    pst_record ** lst = ie->items;
    pst_export * ppe = pst_export_new(pst_export_conf_default);
    ppe->pstfile = ie->file; // misconception

    int nth = 0;

    while(*lst) {
        pst_record * pr = *lst;
        lst++;
        if(!pr) continue;
        printf("Known type with path %s / %s\n", pr->logical_path, pr->name);

        char * rename = calloc(128, sizeof(char));
        snprintf(rename, 96, "output_%d.eml", nth++ );
        pr->renaming = rename;

        int error;
        uint64_t written = pst_record_to_file(pr, ppe, &error);

        free(rename);
        pr->renaming = NULL;

        printf("Written %lu, error: %d\n", written, error);
    }

    record_enumerator_destroy(ie);
    pst_export_destroy(ppe);

    return 0;
}
```
