#include <complex>
#include <iostream>
#include <fstream>
#include <valarray>
using namespace std;
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
#define FFT_size 2048

// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

int main(int argc, char **argv)
{
    double time, interval;
    fstream infile;

    if(argc < 2){
        cout << "usage: infile" << endl;
        return 1;
    }

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    CArray data(FFT_size);

    for(int i = 0; i < FFT_size; i++){
        infile >> time >> interval;
        data[i] = interval*pow(-1, i);
    }

    fft(data);
    for(int i = 0; i < FFT_size; i++){
        cout << i << "\t" << log(abs(data[i])+1) << endl;
    }

    return 0;
}