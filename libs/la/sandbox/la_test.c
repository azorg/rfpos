#include "la.h"
#include "la_cine.h"
#include "rnd.h"

#include <math.h>
#include <pthread.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// return signed random float in [-s, s]
float frand_s(float s)
{
  float ret = (float) rand();   // [0, RND_MAX]
  ret *= 2. / (float) RAND_MAX; // [0..2]
  ret -= 1.0;                   // [-1..1]
  ret *= s;                     // [-s..s]
  return ret;
}
//----------------------------------------------------------------------------
double get_rt()
{
  struct timespec tv;
  double t;
  clock_gettime(CLOCK_REALTIME, &tv);
  t  = ((double) tv.tv_nsec) * 1e-9;
  t += ((double) tv.tv_sec);
  return t;
}
//----------------------------------------------------------------------------
int main()
{
  if (1)
  {
    static la_float_t V1[] = {10., 20., 30., 40.};
    static la_float_t M1[] = {
       1,  2,  3,
       4,  5,  6,
       7,  8,  9,
      10, 11, 12
    };
    la_vector_t v  = la_vector_create(V1, 4);
    la_matrix_t m1 = la_matrix_create(M1, 4, 3);
    la_matrix_t m2 = la_matrix_trans(&m1);
    fprintf(stdout, "v = ");
    la_vector_print(&v, stdout);
    fprintf(stdout, "\n");
    la_matrix_swap_row(&m1, 0, 1);
    la_matrix_swap_col(&m1, 1, 2);
    fprintf(stdout, "m1 = \n");
    la_matrix_print(&m1, stdout);
    fprintf(stdout, "m2 = \n");
    la_matrix_print(&m2, stdout);
    fprintf(stdout, "\n");
    la_vector_free(&v);
    la_matrix_free(&m1);
    la_matrix_free(&m2);
  }

  printf("\n1. la_memory_count = %i\n", la_get_memory_count());

  if (1)
  {
    static la_float_t A[] = {
      1, 2, 3,
      4, 5, 6
    };
    static la_float_t B[] = {
      10, 20,
      30, 40,
      50, 60
    };
    la_matrix_t a = la_matrix_create(A, 2, 3);
    la_matrix_t b = la_matrix_create(B, 3, 2);
    la_matrix_t c = la_matrix_mpy(&a, &b);
    fprintf(stdout, "a =\n");
    la_matrix_print(&a, stdout);
    fprintf(stdout, "\nb =\n");
    la_matrix_print(&b, stdout);
    fprintf(stdout, "\nc =\n");
    la_matrix_print(&c, stdout);
    fprintf(stdout, "\n");
    la_matrix_free(&a);
    la_matrix_free(&b);
    la_matrix_free(&c);
  }
  
  printf("\n2. la_memory_count = %i\n", la_get_memory_count());

  if (1)
  {
    int i, j;
    static la_float_t A[] = {
      1, 2, 3,
      4, 5, 6,
      7, 8, -9
    };
    la_matrix_t a = la_matrix_create(A, 3, 3);
    la_matrix_t b, c;
    b = la_matrix_inverse(&a, &i);
    c = la_matrix_inverse(&b, &j);
    if (i < 0)
      fprintf(stdout, "error of inverse (a) = %i\n", i);
    fprintf(stdout, "\na =\n");
    la_matrix_print(&a, stdout);
    fprintf(stdout, "\nb = inv(a) = \n");
    la_matrix_print(&b, stdout);
    fprintf(stdout, "\nc = inv(b) = a =\n");
    la_matrix_print(&c, stdout);
    fprintf(stdout, "\n");
    la_matrix_free(&a);
    la_matrix_free(&b);
    la_matrix_free(&c);
  }
  
  printf("\n3. la_memory_count = %i\n", la_get_memory_count());

  if (1)
  {
    static la_float_t M_1x1[] = {
      1
    };
    static la_float_t M_2x2[] = {
      1, 2, 
      3, 0
    };
    static la_float_t M_3x3[] = {
       1,  2,  3,
       4,  0,  6,
       7,  8,  9
    };
    static la_float_t M_4x4[] = {
       1,  2,  3,   4,
       5,  0,  7,   8,
       9, 10, 11,  12,
      13, 14, 15, -16
    };
    static la_float_t M_5x5[] = {
       0, 1, 0, 0, 0,
       1, 0, 0, 0, 0,
       0, 0, 1, 0, 0,
       0, 0, 0, 0, 1,
       0, 0, 0, 1, 0
    };
    la_matrix_t m_1x1 = la_matrix_create(M_1x1, 1, 1);
    la_matrix_t m_2x2 = la_matrix_create(M_2x2, 2, 2);
    la_matrix_t m_3x3 = la_matrix_create(M_3x3, 3, 3);
    la_matrix_t m_4x4 = la_matrix_create(M_4x4, 4, 4);
    la_matrix_t m_5x5 = la_matrix_create(M_5x5, 5, 5);
  
    fprintf(stdout, "la_matrix_det_v1(m_1x1) = %f\n", (double) la_matrix_det_v1(&m_1x1));
    fprintf(stdout, "la_matrix_det_v2(m_1x1) = %f\n", (double) la_matrix_det_v2(&m_1x1));
    fprintf(stdout, "la_matrix_det_v1(m_2x2) = %f\n", (double) la_matrix_det_v1(&m_2x2));
    fprintf(stdout, "la_matrix_det_v2(m_2x2) = %f\n", (double) la_matrix_det_v2(&m_2x2));
    fprintf(stdout, "la_matrix_det_v1(m_3x3) = %f\n", (double) la_matrix_det_v1(&m_3x3));
    fprintf(stdout, "la_matrix_det_v2(m_3x3) = %f\n", (double) la_matrix_det_v2(&m_3x3));
    fprintf(stdout, "la_matrix_det_v1(m_4x4) = %f\n", (double) la_matrix_det_v1(&m_4x4));
    fprintf(stdout, "la_matrix_det_v2(m_4x4) = %f\n", (double) la_matrix_det_v2(&m_4x4));
    fprintf(stdout, "la_matrix_det_v1(m_5x5) = %f\n", (double) la_matrix_det_v1(&m_5x5));
    fprintf(stdout, "la_matrix_det_v2(m_5x5) = %f\n", (double) la_matrix_det_v2(&m_5x5));

    la_matrix_free(&m_1x1);
    la_matrix_free(&m_2x2);
    la_matrix_free(&m_3x3);
    la_matrix_free(&m_4x4);
    la_matrix_free(&m_5x5);
  }
  
  printf("\n4. la_memory_count = %i\n", la_get_memory_count());

  if (1)
  {
    int err;
    static la_float_t A[] = {
      1, 1, 0,
      0, 1, 0,
      1, 0, 1,
      0, 1, 1,
      1, 1, 1
    };
    static la_float_t M[] = {1, 1, 1, 9000, 1};
    static la_float_t N[] = {
      1, 0, 0,    0, 0,
      0, 1, 0,    0, 0,
      0, 0, 1,    0, 0,
      0, 0, 900, 900, 900,
      0, 0, 0,    0, 1
    };
    static la_float_t Y[] = {3, 2, 4, 5.1, 6};
    la_matrix_t a = la_matrix_create(A, 5, 3);
    la_vector_t y = la_vector_create(Y, 5);
    la_vector_t m = la_vector_create(M, 5);
    la_matrix_t n = la_matrix_create(N, 5, 5);
    la_vector_t x1 = la_svd(&a, &y, &m, &err);
    la_vector_t x2 = la_svd_full(&a, &y, &n, &err);
    fprintf(stdout, "\n");
    fprintf(stdout, "err = %i\n", err);
    fprintf(stdout, "\n");
    la_vector_print(&x1, stdout);
    la_vector_print(&x2, stdout);
    la_matrix_free(&a);
    la_vector_free(&y);
    la_vector_free(&m);
    la_matrix_free(&n);
    la_vector_free(&x1);
    la_vector_free(&x2);
  }
  
  printf("\n5. la_memory_count = %i\n", la_get_memory_count());
  
  if (1)
  {
    int i, j, k, nx = 200, ny = 400;
    double t1, t2, t3;
    la_matrix_t a;
    la_vector_t n;
    la_vector_t y;
    la_vector_t x1, x2;

    la_matrix_init(&a, ny, nx);
    la_vector_init(&n, ny);
    la_vector_init(&y, ny);

    for (i = 0; i < ny; i++)
    {
      for (j = 0; j < nx; j++)
	a.d[i][j] = frand_s(0.00);
      k = i % nx;
      a.d[i][k] += 1.0;
    }
     
    la_vector_fill(&n, 1.);
    
    for (i = 0; i < ny; i++)
      y.d[i] = frand_s(0.00) + (float) (i % nx) + 1;

    t1 = get_rt();
    x1 = la_svd(&a, &y, &n, &i);
    t2 = get_rt();
    x2 = la_svd_simple(&a, &y, &j);
    t3 = get_rt();
    
    
    fprintf(stdout, "la_matrix_rang(a) = %i\n", la_matrix_rang(&a));
    fprintf(stdout, "t2 - t1 = %f\n", t2 - t1);
    fprintf(stdout, "t3 - t2 = %f\n", t3 - t2);
    fprintf(stdout, "err_svd = %i err_svd_simple = %i\n", i, j);
    la_vector_print(&x1, stdout);
    la_vector_print(&x2, stdout);
    fprintf(stdout, "\n");
    
    la_vector_free(&x1);
    la_vector_free(&x2);
    la_vector_free(&y);
    la_vector_free(&n);
    la_matrix_free(&a);
  }
  
  printf("\n6. la_memory_count = %i\n", la_get_memory_count());
  
  if (1)
  {
    static la_float_t M[] = {
      1, 0, 0,
      0, 0, 0,
      3, 1, 0,
      0, 0, 0,
      0, 0, 1
    };
    la_matrix_t m = la_matrix_create(M, 5, 3);
    fprintf(stdout, "la_matrix_rang(m) = %i\n", la_matrix_rang(&m));
    la_matrix_free(&m);
  }

  printf("\n7. la_memory_count = %i\n", la_get_memory_count());

  if (1)
  { // прогон кинематического фильтра
    int i, n = 10000, m_x = 1000, m_v = 500;
    double o = (2. * M_PI) / ((double) n);
    double h1 = 2.;
    double h2 = 6.;
    double a1 = 1.;
    double a2 = .03;
    double t;
    la_cine2_t cine2;
    FILE *fo = fopen("data.txt", "wb");
    
    la_vector_t x = la_vector_new(n);
    la_vector_t v = la_vector_new(n);
    la_vector_t a = la_vector_new(n);
    la_vector_t X = la_vector_new(n);
    la_vector_t V = la_vector_new(n);
    la_vector_t k_x = la_vector_new(m_x);
    la_vector_t k_v = la_vector_new(m_v);
    
    // построить импульсную характеристику
    t = 0.;
    for (i = 0; i < m_x; i++)
    {
      k_x.d[i] = (1. - cos((((double) i) + 0.5) * (2*M_PI) / (double)  m_x));
      t += k_x.d[i];
    }
    for (i = 0; i < m_x; i++)
      k_x.d[i] /= t;
    la_vector_fill(&k_x, 1. / (double) m_x);
    
    t = 0.;
    for (i = 0; i < m_v; i++)
    {
      k_v.d[i] = (1. - cos((((double) i) + 0.5) * (2*M_PI) / (double)  m_v));
      t += k_v.d[i];
    }
    for (i = 0; i < m_v; i++)
      k_v.d[i] /= t;
    la_vector_fill(&k_v, 1. / (double) m_v);

    //la_cine_init(&cine, 1, m, la_vector_data(&k));
    la_cine2_init(
      &cine2, // структура фильтра
      1,      // размер фильтруемого вектора
      m_x,    // размер FIR фильтра для координаты (X)
      m_v,    // размер FIR фильтра для скорости (V)
      k_x.d,  // импульсная характеристика FIR фильтра (по X)
      k_v.d); // импульсная характеристика FIR фильтра (по V)
    
    la_vector_free(&k_x);
    la_vector_free(&k_v);

    // заполнить входной массив
    for (i = 0; i < n; i++)
    {
      double df = M_PI/8.;
      double fi1 = h1 * o * ((double) i) + df;
      double fi2 = h2 * o * ((double) i) + df;
      double ps1 = h1 * o * ((double) (i & 0xFFFFFFFF)) + df;
      double ps2 = h2 * o * ((double) (i & 0xFFFFFFFF)) + df;
      double qs1 = h1 * o * ((double) (i & 0xFFFFFFFF)) + df;
      double qs2 = h2 * o * ((double) (i & 0xFFFFFFFF)) + df;
      
      la_vector_data(&x)[i] = a1 * sin(ps1) + a2 * sin(ps2);
      la_vector_data(&v)[i] = o * (a1 * h1 * cos(qs1) + a2 * h2 * cos(qs2));
      la_vector_data(&a)[i] = - o * o * (a1 * h1 * h1 * sin(fi1) +
                                         a2 * h2 * h2 * sin(fi2));
      
      //la_vector_data(&x)[i] += rnd_noise_slow(0.03);
      //la_vector_data(&v)[i] += rnd_noise_slow(0.0001);
      //la_vector_data(&a)[i] += rnd_noise_slow(0.0000001);

      la_vector_data(&x)[i] += rnd_noise_slow(0.003);
      la_vector_data(&v)[i] += rnd_noise_slow(0.00001);
      //la_vector_data(&a)[i] += rnd_noise_slow(0.00000001);
      
      //la_vector_data(&a)[i] *= 0.5;
    }
    
    // разгон фильтра
#if 1
    la_cine2_start(
      &cine2,                  // структура фильтра
      &la_vector_data(&x)[0],  // выборка вектора
      &la_vector_data(&v)[0],  // выборка производной вектора
      &la_vector_data(&a)[0]); // выборка второй производной вектора
#endif

    // заполнить выходной массив
    for (i = 0; i < n; i++)
    {
      la_cine2_step(
        &cine2,                  // структура фильтра
        1.,                      // шаг разбиения (обычно приращение времени)
        &la_vector_data(&x)[i],  // входная выборка вектора
        &la_vector_data(&v)[i],  // входная выборка производной вектора
        &la_vector_data(&a)[i],  // входная выборка второй производной вектора
        &la_vector_data(&X)[i],  // выходная выборка вектора
        &la_vector_data(&V)[i]); // выходная выборка производной вектора
    }

    // сохранить все в файл
    for (i = 0; i < n; i++)
    {
      fprintf(fo, "%10.8f %10.8f %10.8f %10.8f %10.8f\n",
        la_vector_data(&x)[i],
        la_vector_data(&v)[i] * 300.,
        la_vector_data(&a)[i] * 300000.,
        la_vector_data(&X)[i],
        la_vector_data(&V)[i] * 300.);
    }

    la_cine2_free(&cine2);
    la_vector_free(&x);
    la_vector_free(&v);
    la_vector_free(&a);
    la_vector_free(&X);
    la_vector_free(&V);

    fclose(fo);
  }

#ifdef LA_MEMORY_DEBUG
  if (la_get_memory_count() == 0)
  {
    printf("\nУтечек памяти не обнаружно. Все OK!\n");
  }
  else
  {
    printf("\n!!! ERROR !!!\n");
  }
#endif // LA_MEMORY_DEBUG

  return 0;
}
//----------------------------------------------------------------------------

/*** end of "la_test.c" file ***/

