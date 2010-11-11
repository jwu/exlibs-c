// ======================================================================================
// File         : math_inc.h
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 08:56:24 AM | Monday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MATH_INC_H_1277686586
#define MATH_INC_H_1277686586
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
//  config macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: 
//  EX_MATRIX_ROW_MAJOR (each row of the matrix is stored in a single constant register)
//  the row major matrix can multiply by row easily, as r1 * vec
//
//   layer out for register pack           layer out for geometry meanning
//
//                                               I  J  K  T
//                                               -- -- -- --
//      r1 | 11 12 13 14 |                       11 12 13 14  
//      r2 | 21 22 23 24 |                       21 22 23 24  
//      r3 | 31 32 33 34 |                       31 32 33 34  
//      r4 | 41 42 43 44 |                       41 42 43 44  
//                                               -- -- -- --
//      
//   multiply vector:
//      mat2 * mat1 * vec 
//
//  EX_MATRIX_COLUMN_MAJOR (each row of the matrix is stored in a single constant register)
//  the column major matrix can multiply by column easily, as vec * r1
//
//   layer out for register pack           layer out for geometry meanning
//
//      r1 r2 r3 r4
//      -- -- -- --                            
//      11 21 31 41                            I | 11 21 31 41 |
//      12 22 32 42                            J | 12 22 32 42 |
//      13 23 33 43                            K | 13 23 33 43 |
//      14 24 34 44                            T | 14 24 34 44 |
//      -- -- -- --
//
//   multiply vector:
//      vec * mat1 * mat2
//
//  Warning:
//      Don't modify this macro, currently the engine just support column major calculate
// ------------------------------------------------------------------

#define EX_MATRIX_COLUMN_MAJOR 1
#define EX_MATRIX_ROW_MAJOR    2

#define EX_MATRIX_ORDER EX_MATRIX_COLUMN_MAJOR

// ------------------------------------------------------------------
// Desc: 
// EX_MULTIPLY_LEFT_TO_RIGHT
//     vRot = vec * mat1 * mat2 * mat3 * ...
//     vRot = vec * quad1 * quad2 * quad3 * ...
//
//     vector express:
//         vec = [ v1, v2, v3, v4, ... ]
//
// EX_MULTIPLY_RIGHT_TO_LEFT
//     vRot = ... * mat3 * mat2 * mat1 * vec
//     vRot = ... * quad3 * quad2 * quad1 * vec
//
//     vector express:
//         vec = [ v1, 
//                 v2, 
//                 v3, 
//                 v4,
//                 ... ]
// ------------------------------------------------------------------

#define EX_MULTIPLY_LEFT_TO_RIGHT 1
#define EX_MULTIPLY_RIGHT_TO_LEFT 2

#if ( EX_MATRIX_ORDER == EX_MATRIX_COLUMN_MAJOR )
    #define EX_MULTIPLY_ORDER EX_MULTIPLY_LEFT_TO_RIGHT
#elif ( EX_MATRIX_ORDER == EX_MATRIX_ROW_MAJOR ) 
    #define EX_MULTIPLY_ORDER EX_MULTIPLY_RIGHT_TO_LEFT
#endif

// ------------------------------------------------------------------
// Desc: 
// EX_COORDINATE_LEFT_HANDED
//    
//          Y 
//           ^   Z
//           | / 
//           |/
//            -----> X      
//
// EX_COORDINATE_RIGHT_HANDED
//
//          Y 
//           ^   
//           |  
//           |
//          / -----> X      
//         /
//       Z
//
// ------------------------------------------------------------------

#define EX_COORDINATE_LEFT_HANDED  1
#define EX_COORDINATE_RIGHT_HANDED 2

#define EX_COORDINATE_SYSTEM EX_COORDINATE_LEFT_HANDED

// ------------------------------------------------------------------
// Desc: 
//  EX_ANGLE_RADIANS
//      [ 0.0 - 2pi ]
//
//  EX_ANGLE_DEGREE
//      [ 0.0 - 360.0 ]
//
//  NOTE:
//      cause most c/cpp math function like sinf,cosf... use radians as input value. 
//      if we use the degree, then we need one more calculation to transform the input.
// ------------------------------------------------------------------

#define EX_ANGLE_RADIANS 1
#define EX_ANGLE_DEGREE  2

#define EX_ANGLE_UNIT EX_ANGLE_RADIANS

// ------------------------------------------------------------------
// Desc: 
//  EX_ROTATE_CW
//      Angles are measured clockwise when looking along the rotation axis toward the "origin".
//      As know as Left-Hand Rotate ( use thumb as axis, other finger as rotation )
//
//  EX_ROTATE_CCW
//      Angles are measured counter clockwise when looking along the rotation axis toward the "origin".
//      As know as Right-Hand Rotate ( use thumb as axis, other finger as rotation )
//
// ------------------------------------------------------------------

#define EX_ROTATE_CW   1
#define EX_ROTATE_CCW  2

#define EX_ROTATE_METHOD EX_ROTATE_CW

// ------------------------------------------------------------------
//            Operation       |  Result
//   -------------------------+------------
//             n / +-infinity |     0
//              +-nonzero / 0 | +-infinity
//    +-infinity * +-infinity | +-infinity
//        infinity + infinity |   infinity
//        infinity - infinity |    NaN
//    +-infinity / +-infinity |    NaN
//             +-infinity * 0 |    NaN
//                  +-0 / +-0 |    NaN
// ------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//  value defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: values about PI 
// ------------------------------------------------------------------

#define EX_PI               (3.1415926535897932)
#define EX_INV_PI			(0.31830988618)
#define EX_TWO_PI           (6.2831853071795864)
#define EX_HALF_PI			(1.57079632679)

// ------------------------------------------------------------------
// Desc: degree/radius convertor 
// ------------------------------------------------------------------

#define EX_DEG_TO_RAD       (0.017453292519943296) /* pi/180.0 */
#define EX_RAD_TO_DEG       (57.29577951308232158) /* 180.0/pi */

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "mathop.h"
#include "vec2f.h"
#include "vec3f.h"

// #################################################################################
#endif // END MATH_INC_H_1277686586
// #################################################################################


