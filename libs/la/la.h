/*
 * Библиотека функций линейной алгебры (операции с векторами/матрицами)
 * Version: 0.7a
 * File: "la.h"
 * Coding: UTF8
 * (C) 2007 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2008.02.01
 */

#ifndef LA_H
#define LA_H

#include <stdlib.h> // malloc(), free()
#include <stdio.h>  // fprintf(), fopen(), fclose(), fwrite()

// inline macro
#ifndef LA_INLINE
#  if __GNUC__
#    define LA_INLINE static inline
#  else
#    define LA_INLINE inline
#  endif
#endif // LA_INLINE

// использовать `long double`, а не `float` или `double`
//#define LA_LONG_DOUBLE

// использовать `float`, а не `double`
//#define LA_FLOAT

// опция отладочной печати на stderr
//#define LA_DEBUG

// опция отладки для поиска утечек памяти
//#define LA_MEMORY_DEBUG

// опция дополнительных проверок
//#define LA_PARANOIC

// вызывать функцию exit() при критических ошибках
//#define LA_USE_EXIT

#ifdef LA_DEBUG
#  ifdef LA_WIN32
#    define LA_DBG(fmt, ...) fprintf(stderr, "LA: " fmt "\n", __VA_ARGS__)
#  else
#    define LA_DBG(fmt, arg...) fprintf(stderr, "LA: " fmt "\n", ## arg)
#  endif
#else
#  ifdef LA_WIN32
#    define LA_DBG(fmt, ...)
#  else
#    define LA_DBG(fmt, arg...)
#  endif
#endif // LA_DEBUG

// меньшее и большее из двух чисел
#define LA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define LA_MAX(a, b) ((a) > (b) ? (a) : (b))

// ограничение параметра по абсолютной величине
#define LA_LIMIT_ABS(x, max) \
  ((x) > (max) ? (max) : ((x) < -(max) ? -(max) : (x)))

// ограничение параметра сверху и снизу
#define LA_LIMIT(x, min, max) \
  ((x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

// перестановка 2-х `la_float_t` значений
#define LA_FLOAT_SWAP(a, b) {la_float_t tmp = (a); (a) = (b); (b) = tmp;}

// вычисление определителя матрицы 2x2
#define LA_MATRIX_DET_2X2(a00, a01, \
                          a10, a11) ((a00) * (a11) - (a01) * (a10))

// вычисление определителя матрицы 3x3
#define LA_MATRIX_DET_3X3(a00, a01, a02, \
                          a10, a11, a12, \
                          a20, a21, a22) \
( \
  (a00) * ((a11) * (a22) - (a12) * (a21)) + \
  (a01) * ((a12) * (a20) - (a10) * (a22)) + \
  (a02) * ((a10) * (a21) - (a11) * (a20))   \
)

// ***********
// типы данных
// ***********

// type of integer value
typedef int la_int_t;

// type of float value
#if defined(LA_LONG_DOUBLE)
typedef long double la_float_t;
#elif defined(LA_FLOAT)
typedef float la_float_t;
#else
typedef double la_float_t; // by default
#endif

// типы для сохранения в BMP
typedef unsigned char  la_uint8_t;
typedef unsigned short la_uint16_t;
typedef unsigned long  la_uint32_t;

// type of integer vector (тип вектора целых чисел)
typedef struct {
  int size;    // vector size (размер вектора)
  la_int_t *d; // vector data [size]
} la_ivector_t;

// type of vector (тип вектора вещественных чисел)
typedef struct {
  int size;      // vector size (размер вектора)
  la_float_t *d; // vector data [size]
} la_vector_t;

// type of matrix (тип матрицы вещественных чисел)
typedef struct {
  int nrow;       // number of row (число строк)
  int ncol;       // number of column (число столбцов)
  la_float_t **d; // matrix data (указатели на начало строк)
} la_matrix_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// malloc()/free() wrappers
LA_INLINE void *la_malloc(int size) { return (void*) malloc(size); }
LA_INLINE void la_free(void *p) { free(p); }

// дать информацию о размере выделенной памяти
#ifdef LA_MEMORY_DEBUG
int la_get_memory_count();
#else // LA_MEMORY_DEBUG
LA_INLINE int la_get_memory_count() { return 0; }
#endif // LA_MEMORY_DEBUG

