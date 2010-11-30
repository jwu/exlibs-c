// ======================================================================================
// File         : encoding.c
// Author       : Wu Jie 
// Last Change  : 08/08/2010 | 23:02:17 PM | Sunday,August
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "encoding.h"

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int _utf_char2bytes( int c, uint8* buf )
{
    if (c < 0x80)		/* 7 bits */
    {
        buf[0] = c;
        return 1;
    }
    if (c < 0x800)		/* 11 bits */
    {
        buf[0] = 0xc0 + ((unsigned)c >> 6);
        buf[1] = 0x80 + (c & 0x3f);
        return 2;
    }
    if (c < 0x10000)		/* 16 bits */
    {
        buf[0] = 0xe0 + ((unsigned)c >> 12);
        buf[1] = 0x80 + (((unsigned)c >> 6) & 0x3f);
        buf[2] = 0x80 + (c & 0x3f);
        return 3;
    }
    if (c < 0x200000)		/* 21 bits */
    {
        buf[0] = 0xf0 + ((unsigned)c >> 18);
        buf[1] = 0x80 + (((unsigned)c >> 12) & 0x3f);
        buf[2] = 0x80 + (((unsigned)c >> 6) & 0x3f);
        buf[3] = 0x80 + (c & 0x3f);
        return 4;
    }
    if (c < 0x4000000)		/* 26 bits */
    {
        buf[0] = 0xf8 + ((unsigned)c >> 24);
        buf[1] = 0x80 + (((unsigned)c >> 18) & 0x3f);
        buf[2] = 0x80 + (((unsigned)c >> 12) & 0x3f);
        buf[3] = 0x80 + (((unsigned)c >> 6) & 0x3f);
        buf[4] = 0x80 + (c & 0x3f);
        return 5;
    }
    /* 31 bits */
    buf[0] = 0xfc + ((unsigned)c >> 30);
    buf[1] = 0x80 + (((unsigned)c >> 24) & 0x3f);
    buf[2] = 0x80 + (((unsigned)c >> 18) & 0x3f);
    buf[3] = 0x80 + (((unsigned)c >> 12) & 0x3f);
    buf[4] = 0x80 + (((unsigned)c >> 6) & 0x3f);
    buf[5] = 0x80 + (c & 0x3f);
    return 6;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int _utf_char2len ( int c )
{
    if (c < 0x80)
        return 1;
    if (c < 0x800)
        return 2;
    if (c < 0x10000)
        return 3;
    if (c < 0x200000)
        return 4;
    if (c < 0x4000000)
        return 5;
    return 6;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_ucs2_to_utf8 ( const wchar_t* _instr, int _inlen, char* _outstr )
{
    int outlen = 0;
    int todo = _inlen;
    const uint16 *p = _instr;
    int l;

    while ( todo > 0 ) {
        if (_outstr != NULL) {
            l = _utf_char2bytes(*p, _outstr);
            _outstr += l;
        }
        else
            l = _utf_char2len(*p);
        ++p;
        outlen += l;
        --todo;
    }

    return outlen;
}
