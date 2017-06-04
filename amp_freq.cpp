#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
#define smprate 1000

struct Cycle
{
    double max_peak = 0, min_peak= 0, Crest_t= 0;
};

int main(int argc, char** argv)
{
    fstream infile, outfile;
    vector<Cycle> cycles;
    double max = -INFINITY, r_time, pre_sample = 0;
    int up_c = 0, confidence = 250;
    double time, sample;

    if(argc < 2){
        cout << "usage: infile.txt [confidence]" << endl;
        return 1;
    }
    if(argc == 3)
        confidence = atoi(argv[2]);

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    string filename = string(argv[1]);
    unsigned int pos = filename.find(".txt");
    filename.insert(pos, "_freq");
    outfile.open(filename.c_str(), fstream::out);


    bool write = false;
    while(!infile.eof()){

        infile >> time >> sample;
        if(up_c == confidence){
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
        else if(sample > pre_sample && up_c < confidence){
            up_c++;
        }

        pre_sample = sample;
    }

    infile.clear();
    infile.seekg(0);
    infile >> time >> sample;
    while(time != cycles.begin()->Crest_t)
        infile >> time >> sample;

    double Min;
    for(auto i = cycles.begin()+1; i != cycles.end(); i++) {
        Min = INFINITY;
        while(time != i->Crest_t){
            infile >> time >> sample;
            Min = min(sample, Min);
        }
        (i-1)->min_peak = Min;
    }

    double aveg_amp = 0, aveg_interval = 0;
    for(auto i = cycles.begin(); i+1 != cycles.end(); i++) {
        double interval = (i+1)->Crest_t - i->Crest_t;
        outfile <<  1/interval << endl;
        aveg_interval += interval;
        aveg_amp += i->max_peak-i->min_peak;
    }

    aveg_amp/=cycles.size()-1;
    aveg_interval/=cycles.size()-1;
    cout << "total " << cycles.size() << " peaks" << endl;

    cout << "average freq: " << 1/aveg_interval << ", average amp: " << aveg_amp << endl;

    return 0;
}