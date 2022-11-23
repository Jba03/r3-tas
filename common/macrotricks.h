//
//  macrotricks.h
//  r3lib
//
//  Created by Jba03 on 2022-10-03.
//  Copyright © 2022 Jba03. All rights reserved.
//

#ifndef macrotricks_h
#define macrotricks_h

#include "endianness.h"

#pragma mark - Reversing arguments

#define EXPAND(x) x

#define REVERSE_1(a) a
#define REVERSE_2(a,b) b,a
#define REVERSE_3(a,...) EXPAND(REVERSE_2(__VA_ARGS__)),a
#define REVERSE_4(a,...) EXPAND(REVERSE_3(__VA_ARGS__)),a
#define REVERSE_5(a,...) EXPAND(REVERSE_4(__VA_ARGS__)),a
#define REVERSE_6(a,...) EXPAND(REVERSE_5(__VA_ARGS__)),a
#define REVERSE_7(a,...) EXPAND(REVERSE_6(__VA_ARGS__)),a
#define REVERSE_8(a,...) EXPAND(REVERSE_7(__VA_ARGS__)),a
#define REVERSE_9(a,...) EXPAND(REVERSE_8(__VA_ARGS__)),a
#define REVERSE_10(a,...) EXPAND(REVERSE_9(__VA_ARGS__)),a
#define REVERSE_11(a,...) EXPAND(REVERSE_10(__VA_ARGS__)),a
#define REVERSE_12(a,...) EXPAND(REVERSE_11(__VA_ARGS__)),a
#define REVERSE_13(a,...) EXPAND(REVERSE_12(__VA_ARGS__)),a
#define REVERSE_14(a,...) EXPAND(REVERSE_13(__VA_ARGS__)),a
#define REVERSE_15(a,...) EXPAND(REVERSE_14(__VA_ARGS__)),a
#define REVERSE_16(a,...) EXPAND(REVERSE_15(__VA_ARGS__)),a
#define REVERSE1(N,...) EXPAND(REVERSE_ ## N(__VA_ARGS__))
#define REVERSE(N, ...) REVERSE1(N, __VA_ARGS__)

#pragma mark - Vector definition

#define VECTOR_XYZ2 x, y
#define VECTOR_XYZ3 x, y, z
#define VECTOR_XYZ4 x, y, z, w

#define VECTOR_CHROMA2 U, V
#define VECTOR_CHROMA3 r, g, b
#define VECTOR_CHROMA4 r, g, b, a

#define VECTOR_STRUCT(T, ...) struct {T __VA_ARGS__;};
#ifdef LSB_FIRST
#   define VECTOR_DEFINITION(T, N) union Vector##N { VECTOR_STRUCT(T, VECTOR_XYZ##N); VECTOR_STRUCT(T, VECTOR_CHROMA##N); T v[N]; address offset; };
#else
#   define VECTOR_DEFINITION(T, N) union Vector##N { VECTOR_STRUCT(T, REVERSE(N, VECTOR_XYZ##N)); VECTOR_STRUCT(T, REVERSE(N,VECTOR_CHROMA##N)); T v[N]; address offset; };
#endif

#pragma mark - Matrix definition

/* Defines a matrix row by n'th column */
#define MAT2_ROW(n) m##n##0, m##n##1
#define MAT3_ROW(n) m##n##0, m##n##1, m##n##2
#define MAT4_ROW(n) m##n##0, m##n##1, m##n##2, m##n##3

#define MAT2(ORDER) ORDER(4, MAT2_ROW(0), MAT2_ROW(1))
#define MAT3(ORDER) ORDER(9, MAT3_ROW(0), MAT3_ROW(1), MAT3_ROW(2))
#define MAT4(ORDER) ORDER(16, MAT4_ROW(0), MAT4_ROW(1), MAT4_ROW(2), MAT4_ROW(3))

#define MAT_ROWS2 row0, row1
#define MAT_ROWS3 row0, row1, row2
#define MAT_ROWS4 row0, row1, row2, row3

#define MATSPEC(N, ORDER) MAT##N(ORDER) /* Selects matrix element set `N` */
#define MAT_STRUCT(T, ...) struct {T __VA_ARGS__;} /* Creates the element structure */
#define DEFAULT_ORDER(N, ...) __VA_ARGS__ /* Default order sort function */

#ifdef LSB_FIRST
#   define MATRIX_DEFINITION(T, N) union Matrix##N { MAT_STRUCT(T, MATSPEC(N, DEFAULT_ORDER)); struct {union Vector##N MAT_ROWS##N; }; struct { T m[N * N]; }; address offset; };
#else
#   define MATRIX_DEFINITION(T, N) union Matrix##N { MAT_STRUCT(T, MATSPEC(N, REVERSE)); struct {union Vector##N MAT_ROWS##N; }; struct { T m[N * N]; }; address offset; };
#endif

#endif /* macrotricks_h */
