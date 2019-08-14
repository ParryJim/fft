//  In place complex FFT

#include <complex>

// up to 65536 point transforms
#define MAXLG2 (16)

int reverse(int N, int n);    // bit reverse number

void fft(std::complex<double>* f, int lg2N); // in place transform