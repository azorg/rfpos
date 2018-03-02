/*
 * Генераторы псевдослучайных случайных величин
 * Version: 0.3b
 * File: "rnd.c"
 * Coding: UTF8
 * (C) 2005..2018 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2012.03.23
 */

#include <math.h>
#include "rnd.h"
//----------------------------------------------------------------------------
#ifndef RND_USE_STDLIB
static unsigned long rnd_rand_next = 1;
#endif
//----------------------------------------------------------------------------
// simple rand() (from old stdlib.h) analog
int rnd_rand(void)
{
#ifndef RND_USE_STDLIB
  rnd_rand_next = rnd_rand_next * 1103515245 + 12345;
  return (unsigned) (rnd_rand_next / 65536) % 32768;
#else
  return rand();
#endif
}
//----------------------------------------------------------------------------
// simple srand() (from stdlib.h) analog
void rnd_srand(unsigned seed)
{
#ifndef RND_USE_STDLIB
  rnd_rand_next = seed;
#else
  srand(seed);
#endif
}
//----------------------------------------------------------------------------
// return rundom value 0...RND_MAX2
int rnd_rand2(void)
{
  return ((rnd_rand() << 15) ^ rnd_rand()) & RND_MAX2;
}
//----------------------------------------------------------------------------
// return unsigned random float in [0, s]
float rnd_frand_u(float s)
{
  float ret = (float) rnd_rand();  // [0, RND_MAX]
  ret *= s / (float) RND_MAX; // [0, s]
  return ret;
}
//----------------------------------------------------------------------------
// return unsigned random float in [0, s]
double rnd_frand_u2(double s)
{
  double ret = (double) rnd_rand2(); // [0, RND_MAX2]
  ret *= s / (double) RND_MAX2;      // [0, s]
  return ret;
}
//----------------------------------------------------------------------------
// return signed random float in [-s, s]
float rnd_frand_s(float s)
{
  float ret = (float) rnd_rand(); // [0, RND_MAX]
  ret *= 2. / (float) RND_MAX;    // [0..2]
  ret -= 1.0;                     // [-1..1]
  ret *= s;                       // [-s..s]
  return ret;
}
//----------------------------------------------------------------------------
// return signed random float in [-s, s]
double rnd_frand_s2(double s)
{
  double ret = (double) rnd_rand2(); // [0, RND_MAX2]
  ret *= 2. /  (double) RND_MAX2;    // [0..2]
  ret -= 1.0;                        // [-1..1]
  ret *= s;                          // [-s..s]
  return ret;
}
//----------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #1)
float rnd_noise_slow(float s)
{
  float ret = 0.;
  int i = 24;
  do {
    ret += rnd_frand_s(s);
  } while (--i);
  // помним, что для равномерного распределения [-1...1] СКО=1/6
  return ret * 0.5; // ret * sqrt(6. / 24.);
}
//----------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #1)
double rnd_noise_slow2(double s)
{
  double ret = 0.;
  int i = 24;
  do {
    ret += rnd_frand_s2(s);
  } while (--i);
  // помним, что для равномерного распределения [-1...1] СКО=1/6
  return ret * 0.5; // ret * sqrt(6. / 24.);
}
//---------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #2)
float rnd_noise(float s)
{
  float a = rnd_frand_u(1.); // [0, 1]
  if (a <= 0.) a = 1e-12;
  a = -2. * log(a); // [0, 55.26xxx]
  if (a < 0.) a = 0.; // paranoic check
  s *= sqrt(a) * sin(rnd_frand_u(2. * M_PI));
  return s; // [-7.43xx * s, +7.43xx * s]
}
//---------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #2)
double rnd_noise2(double s)
{
  double a = rnd_frand_u2(1.); // [0, 1]
  if (a <= 0.) a = 1e-16;
  a = -2. * log(a); // [0, 73.68xxx]
  if (a < 0.) a = 0.; // paranoic check
  s *= sqrt(a) * sin(rnd_frand_u2(2. * M_PI));
  return s; // [-8.58xx * s, +8.58xx * s]
}
//---------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #1)
float rnd_noise_r_slow(float s)
{
  float a = rnd_noise_slow(s);
  float b = rnd_noise_slow(s);
  return sqrt(a*a + b*b) / 1.05;
}
//---------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #1)
double rnd_noise_r_slow2(double s)
{
  double a = rnd_noise_slow2(s);
  double b = rnd_noise_slow2(s);
  return sqrt(a*a + b*b) / 1.05;
}
//---------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #2)
float rnd_noise_r(float s)
{
  float a = rnd_noise(s);
  float b = rnd_noise(s);
  return sqrt(a*a + b*b) / 1.05;
}
//---------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #2)
double rnd_noise_r2(double s)
{
  double a = rnd_noise(s);
  double b = rnd_noise(s);
  return sqrt(a*a + b*b) / 1.05;
}
//----------------------------------------------------------------------------

/*** end of "rnd.c" file ***/