// ********************
// операции с векторами
// ********************

// инициализация вектора (выделение памяти)
void la_ivector_init(la_ivector_t *iv, int size);
void la_vector_init(la_vector_t *v, int size);
la_vector_t la_vector_new(int size);

// инициализация вектора на основе массива
la_vector_t la_vector_create(const la_float_t *d, int size);

// деинициализация вектора (освобождение памяти)
void la_ivector_free(la_ivector_t *iv);
void la_vector_free(la_vector_t *v);

// изменение размера вектора
void la_vector_resize(la_vector_t *v, int new_size);

// вернуть размер вектора
LA_INLINE int la_vector_get_size(const la_vector_t *v) { return v->size; }
LA_INLINE int la_ivector_get_size(const la_ivector_t *v) { return v->size; }

// вернуть указатель на данные вектора
LA_INLINE la_float_t *la_vector_data(const la_vector_t *v) { return v->d; }
LA_INLINE la_int_t *la_ivector_data(const la_ivector_t *v) { return v->d; }

// создание копии вектора
la_vector_t la_vector_copy(const la_vector_t *source);

// сложение векторов
la_vector_t la_vector_sum(const la_vector_t *v1, const la_vector_t *v2);

// вычитание векторов (V1 - V2)
la_vector_t la_vector_sub(const la_vector_t *v1, const la_vector_t *v2);

// модификация вектора v1
void la_vector_inc(la_vector_t *v1, const la_vector_t *v2); // v1 += v2
void la_vector_dec(la_vector_t *v1, const la_vector_t *v2); // v1 -= v2

// весовое сложение векторов (C1 * V1 + C2 * V2)
la_vector_t la_vector_csum(const la_vector_t *v1, la_float_t c1,
                           const la_vector_t *v2, la_float_t c2);

// умножение вектора на вещественное число
la_vector_t la_vector_cmpy(const la_vector_t *v, la_float_t c);

// скалярное умножение векторов
la_float_t la_vector_mpy(const la_vector_t *v1, const la_vector_t *v2);

// косое умножение векторов (V1 x V2) размера 2 (в 2d пространстве)
la_float_t la_vector_vmpy_2d(const la_vector_t *v1, const la_vector_t *v2);

// векторное умножение векторов (V1 x V2) размера 3 (в 3d пространстве)
la_vector_t la_vector_vmpy_3d(const la_vector_t *v1, const la_vector_t *v2);

// вычисление квадрата модуля вектора
la_float_t la_vector_module2(const la_vector_t *v);

// вычисление модуля вектора
la_float_t la_vector_module(const la_vector_t *v);

// заполнение вектора
void la_vector_fill(la_vector_t *v, la_float_t c);

// печать вектора в файл
void la_vector_print(const la_vector_t *v, FILE *f);

// ********************
// операции с матрицами
// ********************

// инициализация матрицы (выделение памяти)
void la_matrix_init(la_matrix_t *m, int nrow, int ncol);
la_matrix_t la_matrix_new(int nrow, int ncol);

// инициализация матрицы на основе массива
la_matrix_t la_matrix_create(const la_float_t *d, int nrow, int ncol);

// инициализация диагональной матрицы на основе массива
la_matrix_t la_matrix_diag(const la_float_t *d, int size);

// деинициализация матрицы (освобождение памяти)
void la_matrix_free(la_matrix_t *m);

// создание копии матрицы
la_matrix_t la_matrix_copy(const la_matrix_t *source);

// вернуть число строк матрицы
LA_INLINE int la_matrix_get_nrow(const la_matrix_t *m) { return m->nrow; }

// вернуть число солбцов матрицы
LA_INLINE int la_matrix_get_ncol(const la_matrix_t *m) { return m->ncol; }

// вернуть указатель на начало строки матрицы [0 <= row < nrow ] 
LA_INLINE la_float_t *la_matrix_data(const la_matrix_t *m, int row)
{ return m->d[row]; }

// вернуть сторку матрицы
la_vector_t la_matrix_get_row(const la_matrix_t *m, int i);

// вернуть столбец матрицы
la_vector_t la_matrix_get_col(const la_matrix_t *m, int j);

// вернуть значение матрици в виде вектора
la_vector_t la_matrix_as_vector(const la_matrix_t *m);

