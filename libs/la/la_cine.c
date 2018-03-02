/*
 * Библиотека функций линейной алгебры (кинематические фильтры)
 * Version: 0.7a
 * File: "la_cine.c"
 * Coding: UTF8
 * (C) 2007 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2008.01.31
 */

#include "la_cine.h"

//----------------------------------------------------------------------------
// инициализация кинематического фильтра
void la_cine_init(
  la_cine_t *cine,     // структура фильтра
  int vector_size,     // размер фильтруемого вектора
  int filt_size,       // размер фильтра
  const la_float_t *k) // импульсная характеристика фильтра [filt_size]
{
  // инициализировать интегратор для производной
  la_integ_init(&cine->integ, vector_size);
  
#ifdef LA_CINE_FIR
  // инициализировтаь фильтр для значения вектора
  la_fir_init(&cine->f1, vector_size, filt_size, k);

  // инициализировтаь фильтр для интегрированного значения производной
  la_fir_init(&cine->f2, vector_size, filt_size, k);
#else // LA_CINE_FIR
  // инициализировтаь фильтр для значения вектора
  la_filt_init(&cine->f1, vector_size, filt_size);

  // инициализировтаь фильтр для интегрированного значения производной
  la_filt_init(&cine->f2, vector_size, filt_size);
#endif // LA_CINE_FIR

  // инициализировтаь вспомагательные вектора
  la_vector_init(&cine->v1, vector_size);
  la_vector_init(&cine->v2, vector_size);
  la_vector_init(&cine->v3, vector_size);
  la_vector_init(&cine->v4, vector_size);
  la_vector_fill(&cine->v2, 0.); // сбросить накапливаемое значение
}
//----------------------------------------------------------------------------
// деинициализация кинематического фильтра
void la_cine_free(la_cine_t *cine)
{
  la_vector_free(&cine->v4);
  la_vector_free(&cine->v3);
  la_vector_free(&cine->v2);
  la_vector_free(&cine->v1);
#ifdef LA_CINE_FIR
  la_fir_free(&cine->f2);
  la_fir_free(&cine->f1);
#else // LA_CINE_FIR
  la_filt_free(&cine->f2);
  la_filt_free(&cine->f1);
#endif // LA_CINE_FIR
  la_integ_free(&cine->integ);
}
//----------------------------------------------------------------------------
// произвести установку кинематического фильтра в начальное состояние
// ("форсированный разгон тяжелого фильтра")
void la_cine_start(
  la_cine_t *cine,     // структура фильтра
  const la_float_t *x, // выборка вектора (X)
  const la_float_t *v) // выборка производной вектора (X с точкой)
{
#ifndef LA_CINE_FAST
  int i;
  // I.
  // реализация первая (базовая идея)

  // подготовить интегратор
  la_integ_step(
    &cine->integ,               // структура фильтра
    0.,                         // шаг разбиения (обычно приращение времени)
    v,                          // входная выборка производной вектора
    la_vector_data(&cine->v1)); // выходная выборка конечного приращения
  
  // загрузить линии задержек
#ifdef LA_CINE_FIR
  la_fir_fill(&cine->f1, x);
  la_fir_fill(&cine->f2, x);
#else // LA_CINE_FIR
  la_filt_fill(&cine->f1, x);
  la_filt_fill(&cine->f2, x);
#endif // LA_CINE_FIR

  // загрузить интегрируемое значение
  for (i = 0; i < la_vector_get_size(&cine->v2); i++)
    la_vector_data(&cine->v2)[i] = x[i];

#else // LA_CINE_FAST 
  // II.
  // реализация вторая (используем свойство линейности)
  
  // загрузить интегрируемое значение
  la_vector_fill(&cine->v2, 0.);
  
  // подготовить интегратор
  la_integ_step(
    &cine->integ,               // структура фильтра
    0.,                         // шаг разбиения (обычно приращение времени)
    la_vector_data(&cine->v2),  // входная выборка производной вектора
    la_vector_data(&cine->v1)); // выходная выборка конечного приращения
  
  // загрузить линии задержек
#ifdef LA_CINE_FIR
  la_fir_fill(&cine->f1, la_vector_data(&cine->v2));
  la_fir_fill(&cine->f2, x);
#else // LA_CINE_FIR
  la_filt_fill(&cine->f1, la_vector_data(&cine->v2));
  la_filt_fill(&cine->f2, x);
#endif // LA_CINE_FIR

#endif // LA_CINE_FAST 
}
//----------------------------------------------------------------------------
// выполнить шаг кинематического фильтра
void la_cine_step(
  la_cine_t *cine,     // структура фильтра
  la_float_t dt,       // шаг разбиения (обычно приращение времени)
  const la_float_t *x, // входная выборка вектора (X)
  const la_float_t *v, // входная выборка производной вектора (X с точкой)
  la_float_t *X)       // выходная выборка вектора
{
  int i;

#ifndef LA_CINE_FAST
  // I.
  // реализация первая (базовая идея)

  // вычислить линейное приращение по производной (результат в v1)
  la_integ_step(&cine->integ, dt, v, la_vector_data(&cine->v1));

  // произвести интегрирование производной (резултат в v2)
  la_vector_inc(&cine->v2, &cine->v1);

#ifdef LA_CINE_FIR
  // выполнить шаг фильтрации интегрируемого вектора (результат в v3)
  la_fir_step(&cine->f1, la_vector_data(&cine->v2),
                         la_vector_data(&cine->v3));

  // выполнить шаг фильтрации вектора (результат в v4)
  la_fir_step(&cine->f2, x, la_vector_data(&cine->v4));
#else // LA_CINE_FIR
  // выполнить шаг фильтрации интегрируемого вектора (результат в v3)
  la_filt_step(&cine->f1, la_vector_data(&cine->v2),
                          la_vector_data(&cine->v3));

  // выполнить шаг фильтрации вектора (результат в v4)
  la_filt_step(&cine->f2, x, la_vector_data(&cine->v4));
#endif // LA_CINE_FIR

  // сформировать выход фильтра (результат в v4)
  la_vector_inc(&cine->v4, &cine->v2);
  la_vector_dec(&cine->v4, &cine->v3);
  for (i = 0; i < la_vector_get_size(&cine->v4); i++)
    X[i] = la_vector_data(&cine->v4)[i];

  // скорректировать инегрируемые значения с целью избежания "переполнений"
  la_vector_dec(&cine->v2, &cine->v3);
#ifdef LA_CINE_FIR
  la_fir_corr(&cine->f1, la_vector_data(&cine->v3));
#else // LA_CINE_FIR
  la_filt_corr(&cine->f1, la_vector_data(&cine->v3));
#endif // LA_CINE_FIR

#else // LA_CINE_FAST 
  // II.
  // реализация вторая (используем свойство линейности)

#ifdef LA_CINE_FIR
  // выполнить шаг фильтрации скорости (результат в v1)
  la_fir_step(&cine->f1, v, la_vector_data(&cine->v1));
  
  // выполнить шаг фильтрации вектора (результат в v4)
  la_fir_step(&cine->f2, x, la_vector_data(&cine->v4));
#else // LA_CINE_FIR 
  // выполнить шаг фильтрации скорости (результат в v1)
  la_filt_step(&cine->f1, v, la_vector_data(&cine->v1));

  // выполнить шаг фильтрации вектора (результат в v4)
  la_filt_step(&cine->f2, x, la_vector_data(&cine->v4));
#endif // LA_CINE_FIR

  // превратить ФНЧ в ФВЧ (с инверсией в v1)
  for (i = 0; i < la_vector_get_size(&cine->v1); i++)
    la_vector_data(&cine->v1)[i] -= v[i];

  // вычислить линейное приращение по производной (результат в v3)
  la_integ_step(&cine->integ, dt, la_vector_data(&cine->v1), 
                                  la_vector_data(&cine->v3));

  // произвести интегрирование (резултат в v2)
  la_vector_dec(&cine->v2, &cine->v3);

  // скорректировать инегрируемое значения с целью избежания "переполнений"
  // с помощью "очень слабой" обратной отрицательной связи
  for (i = 0; i < la_vector_get_size(&cine->v2); i++)
    la_vector_data(&cine->v2)[i] *= 1. - (1. / LA_CINE_FAST_CORR);

  // сформировать выход фильтра (X <= v4 + v2)
  for (i = 0; i < la_vector_get_size(&cine->v4); i++)
    X[i] = la_vector_data(&cine->v4)[i] + la_vector_data(&cine->v2)[i];
#endif // LA_CINE_FAST 
}  
//----------------------------------------------------------------------------
// инициализация двойного кинематического фильтра
void la_cine2_init(
  la_cine2_t *cine2,     // структура фильтра
  int vector_size,       // размер фильтруемого вектора
  int filt_size_x,       // размер фильтра для координаты (X)
  int filt_size_v,       // размер фильтра для скорости (V)
  const la_float_t *k_x, // импульсная характеристика фильтра (по X)
  const la_float_t *k_v) // импульсная характеристика фильтра (по V)
{
  la_cine_init(
    &cine2->fx,   // структура фильтра
    vector_size, // размер фильтруемого вектора
    filt_size_x, // размер FIR фильтра
    k_x);        // импульсная характеристика FIR фильтра [filt_size]
  
  la_cine_init(
    &cine2->fv,   // структура фильтра
    vector_size, // размер фильтруемого вектора
    filt_size_v, // размер FIR фильтра
    k_v);        // импульсная характеристика FIR фильтра [filt_size]
}
//----------------------------------------------------------------------------
// деинициализация двойного кинематического фильтра
void la_cine2_free(la_cine2_t *cine2)
{
  la_cine_free(&cine2->fv);
  la_cine_free(&cine2->fx);
}
//----------------------------------------------------------------------------
// произвести установку двойного кинематического фильтра в начальное состояние
// ("форсированный разгон тяжелого фильтра")
void la_cine2_start(
  la_cine2_t *cine2,   // структура фильтра
  const la_float_t *x, // выборка вектора (X)
  const la_float_t *v, // выборка производной вектора (X с точкой)
  const la_float_t *a) // выборка второй производной вектора
{
  la_cine_start(&cine2->fx, x, v);
  la_cine_start(&cine2->fv, v, a);
}
//----------------------------------------------------------------------------
// выполнить шаг двойного кинематического фильтра
void la_cine2_step(
  la_cine2_t *cine2,   // структура фильтра
  la_float_t dt,       // шаг разбиения (обычно приращение времени)
  const la_float_t *x, // входная выборка вектора
  const la_float_t *v, // входная выборка производной вектора
  const la_float_t *a, // входная выборка второй производной вектора
  la_float_t *X,       // выходная выборка вектора
  la_float_t *V)       // выходная выборка производной вектора
{
  // выполнить шаг фильтрации скорости (результат в V)
  la_cine_step(
    &cine2->fv, // структура фильтра
    dt,         // шаг разбиения (обычно приращение времени)
    v,          // входная выборка вектора (X)
    a,          // входная выборка производной вектора (X с точкой)
    V);         // выходная выборка вектора

  // выполнить шаг фильтрации координаты (результат в X)
  la_cine_step(
    &cine2->fx, // структура фильтра
    dt,         // шаг разбиения (обычно приращение времени)
    x,          // входная выборка вектора (X)
    V,          // входная выборка производной вектора (X с точкой)
    X);         // выходная выборка вектора
}
//----------------------------------------------------------------------------

/*** end of "la_cine.c" file ***/

