#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
#define smprate 1000

int main(int argc, char** argv)
{
    fstream infile;
    double slope = 0.005, Max, r_time, confidence = 0, threshold = 0.55;
    double time, sample, pre_sample=INFINITY;

    if(argc < 2){
        cout << "usage: infile [threshold]" << endl;
        return 1;
    }
    else if(argc == 3)
        threshold = atof(argv[2]);

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }


    bool QRS = false;
    vector<pair<double, double>> R;

    while(!infile.eof()){
        infile >> time >> sample;

        if(QRS){
            if(sample > Max){
                Max = sample;
                r_time = time;
            }

            if(pre_sample - sample > slope){
                QRS = false;
                if(Max > threshold){
                    pair<double, double> temp;
                    temp.first = r_time;
                    temp.second = Max;
                    R.push_back(temp);
                    //cout << temp.first << "\t" << temp.second << endl;
                }
            }
        }
        else{
            if(sample - pre_sample > slope)
                confidence++;
            else
                confidence = 0;

            if(confidence > 3){
                QRS = true;
                Max = -INFINITY;
                confidence = 0;
            }
        }

        pre_sample = sample;
    }

    double interval;
    for(auto i = R.begin(); i+1 != R.end(); i++){
        interval = ((i+1)->first - i->first);
        i->first += interval/2;
        i->second = interval;
        cout << i->first << "\t" << i->second << endl;
    }

    return 0;
}