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

void init() {

  pi = 4.*atan(1.);
  fptildemin = (1.0/2.0/pi) * pow((4.0 * b / 5.0), (1.0/4.0));
  aX  = (log(a)-log(aC))/log(fptildemin);
  gX  = -log(gC)/log(fptildemin);
  vA = pow(g, 2) * pow((2*pi), -4);
}

double function_j(double f, double fp, double sigma, double alpha, double gamma) {
   double exp1arg = -1.25 * pow((f/fp),-4);
 
   double exp2arg = -0.5 * pow((f-fp)/(sigma*fp), 2);

   double S = alpha * vA * pow(f,-5) * exp(exp1arg) * pow(gamma, exp(exp2arg));

   return S;
}

int main() {
  // Initialize global variables
  init();

  double S, f, fp, fptilde;
  for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {
    double fpt = MAX(fptilde, fptildemin);

    double alpha   = aC  * pow(fpt, aX);
    double gamma   = gC  * pow(fpt, gX);
    double sigma_a = saC * pow(fpt, saX);
    double sigma_b = sbC * pow(fpt, sbX);
    for (f = -5.; f <= 5.; f += 0.01) {
        for (fp = 0.; fp <= 10.; fp += 0.01) {
            double sigma = (f <= fp) * sigma_a + (f > fp) * sigma_b;
            S = function_j(f, fp, sigma, alpha, gamma);
        }
    }

  }
  return 0;
}
