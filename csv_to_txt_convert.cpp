#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;
#define smprate 1000

int main(int argc, char **argv) {
    fstream infile, outfile;
    int h, m, s, sign;

    if(argc != 3)
        return 1;
    double time = 0.;

    infile.open(argv[1], fstream::in);
    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }
    if(argv[2][0] == '1')
        sign = -1;
    else
        sign = 1;

    cout << "input start time " << endl;
    cin >> h >> m >> s;
    int start = h*3600 + m*60 + s;

    cout << "input end time " << endl;
    cin >> h >> m >> s;
    int end = h*3600 + m*60 + s;

    infile >> h;
    infile.ignore(1, ':');
    infile >> m;
    infile.ignore(1, ':');
    infile >> s;
    infile.ignore(1, '.');

    time = 0;
    double v;
    string outname(argv[1]);
    outname.replace(outname.length()-3, 3, "txt");
    outfile.open(outname.c_str(), fstream::out);

    bool write = false;

    while(!infile.eof()){
        for(int i = 0; i < smprate; i++, time+=0.001){
            infile.ignore(1, ',');
            infile >> v;
            if(write)
                outfile << time << "\t" << setprecision(6) << v*sign << endl;
        }
        infile >> h;
        infile.ignore(1, ':');
        infile >> m;
        infile.ignore(1, ':');
        infile >> s;
        infile.ignore(1, '.');
        time = h*3600 + m*60 + s;
        if(time >= start)
            write = true;
        if(time > end)
            write = false;
        if(write)
            time -= start;
    }

    infile.close();
    outfile.close();
    return 0;
}