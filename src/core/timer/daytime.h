// ======================================================================================
// File         : daytime.h
// Author       : Wu Jie 
// Last Change  : 12/30/2010 | 21:03:28 PM | Thursday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef DAYTIME_H_1293714209
#define DAYTIME_H_1293714209
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_daytime_t {
    uint _milli_secs;
} ex_daytime_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_is_valid_daytime ( uint _hour, uint _min, uint _secs, uint _ms );
extern bool ex_is_valid_daytime2 ( uint _ms );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_daytime_set ( ex_daytime_t* _dayTime, uint _hour, uint _min, uint _secs, uint _ms );
extern void ex_daytime_set2 ( ex_daytime_t* _dayTime, uint _ms );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern uint ex_daytime_hours ( const ex_daytime_t* _dayTime );
extern uint ex_daytime_minutes ( const ex_daytime_t* _dayTime );
extern uint ex_daytime_seconds ( const ex_daytime_t* _dayTime );
extern uint ex_daytime_milliseconds ( const ex_daytime_t* _dayTime );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern uint ex_daytime_to_msecs ( const ex_daytime_t* _dayTime );
extern uint64 ex_daytime_to_usecs ( const ex_daytime_t* _dayTime );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_daytime_add ( ex_daytime_t* _result, const ex_daytime_t* _dayTime, const ex_timespan_t* _ts );
extern void ex_daytime_sub ( ex_daytime_t* _result, const ex_daytime_t* _dayTime, const ex_timespan_t* _ts );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_daytime_timespan ( ex_timespan_t* _ts, const ex_daytime_t* _lhs, const ex_daytime_t* _rhs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_eq ( const ex_daytime_t* _lhs, const ex_daytime_t* _rhs ) 
{
    return _lhs->_milli_secs == _rhs->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_ne ( const ex_daytime_t* _lhs, const ex_daytime_t* _rhs ) 
{
    return _lhs->_milli_secs != _rhs->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_gt ( const ex_daytime_t* _lhs, const ex_daytime_t* _rhs ) 
{
    return _lhs->_milli_secs > _rhs->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_ge ( const ex_daytime_t* _lhs, const ex_daytime_t* _rhs ) 
{
    return _lhs->_milli_secs >= _rhs->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_lt ( const ex_daytime_t* _lhs, const ex_daytime_t* _rhs ) 
{
    return _lhs->_milli_secs < _rhs->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_daytime_le ( const ex_daytime_t* _from, const ex_daytime_t* _to ) 
{
    return _lhs->_milli_secs <= _rhs->_milli_secs;
}

// #################################################################################
#endif // END DAYTIME_H_1293714209
// #################################################################################
