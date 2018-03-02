/*
 * Генераторы псевдослучайных случайных величин
 * Version: 0.3b
 * File: "rnd.c"
 * Coding: UTF8
 * (C) 2005..2018 Alex Zorg <azorg@mail.ru>
 * Licensed by GNU General Public License version 2
 * Last update: 2012.03.23
 */

#ifndef RND_H
#define RND_H
//----------------------------------------------------------------------------
// max result of rnd_rand() and rnd_rand2() function
#ifndef RND_USE_STDLIB
#  define RND_MAX 32767
#else
#  include <stdlib.h>
#  define RND_MAX RAND_MAX
#endif
#define RND_MAX2 ((1<<30)-1) // 1073741823
//----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
//----------------------------------------------------------------------------
// simple rand() (from old stdlib.h) analog
int rnd_rand(void);
//----------------------------------------------------------------------------
// simple srand() (from stdlib.h) analog
void rnd_srand(unsigned seed);
//----------------------------------------------------------------------------
// return rundom value 0...RND_MAX2
int rnd_rand2(void);
//----------------------------------------------------------------------------
// return unsigned random float in [0, s]
float rnd_frand_u(float s);
double rnd_frand_u2(double s);
//----------------------------------------------------------------------------
// return signed random float in [-s, s]
float rnd_frand_s(float s);
double rnd_frand_s2(double s);
//----------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #1)
float rnd_noise_slow(float s);
double rnd_noise_slow2(double s);
//----------------------------------------------------------------------------
// generate normal noise with zero average value and energe s^2 (variant #2)
float rnd_noise(float s);
double rnd_noise2(double s);
//----------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #1)
float rnd_noise_r_slow(float s);
double rnd_noise_r_slow2(double s);
//----------------------------------------------------------------------------
// generate Reley noise with energe s^2 (variant #2)
float rnd_noise_r(float s);
double rnd_noise_r2(double s);
//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
//----------------------------------------------------------------------------
#endif // RND_H

/*** end of "rnd.h" file ***/
