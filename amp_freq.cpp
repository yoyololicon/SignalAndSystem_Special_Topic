#include <iostream>
#include <fstream>
#include <aquila/aquila.h>
using namespace std;
#define smprate 1000

int main(int argc, char** argv)
{
    fstream infile;

    if(argc == 1)
        return 1;
    double interval = 1./smprate, time, sample;

    infile.open(argv[1], fstream::in);
    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    double max = 0., min = INFINITY;
    int phase = 1, count = 0;
    while(!infile.eof()){
        infile >> time >> sample;
        if(sample > max)
            max = sample;
        if(sample < min)
            min = sample;
    }

    return 0;
}