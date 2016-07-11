#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/monitor.h"
#include "monitors/freq.h"

using namespace std;



namespace qbox {

    void monitor::set_freq(shared_ptr<freq_data> new_freq) {
        //freq = unique_ptr<double[]> (new double(*new_freq));
        freq = new_freq;
    }

    void monitor::set_F(Field2D *newF) {
        F = newF;
    }

    int get_direction(vector<int> p1, vector<int> p2) {
        if (p1[0] == p2[0])
            return 1;
        else if (p1[1] == p2[1])
            return 0;
        else
            return 2;
    }
}
