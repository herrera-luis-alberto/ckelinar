#ifndef GROUND_H
#define GROUND_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using std::ifstream;
using std::vector;
using std::cout;

class Ground
{
public:
        Ground(ifstream &groundfile);
        double altitude(double latitude, double longitude);

private:
        vector<int*> heights;

        int ncols;
        int nrows;
        int xllcorner;
        int yllcorner;
        double cellsize;
        int NODATA_value;
};

#endif