// сложение матриц
la_matrix_t la_matrix_sum(const la_matrix_t *m1, const la_matrix_t *m2);

// вычитание матриц (M1 - M2)
la_matrix_t la_matrix_sub(const la_matrix_t *m1, const la_matrix_t *m2);

// весовое сложение матриц (C1 * M1 + C2 * M2)
la_matrix_t la_matrix_csum(const la_matrix_t *m1, la_float_t c1,
                           const la_matrix_t *m2, la_float_t c2);

// умножение матрицы на вещественное число
la_matrix_t la_matrix_cmpy(const la_matrix_t *m, la_float_t c);

// умножение матриц (M1 x M2)
la_matrix_t la_matrix_mpy(const la_matrix_t *m1, const la_matrix_t *m2);

// заполнение матрицы
void la_matrix_fill(la_matrix_t *m, la_float_t c);

// транспонирование матрицы
la_matrix_t la_matrix_trans(const la_matrix_t *m);

// перестановка строк матрицы (перестановки указателей, не данных)
void la_matrix_swap_row(la_matrix_t *m, int i1, int i2);

// перестановка столбцов матрицы (перестановка данных)
void la_matrix_swap_col(la_matrix_t *m, int j1, int j2);

// вычисление определителя матрицы (вариант 1)
la_float_t la_matrix_det_v1(const la_matrix_t *m);

// вычисление определителя матрицы (вариант 2)
la_float_t la_matrix_det_v2(const la_matrix_t *m);

// вычисление определителя матрицы (используемый вариант)
LA_INLINE la_float_t la_matrix_det(const la_matrix_t *m)
{
  return la_matrix_det_v2(m);
}

// вычисление ранга матрицы
// (возвращается число линейно независимых строк)
int la_matrix_rang(const la_matrix_t *m);

// обращение матрицы 3x3
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_3x3(const la_matrix_t *m, int *err);

// обращение матрицы (вариант 1)
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_v1(const la_matrix_t *m, int *err);

// обращение матрицы (вариант 2)
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_v2(const la_matrix_t *m, int *err);

// обращение матрицы (используемый вариант)
// (в случае успеха возвращается err=0, ошибки - err<0)
LA_INLINE la_matrix_t la_matrix_inverse(const la_matrix_t *m, int *err)
{
  return la_matrix_inverse_v1(m, err);
}

// печать матрицы в файл
void la_matrix_print(const la_matrix_t *m, FILE *f);

// сохранение матрицы в виде BMP файла черно-белого изображения
// (в случае успеха возвращается 0, в случае ошибки < 0)
int la_matrix_bmp_save(
  const la_matrix_t *m, // указатель на матрицу
  const char *fname);   // имя BMP файла для сохранения

// ********************************
// операции с векторами и матрицами
// ********************************

// линейное преобразование вектора (M x V)
la_vector_t la_transform(const la_matrix_t *m, const la_vector_t *v);

// обратное линейное преобразование вектора (M^(-1) x V)
la_vector_t la_reform(const la_matrix_t *m, const la_vector_t *v);

// **************************************************************
// функции для решения переопределенных систем линейных уравнений
// **************************************************************

// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия без учета отличия дисперсии шумов правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse_simple(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  int *err);            // код ошибки (при ощибке <0, при успехе 0)

// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия с учетом разных шумов правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  const la_vector_t *n, // дисперсия шумов правых частей Y [m]
  int *err);            // код ошибки (при ощибке <0, при успехе 0)

// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия с учетом корреляционной матрицы правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse_full(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  const la_matrix_t *n, // корреляционная матрицы правых частей Y [m][m]
  int *err);            // код ошибки (при ошибке <0, при успехе 0)

// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия без учета отличия дисперсии шумов правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd_simple(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  int *err);            // код ошибки (при ошибке <0, при успехе 0)

// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия с учетом разных шумов правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  const la_vector_t *n, // дисперсия шумов правых частей Y [m]
                        // (диагональ корреляционной матрицы `y`)
  int *err);            // код ошибки (при ошибке <0, при успехе 0)

// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия с учетом корреляционной матрицы правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd_full(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  const la_matrix_t *n, // корреляционная матрица правых частей Y [m][m]
  int *err);            // код ошибки (при ошибке <0, при успехе 0)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LA_H

/*** end of "la.h" file ***/
