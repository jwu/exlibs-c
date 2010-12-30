// ======================================================================================
// File         : timespan.c
// Author       : Wu Jie 
// Last Change  : 12/30/2010 | 21:03:56 PM | Thursday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static const int64 __usecs_per_msec = 1000;
static const int64 __usecs_per_sec  = __usecs_per_msec * 1000;
static const int64 __usecs_per_min  = __usecs_per_sec  * 60;
static const int64 __usecs_per_hour = __usecs_per_min  * 60;
static const int64 __usecs_per_day  = __usecs_per_hour * 24;

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set ( ex_timespan_t* _ts, 
                       int _seconds, 
                       int _milliseconds ) 
{
    _ts->_micro_secs = _seconds * __usecs_per_sec + _milliseconds * __usecs_per_msec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set2 ( ex_timespan_t* _ts, 
                        int _days, 
                        int _hours, 
                        int _miniutes, 
                        int _seconds, 
                        int _milliseconds ) 
{
    _ts->_micro_secs = _days * __usecs_per_day
                    + _hours * __usecs_per_hour
                    + _miniutes * __usecs_per_min
                    + _seconds * __usecs_per_sec
                    + _milliseconds * __usecs_per_msec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set_secs_f32 ( ex_timespan_t* _ts, float _s ) {
    int64 int_sec = (int64)ex_truncf(_s);
    float f_ms = (_s - int_sec) * 1000.0f;
    int64 int_ms = (int64)ex_truncf(f_ms);
    int64 int_us = (int64)ex_roundf((f_ms - int_ms) * 1000.0f);

    _ts->_micro_secs = int_sec * __usecs_per_sec
                    + int_ms  * __usecs_per_msec
                    + int_us;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set_secs_f64 ( ex_timespan_t* _ts, double _s ) {
    int64 int_sec = (int64)ex_trunc(_s);
    float f_ms = (_s - int_sec) * 1000.0;
    int64 int_ms = (int64)ex_trunc(f_ms);
    int64 int_us = (int64)ex_round((f_ms - int_ms) * 1000.0);

    _ts->_micro_secs = int_sec * __usecs_per_sec
                    + int_ms  * __usecs_per_msec
                    + int_us;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set_msecs_f32 ( ex_timespan_t* _ts, float _ms ) {
    int64 int_ms = (int64)ex_truncf(_ms);
    _ts->_micro_secs = int_ms * __usecs_per_msec + int64( ex_roundf((_ms - int_ms) * 1000.0f) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timespan_set_msecs_f64 ( ex_timespan_t* _ts, double _ms ) {
    int64 int_ms = (int64)ex_trunc(_ms);
    _ts->_micro_secs = int_ms * __usecs_per_msec + int64( ex_round((_ms - int_ms) * 1000.0) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

double ex_timespan_total_days ( ex_timespan_t* _ts ) {
    return (double)_ts->_micro_secs / (double)__usecs_per_day;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

double ex_timespan_total_hours ( ex_timespan_t* _ts ) {
    return (double)_ts->_micro_secs / (double)__usecs_per_hour;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

double ex_timespan_total_minutes ( ex_timespan_t* _ts ) {
    return (double)_ts->_micro_secs / (double)__usecs_per_min;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

double ex_timespan_total_seconds ( ex_timespan_t* _ts ) {
    return (double)_ts->_micro_secs / (double)__usecs_per_sec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

double ex_timespan_total_milliseconds ( ex_timespan_t* _ts ) {
    return (double)_ts->_micro_secs / (double)__usecs_per_msec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int64 ex_timespan_to_secs ( ex_timespan_t* _ts ) {
    return (int64)ex_round( ex_timespan_total_seconds(_ts) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int64 ex_timespan_to_msecs ( ex_timespan_t* _ts ) {
    return (int64)ex_round( ex_timespan_total_milliseconds(_ts) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int64 ex_timespan_to_usecs ( ex_timespan_t* _ts ) {
    return _ts->_micro_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_days ( ex_timespan_t* _ts ) {
    return int(_ts->_micro_secs / __usecs_per_day);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_hours ( ex_timespan_t* _ts ) {
    int64 mod_day = _ts->_micro_secs % __usecs_per_day;
    return int(mod_day / __usecs_per_hour);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_minutes ( ex_timespan_t* _ts ) {
    int64 mod_day = _ts->_micro_secs % __usecs_per_day;
    int64 mod_hour = mod_day % __usecs_per_hour;
    return int(mod_hour / __usecs_per_min);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_seconds ( ex_timespan_t* _ts ) {
    int64 mod_day = _ts->_micro_secs % __usecs_per_day;
    int64 mod_hour = mod_day % __usecs_per_hour;
    int64 mod_min = mod_hour % __usecs_per_min;
    return int(mod_min / __usecs_per_sec);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_milliseconds ( ex_timespan_t* _ts ) {
    int64 mod_day = _ts->_micro_secs % __usecs_per_day;
    int64 mod_hour = mod_day % __usecs_per_hour;
    int64 mod_min = mod_hour % __usecs_per_min;
    int64 mod_sec = mod_min % __usecs_per_sec;
    return int(mod_sec / __usecs_per_msec);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_timespan_microseconds ( ex_timespan_t* _ts ) {
    int64 mod_day = _ts->_micro_secs % __usecs_per_day;
    int64 mod_hour = mod_day % __usecs_per_hour;
    int64 mod_min = mod_hour % __usecs_per_min;
    int64 mod_sec = mod_min % __usecs_per_sec;
    int64 mod_msec = mod_sec % __usecs_per_msec;
    return int(mod_msec);
}
