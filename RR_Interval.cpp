#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
#define smprate 1000
#define fft_size 2048.0

int main(int argc, char** argv)
{
    fstream infile;

    if(argc < 3){
        cout << "usage: infile totalTime" << endl;
        return 1;
    }
    double time, sample, pre_sample=INFINITY;

    infile.open(argv[1], fstream::in);

    if(!infile.is_open()) {
        cerr << "error" << endl;
        return 1;
    }

    double slope = 0.005, Max, r_time, confidence = 0, threshold = 0.55;
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
        //cout << i->first << "\t" << i->second << endl;
    }
    R.resize(R.size()-1);

    double increment = atof(argv[2])/fft_size;
    pair<double, double> t1, t2;
    t1.first = t1.second = 0;
    time = 0;

    int count = 0;

    for(auto i = R.begin(); i != R.end(); i++){
        t2 = *i;

        while(time < t2.first){
            cout << time << "\t" << t1.second +(t2.second-t1.second)*(time - t1.first)/(t2.first - t1.first) << endl;
            time+=increment;
            count++;
        }
        t1 = t2;
    }

    while(count < fft_size){
        cout << time << "\t" << 0 << endl;
        time+=increment;
        count++;
    }

    return 0;
}