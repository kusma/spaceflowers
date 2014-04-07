#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"

typedef float matrix[16];

void matrix_identity( matrix m );
void matrix_rotate( matrix m, vector v );
void matrix_translate( matrix m, vector v );
vector matrix_get_translation( matrix m );
void matrix_scale( matrix m, vector v );
void matrix_lookat( matrix m, vector position, vector target, float roll );

void matrix_multiply(matrix m1, matrix m2);
vector matrix_transformvector( matrix m, vector v );
vector matrix_rotatevector( matrix m, vector v );
void matrix_transponate( matrix target, matrix source );
vector matrix_inverserotatevector( matrix m, vector v );

#endif /* __MATRIX_H__ */