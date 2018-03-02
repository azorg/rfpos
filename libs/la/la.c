/*
 * Библиотека функций линейной алгебры (операции с векторами/матрицами)
 * Version: 0.7a
 * File: "la.h"
 * Coding: UTF8
 * (C) 2007 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2008.02.01
 */

#include "la.h"
#include <math.h> // sqrt(), fabs()

//----------------------------------------------------------------------------
#ifdef LA_MEMORY_DEBUG
static int la_memory_count = 0; // глобальный счетчик выделенной памяти
#endif // LA_MEMORY_DEBUG
//----------------------------------------------------------------------------
#ifdef LA_MEMORY_DEBUG
// дать информацию о размере выделенной памяти
int la_get_memory_count()
{
  return la_memory_count;
}
#endif // LA_MEMORY_DEBUG
//----------------------------------------------------------------------------
// инициализация вектора целых чисел (выделение памяти)
void la_ivector_init(la_ivector_t *iv, int size)
{
  iv->size = size;
  size *= sizeof(la_int_t);
  iv->d = (la_int_t*) la_malloc(size);
  if (!iv->d)
  {
    LA_DBG("error: la_malloc(%i) return NULL in la_ivector_init()", size);
#ifdef LA_USE_EXIT
    exit(1);
#else
    return;
#endif // LA_USE_EXIT
  }
#ifdef LA_MEMORY_DEBUG
  la_memory_count += size;
#endif
}
//----------------------------------------------------------------------------
// инициализация вектора (выделение памяти)
void la_vector_init(la_vector_t *v, int size)
{
  v->size = size;
  size *= sizeof(la_float_t);
  v->d = (la_float_t*) la_malloc(size);
  if (!v->d)
  {
    LA_DBG("error: la_malloc(%i) return NULL in la_vector_init()", size);
#ifdef LA_USE_EXIT
    exit(1);
#else
    return;
#endif // LA_USE_EXIT

  }
#ifdef LA_MEMORY_DEBUG
  la_memory_count += size;
#endif
}
//----------------------------------------------------------------------------
// инициализация вектора (выделение памяти)
la_vector_t la_vector_new(int size)
{
  la_vector_t v;
  la_vector_init(&v, size);
  return v;
}
//----------------------------------------------------------------------------
// инициализация вектора на основе массива
la_vector_t la_vector_create(const la_float_t *d, int size)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, size);
  for (i = 0; i < size; i++)
    v.d[i] = d[i];
  return v;
}
//----------------------------------------------------------------------------
// деинициализация вектора целых чисел (освобождение памяти)
void la_ivector_free(la_ivector_t *iv)
{
  if (iv->size > 0)
  {
    la_free((void*) iv->d);
#ifdef LA_MEMORY_DEBUG
    la_memory_count -= iv->size * sizeof(la_int_t);
#endif
  }
}
//----------------------------------------------------------------------------
// деинициализация вектора (освобождение памяти)
void la_vector_free(la_vector_t *v)
{
  if (v->size > 0)
  {
    la_free((void*) v->d);
#ifdef LA_MEMORY_DEBUG
    la_memory_count -= v->size * sizeof(la_float_t);
#endif
  }
}
//----------------------------------------------------------------------------
// изменение размера вектора
void la_vector_resize(la_vector_t *v, int new_size)
{
  if (new_size != v->size)
  {
    int n = LA_MIN(new_size, v->size);
    int i = new_size * sizeof(la_float_t);
    la_float_t *d = (la_float_t*) la_malloc(i);
    if (!d)
    {
      LA_DBG("error: la_malloc(%i) return NULL in la_vector_resize()", i);
#ifdef LA_USE_EXIT
      exit(1);
#else
      return;
#endif // LA_USE_EXIT
    }
    for (i = 0; i < n; i++)
      d[i] = v->d[i];
    if (v->size > 0)
      la_free((void*) v->d);
    v->d = d;
#ifdef LA_MEMORY_DEBUG
    la_memory_count += (new_size - v->size) * sizeof(la_float_t);
#endif
    v->size = new_size;
  }
}
//----------------------------------------------------------------------------
// создание копии вектора
la_vector_t la_vector_copy(const la_vector_t *source)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, source->size);
  for (i = 0; i < v.size; i++)
    v.d[i] = source->d[i];
  return v;
}
//----------------------------------------------------------------------------
// сложение векторов
la_vector_t la_vector_sum(const la_vector_t *v1, const la_vector_t *v2)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, LA_MIN(v1->size, v2->size));
  for (i = 0; i < v.size; i++)
    v.d[i] = v1->d[i] + v2->d[i];
  return v;
}
//----------------------------------------------------------------------------
// вычитание векторов (V1 - V2)
la_vector_t la_vector_sub(const la_vector_t *v1, const la_vector_t *v2)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, LA_MIN(v1->size, v2->size));
  for (i = 0; i < v.size; i++)
    v.d[i] = v1->d[i] - v2->d[i];
  return v;
}
//----------------------------------------------------------------------------
// v1 += v2
void la_vector_inc(la_vector_t *v1, const la_vector_t *v2)
{
  int i, n = LA_MIN(v1->size, v2->size);
  for (i = 0; i < n; i++)
    v1->d[i] += v2->d[i];
}
//----------------------------------------------------------------------------
// v1 -= v2
void la_vector_dec(la_vector_t *v1, const la_vector_t *v2)
{
  int i, n = LA_MIN(v1->size, v2->size);
  for (i = 0; i < n; i++)
    v1->d[i] -= v2->d[i];
}
//----------------------------------------------------------------------------
// весовое сложение векторов (C1 * V1 + C2 * V2)
la_vector_t la_vector_csum(const la_vector_t *v1, la_float_t c1,
                           const la_vector_t *v2, la_float_t c2)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, LA_MIN(v1->size, v2->size));
  for (i = 0; i < v.size; i++)
    v.d[i] = c1 * v1->d[i] + c2 * v2->d[i];
  return v;
}
//----------------------------------------------------------------------------
// умножение вектора на вещественное число
la_vector_t la_vector_cmpy(const la_vector_t *v, la_float_t c)
{
  int i;
  la_vector_t u;
  la_vector_init(&u, v->size);
  for (i = 0; i < u.size; i++)
    u.d[i] = v->d[i] * c;
  return u;
}
//----------------------------------------------------------------------------
// скалярное умножение векторов
la_float_t la_vector_mpy(const la_vector_t *v1, const la_vector_t *v2)
{
  la_float_t s = 0.;
  int i, n = LA_MIN(v1->size, v2->size);
  for (i = 0; i < n; i++)
    s += v1->d[i] * v2->d[i];
  return s;
}
//----------------------------------------------------------------------------
// косое умножение векторов (V1 x V2) размера 2 (в 2d пространстве)
la_float_t la_vector_vmpy_2d(const la_vector_t *v1, const la_vector_t *v2)
{
#ifdef LA_PARANOIC
  if (v1->size < 2 || v2->size < 2)
  {
    LA_DBG("error: bad arguments in la_vector_vmpy_2d()");
    return 0.;
  }
#endif // LA_PARANOIC
  return v1->d[0] * v2->d[1] - v1->d[1] * v2->d[0];
}
//----------------------------------------------------------------------------
// векторное умножение векторов (V1 x V2) размера 3 (в 3d пространстве)
la_vector_t la_vector_vmpy_3d(const la_vector_t *v1, const la_vector_t *v2)
{
  la_vector_t v;
  la_vector_init(&v, 3);
#ifdef LA_PARANOIC
  if (v1->size < 3 || v2->size < 3)
  {
    la_vector_fill(&v, 0.);
    LA_DBG("error: bad arguments in la_vector_vmpy_3d()");
    return v;
  }
#endif // LA_PARANOIC
  v.d[2] = v1->d[0] * v2->d[1] - v1->d[1] * v2->d[0];
  v.d[0] = v1->d[1] * v2->d[2] - v1->d[2] * v2->d[1];
  v.d[1] = v1->d[2] * v2->d[0] - v1->d[0] * v2->d[2];
  return v;
}
//----------------------------------------------------------------------------
// вычисление квадрата модуля вектора
la_float_t la_vector_module2(const la_vector_t *v)
{
  int i;
  la_float_t s = 0.;
  for (i = 0; i < v->size; i++)
    s += v->d[i] * v->d[i];
  return s;
}
//----------------------------------------------------------------------------
// вычисление модуля вектора
la_float_t la_vector_module(const la_vector_t *v)
{
  return (la_float_t) sqrt(la_vector_module2(v));
}
//----------------------------------------------------------------------------
// заполнение вектора
void la_vector_fill(la_vector_t *v, la_float_t c)
{
  int i;
  for (i = 0; i < v->size; i++)
    v->d[i] = c;
}
//----------------------------------------------------------------------------
// печать вектора в файл
void la_vector_print(const la_vector_t *v, FILE *f)
{
  int i;
  fprintf(f, "[ ");
  for (i = 0; i < (v->size - 1); i++)
    fprintf(f, "%9.4g ", (double) v->d[i]);
  fprintf(f, "%9.4g ]\n", (double) v->d[i]);
}
//----------------------------------------------------------------------------
// инициализация матрицы (выделение памяти)
void la_matrix_init(la_matrix_t *m, int nrow, int ncol)
{
  int i = (nrow + 1) * sizeof(la_float_t*);
  int j = nrow * ncol * sizeof(la_float_t);
  la_float_t *ptr;
  m->nrow = nrow;
  m->ncol = ncol;
  m->d = (la_float_t**) la_malloc(i);
  if (!m->d)
  {
    LA_DBG("error: malloc(%i) return NULL in la_matrix_init() (#1)", i);
#ifdef LA_USE_EXIT
    exit(1);
#else
    return;
#endif // LA_USE_EXIT
  }
  m->d[nrow] = (la_float_t*) la_malloc(j);
  if (!m->d[nrow])
  {
    LA_DBG("error: malloc(%i) return NULL in la_matrix_init() (#2)", j);
#ifdef LA_USE_EXIT
    exit(1);
#else
    return;
#endif // LA_USE_EXIT
  }
#ifdef LA_MEMORY_DEBUG
  la_memory_count += i + j;
#endif
  ptr = m->d[nrow]; // pointer to data
  for (i = 0; i < nrow; i++)
    m->d[i] = &ptr[i * ncol];
}
//----------------------------------------------------------------------------
// инициализация матрицы (выделение памяти)
la_matrix_t la_matrix_new(int nrow, int ncol)
{
  la_matrix_t m;
  la_matrix_init(&m, nrow, ncol);
  return m;
}
//----------------------------------------------------------------------------
// инициализация матрицы на основе массива
la_matrix_t la_matrix_create(const la_float_t *d, int nrow, int ncol)
{
  int i, j;
  la_matrix_t m;
  la_matrix_init(&m, nrow, ncol);
  for (i = 0; i < nrow; i++)
    for (j = 0; j < ncol; j++)
      m.d[i][j] = *d++;
  return m;
}
//----------------------------------------------------------------------------
// инициализация диагональной матрицы на основе массива
la_matrix_t la_matrix_diag(const la_float_t *d, int size)
{
  int i, j;
  la_matrix_t m;
  la_matrix_init(&m, size, size);
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
      m.d[i][j] = 0.;
    m.d[i][i] = *d++;
  }
  return m;
}
//----------------------------------------------------------------------------
// деинициализация матрицы (освобождение памяти)
void la_matrix_free(la_matrix_t *m)
{
#ifdef LA_MEMORY_DEBUG
  la_memory_count -= (m->nrow + 1) * sizeof(la_float_t*) + 
                      m->nrow * m->ncol * sizeof(la_float_t);
#endif
  la_free((void*) m->d[m->nrow]);
  la_free((void*) m->d);
}
//----------------------------------------------------------------------------
// создание копии матрицы
la_matrix_t la_matrix_copy(const la_matrix_t *source)
{
  int i, j;
  la_matrix_t m;
  la_matrix_init(&m, source->nrow, source->ncol);
  for (i = 0; i < m.nrow; i++)
    for (j = 0; j < m.ncol; j++)
      m.d[i][j] = source->d[i][j];
  return m;
}
//----------------------------------------------------------------------------
// вернуть сторку матрицы
la_vector_t la_matrix_get_row(const la_matrix_t *m, int i)
{
  int j;
  la_vector_t v;
  la_vector_init(&v, m->ncol);
  for (j = 0; j < v.size; j++)
    v.d[j] = m->d[i][j];
  return v;
}
//----------------------------------------------------------------------------
// вернуть столбец матрицы
la_vector_t la_matrix_get_col(const la_matrix_t *m, int j)
{
  int i;
  la_vector_t v;
  la_vector_init(&v, m->nrow);
  for (i = 0; i < v.size; i++)
    v.d[i] = m->d[i][j];
  return v;
}
//----------------------------------------------------------------------------
// вернуть значение матрици в виде вектора
la_vector_t la_matrix_as_vector(const la_matrix_t *m)
{
  int i, j, k;
  
  la_vector_t v;
  la_vector_init(&v, m->nrow * m->ncol);
  
  k = 0;
  for (i = 0; i < m->nrow; i++)
    for (j = 0; j < m->ncol; j++)
      v.d[k++] = m->d[i][j];
  
  return v;
}
//----------------------------------------------------------------------------
// сложение матриц
la_matrix_t la_matrix_sum(const la_matrix_t *m1, const la_matrix_t *m2)
{
  int i, j;
  int nrow = LA_MIN(m1->nrow, m2->nrow);
  int ncol = LA_MIN(m1->ncol, m2->ncol);
  la_matrix_t m;
  la_matrix_init(&m, nrow, ncol);
  for (i = 0; i < nrow; i++)
    for (j = 0; j < ncol; j++)
      m.d[i][j] = m1->d[i][j] + m2->d[i][j];
  return m;
}
//----------------------------------------------------------------------------
// вычитание матриц (M1 - M2)
la_matrix_t la_matrix_sub(const la_matrix_t *m1, const la_matrix_t *m2)
{
  int i, j;
  int nrow = LA_MIN(m1->nrow, m2->nrow);
  int ncol = LA_MIN(m1->ncol, m2->ncol);
  la_matrix_t m;
  la_matrix_init(&m, nrow, ncol);
  for (i = 0; i < nrow; i++)
    for (j = 0; j < ncol; j++)
      m.d[i][j] = m1->d[i][j] - m2->d[i][j];
  return m;
}
//----------------------------------------------------------------------------
// весовое сложение матриц (C1 * M1 + C2 * M2)
la_matrix_t la_matrix_csum(const la_matrix_t *m1, la_float_t c1,
                           const la_matrix_t *m2, la_float_t c2)
{
  int i, j;
  int nrow = LA_MIN(m1->nrow, m2->nrow);
  int ncol = LA_MIN(m1->ncol, m2->ncol);
  la_matrix_t m;
  la_matrix_init(&m, nrow, ncol);
  for (i = 0; i < nrow; i++)
    for (j = 0; j < ncol; j++)
      m.d[i][j] = c1 * m1->d[i][j] + c2 * m2->d[i][j];
  return m;
}

