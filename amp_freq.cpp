#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
#define smprate 1000

struct Cycle
{
    double max_peak = 0, min_peak= 0, Crest_t= 0, Trough_t= 0;
};

int main(int argc, char** argv)
{
    fstream infile, outfile;
    vector<Cycle> cycles;

    if(argc != 3)
        return 1;
    double time, sample;

    infile.open(argv[1], fstream::in);
    outfile.open(argv[2], fstream::out);
    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    double max = -INFINITY, r_time, pre_sample = 0;
    int up_c = 0;
    bool write = false;
    while(!infile.eof()){

        infile >> time >> sample;
        if(up_c == 300){
            if(sample > max){
                max = sample;
                r_time = time;
                write = true;
            }
        }
        else if(up_c == 0 && write){
            //cout << r_time << ";" << max << endl;
            Cycle temp;
            temp.max_peak = max;
            temp.Crest_t = r_time;
            cycles.push_back(temp);
            max = -INFINITY;
            write = false;
        }

        if(sample < pre_sample && up_c > 0){
            up_c--;
        }
        else if(sample > pre_sample && up_c < 300){
            up_c++;
        }

        pre_sample = sample;
    }

    infile.clear();
    infile.seekg(0);
    infile >> time >> sample;
    while(time != cycles.begin()->Crest_t)
        infile >> time >> sample;

    double min;
    for(auto i = cycles.begin()+1; i != cycles.end(); i++) {
        min = INFINITY;
        while(time != i->Crest_t){
            infile >> time >> sample;
            if(sample < min) {
                min = sample;
                r_time = time;
            }
        }
        (i-1)->min_peak = min;
        (i-1)->Trough_t = r_time;
    }

    double aveg_amp = 0, aveg_interval = 0;
    for(auto i = cycles.begin(); i+1 != cycles.end(); i++) {
        outfile << (i+1)->Crest_t << "\t" << 1/((i+1)->Crest_t - i->Crest_t) << endl;
        aveg_interval += (i+1)->Crest_t - i->Crest_t;
        aveg_amp += i->max_peak-i->min_peak;
    }

    aveg_amp/=cycles.size()-1;
    aveg_interval/=cycles.size()-1;
    cout << cycles.size() << endl;

    cout << "average freq: " << 1/aveg_interval << ", average amp: " << aveg_amp << endl;

    return 0;
}