#include "Ground.h"

Ground::Ground(ifstream &groundfile)
{
    string trash;
    groundfile>>trash>>ncols;
    groundfile>>trash>>nrows;
    groundfile>>trash>>xllcorner;
    groundfile>>trash>>yllcorner;
    groundfile>>trash>>cellsize;
    groundfile>>trash>>NODATA_value;

    heights = vector<int*>(nrows);
    for(int i = 0; i<nrows; i++) heights[i] = new int[ncols];

    for(int i = 0; i<nrows; i++)
    {
        for(int k = 0; k<ncols; k++)
        {
            groundfile>>heights[i][k];
        }
    }
}

double Ground::altitude(double latitude, double longitude)//latitude=y;longitude=x
{
    double X = longitude-xllcorner;
    int iX = int(X/cellsize);

    double Y = latitude-yllcorner;
    int iY = int(Y/cellsize);

    double alphaX = ((iX+1)*cellsize-X)/cellsize;
    double alphaY = ((iY+1)*cellsize-Y)/cellsize;

    double Z1 = alphaX*heights[iX][iY]  +(1-alphaX)*heights[iX+1][iY];
    double Z2 = alphaX*heights[iX][iY+1]+(1-alphaX)*heights[iX+1][iY+1];

    return alphaY*Z1+(1-alphaY)*Z2;
}
