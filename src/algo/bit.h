// ======================================================================================
// File         : bit.h
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 10:53:23 AM | Monday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BIT_H_1277693605
#define BIT_H_1277693605
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: pow2 
// ------------------------------------------------------------------ 

inline uint32 pow2 ( uint32 _count ) { return ((uint32)1<<_count); }

// ------------------------------------------------------------------
// Desc: Detect is power of 2 or not.
//  If the _value is power of 2, then it is aligned
// ------------------------------------------------------------------

inline bool is_pow_of_2 ( uint _value ) {
    return !((_value-1) & _value);
}

// ------------------------------------------------------------------
// Desc: Detect is aligned or not.
//  If the mod of the _value with _align is 0, then it is aligned
// ------------------------------------------------------------------

inline bool is_aligned ( uint _value, uint _align ) {
    ex_assert( is_pow_of_2(_align), "_align must be pow of 2" );
    return ( (_value & (_align-1)) == 0 );
}

// ------------------------------------------------------------------
// Desc: bitmask
// ------------------------------------------------------------------

inline uint32 bitmask( uint32 _count ) {
    return pow2(_count)-1;
}

// ------------------------------------------------------------------
// Desc: use bits from 0 to _count.
// ------------------------------------------------------------------

inline uint32 usebits ( uint32 _value, uint32 _count )
{
    return (_value & bitmask(_count));
}

// ------------------------------------------------------------------
// Desc: get bit at _bit_pos
// ------------------------------------------------------------------

inline uint32 getbit ( uint32 _value, uint32 _bit_pos )
{
    return (_value & pow2(_bit_pos));
}

///////////////////////////////////////////////////////////////////////////////
// bit rotation
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline uint32 bit_rotl ( uint32 _value, uint32 _rot_count ) {
    const uint size = sizeof(uint32) * 4;
    _rot_count %= size;
    return (_value << _rot_count) | (_value >> (size - _rot_count));
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline uint32 bit_rotr ( uint32 _value, uint32 _rot_count ) {
    const uint size = sizeof(uint32) * 4;
    _rot_count %= size;
    return (_value >> _rot_count) | (_value << (size - _rot_count));
}

///////////////////////////////////////////////////////////////////////////////
// 64-bit op
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline uint64 gen_uint64 ( uint32 _highVal, uint32 _lowVal ) {
    return (((uint64)_highVal)<<32ull) + (uint64)_lowVal;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline int64 gen_int64 ( int32 _highVal, int32 _lowVal ) {
    return (((int64)_highVal)<<32ll) + (int64)_lowVal;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void split_uint64 ( uint64 _value, uint32* _pHighVal, uint32* _pLowVal ) {
    if ( _pLowVal )
    {
        (*_pLowVal) = (uint32)(_value & 0xffffffffull);
    }

    if ( _pHighVal )
    {
        (*_pHighVal) = (uint32)(_value>>32ull);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void split_int64 ( int64 _value, int32* _pHighVal, int32* _pLowVal ) {
    if ( _pLowVal )
    {
        (*_pLowVal) = (int32)(_value & 0xffffffffull);
    }

    if ( _pHighVal )
    {
        (*_pHighVal) = (int32)(_value>>32ull);
    }
}

///////////////////////////////////////////////////////////////////////////////
// swap byte
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void swap_n_bytes ( int _n, void* _value ) {
    uint8* p = (uint8*)_value;
    uint32 i = 0;
    uint32 j = _n - 1;
    uint8  t;
    
    while(i < j)
    {
        t = p[i];

        p[i] = p[j];
        p[j] = t;

        i++;
        j--;
    }
}

// ------------------------------------------------------------------ 
// Desc: swap 1 byte. Does nothing (no swapping needed).
// ------------------------------------------------------------------ 

inline void swap_1_bytes ( void* _value ) {
    EX_UNUSED(_value);
}

// ------------------------------------------------------------------ 
// Desc: swap 2 bytes
// ------------------------------------------------------------------ 

inline void swap_2_bytes ( void* _value ) {
    uint16* pVal = (uint16*)_value;
    *pVal = (((*pVal) >> 8) & 0xff) | (((*pVal) & 0xff) << 8);
}

// ------------------------------------------------------------------ 
// Desc: swap 4 bytes
// ------------------------------------------------------------------ 

inline void swap_4_bytes ( void* _value ) {
#if ( EX_PLATFORM == EX_WIN32 ) && ( EX_COMPILER == EX_MSVC )
    __asm
    {
        mov     esi, _value
        mov		eax, [esi]
        bswap	eax
        mov     [esi], eax
    }
#else
    // TODO: other platform
#endif
}

// ------------------------------------------------------------------ 
// Desc: swap 8 bytes
// ------------------------------------------------------------------ 

inline void swap_8_bytes ( void* _value ) {
#if ( EX_PLATFORM == EX_WIN32 ) && ( EX_COMPILER == EX_MSVC )
    __asm
    {
        mov     esi, _value
        mov		eax, [esi]
        mov		ebx, [esi+4]
        bswap	eax
        bswap	ebx		
        mov     [esi], ebx
        mov     [esi+4], eax
    }
#else
    // TODO: other platform
#endif
}

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END BIT_H_1277693605
// #################################################################################
