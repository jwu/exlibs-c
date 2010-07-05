// ======================================================================================
// File         : constants.h
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 16:06:03 PM | Sunday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef CONSTANTS_H_1277625965
#define CONSTANTS_H_1277625965
// #################################################################################

// ------------------------------------------------------------------
// Desc: EX_RESULT
// ------------------------------------------------------------------

#define EX_RESULT int

// general
#define EX_RESULT_UNKNOWN               1000
#define EX_RESULT_OK                    1001
#define EX_RESULT_FAILED                1002
#define EX_RESULT_INVALID_INPUT         1003
#define EX_RESULT_INVALID_OUTPUT        1004

// container operations
#define EX_RESULT_NOT_FOUND             1005
#define EX_RESULT_ALREADY_EXISTED       1006
#define EX_RESULT_ALREADY_FREED         1007
#define EX_RESULT_OUT_OF_RANGE          1008

// math
#define EX_RESULT_INFINITY              1009
#define EX_RESULT_NAN                   1010

// file and directory
#define EX_RESULT_END_OF_FILE           1011
#define EX_RESULT_READ_FAILED           1012
#define EX_RESULT_WRITE_FAILED          1013
#define EX_RESULT_SERIALIZE_FAILED      1014
#define EX_RESULT_PARSE_FAILED          1015
#define EX_RESULT_NOT_EXISTS            1016

// createion,init,destroy.
#define EX_RESULT_CREATE_FAILED         1017
#define EX_RESULT_INIT_FAILED           1018
#define EX_RESULT_DESTROY_FAILED        1019
#define EX_RESULT_VERIFY_FAILED         1020
#define EX_RESULT_NO_CHANGE             1021

// thread, buffer
#define EX_RESULT_LOCK_FAILED           1022

// graphics
#define EX_RESULT_RENDER_PAUSED         1023   
#define EX_RESULT_DEVICE_LOST           1024
#define EX_RESULT_DEVICE_RESET_FAILED   1025

// engine

// code process
#define EX_RESULT_COMPILE_FAILED        1026


// #################################################################################
#endif // END CONSTANTS_H_1277625965
// #################################################################################


