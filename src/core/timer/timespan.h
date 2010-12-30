// ======================================================================================
// File         : timespan.h
// Author       : Wu Jie 
// Last Change  : 12/30/2010 | 21:03:45 PM | Thursday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TIMESPAN_H_1293714226
#define TIMESPAN_H_1293714226
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: ex_timespan_t
// ------------------------------------------------------------------ 

typedef struct ex_timespan_t {
    int64 _micro_secs;
} ex_timespan_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_timespan_t ex_timespan_infinity () {
    ex_timespan_t ts;
    ts._micro_secs = EX_INT64_MAX;
    return ts;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_timespan_set ( ex_timespan_t* _ts, int _seconds, int _milliseconds );
extern void ex_timespan_set2 ( ex_timespan_t* _ts, int _days, int _hours, int _miniutes, int _seconds, int _milliseconds );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_timespan_set_secs_f32 ( ex_timespan_t* _ts, float _s );
extern void ex_timespan_set_secs_f64 ( ex_timespan_t* _ts, double _s );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_timespan_set_msecs_f32 ( ex_timespan_t* _ts, float _ms );
extern void ex_timespan_set_msecs_f64 ( ex_timespan_t* _ts, double _ms );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern double ex_timespan_total_days ( ex_timespan_t* _ts );
extern double ex_timespan_total_hours ( ex_timespan_t* _ts );
extern double ex_timespan_total_minutes ( ex_timespan_t* _ts );
extern double ex_timespan_total_seconds ( ex_timespan_t* _ts );
extern double ex_timespan_total_milliseconds ( ex_timespan_t* _ts );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int64 ex_timespan_to_secs ( ex_timespan_t* _ts );
extern int64 ex_timespan_to_msecs ( ex_timespan_t* _ts );
extern int64 ex_timespan_to_usecs ( ex_timespan_t* _ts );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_timespan_days ( ex_timespan_t* _ts );
extern int ex_timespan_hours ( ex_timespan_t* _ts );
extern int ex_timespan_minutes ( ex_timespan_t* _ts );
extern int ex_timespan_seconds ( ex_timespan_t* _ts );
extern int ex_timespan_milliseconds ( ex_timespan_t* _ts );
extern int ex_timespan_microseconds ( ex_timespan_t* _ts );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_eq ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs == _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_ne ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs != _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_gt ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs > _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_ge ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs >= _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_lt ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs < _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_timespan_le ( const ex_timespan_t* _lhs, const ex_timespan_t* _rhs ) 
{
    return _lhs->_micro_secs <= _rhs->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_timespan_add ( ex_timespan_t* _result, ex_timespan_t* _lhs, ex_timespan_t* _rhs ) {
    _result->_micro_secs = _lhs->_micro_secs + _rhs->_micro_secs;
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_timespan_sub ( ex_timespan_t* _result, ex_timespan_t* _lhs, ex_timespan_t* _rhs ) {
    _result->_micro_secs = _lhs->_micro_secs - _rhs->_micro_secs;
} 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TIMESPAN_H_1293714226
// #################################################################################
