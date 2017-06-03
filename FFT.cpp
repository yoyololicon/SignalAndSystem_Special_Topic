#include <complex>
#include <iostream>
#include <vector>
#include <fstream>
#include <valarray>
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
using namespace std;

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
    int n = 1;
    fstream infile;

    if(argc < 2){
        cout << "usage: infile totaltime" << endl;
        return 1;
    }

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    vector<double> rawdata;


    while(infile){
        infile >> time >> interval;
        rawdata.push_back(interval);
    }

    while(n < rawdata.size())
        n*=2;

    CArray data(n);

    for(int i = 0; i < n; i++){
        if(i < rawdata.size())
            data[i] = rawdata[i];
        else
            data[i] = 0;
    }

    fft(data);
    for(int i = 0; i < n; i++){
        cout << i << "\t" << pow(abs(data[i]), 2) << endl;
    }

    return 0;
}