
#include "define.h"

#define ASSERT(x,...) { if( !(x) ) DIE(( __VA_ARGS__)); }


/** DESTRUCTIVELY grow or shrink buffer
 */
static void   pst_vbresize(pst_vbuf *vb, size_t len);
static void pst_vbresize(pst_vbuf *vb, size_t len)
{
    vb->dlen = 0;

    if (vb->blen >= len) {
        vb->b = vb->buf;
        return;
    }

    vb->buf  = realloc(vb->buf, len);
    vb->b    = vb->buf;
    vb->blen = len;
}


static size_t pst_vbavail(pst_vbuf * vb);
static size_t pst_vbavail(pst_vbuf * vb)
{
    return vb->blen  - vb->dlen - (size_t)(vb->b - vb->buf);
}

// REFACTORED:
static void open_targets(pst_vbuf_context ctx, const char* charset)
{
    if (!ctx.target_charset || strcasecmp(ctx.target_charset, charset)) {
        if (ctx.target_open_from) iconv_close(ctx.i8totarget);
        if (ctx.target_open_to)   iconv_close(ctx.target2i8);
        if (ctx.target_charset)   free((char *)ctx.target_charset);
        ctx.target_charset   = strdup(charset);
        ctx.target_open_from = 1;
        ctx.target_open_to   = 1;
        ctx.i8totarget = iconv_open(ctx.target_charset, "utf-8");
        if (ctx.i8totarget == (iconv_t)-1) {
            ctx.target_open_from = 0;
            DEBUG_WARN(("Couldn't open iconv descriptor for utf-8 to %s.\n", ctx.target_charset));
        }
        ctx.target2i8 = iconv_open("utf-8", ctx.target_charset);
        if (ctx.target2i8 == (iconv_t)-1) {
            ctx.target_open_to = 0;
            DEBUG_WARN(("Couldn't open iconv descriptor for %s to utf-8.\n", ctx.target_charset));
        }
    }
}


static size_t sbcs_conversion(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen, iconv_t conversion)
{
    size_t inbytesleft  = iblen;
    size_t icresult     = (size_t)-1;
    size_t outbytesleft = 0;
    char *outbuf        = NULL;
    int   myerrno;

    DEBUG_ENT("sbcs_conversion");
    pst_vbresize(dest, 2*iblen);

    do {
        outbytesleft = dest->blen - dest->dlen;
        outbuf = dest->b + dest->dlen;
        icresult = iconv(conversion, (ICONV_CONST char**)&inbuf, &inbytesleft, &outbuf, &outbytesleft);
        myerrno  = errno;
        dest->dlen = outbuf - dest->b;
        if (inbytesleft) pst_vbgrow(dest, 2*inbytesleft);
    } while ((size_t)-1 == icresult && E2BIG == myerrno);

    if (icresult == (size_t)-1) {
        DEBUG_WARN(("iconv failure: %s\n", strerror(myerrno)));
        pst_unicode_init(ctx);
        DEBUG_RET();
        return (size_t)-1;
    }
    DEBUG_RET();
    return 0;
}


static void pst_unicode_close(pst_vbuf_context ctx)
{
    iconv_close(ctx.i16to8);
    if (ctx.target_open_from) iconv_close(ctx.i8totarget);
    if (ctx.target_open_to)   iconv_close(ctx.target2i8);
    if (ctx.target_charset)   free((char *)ctx.target_charset);
    ctx.target_charset   = NULL;
    ctx.target_open_from = 0;
    ctx.target_open_to   = 0;
    ctx.unicode_up = 0;
}


static int utf16_is_terminated(const char *str, int length);
static int utf16_is_terminated(const char *str, int length)
{
    int len = -1;
    int i;
    for (i = 0; i < length; i += 2) {
        if (str[i] == 0 && str[i + 1] == 0) {
            len = i;
        }
    }

    if (len == -1) {
        DEBUG_WARN(("utf16 string is not zero terminated\n"));
    }

    return (len == -1) ? 0 : 1;
}


