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
    fstream infile, outfile;

    if(argc < 3){
        cout << "usage: infile.txt totaltime" << endl;
        return 1;
    }

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    string filename = string(argv[1]);
    unsigned int pos = filename.find(".txt");
    filename.insert(pos, "_psd");
    outfile.open(filename.c_str(), fstream::out);

    double totaltime = atof(argv[2]);

    vector<double> rawdata;

    while(infile){
        infile >> time >> interval;
        rawdata.push_back(interval);
    }

    while(n < rawdata.size())
        n*=2;

    totaltime *= n/rawdata.size();
    double dfreq = 1/totaltime;
    CArray data(n);

    for(int i = 0; i < n; i++){
        if(i < rawdata.size())
            data[i] = rawdata[i];
        else
            data[i] = 0;
    }

    fft(data);

    double lf_power = 0, hf_power = 0;
    for(int i = 0; i < n/2; i++){
        double hz = i*dfreq, power = pow(abs(data[i]), 2);
        if(hz > 0.04 && hz <= 0.15)
            lf_power += power;
        else if(hz > 0.15 && hz <= 0.4)
            hf_power += power;
        outfile << hz << "\t" << power << endl;
    }

    cout << "lf " << lf_power << "; hf " << hf_power << endl;

    return 0;
}