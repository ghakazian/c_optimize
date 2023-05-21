#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// gcc -lm test_j.c -o test_j

// constants -- used for values that are not calculated based on inputs
const double a  = 0.0081;
const double b  = 0.6;
const double g  = 9.807;
const double gC = 5.87;
const double aC = 0.0317;
const double saC = 0.0547;
const double saX = 0.32;
const double sbC = 0.0783;
const double sbX = 0.16;
double pi, fptildemin, aX, gX, vA;

typedef struct _fptParams {
  double alpha;
  double gamma;
  double sigma_a;
  double sigma_b;
} fptParams;

fptParams *_fptParams[1001];

/// @brief 
void fptParams_init() {
  // Ensure set of fptParams is initialized to NULL pointers
  memset(_fptParams, 0, sizeof(fptParams*) * 1001);
}

void fptParams_free() {
  // Walk through each possible value of fpt by index and free any stored value
  for(int i = 0; i < 1001; i++) {
    if (_fptParams[i] != NULL) {
      free(_fptParams[i]);
      _fptParams[i] = NULL;
    }
  }
}

fptParams* fptParams_get(double fpt) {
  // convert value of FPT to an integer index which works since we know it is called in steps of 0.01
  int index = lround(fpt * 100.0);
  if (_fptParams[index] != NULL) {
    return _fptParams[index];
  }

  // TODO: Ensure we successfully allocate memory from the heap
  fptParams* params = malloc(sizeof(fptParams));
  params->alpha = aC  * pow(fpt, aX);
  params->gamma = gC  * pow(fpt, gX);
  params->sigma_a = saC * pow(fpt, saX);
  params->sigma_b = sbC * pow(fpt, sbX);
  _fptParams[index] = params;
  return params;
}

void init() {
  fptParams_init();

  pi = 4.*atan(1.);
  fptildemin = (1.0/2.0/pi) * pow((4.0 * b / 5.0), (1.0/4.0));
  aX  = (log(a)-log(aC))/log(fptildemin);
  gX  = -log(gC)/log(fptildemin);
  vA = pow(g, 2) * pow((2*pi), -4);
}

double function_j(double f, double fp, double fptilde) {
   double fpt = MAX(fptilde, fptildemin);
   fptParams *params = fptParams_get(fpt);

   double exp1arg = -1.25 * pow((f/fp),-4);
   double sigma   = (f <= fp) * params->sigma_a + (f > fp) * params->sigma_b;

   double exp2arg = -0.5 * pow((f-fp)/(sigma*fp), 2);

   double S = params->alpha * vA * pow(f,-5) * exp(exp1arg) * pow(params->gamma, exp(exp2arg));

   return S;
}

int main() {
  // Initialize global variables and fptilde cache
  init();

  double S, f, fp, fptilde;

  for (f = -5.; f <= 5.; f += 0.01) {
    for (fp = 0.; fp <= 10.; fp += 0.01) {
      for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {
        S = function_j(f, fp, fptilde);
      }
    }
  }

  // free fptilde cache
  fptParams_free();
  return 0;
}
