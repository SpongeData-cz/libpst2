
#ifndef __PST_VBUF_H
#define __PST_VBUF_H

#include "common.h"
#include <iconv.h> // TODO: conditinal include

#define MAX_PTR ((void*)-1)

// Variable-length buffers
struct pst_varbuf {
	size_t dlen; 	//length of data stored in buffer
	size_t blen; 	//length of buffer
	char *buf; 	    //buffer
	char *b;	    //start of stored data
};

typedef struct pst_vbuf_context {
    int unicode_up;
    iconv_t i16to8;
    const char *target_charset;
    int         target_open_from;
    int         target_open_to;
    iconv_t     i8totarget;
    iconv_t     target2i8;
} pst_vbuf_context;

static const pst_vbuf_context pst_vbuf_context_default = {
    0, NULL, NULL, 0, 0, MAX_PTR, MAX_PTR
};


typedef struct pst_varbuf pst_vbuf;

pst_vbuf  *pst_vballoc(size_t len);
void       pst_vbgrow(pst_vbuf *vb, size_t len);    // grow buffer by len bytes, data are preserved
void       pst_vbset(pst_vbuf *vb, void *data, size_t len);
void       pst_vbappend(pst_vbuf *vb, void *data, size_t length);
void       pst_unicode_init(pst_vbuf_context ctx);

size_t pst_vb_utf16to8(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen);
size_t pst_vb_utf8to8bit(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen, const char* charset);
size_t pst_vb_8bit2utf8(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen, const char* charset);

#endif
