// ======================================================================================
// File         : path_op.h
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 15:21:41 PM | Thursday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef PATH_OP_H_1310628105
#define PATH_OP_H_1310628105
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
//      "assets/foo/bar/foobar.txt" ==> "assets/foo/bar/" 
//      "assets/foo/bar/foobar" ==> "assets/foo/bar/" 
//      "assets/foo/bar/.foobar" ==> "assets/foo/bar/" 
//      "assets/foo/bar/.foobar/" ==> "assets/foo/bar/.foobar/" 
//      "foobar.txt" ==> "" 
//      "foobar" ==> "" 
//      "foobar/" ==> "foobar/" 
// ------------------------------------------------------------------ 

extern void ex_path_get_dir( char _out[MAX_PATH], const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
//      "assets/foo/bar/foobar.txt" ==> "foobar.txt" 
//      "assets/foo/bar/foobar" ==> "foobar" 
//      "assets/foo/bar/.foobar" ==> ".foobar" 
//      "foobar.txt" ==> "foobar.txt" 
//      "foobar" ==> "foobar" 
//      "foobar/" ==> "" 
// ------------------------------------------------------------------ 

extern void ex_path_get_filename( char _out[MAX_PATH], const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
//      "assets/foo/bar/foobar.txt" ==> ".txt" 
//      "assets/foo/bar/foobar" ==> "" 
//      "assets/foo/bar/.foobar" ==> "" 
//      "foobar.txt" ==> ".txt" 
//      "foobar" ==> "" 
//      "foobar/" ==> "" 
// ------------------------------------------------------------------ 

extern void ex_path_get_ext( char _out[MAX_PATH], const char *_path );

// ------------------------------------------------------------------ 
// Desc: 
//      "assets/foo/bar/foobar.txt" ==> "foobar" 
//      "assets/foo/bar/foobar" ==> "foobar" 
//      "assets/foo/bar/.foobar" ==> ".foobar" 
//      "foobar.txt" ==> "foobar" 
//      "foobar" ==> "foobar" 
//      "foobar/" ==> "" 
// ------------------------------------------------------------------ 

extern void ex_path_get_file_without_ext( char _out[MAX_PATH], const char *_path );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END PATH_OP_H_1310628105
// #################################################################################


