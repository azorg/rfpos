/*
 * Библиотека функций линейной алгебры (линейные фильтры)
 * Version: 0.7a
 * File: "la_filt.c"
 * Coding: UTF8
 * (C) 2007 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2008.01.31
 */

#include "la_filt.h"

//----------------------------------------------------------------------------
// инициализация простого рекурентного ФНЧ с АЧХ "sin(x)/x"
void la_filt_init(
  la_filt_t *filt, // структура фильтра
  int vector_size, // размер фильтруемого вектора
  int filt_size)   // размер фильтра
{
  // создать матрицу для линии задержки
  la_matrix_init(&filt->d, filt_size, vector_size);

  // для избежания ислючений заполнить линию задержки нулями
  la_matrix_fill(&filt->d, 0.);

  // инициализировать рекурентный и прямой накопитель
  la_vector_init(&filt->rs, vector_size);
  la_vector_init(&filt->ds, vector_size);
  la_vector_fill(&filt->rs, 0.);
  la_vector_fill(&filt->ds, 0.);
  
  // инициализировать индекс
  filt->index = 0;

  // нормирующий коэффициент 1/n
  filt->k = 1. / ((la_float_t) filt_size);
}
//----------------------------------------------------------------------------
// денициализация простого рекурентного ФНЧ с АЧХ "sin(x)/x"
void la_filt_free(la_filt_t *filt)
{
  la_matrix_free(&filt->d);
  la_vector_free(&filt->rs);
  la_vector_free(&filt->ds);
}
//----------------------------------------------------------------------------
// заполнить линию задержки ФНЧ фильтра
void la_filt_fill(la_filt_t *filt, const la_float_t *v)
{
  int row, col;
  for (col = 0; col < la_matrix_get_ncol(&filt->d); col++)
  {
    for (row = 0; row < la_matrix_get_nrow(&filt->d); row++)
      la_matrix_data(&filt->d, row)[col] = v[col];
  
    la_vector_data(&filt->rs)[col] = v[col] / filt->k;
    la_vector_data(&filt->ds)[col] = 0.;
  }
  filt->index = 0;
}
//----------------------------------------------------------------------------
// выполнить коррекцию линию задержки ФНЧ фильтра (вычесть вектор)
void la_filt_corr(la_filt_t *filt, const la_float_t *v)
{
  int row, col;
  for (col = 0; col < la_matrix_get_ncol(&filt->d); col++)
  {
    for (row = 0; row < la_matrix_get_nrow(&filt->d); row++)
      la_matrix_data(&filt->d, row)[col] -= v[col];
  
    la_vector_data(&filt->rs)[col] -= v[col] / filt->k;
    la_vector_data(&filt->ds)[col] -= v[col] * (la_float_t) filt->index;
  }
}
//----------------------------------------------------------------------------
// выполнить шаг ФНЧ фильтра с АЧХ "sin(x)/x"
void la_filt_step(
  la_filt_t *filt,        // структура фильтра
  const la_float_t *inp,  // указатель на данные входного вектора 
  la_float_t *out)        // указатель на данные выходного вектора
{
  int i;
  for (i = 0; i < la_matrix_get_ncol(&filt->d); i++)
  {
    // накопить в рекурентном накопителе
    la_vector_data(&filt->rs)[i] +=
      inp[i] - la_matrix_data(&filt->d, filt->index)[i];

    // поместить в линию задержки поступившее значение
    la_matrix_data(&filt->d, filt->index)[i] = inp[i];
    
    // накопить в прямом накопителе
    la_vector_data(&filt->ds)[i] += inp[i];
  }
  
  // изменить индекс линии задержки
  if (++filt->index >= la_matrix_get_nrow(&filt->d))
  {  
    filt->index = 0;

    // скорректировать рекурентный накопитель прямым
    // (защита от накопления ошибок)
    for (i = 0; i < la_matrix_get_ncol(&filt->d); i++)
    {
      la_vector_data(&filt->rs)[i] = la_vector_data(&filt->ds)[i];
      la_vector_data(&filt->ds)[i] = 0.;
    }
  }

  // поместить на выход отнормированное значение рекуретного накопителя
  for (i = 0; i < la_matrix_get_ncol(&filt->d); i++)
    out[i] = la_vector_data(&filt->rs)[i] * filt->k;
}
//----------------------------------------------------------------------------
// инициализация FIR фильтра с вещесвенными коэффицентами
void la_fir_init(
  la_fir_t *fir,       // структура фильтра
  int vector_size,     // размер фильтруемого вектора
  int filt_size,       // размер фильтра
  const la_float_t *k) // импульсная характеристика [filt_size]
{
  // инициализировать вектор импульсной характеристики
  fir->k = la_vector_create(k, filt_size);

  // создать матрицу для линии задержки
  la_matrix_init(&fir->d, filt_size, vector_size);

  // для избежания ислючений заполнить линию задержки нулями
  la_matrix_fill(&fir->d, 0.);

  // инициализировать индекс
  fir->index = 0;
}
//----------------------------------------------------------------------------
// деинициализация FIR фильтра с вещесвенными коэффицентами
void la_fir_free(la_fir_t *fir)
{
  la_matrix_free(&fir->d);
  la_vector_free(&fir->k);
}
//---------------------------------------------------------------------------
// заполнить линию задержки FIR фильтра
void la_fir_fill(la_fir_t *fir, const la_float_t *v)
{
  int row, col;
  for (row = 0; row < la_matrix_get_nrow(&fir->d); row++)
    for (col = 0; col < la_matrix_get_ncol(&fir->d); col++)
      la_matrix_data(&fir->d, row)[col] = v[col];
}
//----------------------------------------------------------------------------
// выполнить коррекцию линию задержки FIR фильтра (вычесть вектор)
void la_fir_corr(la_fir_t *fir, const la_float_t *v)
{
  int row, col;
  for (row = 0; row < la_matrix_get_nrow(&fir->d); row++)
    for (col = 0; col < la_matrix_get_ncol(&fir->d); col++)
      la_matrix_data(&fir->d, row)[col] -= v[col];
}
//----------------------------------------------------------------------------
// выполнить шаг FIR фильтра
void la_fir_step(
  la_fir_t *fir,         // структура фильтра
  const la_float_t *inp, // указатель на данные входного вектора 
  la_float_t *out)       // указатель на данные выходного вектора
{
  int row, col, i;

  // произвести размещение входного вектора в линии задержки
  for (col = 0; col < la_matrix_get_ncol(&fir->d); col++)
  {
    la_matrix_data(&fir->d, fir->index)[col] = inp[col];
    out[col] = 0.;
  }

  // произвести весовое суммирование
  i = fir->index;
  for (row = 0; row < la_matrix_get_nrow(&fir->d); row++)
  {
    la_float_t k = la_vector_data(&fir->k)[row];
    for (col = 0; col < la_matrix_get_ncol(&fir->d); col++)
      out[col] += k * la_matrix_data(&fir->d, i)[col];
    if (--i < 0) i = la_matrix_get_nrow(&fir->d) - 1;
  }
  
  // изменить индекс
  if (++fir->index >= la_matrix_get_nrow(&fir->d)) fir->index = 0;
}
//----------------------------------------------------------------------------
// инициализация интегратора методом трапеции
void la_integ_init(
  la_integ_t *integ, // структура фильтра
  int vector_size)   // размер фильтруемого вектора
{
  la_vector_init(&integ->prev, vector_size);
  la_vector_fill(&integ->prev, 0.);
}
//----------------------------------------------------------------------------
// деинициализация интегратора методом трапеции
void la_integ_free(la_integ_t *integ)
{
  la_vector_free(&integ->prev);
}
//----------------------------------------------------------------------------
// выполнить шаг интегратора методом трапеции
void la_integ_step(
  la_integ_t *integ,     // структура фильтра
  la_float_t dt,         // шаг разбиения (обычно приращение времени)
  const la_float_t *inp, // входная выборка производной вектора
  la_float_t *out)       // выходная выборка конечного приращения
{
  int i;
  // вычислить конечное приращение методом трапеции
  // и запомнить текущую производную для следующей итерации
  dt *= 0.5;
  for (i = 0; i < la_vector_get_size(&integ->prev); i++)
  {
    out[i] = (la_vector_data(&integ->prev)[i] + inp[i]) * dt;
    la_vector_data(&integ->prev)[i] = inp[i];
  }
}
//----------------------------------------------------------------------------

/*** end of "la_filt.c" file ***/