//----------------------------------------------------------------------------
// умножение матрицы на вещественное число
la_matrix_t la_matrix_cmpy(const la_matrix_t *m, la_float_t c)
{
  int i, j;
  la_matrix_t a;
  la_matrix_init(&a, m->nrow, m->ncol);
  for (i = 0; i < a.nrow; i++)
    for (j = 0; j < a.ncol; j++)
      a.d[i][j] = m->d[i][j] * c;
  return a;
}
//----------------------------------------------------------------------------
// умножение матриц (M1 x M2)
la_matrix_t la_matrix_mpy(const la_matrix_t *m1, const la_matrix_t *m2)
{
  int i, j, k;
  la_matrix_t m;
  la_matrix_init(&m, m1->nrow, m2->ncol);
  int n = LA_MIN(m1->ncol, m2->nrow);
  for (i = 0; i < m.nrow; i++)
  {
    for (j = 0; j < m.ncol; j++)
    {
      la_float_t s = 0.;
      for (k = 0; k < n; k++)
	s += m1->d[i][k] * m2->d[k][j];
      m.d[i][j] = s;
    }
  }
  return m;
}
//----------------------------------------------------------------------------
// заполнение матрицы
void la_matrix_fill(la_matrix_t *m, la_float_t c)
{
  int i, j;
  for (i = 0; i < m->nrow; i++)
    for (j = 0; j < m->ncol; j++)
      m->d[i][j] = c;
}
//----------------------------------------------------------------------------
// вернуть матрицу полученную из исходной "вычеркиванием"
// i-ой строки и j-го столбца
la_matrix_t la_matrix_minor(const la_matrix_t *m, int i, int j)
{
  la_matrix_t a;
  int x, y;
#ifdef LA_PARANOIC
  if (i < 0 || i >= m->nrow || j < 0 || j >= m->ncol ||
      m->nrow <= 1 || m->ncol <= 1)
  {
    LA_DBG("error: bad arguments in la_matrix_minor()");
    return la_matrix_copy(m);
  }
#endif // LA_PARANOIC
  la_matrix_init(&a, m->nrow - 1, m->ncol - 1);
  for (x = 0; x < i; x++)
  {
    for (y = 0; y < j; y++)
      a.d[x][y] = m->d[x][y];
    for (y = j; y < a.ncol; y++)
      a.d[x][y] = m->d[x][y + 1];
  }
  for (x = i; x < a.nrow; x++)
  {
    for (y = 0; y < j; y++)
      a.d[x][y] = m->d[x + 1][y];
    for (y = j; y < a.ncol; y++)
      a.d[x][y] = m->d[x + 1][y + 1];
  }
  return a;
}
//----------------------------------------------------------------------------
// транспонирование матрицы
la_matrix_t la_matrix_trans(const la_matrix_t *m)
{
  int i, j;
  la_matrix_t a;
  la_matrix_init(&a, m->ncol, m->nrow);
  for (i = 0; i < a.nrow; i++)
    for (j = 0; j < a.ncol; j++)
      a.d[i][j] = m->d[j][i];
  return a;
}
//----------------------------------------------------------------------------
// перестановка строк матрицы (перестановки указателей, не данных)
void la_matrix_swap_row(la_matrix_t *m, int i1, int i2)
{
  la_float_t *tmp = m->d[i1];
  m->d[i1] = m->d[i2];
  m->d[i2] = tmp;
}
//----------------------------------------------------------------------------
// перестановка столбцов матрицы (перестановка данных)
void la_matrix_swap_col(la_matrix_t *m, int j1, int j2)
{
  int i;
  for (i = 0; i < m->nrow; i++)
    LA_FLOAT_SWAP(m->d[i][j1], m->d[i][j2]);
}
//----------------------------------------------------------------------------
// вычисление определителя матрицы (вариант 1)
la_float_t la_matrix_det_v1(const la_matrix_t *m)
{
  int n = LA_MIN(m->nrow, m->ncol);
  if (n > 3)
  { // рекурентный алгоритм (разложение по первой строке)
    int i, j, k;
    la_float_t det = 0., sig = 1.;
    la_matrix_t a;
    n--;
    la_matrix_init(&a, n, n);
    for (k = 0; k <= n; k++)
    {
      for (j = 0; j < k; j++)
	for (i = 1; i <= n; i++)
	  a.d[i - 1][j] = m->d[i][j];
      for (j = k + 1; j <= n; j++)
	for (i = 1; i <= n; i++)
	  a.d[i - 1][j - 1] = m->d[i][j];
      det += sig * m->d[0][k] * la_matrix_det(&a);
      sig = -sig;
    }
    la_matrix_free(&a);
    return det;
  }
  else if (n == 3)
    return LA_MATRIX_DET_3X3(m->d[0][0], m->d[0][1], m->d[0][2],
                             m->d[1][0], m->d[1][1], m->d[1][2],
                             m->d[2][0], m->d[2][1], m->d[2][2]);
  else if (n == 2)
    return LA_MATRIX_DET_2X2(m->d[0][0], m->d[0][1],
			     m->d[1][0], m->d[1][1]);
  else if (n == 1)
    return m->d[0][0];
  else // if n < 1
  {
    LA_DBG("error: bad argument in la_matrix_det_v1()");
    return 0.; // error
  }
}
//----------------------------------------------------------------------------
// вычисление определителя матрицы (вариант 2)
la_float_t la_matrix_det_v2(const la_matrix_t *m)
{
  int n = LA_MIN(m->nrow, m->ncol);
  if (n > 3)
  {
    int k;
    la_float_t det = 1.;
    la_matrix_t a = la_matrix_copy(m);

    // move along main diagonal
    for (k = 0; k < n; k++)
    {
      // search non-null element in (k,k, n,n)-part of the matrix
      la_float_t tmp;
      int i, j, ni = -1, nj = -1;
      for (i = k; i < n && nj < 0; i++)
	for (j = k; j < n; j++)
	  if (a.d[i][j] != 0.)
	  {
	    ni = i;
	    nj = j;
	    break;
	  }

      if (nj < 0)
      { // no non-null elements
	det = 0.;
	break;
      }

      if (ni != k)
      {
	la_matrix_swap_row(&a, k, ni);
	det = -det;
      }

      if (nj != k)
      {
	la_matrix_swap_col(&a, k, nj);
	det = -det;
      }

      // now a.d[k][k] != 0
      det *= (tmp = a.d[k][k]);
      for (i = k; i < n; i++)
	a.d[i][k] /= tmp;

      // now a.d[k][k] == 1;
      for (j = k + 1; j < n; j++)
      {
	tmp = a.d[k][j];
	for (i = k; i < n; i++)
	  a.d[i][j] -= a.d[i][k] * tmp;
      }
    } // for (k...

    la_matrix_free(&a);
    return det;
  }
  else if (n == 3)
    return LA_MATRIX_DET_3X3(m->d[0][0], m->d[0][1], m->d[0][2],
                             m->d[1][0], m->d[1][1], m->d[1][2],
                             m->d[2][0], m->d[2][1], m->d[2][2]);
  else if (n == 2)
    return LA_MATRIX_DET_2X2(m->d[0][0], m->d[0][1],
			     m->d[1][0], m->d[1][1]);
  else if (n == 1)
    return m->d[0][0];
  else // if n < 1
  {
    LA_DBG("error: bad argument in la_matrix_det_v2()");
    return 0.; // error
  }
}
//----------------------------------------------------------------------------
// вычисление ранга матрицы
// (возвращается число линейно независимых строк)
int la_matrix_rang(const la_matrix_t *m)
{
  la_matrix_t a;
  int k, n = LA_MIN(m->nrow, m->ncol);
#ifdef LA_PARANOIC
  if (n <= 0)
  {
    LA_DBG("error: bad argument in la_matrix_rang()");
    return 0;
  }
#endif // LA_PARANOIC
  if (n == 1)
    return 1;
  a = la_matrix_copy(m);

  // move along main diagonal
  for (k = 0; k < n; k++)
  {
    // search non-null element in (k,k, n,n)-part of the matrix
    int i, j, ni = -1, nj = -1;
    la_float_t tmp;
    for (i = k; i < a.nrow && nj < 0; i++)
      for (j = k; j < a.ncol; j++)
	if (a.d[i][j] != 0.)
	{
	  ni = i;
	  nj = j;
	  break;
	}

    if (nj < 0)
      break; // no non-null elements

    if (ni != k)
      la_matrix_swap_row(&a, k, ni);
    if (nj != k)
      la_matrix_swap_col(&a, k, nj);

    // now a.d[k][k] != 0
    tmp = a.d[k][k];
    for (j = k; j < a.ncol; j++)
      a.d[k][j] /= tmp;

    // now a.d[k][k] == 1;
    // зануляем все элементы стоящие под (k,k)-ым элементом и справа от него
    for (i = k + 1; i < a.nrow; i++)
    {
      tmp = a.d[i][k];
      for (j = k; j < a.ncol; j++)
        a.d[i][j] -= a.d[k][j] * tmp;
    }
    for (j = k + 1; j < a.ncol; j++)
    {
      tmp = a.d[k][j];
      for (i = k; i < a.nrow; i++)
        a.d[i][j] -= a.d[i][k] * tmp;
    }
  } // for (k..

  // считаем сколько единичек на главной диагонали
  for (k = 0; k < n; k++)
    if (a.d[k][k] == 0)
      break;
  if (k == 0) k++;

  la_matrix_free(&a);
  return k;
}
//----------------------------------------------------------------------------
// обращение матрицы 3x3
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_3x3(const la_matrix_t *m, int *err)
{
  la_float_t det;
  la_matrix_t o;
  la_matrix_init(&o, 3, 3);

#ifdef LA_PARANOIC
  if (m->nrow < 3 || m->ncol < 3)
  {
    *err = -1;
    LA_DBG("error: bad argument in la_matrix_inverse_3x3()");
    return o;
  }
#endif // LA_PARANOIC

  det = la_matrix_det(m);
  if (det == 0.)
  {
    *err = -2;
    return o;
  }

  o.d[0][0] = (m->d[1][1] * m->d[2][2] - m->d[2][1] * m->d[1][2]) / det;
  o.d[0][1] = (m->d[2][1] * m->d[0][2] - m->d[0][1] * m->d[2][2]) / det;
  o.d[0][2] = (m->d[0][1] * m->d[1][2] - m->d[1][1] * m->d[0][2]) / det;

  o.d[1][0] = (m->d[2][0] * m->d[1][2] - m->d[1][0] * m->d[2][2]) / det;
  o.d[1][1] = (m->d[0][0] * m->d[2][2] - m->d[2][0] * m->d[0][2]) / det;
  o.d[1][2] = (m->d[1][0] * m->d[0][2] - m->d[0][0] * m->d[1][2]) / det;

  o.d[2][0] = (m->d[1][0] * m->d[2][1] - m->d[2][0] * m->d[1][1]) / det;
  o.d[2][1] = (m->d[2][0] * m->d[0][1] - m->d[0][0] * m->d[2][1]) / det;
  o.d[2][2] = (m->d[0][0] * m->d[1][1] - m->d[1][0] * m->d[0][1]) / det;

  *err = 0;
  return o;
}
//----------------------------------------------------------------------------
// Linear equation solution by Gauss-Jordan elimination. a[0..n-1][0..n-1]
// is the input matrix. b[0..n-1][0..m-1] is input containing the m right-hand
// side vectors. On output, a is replaced by its matrix invers, and b is
// replaced by the corresponding set of solution vectors.
// References: [1] Press, William H., et. al, Numerical Recipes in C,
//                 2nd edition, Cambridge University Press, 1992
static int la_gaussj(la_float_t **a, int n, la_float_t **b, int m)
{
  la_ivector_t indxc, indxr, ipiv;
  int i, icol = 0, irow = 0, j, k, l, ll;
  la_float_t big, dum, pivinv;
  int bexists = ((m != 0) || (b == 0));

  la_ivector_init(&indxc, n); // the integer arrays ipiv, indxr, and
  la_ivector_init(&indxr, n); // indxc are used for pivot bookkeeping
  la_ivector_init(&ipiv,  n);

  for (j = 0; j < n; j++)
    ipiv.d[j] = 0;

  for (i = 0; i < n; i++) // this is the main loop over columns
  {
    big = 0.;
    for (j = 0; j < n; j++) // this is outer loop of pivot search
    {
      if (ipiv.d[j] != 1)
      {
	for (k = 0; k < n; k++)
	{
	  if (ipiv.d[k] == 0)
	  {
            la_float_t mod = fabs(a[j][k]);
	    if (mod >= big)
	    {
	      big = mod;
	      irow = j;
	      icol = k;
	    }
	  }
	  else
	  {
	    if (ipiv.d[k] > 1)
	      goto err;
	  }
	}
      }
    }
    ipiv.d[icol]++;

    // We now have the pivot element, so we interchange rows, if neede,
    // to put the pivot element on the diagonal.  The columns are not
    // physically interchanged, only relabeled: indxc[i], the column of the
    // ith pivot element, is the ith column that is reduced, while indxr[i]
    // is the row in which that pivot element was orignally located. If
    // indxr[i] != indxc[i] there is an implied column interchange.  With
    // this form of bookkeeping, the solution b's will end up in the correct
    // order, and the inverse matrix will be scrambed by columns.
    if (irow != icol)
    {
      for (l = 0; l < n; l++)
        LA_FLOAT_SWAP(a[irow][l], a[icol][l])
      if (bexists)
        for (l = 0; l < m; l++)
	  LA_FLOAT_SWAP(b[irow][l], b[icol][l])
    }

    indxr.d[i] = irow; // we are now ready to divide the pivot row
    indxc.d[i] = icol; // by the pivot element, a[irow][icol]
    if (a[icol][icol] == 0.)
      goto err;
    pivinv = 1. / a[icol][icol];
    a[icol][icol] = 1.;
    for (l = 0; l < n; l++)
      a[icol][l] *= pivinv;
    if (bexists)
      for (l = 0; l < m; l++)
        b[icol][l] *= pivinv;
    for (ll = 0; ll < n; ll++) // next, we reduce the rows...
      if (ll != icol) // .. except for the pivot one
      {
        dum = a[ll][icol];
        a[ll][icol] = 0.;
        for (l = 0; l < n; l++)
	  a[ll][l] -= a[icol][l] * dum;
	if (bexists)
	  for (l = 0; l < m; l++)
	    b[ll][i] -= b[icol][l] * dum;
      }
  }

  // This is the end of the mail loop over columns of the reduction. It
  // only remains to unscrambled the solution in view of the column
  // interchanges. We do this by interchanging pairs of columns in
  // the reverse order that the permutation was built up.
  for (l = n - 1; l >= 0; l--)
  {
    if (indxr.d[l] != indxc.d[l])
    for (k = 0; k < n; k++)
      LA_FLOAT_SWAP(a[k][indxr.d[l]], a[k][indxc.d[l]])
  }

  la_ivector_free(&ipiv);
  la_ivector_free(&indxr);
  la_ivector_free(&indxc);
  return 0; // indicate success

err:
  la_ivector_free(&ipiv);
  la_ivector_free(&indxr);
  la_ivector_free(&indxc);
  return -1; // indicate error
}
//----------------------------------------------------------------------------
// обращение матрицы (вариант 1)
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_v1(const la_matrix_t *m, int *err)
{
  la_matrix_t o = la_matrix_copy(m);
  int n = LA_MIN(o.nrow, o.ncol);
  *err = la_gaussj(o.d, n, (la_float_t**) 0, 0);
  return o;
}
//----------------------------------------------------------------------------
// обращение матрицы (вариант 2)
// (в случае успеха возвращается err=0, ошибки - err<0)
la_matrix_t la_matrix_inverse_v2(const la_matrix_t *m, int *err)
{
  la_matrix_t b, a = la_matrix_copy(m);
  int i, j, k, n = LA_MIN(a.nrow, a.ncol);

#ifdef LA_PARANOIC
  if (n == 0)
  {
    LA_DBG("error: bad argument in la_matrix_inverse()");
    *err = -1;
    return a;
  }
  if (la_matrix_rang(&a) != n)
  {
    LA_DBG("error: bad argument (low rang) in la_matrix_inverse()");
    *err = -2;
    return a;
  }
#endif // LA_PARANOIC

  // инициализируем единичную матрицу (b)
  la_matrix_init(&b, n, n);
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
      b.d[i][j] = 0.;
    b.d[i][i] = 1.;
  }

  // move along main diagonal
  for (k = 0; k < n; k++)
  {
    // search non-null element in (k,k, n,n)-part of the matrix
    int ni = -1, nj = -1;
    la_float_t tmp;
    for (i = k; i < n && nj < 0; i++)
      for (j = k; j < n; j++)
	if (a.d[i][j] != 0.)
	{
	  ni = i;
	  nj = j;
	  break;
	}

    if (nj < 0)
    { // no non-null elements
      la_matrix_free(&a);
      LA_DBG("error: can't inverse matrix in la_matrix_inverse()");
      *err = -3;
      return b;
    }

    if (ni != k)
    {
      la_matrix_swap_row(&a, k, ni);
      la_matrix_swap_row(&b, k, ni);
    }

    if (nj != k)
    {
      la_matrix_swap_col(&a, k, nj);
      la_matrix_swap_col(&b, k, nj);
    }

    // now a.d[k][k] != 0
    tmp = a.d[k][k];
    for (j = k; j < n; j++)
      a.d[k][j] /= tmp;
    for (j = 0; j < n; j++)
      b.d[k][j] /= tmp;

    // now a.d[k][k] == 1;
    // зануляем все элементы стоящие под и над (k, k)-ым элементом матрицы `a`
    for (i = 0; i < n; i++)
    {
      if (i == k) continue;
      tmp = a.d[i][k];
      for (j = 0; j < n; j++)
      {
        a.d[i][j] -= a.d[k][j] * tmp;
        b.d[i][j] -= b.d[k][j] * tmp;
      }
    }
  } // for (k..

  la_matrix_free(&a);
  *err = 0;
  return b;
}
//----------------------------------------------------------------------------
// печать матрицы в файл
void la_matrix_print(const la_matrix_t *m, FILE *f)
{
  int i, j;
  for (i = 0; i < m->nrow; i++)
  {
    fprintf(f, "| ");
    for (j = 0; j < (m->ncol - 1); j++)
      fprintf(f, "%9.4g ", (double) m->d[i][j]);
    fprintf(f, "%9.4g |\n", (double) m->d[i][j]);
  }
}
//----------------------------------------------------------------------------
// сохранение матрицы в виде BMP файла черно-белого изображения
// (в случае успеха возвращается 0, в случае ошибки < 0)
int la_matrix_bmp_save(
  const la_matrix_t *m, // указатель на матрицу
  const char *fname)    // имя BMP файла для сохранения
{
  int real_X, X, Y;
  int hsize, isize, fsize, i, j;
  la_uint8_t *fdata, *ptr;
  la_float_t norm;
  FILE *fo;

  real_X = X = m->ncol;
  Y = m->nrow;

  // учесть, что для BMP ширина изображения должна быть кратна 4
  X = (X + 3) / 4;
  X *= 4;

  // выделяем память под файл:
  hsize = 54;            // размер заголовка BMP файла
  isize = X * Y * 3;     // размер изображения
  fsize = isize + hsize; // размер BMP файла
  ptr = fdata = (la_uint8_t*) la_malloc(fsize * sizeof(la_uint8_t));
#ifdef LA_MEMORY_DEBUG
  la_memory_count += fsize * sizeof(la_uint8_t);
#endif

  if (ptr == (la_uint8_t*) NULL)
    return -1; // ошибка выделения памяти

  // заполняем заголовок
  *((la_uint16_t*)(ptr +  0)) = 0x4D42; // сигнатура "BM"
  *((la_uint32_t*)(ptr +  2)) = fsize;  // размер файла
  *((la_uint16_t*)(ptr +  6)) = 0;  // зарезервировано
  *((la_uint16_t*)(ptr +  8)) = 0;  // зарезервировано
  *((la_uint32_t*)(ptr + 10)) = hsize; // смещение изображения от начала файла
  *((la_uint32_t*)(ptr + 14)) = 40; // длина заголовка (40 = 0x00000028) !!!
  *((la_uint32_t*)(ptr + 18)) = X;  // ширина изображения, точки
  *((la_uint32_t*)(ptr + 22)) = Y;  // высота изображения, точки
  *((la_uint16_t*)(ptr + 26)) = 1;  // число плоскостей 0x0001
  *((la_uint16_t*)(ptr + 28)) = 24; // глубина цвета, бит на точку (24 - truecolor)
  *((la_uint32_t*)(ptr + 30)) = 0;  // тип компрессии (0 - несжатое изображение)
  *((la_uint32_t*)(ptr + 34)) = isize;  // размер изображения, байт (типа Width*Height*BitCount/8)
  *((la_uint32_t*)(ptr + 38)) = 0x0B12; // горизонтальное разрешение, точки на метр (0x0B12)
  *((la_uint32_t*)(ptr + 42)) = 0x0B12; // вертикальное разрешение, точки на метр (0x0B12)
  *((la_uint32_t*)(ptr + 46)) = 0; // число используемых цветов (0 - максимально возможное для данной глубины цвета)
  *((la_uint32_t*)(ptr + 50)) = 0; // число основных цветов (0)

  ptr += hsize;

  // поиск максимума (для нормировки)
  norm = m->d[0][0];
  for (i = 0; i < m->nrow; i++)
    for (j = 0; j < m->ncol; j++)
      if (norm < m->d[i][j])
        norm = m->d[i][j];
  if (norm < 1e-12)
    norm = 1e-12;
  norm = 255. / norm;

  // заполняем данными
  for (i = 0; i < Y; i++)
  { // цикл по X
    la_float_t *data = m->d[i];
    for (j = 0; j < real_X; j++)
    { // цикл по Y
      la_float_t a = *data++ * norm;
      la_uint8_t c = (la_uint8_t) LA_LIMIT(a, 0., 255.);
      *ptr++ = c; // blue
      *ptr++ = c; // green
      *ptr++ = c; // red
    }
    for (; j < X; j++)
    {
      *ptr++ = 0; // blue
      *ptr++ = 0; // green
      *ptr++ = 0; // red
    }
  }

  // сбрасываем в файл
  fo = fopen(fname, "wb");
  if (fo == NULL)
  {
    free(fdata);
#ifdef LA_MEMORY_DEBUG
    la_memory_count -= fsize * sizeof(la_uint8_t);
#endif
    return -2; // невозможно открыть файл на запись
  }
  i = ((int) fwrite(fdata, sizeof(la_uint8_t), fsize, fo) == fsize) ? 0 : -3;

  fclose(fo); // закрываем файл
  la_free(fdata); // освобождаем память
#ifdef LA_MEMORY_DEBUG
  la_memory_count -= fsize * sizeof(la_uint8_t);
#endif

  return i; // 0 - успех, -3 - невозможно записать
}
//----------------------------------------------------------------------------
// линейное преобразование вектора (M x V)
la_vector_t la_transform(const la_matrix_t *m, const la_vector_t *v)
{
  int i, j, n = LA_MIN(m->ncol, v->size);
  la_vector_t u;
  la_vector_init(&u, m->nrow);
  for (i = 0; i < u.size; i++)
  {
    la_float_t s = 0.;
    for (j = 0; j < n; j++)
      s += m->d[i][j] * v->d[j];
    u.d[i] = s;
  }
  return u;
}
//----------------------------------------------------------------------------
// обратное линейное преобразование вектора (M^(-1) x V)
la_vector_t la_reform(const la_matrix_t *m, const la_vector_t *v)
{
  int i, j, n = LA_MIN(m->ncol, v->size);
  la_matrix_t mi = la_matrix_inverse(m, &i);
  la_vector_t u;
  la_vector_init(&u, m->nrow);
  if (i < 0) // inverse error
    la_vector_fill(&u, 0.);
  else
    for (i = 0; i < u.size; i++)
    {
      la_float_t s = 0.;
      for (j = 0; j < n; j++)
	s += mi.d[i][j] * v->d[j];
      u.d[i] = s;
    }
  la_matrix_free(&mi);
  return u;
}
//----------------------------------------------------------------------------
// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия без учета отличия дисперсии шумов правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse_simple(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  int *err)             // код ошибки (при ощибке <0, при успехе 0)
{
  la_matrix_t b, c, d, e;
  int i;

  // вычислить c = a^(T)
  c = la_matrix_trans(a);

  // вычислить d = a^(T) * a
  d = la_matrix_mpy(&c, a);

  // вычислить e = (a^(T) * a)^(-1)
  e = la_matrix_inverse(&d, &i);
  if (i < 0)
  {
    la_matrix_free(&e);
    la_matrix_free(&d);
    la_matrix_fill(&c, 0.);
    LA_DBG("error: la_matrix_inverse return error la_svd_inverse_simple()");
    *err = -2;
    return c;
  }

  // вычислить b = (a^(T) * a)^(-1) * a^(T)
  b = la_matrix_mpy(&e, &c);

  la_matrix_free(&e);
  la_matrix_free(&d);
  la_matrix_free(&c);

  *err = 0;
  return b;
}
//----------------------------------------------------------------------------
// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия с учетом разных шумов правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  const la_vector_t *n, // дисперсия шумов правых частей Y [m]
  int *err)             // код ошибки (при ощибке <0, при успехе 0)
{
  la_matrix_t b, c, d, e;
  int i, j;

#ifdef LA_PARANOIC
  if (a->nrow != n->size)
  {
    LA_DBG("error: bad arguments in la_svd_inverse()");
    *err = -1;
    goto err;
  }
#endif // LA_PARANOIC

  // вычислить c = a^(T) * N^(-1), где N - корреляционная матрица Y
  la_matrix_init(&c, a->ncol, a->nrow);
  for (i = 0; i < c.nrow; i++)
    for (j = 0; j < c.ncol; j++)
      c.d[i][j] = a->d[j][i] / n->d[j];

  // вычислить d = a^(T) * N^(-1) * a, где N - корреляционная матрица Y
  d = la_matrix_mpy(&c, a);

  // вычислить e = (a^(T) * N^(-1) * a)^(-1)
  e = la_matrix_inverse(&d, &i);
  if (i < 0)
  {
    la_matrix_free(&e);
    la_matrix_free(&d);
    la_matrix_free(&c);
    LA_DBG("error: la_matrix_inverse return error la_svd_inverse()");
    *err = -2;
    goto err;
  }

  // вычислить b = (a^(T) * N^(-1) * a)^(-1) * (a^(T) * n^(-1))
  b = la_matrix_mpy(&e, &c);

  la_matrix_free(&e);
  la_matrix_free(&d);
  la_matrix_free(&c);
  *err = 0;
  return b;

err:
  la_matrix_init(&b, a->ncol, a->nrow);
  la_matrix_fill(&b, 0.);
  return b;
}
//----------------------------------------------------------------------------
// вычисление псевдо-обратной матрицы b[nrow=n][ncol=m]
// переопределенной системы линейных уравнений
// (если Y = A x X, найти B при котором X = B x Y)
// Версия с учетом корреляционной матрицы правых частей
// переопределенной системы системы линейных уравнений.
la_matrix_t la_svd_inverse_full(
  const la_matrix_t *a, // входная матрица [nrow=m][ncol=n]
  const la_matrix_t *n, // корреляционная матрицы правых частей Y [m][m]
  int *err)             // код ошибки (при ошибке <0, при успехе 0)
{
  la_matrix_t b, c, d, e, f, g;
  int i;

#ifdef LA_PARANOIC
  if (a->nrow != n->nrow || n->nrow != n->ncol)
  {
    LA_DBG("error: bad arguments in la_svd_inverse_full()");
    *err = -1;
    goto err;
  }
#endif // LA_PARANOIC

  // вычислить f = N^(-1)
  f = la_matrix_inverse(n, &i);
  if (i < 0)
  {
    la_matrix_free(&f);
    LA_DBG("error: la_matrix_inverse() return error in la_svd_inverse_full()");
    *err = -2;
    goto err;
  }

  // вычислить g = a^(T)
  g = la_matrix_trans(a);

  // вычислить c = a^(T) * N^(-1), где N - корреляционная матрица Y
  c = la_matrix_mpy(&g, &f);

  // вычислить d = a^(T) * N^(-1) * a, где N - корреляционная матрица Y
  d = la_matrix_mpy(&c, a);

  // вычислить e = (a^(T) * N^(-1) * a)^(-1)
  e = la_matrix_inverse(&d, &i);
  if (i < 0)
  {
    la_matrix_free(&g);
    la_matrix_free(&f);
    la_matrix_free(&e);
    la_matrix_free(&d);
    la_matrix_free(&c);
    LA_DBG("error: la_matrix_inverse return error la_svd_inverse_full()");
    *err = -3;
    goto err;
  }

  // вычислить b = (a^(T) * N^(-1) * a)^(-1) * (a^(T) * n^(-1))
  b = la_matrix_mpy(&e, &c);

  la_matrix_free(&g);
  la_matrix_free(&f);
  la_matrix_free(&e);
  la_matrix_free(&d);
  la_matrix_free(&c);

  *err = 0;
  return b;

err:
  la_matrix_init(&b, a->ncol, a->nrow);
  la_matrix_fill(&b, 0.);
  return b;
}
//----------------------------------------------------------------------------
// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия без учета отличия дисперсии шумов правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd_simple(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  int *err)             // код ошибки (при ошибке <0, при успехе 0)
{
  la_matrix_t b;
  la_vector_t x;

#ifdef LA_PARANOIC
  if (y->size != a->nrow || a->nrow < a->ncol)
  {
    LA_DBG("error: bad arguments in la_svd_simple()");
    *err = -1;
    goto err;
  }
#endif // LA_PARANOIC

  // вычисление псевдо-обратной матрицы
  b = la_svd_inverse_simple(a, err);
  if (*err < 0)
  {
    la_matrix_free(&b);
    LA_DBG("error: la_svd_inverse_simple() return error in la_svd_simple()");
    *err = -2;
    goto err;
  }

  // вычисление решения
  x = la_transform(&b, y);
  la_matrix_free(&b);

  *err = 0;
  return x;

err:
  la_vector_init(&x, a->ncol);
  la_vector_fill(&x, 0.);
  return x;
}
//----------------------------------------------------------------------------
// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия с учетом разных шумов правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  const la_vector_t *n, // дисперсия шумов правых частей Y [m]
                        // (диагональ корреляционной матрицы `y`)
  int *err)             // код ошибки (при ошибке <0, при успехе 0)
{
  la_matrix_t b;
  la_vector_t x;

#ifdef LA_PARANOIC
  if (y->size != a->nrow || y->size != n->size || a->nrow < a->ncol)
  {
    LA_DBG("error: bad arguments in la_svd()");
    *err = -1;
    goto err;
  }
#endif // LA_PARANOIC

  // вычисление псевдо-обратной матрицы
  b = la_svd_inverse(a, n, err);
  if (*err < 0)
  {
    la_matrix_free(&b);
    LA_DBG("error: la_svd_inverse() return error in la_svd()");
    *err = -2;
    goto err;
  }

  // вычисление решения
  x = la_transform(&b, y);
  la_matrix_free(&b);

  *err = 0;
  return x;

err:
  la_vector_init(&x, a->ncol);
  la_vector_fill(&x, 0.);
  return x;
}
//----------------------------------------------------------------------------
// решение переопределенной системы линейных уравнений (m > n)
// (поиск X при котором A x X = Y)
// Версия с учетом корреляционной матрицы правых частей
// переопределенной системы системы линейных уравнений.
la_vector_t la_svd_full(
  const la_matrix_t *a, // матрица коэффициентов [nrow=m][ncol=n]
  const la_vector_t *y, // столбец правых частей [m]
  const la_matrix_t *n, // корреляционная матрица правых частей Y [m][m]
  int *err)             // код ошибки (при ошибке <0, при успехе 0)
{
  la_matrix_t b;
  la_vector_t x;

#ifdef LA_PARANOIC
  if (y->size != a->nrow || y->size != n->nrow || a->nrow < a->ncol ||
      n->nrow != n->ncol)
  {
    LA_DBG("error: bad arguments in la_svd_full()");
    *err = -1;
    goto err;
  }
#endif // LA_PARANOIC

  // вычисление псевдо-обратной матрицы
  b = la_svd_inverse_full(a, n, err);
  if (*err < 0)
  {
    la_matrix_free(&b);
    LA_DBG("error: la_svd_inverse_full() return error in la_svd_full()");
    *err = -2;
    goto err;
  }

  // вычисление решения
  x = la_transform(&b, y);
  la_matrix_free(&b);

  *err = 0;
  return x;

err:
  la_vector_init(&x, a->ncol);
  la_vector_fill(&x, 0.);
  return x;
}
//----------------------------------------------------------------------------

/*** end of "la.c" file ***/
