// ======================================================================================
// File         : date.h
// Author       : Wu Jie 
// Last Change  : 12/30/2010 | 20:10:15 PM | Thursday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef DATE_H_1293711048
#define DATE_H_1293711048
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_date_t {
    uint _julian;
} ex_date_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

const char* ex_week_day_name ( uint _wday );
const char* ex_month_name ( uint _month );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_is_valid_date ( uint _year, uint _month, uint _day );
extern bool ex_is_leap_year ( uint _year );
extern void ex_greg2jul ( uint* _out_jul, int _year, int _month, int _day );
extern void ex_jul2greg ( int* _out_year, int* _out_month, int* _out_day, uint _jul );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_date_set ( ex_date_t* _date, uint _year, uint _month, uint _day );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_date_day ( const ex_date_t* _date );
extern void ex_date_month ( const ex_date_t* _date );
extern void ex_date_year ( const ex_date_t* _date );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern uint ex_date_day_of_week ( const ex_date_t* _date );
extern uint ex_date_day_of_year ( const ex_date_t* _date );
extern uint ex_date_days_in_month ( const ex_date_t* _date );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_date_is_leap_year ( const ex_date_t* _date );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_eq ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian == _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_ne ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian != _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_gt ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian > _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_ge ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian >= _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_lt ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian < _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_date_le ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian <= _rhs->_julian;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_date_add_days ( ex_date_t* _result, const ex_date_t* _date, int _days ) {
    _result->_julian = _date->_julian + _days;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_date_sub_days ( ex_date_t* _result, const ex_date_t* _date, int _days ) {
    _result->_julian = _date->_julian - _days;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline int ex_date_sub_days ( const ex_date_t* _lhs, const ex_date_t* _rhs ) {
    return _lhs->_julian - _rhs->_julian;
}

// #################################################################################
#endif // END DATE_H_1293711048
// #################################################################################


