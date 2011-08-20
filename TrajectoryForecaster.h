#ifndef TRAJECTORYFORECASTER_H
#define TRAJECTORYFORECASTER_H

#include "EarthTrajectory.h"
#include "boost/multi_array.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class NcFile;

class TrajectoryForecaster
{
public:
	/** Read data from file and load it into memory */
	TrajectoryForecaster(const string &file);

	/** Free all the resources */
	~TrajectoryForecaster();

	/** Calculates the trayectory of the sonde starting from point begin
	  * with up speed upSpeed and calculating eatch deltaT seconds */
	EarthTrajectory getTrayectory(const EarthPoint4D &begin, float upSpeed, float deltaT, float sigmaNoise=0);

protected:
	//! data buffers
	float *lat_data;
	float *lon_data;
	float *u_gr_p_data;
	float *v_gr_p_data;
	float *pressure;
	float *Z_p_data;

	/** multi_array interface to the data, allows to use the c-style
	  * way to acces to the data */
	boost::const_multi_array_ref<float, 2> *lat;
	boost::const_multi_array_ref<float, 2> *lon;
	boost::const_multi_array_ref<float, 4> *u_gr_p;
	boost::const_multi_array_ref<float, 4> *v_gr_p;
	boost::const_multi_array_ref<float, 4> *Z_p;
	vector<ptime> time;

	/** Calculate one iteration step, returns false if calculation was not
	  * performed ( out of bound ) */
	bool iterationStep( const EarthPoint4D &begin, EarthPoint4D &end, float upSpeed, float deltaT, float sigmaNoise);

	/** Conversion from degrees to radians */
	static float deg2rad( float deg);

	/** One dimensional linear interpolation */
	static float linearInterpolation ( float alpha, float lValue, float hValue);

	/** Get the indexes of the arrays given a point in the space
	  * The index is such that val[index] is always less or equal than the point */
	bool getArrayIndex(const EarthPoint4D &point, int *latIndex, int *lonIndex, int *heightIndex, int *timeIndex);

	/** Read the time informacion from the data file */
	void readTime(const NcFile &data );

	/** Next three functions are interfaces for reading N-dimensional data */
	static float *read1DimVar(const NcFile &data, const string& name);


	static boost::const_multi_array_ref<float, 2> *read2DimVar(const NcFile &data, const string& name, float* buffer);

	static boost::const_multi_array_ref<float, 4> *read4DimVar(const NcFile &data, const string& name, float* buffer);

	/**
	 * bilinearInterpolation generates an interpolation of a point in between the points of the grid.
	 *
	 *------&------------------------------&
	 *      |                              |
	 *      |                        *     |
	 *      |                              |
	 *------&------------------------------&
	 */
	float bilinearInterpolation(float latitude, float longitude, boost::const_multi_array_ref<float, 4> *data, int t, int h, int latIndex, int lonIndex);
};

#endif