pst_vbuf *pst_vballoc(size_t len)
{
    pst_vbuf *result = pst_malloc(sizeof(pst_vbuf));
    if (result) {
        result->dlen = 0;
        result->blen = 0;
        result->buf = NULL;
        pst_vbresize(result, len);
    }
    else DIE(("malloc() failure"));
    return result;
}


/** out: vbavail(vb) >= len, data are preserved
 */
void pst_vbgrow(pst_vbuf *vb, size_t len)
{
    if (0 == len)
        return;

    if (0 == vb->blen) {
        pst_vbresize(vb, len);
        return;
    }

    if (vb->dlen + len > vb->blen) {
        if (vb->dlen + len < vb->blen * 1.5)
            len = vb->blen * 1.5;
        char *nb = pst_malloc(vb->blen + len);
        if (!nb) DIE(("malloc() failure"));
        vb->blen = vb->blen + len;
        memcpy(nb, vb->b, vb->dlen);

        free(vb->buf);
        vb->buf = nb;
        vb->b = vb->buf;
    } else {
        if (vb->b != vb->buf)
            memcpy(vb->buf, vb->b, vb->dlen);
    }

    vb->b = vb->buf;

    ASSERT(pst_vbavail(vb) >= len, "vbgrow(): I have failed in my mission.");
}


/** set vbuf b size=len, resize if necessary, relen = how much to over-allocate
 */
void pst_vbset(pst_vbuf * vb, void *b, size_t len)
{
    pst_vbresize(vb, len);
    memcpy(vb->b, b, len);
    vb->dlen = len;
}


/** append len bytes of b to vb, resize if necessary
 */
void pst_vbappend(pst_vbuf *vb, void *b, size_t len)
{
    if (0 == vb->dlen) {
        pst_vbset(vb, b, len);
        return;
    }
    pst_vbgrow(vb, len);
    memcpy(vb->b + vb->dlen, b, len);
    vb->dlen += len;
}


void pst_unicode_init(pst_vbuf_context ctx)
{
    if (ctx.unicode_up) pst_unicode_close(ctx);
    ctx.i16to8 = iconv_open("utf-8", "utf-16le");
    if (ctx.i16to8 == (iconv_t)-1) {
        DEBUG_WARN(("Couldn't open iconv descriptor for utf-16le to utf-8.\n"));
    }
    ctx.unicode_up = 1;
}


size_t pst_vb_utf16to8(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen)
{
    size_t inbytesleft  = iblen;
    size_t icresult     = (size_t)-1;
    size_t outbytesleft = 0;
    char *outbuf        = NULL;
    int   myerrno;

    if (!ctx.unicode_up) return (size_t)-1;   // failure to open iconv
    pst_vbresize(dest, iblen);

    //Bad Things can happen if a non-zero-terminated utf16 string comes through here
    if (!utf16_is_terminated(inbuf, iblen))
        return (size_t)-1;

    do {
        outbytesleft = dest->blen - dest->dlen;
        outbuf = dest->b + dest->dlen;
        icresult = iconv(ctx.i16to8, (ICONV_CONST char**)&inbuf, &inbytesleft, &outbuf, &outbytesleft);
        myerrno  = errno;
        dest->dlen = outbuf - dest->b;
        if (inbytesleft) pst_vbgrow(dest, inbytesleft);
    } while ((size_t)-1 == icresult && E2BIG == myerrno);

    if (icresult == (size_t)-1) {
        DEBUG_WARN(("iconv failure: %s\n", strerror(myerrno)));
        pst_unicode_init(ctx);
        return (size_t)-1;
    }
    return (icresult) ? (size_t)-1 : 0;
}


size_t pst_vb_utf8to8bit(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen, const char* charset)
{
    open_targets(ctx, charset);
    if (!ctx.target_open_from) return (size_t)-1;   // failure to open the target
    return sbcs_conversion(ctx, dest, inbuf, iblen, ctx.i8totarget);
}


size_t pst_vb_8bit2utf8(pst_vbuf_context ctx, pst_vbuf *dest, const char *inbuf, int iblen, const char* charset)
{
    open_targets(ctx, charset);
    if (!ctx.target_open_to) return (size_t)-1;     // failure to open the target
    return sbcs_conversion(ctx, dest, inbuf, iblen, ctx.target2i8);
}

