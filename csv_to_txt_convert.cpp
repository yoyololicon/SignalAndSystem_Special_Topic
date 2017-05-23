#include <iostream>
#include <fstream>
using namespace std;
#define smprate 1000

int main(int argc, char **argv) {
    fstream infile, outfile;
    int h, m, s;
    double table[smprate];

    if(argc == 1)
        return 1;
    double interval = 1./smprate, time = 0.;
    for(int i = 0; i < smprate; i++, time+=interval)
        table[i] = time;

    infile.open(argv[1], fstream::in);
    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }
    infile >> h;
    infile.ignore(1, ':');
    infile >> m;
    infile.ignore(1, ':');
    infile >> s;
    infile.ignore(1, '.');
    int offset = h*3600 + m*60 + s;

    time = 0;
    double v;
    string outname(argv[1]);
    outname.replace(outname.length()-3, 3, "txt");
    outfile.open(outname.c_str(), fstream::out);
    while(!infile.eof()){
        infile.ignore(1, ',');
        for(int i = 0; i < smprate; i++, infile.ignore(1, ',')){
            infile >> v;
            outfile << time + table[i]<< "\t" << v + 5.0 << endl;
        }
        infile >> h;
        infile.ignore(1, ':');
        infile >> m;
        infile.ignore(1, ':');
        infile >> s;
        infile.ignore(1, '.');
        time = h*3600 + m*60 + s - offset;
    }

    infile.close();
    outfile.close();
    return 0;
}