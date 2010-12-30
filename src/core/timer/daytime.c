// ======================================================================================
// File         : daytime.c
// Author       : Wu Jie 
// Last Change  : 12/30/2010 | 21:03:41 PM | Thursday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static const uint __msecs_per_sec  = 1000;
static const uint __msecs_per_min  = __msecs_per_sec  * 60;
static const uint __msecs_per_hour = __msecs_per_min  * 60;
static const uint __msecs_per_day  = __msecs_per_hour * 24;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __daytime_add_secs ( ex_daytime_t* _result, 
                                 const ex_daytime_t* _dayTime, 
                                 int _secs ) 
{
    __daytime_add_msecs( _result, _dayTime, _secs * 1000 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __daytime_add_msecs ( ex_daytime_t* _result, 
                                  const ex_daytime_t* _dayTime, 
                                  int64 _ms ) 
{
    int64 msecs = ( _dayTime->_milli_secs + _ms ) % __msecs_per_day;
    msecs = msecs < 0 ? __msecs_per_day + msecs : msecs;
    _result->_milli_secs = (uint)msecs;
}

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_is_valid_daytime ( uint _hour, uint _min, uint _secs, uint _ms ) {
    return _hour < 24 && _min < 60 && _secs < 60 && _ms < 1000;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_is_valid_daytime2 ( uint _ms ) {
    return _ms < __msecs_per_day;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_daytime_set ( ex_daytime_t* _dayTime, 
                      uint _hour, uint _min, uint _secs, uint _ms ) {
    if ( !ex_is_valid_daytime( _hour, _min, _secs, _ms ) ) {
        ex_warning ( "can't set time (%dh,%dm,%ds,%dms). the value is invalid!", _hour, _min, _secs, _ms );
        return;
    }
    _dayTime->_milli_secs = _hour*__msecs_per_hour 
        + _min*__msecs_per_min 
        + _secs*__msecs_per_sec 
        + _ms;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_daytime_set ( ex_daytime_t* _dayTime, uint _ms ) {
    if ( !ex_is_valid_daytime2(_ms) ) {
        ex_warning ( "can't set time (%dms). the value is invalid! large than one-day.", _ms );
        return;
    }
    _dayTime->_milli_secs = _ms;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint ex_daytime_hours ( const ex_daytime_t* _dayTime ) {
    return _dayTime->_milli_secs / __msecs_per_hour;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint ex_daytime_minutes ( const ex_daytime_t* _dayTime ) {
    uint mod_hour = _dayTime->_milli_secs % __msecs_per_hour;
    return mod_hour / __msecs_per_min;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint ex_daytime_seconds ( const ex_daytime_t* _dayTime ) {
    uint mod_hour = _dayTime->_milli_secs % __msecs_per_hour;
    uint mod_min = mod_hour % __msecs_per_min;
    return mod_min / __msecs_per_sec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint ex_daytime_milliseconds ( const ex_daytime_t* _dayTime ) {
    uint mod_hour = _dayTime->_milli_secs % __msecs_per_hour;
    uint mod_min = mod_hour % __msecs_per_min;
    uint mod_sec = mod_min % __msecs_per_sec;
    return mod_sec;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint ex_daytime_to_msecs ( const ex_daytime_t* _dayTime ) {
    return _dayTime->_milli_secs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

uint64 ex_daytime_to_usecs ( const ex_daytime_t* _dayTime ) {
    return _dayTime->_milli_secs * 1000;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_daytime_add ( ex_daytime_t* _result, 
                      const ex_daytime_t* _dayTime, 
                      const ex_timespan_t* _ts ) 
{
    __daytime_add_msecs ( _result, _dayTime, ex_timespan_to_msecs(_ts) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_daytime_sub ( ex_daytime_t* _result, 
                      const ex_daytime_t* _dayTime, 
                      const ex_timespan_t* _ts ) 
{
    __daytime_add_msecs ( _result, _dayTime, -ex_timespan_to_msecs(_ts) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_daytime_timespan ( ex_timespan_t* _ts, 
                           const ex_daytime_t* _from, 
                           const ex_daytime_t* _to ) 
{
    if ( ex_daytime_gt( _from, _to) )
        _ts->_micro_secs = (int64)((_to->_milli_secs + __msecs_per_day - _from->_milli_secs) * 1000);
    else
        _ts->_micro_secs = (int64)((_to->_milli_secs - _from->_milli_secs) * 1000);
}
