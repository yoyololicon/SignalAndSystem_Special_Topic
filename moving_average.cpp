#include <iostream>
#include <fstream>
#include <deque>
#include <iomanip>
#include <stdlib.h>
using namespace std;
#define smprate 1000

int main(int argc, char** argv)
{
    fstream infile, outfile;
    deque<double> mask;

    if(argc < 3){
        cout << "usage: infile mask_size" << endl;
        return 1;
    }
    double time, sample;
    int mask_size = atoi(argv[2]);

    infile.open(argv[1], fstream::in);
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
        if(count < mask_size)
            count++;
        else{
            sum-=mask.front();
            mask.pop_front();
        }
        cout << time-mask_size/2000 << "\t"<< setprecision(6) << sum/count << endl;
    }

    return 0;
}