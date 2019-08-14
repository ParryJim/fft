// In place complex FFT 
// (normalized such that sum sqrs of fft = n * sum sqrs of samples)
//
// Code here is modified from
// http://https://stackoverflow.com/questions/8801158/fft-in-a-single-c-file
// where it was an answer posted by Leos313

// As modified here the effort is to allocate space statically and
// initialize as needed to provide efficient repeated calls.

#include <iostream>
#include "./fft.h"

using namespace std;

int reverse(int N, int n) {   //calculating reverse number
    int j, p = 0, lN = log2(N);
    for(j = 1; j <= lN; j++) { p <<= 1;  p |= 1 & n; n >>= 1;}
    return p;
}

int* brListInit(int lg2N) { // initialize a list of bit-reversed indices with some zeroed
    int N = 1 << lg2N;
    int* list;
    if ((list = (int *)malloc(sizeof(int) * N)) == NULL) {
        cout << "Failed to allocate list of bit reversed indices." << endl;
        exit(1);
    }
    for (int j = 0; j < N; j++) {
        int r = reverse(N, j);
        if (r <= j) r = 0;
        list[j] = r;
        //if (r) cout << "  swap " << r << " with " << j << endl;
    }
    return (list);
}

void ordina(complex<double>* f, int lg2N) {
    static int* brLists[MAXLG2] = { NULL };
    int* brList = brLists[lg2N];
    //for (int j = 0; j < MAXLG2; j++) cout << brLists[j] << " ";
    //cout << endl;
    if (brList == NULL) { brList = brLists[lg2N] = brListInit(lg2N);}
    //for (int j = 0; j < MAXLG2; j++)  cout << brLists[j] << " ";
    //cout << endl;
    int N = 1 << lg2N;
    for(int i = 0; i < N; i++) {
        //cout << " for i = " << i;
        if (int r = brList[i]) {complex<double> temp = f[i]; f[i] = f[r]; f[r] = temp;
            //cout << " swap with " << r;
        //} else {
            //cout << " do not swap";
        }
        //cout << endl;
    }
}

complex<double>* wListInit(int lg2N) { // build list of twiddlle factors
    int N = 1 << lg2N;
    complex<double> *W = (complex<double> *)malloc(sizeof(complex<double>) * N / 2);
    W[1] = polar(1., -2. * M_PI / N);
    W[0] = 1;
    for(int i = 2; i < N / 2; i++) W[i] = pow(W[1], i);
    return (W);
}

void fft(complex<double>* f, int lg2N) {
    int N = 1 << lg2N;
    ordina(f, lg2N);    // reverse order of input data
    static complex<double>* wLists[MAXLG2] = { NULL }; // reusable twiddles
    complex<double> *W = wLists[lg2N];
    if (W == NULL) {W = wLists[lg2N] = wListInit(lg2N);}
    int n = 1;
    int a = N / 2;
    for(int j = 0; j < lg2N; j++) {
        int wrap = n * a;
        for(int i = 0; i < N; i++) {
            if(!(i & n)) {
                complex<double> Temp =
                    W[(i * a) % wrap] * f[i + n];
                f[i + n] = f[i] - Temp;
                f[i] += Temp;
            }
        }
        n *= 2;
        a = a / 2;
    }
}
