#include <iostream>
#include <fstream>
#include <deque>
#include <cmath>
#include <iomanip>
using namespace std;
#define smprate 1000

int main(int argc, char** argv)
{
    fstream infile, outfile;
    deque<double> mask;

    if(argc != 3)
        return 1;
    double time, sample;

    infile.open(argv[1], fstream::in);
    outfile.open(argv[2], fstream::out);
    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    int count = 0;
    double sum = 0.;
    while(!infile.eof()){
        infile >> time >> sample;
        sum+=sample;
        mask.push_back(sample);
        if(count < 500)
            count++;
        else{
            sum-=mask.front();
            mask.pop_front();
        }
        outfile << time-0.25 << "\t"<< setprecision(6) << sum/count << endl;
    }

    return 0;
}