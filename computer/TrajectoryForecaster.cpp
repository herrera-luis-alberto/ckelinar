#include "TrajectoryForecaster.h"
#include <netcdfcpp.h>
#include <math.h>
#include <iostream>
using std::cout;
using std::flush;
using std::endl;

double randn(double mu=0.0, double sigma=1.0) {
	static bool deviateAvailable=false;
	static float storedDeviate;
	double dist, angle;

	if (!deviateAvailable) {
		dist=sqrt( -2.0 * log(double(rand()) / double(RAND_MAX)) );
		angle=2.0 * M_PI * (double(rand()) / double(RAND_MAX));
		storedDeviate=dist*cos(angle);
		deviateAvailable=true;
		return dist * sin(angle) * sigma + mu;
	}

	else
	{
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}


TrajectoryForecaster::TrajectoryForecaster(const string &file)
//safety first ;-)
	: lat_data ( NULL )
	, lon_data ( NULL )
	, u_gr_p_data ( NULL )
	, v_gr_p_data ( NULL )
	, Z_p_data ( NULL )
	, lat ( NULL )
	, lon ( NULL )
	, u_gr_p ( NULL )
	, v_gr_p ( NULL )
	, Z_p ( NULL )
{

	cout<<"Reading data..."<<flush;

	NcFile data(file.c_str());

	cout<<"XLAT..."<<flush;
	lat = read3DimVar(data, "XLAT", lat_data);
	cout<<"OK"<<endl<<"XLONG..."<<flush;
	lon = read3DimVar(data, "XLONG", lon_data);
	cout<<"OK"<<endl<<"U..."<<flush;
	u_gr_p = read4DimVar(data, "U", &u_gr_p_data);
	cout<<"OK"<<endl<<"V..."<<flush;
	v_gr_p = read4DimVar(data, "V", &v_gr_p_data);
	cout<<"OK"<<endl<<"PHB..."<<flush;
	Z_p = read4DimVar(data, "PHB", &Z_p_data);
	cout<<"OK";

	int zSize = Z_p->shape()[0]*Z_p->shape()[1]*Z_p->shape()[2]*Z_p->shape()[3];

	for ( int i=0; i<zSize; i++)
		Z_p_data[i] = Z_p_data[i]/9.8;



	readTime( data );

	cout<<"Done!"<<endl;


}

TrajectoryForecaster::~TrajectoryForecaster()
{
	delete [] lat_data;
	delete [] lon_data;
	delete [] u_gr_p_data;
	delete [] v_gr_p_data;
	delete [] Z_p_data;

	delete lat;
	delete lon;
	delete u_gr_p;
	delete v_gr_p;
	delete Z_p;

}


EarthTrajectory TrajectoryForecaster::getTrayectory(const EarthPoint4D &begin, float upSpeed, float deltaT, float sigmaNoise)
{
	EarthTrajectory result;
	result.push_back( begin );


	EarthPoint4D pre = begin;
	EarthPoint4D post;

	int iterations = 0;

	while ( iterations < 1000 )
	{
		if ( iterationStep( pre, post, upSpeed, deltaT, sigmaNoise) == false )
			break;
		result.push_back( post );

		pre = post;
		iterations++;
	}

	return result;
}

bool TrajectoryForecaster::iterationStep( const EarthPoint4D &begin, EarthPoint4D &end, float upSpeed, float deltaT, float sigmaNoise)
{
	int latIndex, lonIndex, heightIndex, timeIndex;
	if ( getArrayIndex( begin, &latIndex, &lonIndex, &heightIndex, &timeIndex) == false )
		return false;

	// the next lines just interpolate data as shown in
	// http://en.wikipedia.org/wiki/Bilinear_interpolation


	//interpolate the wind in the lower layer
	float uAddL = bilinearInterpolation(begin.latitude, begin.longitude, u_gr_p, timeIndex, heightIndex, latIndex, lonIndex);
	float vAddL = bilinearInterpolation(begin.latitude, begin.longitude, v_gr_p, timeIndex, heightIndex, latIndex, lonIndex);

	float uAddH = bilinearInterpolation(begin.latitude, begin.longitude, u_gr_p, timeIndex, heightIndex + 1, latIndex, lonIndex);
	float vAddH = bilinearInterpolation(begin.latitude, begin.longitude, v_gr_p, timeIndex, heightIndex + 1, latIndex, lonIndex);

	//interpolate heightlation(latAlpha, (*Z_p)[timeIndex][heightIndex][latIndex][lonIndex+1], (*Z_p)[timeIndex][heightIndex][latIndex+1][lonIndex+1]);
	float heightLevelL = bilinearInterpolation(begin.latitude, begin.longitude, Z_p, timeIndex, heightIndex, latIndex, lonIndex);
	float heightLevelH = bilinearInterpolation(begin.latitude, begin.longitude, Z_p, timeIndex, heightIndex + 1, latIndex, lonIndex);

	float heightAlpha = (heightLevelH-begin.height) / (heightLevelH-heightLevelL);

	float uAdd = linearInterpolation(heightAlpha, uAddL, uAddH)+randn(0, sigmaNoise);
	float vAdd = linearInterpolation(heightAlpha, vAddL, vAddH)+randn(0, sigmaNoise);

	const float dx = 111195;
	end.latitude = begin.latitude + vAdd*deltaT/dx;
	end.longitude = begin.longitude + uAdd*deltaT/( dx*cos( deg2rad(end.latitude) ) );
	end.height = begin.height + upSpeed*deltaT;
	end.time = begin.time + boost::posix_time::seconds(deltaT);

	return true;

}

float TrajectoryForecaster::deg2rad( float deg)
{
	return deg*M_PI/180.0;
}

float TrajectoryForecaster::linearInterpolation ( float alpha, float lValue, float hValue)
{
	if ( lValue == -999999)
		return hValue;
	if ( hValue == -999999)
		return lValue;
	return alpha*lValue+(1-alpha)*hValue;
}


bool TrajectoryForecaster::getArrayIndex(const EarthPoint4D &point, int *latIndex, int *lonIndex, int *heightIndex, int *timeIndex)
{
	for ( *timeIndex = time.size()-1; *timeIndex >= 0; (*timeIndex)-- )
		if ( time[*timeIndex] <= point.time )
			break;

	for ( *latIndex = lat->shape()[1]-1; *latIndex >= 0; (*latIndex)-- )
		if ( (*lat)[0][*latIndex][0] <= point.latitude )
			break;

	for ( *lonIndex = lon->shape()[2]-1; *lonIndex >= 0; (*lonIndex)-- )
		if ( (*lon)[0][0][*lonIndex] <= point.longitude )
			break;

	if ( *timeIndex < 0 || *timeIndex == time.size()-1)
		return false;

	if ( *latIndex < 0 || *latIndex == lat->shape()[0]-1)
		return false;

	if ( *lonIndex < 0 || *lonIndex == lon->shape()[1]-1)
		return false;

	bool lookForMinimum = false;
	for ( *heightIndex = Z_p->shape()[1]-1; *heightIndex >= 0; (*heightIndex)-- )
	{
		if ( (*Z_p)[*timeIndex][*heightIndex][*latIndex][*lonIndex] == -999999 )
			if ( lookForMinimum)
				break;
			else
				continue;

		lookForMinimum = true;
		if ( (*Z_p)[*timeIndex][*heightIndex][*latIndex][*lonIndex] <= point.height )
			break;
	}

	if ( lookForMinimum )
		(*heightIndex)++;

	if ( *heightIndex < 0 || *heightIndex == Z_p->shape()[1]-1)
		return false;
	if ( (*Z_p)[*timeIndex][(*heightIndex)+1][*latIndex][*lonIndex] == -999999 )
		return false;

	return true;

}


void TrajectoryForecaster::readTime(const NcFile &data )
{

	string startTimeString = data.get_att("START_DATE")->as_string(0);
	int underScore = startTimeString.find("_");
	startTimeString[underScore] = ' ';
	ptime startTime = boost::posix_time::time_from_string( startTimeString );

	cout<<"StartTime: "<<startTimeString<<endl;
	cout<<"StartTime: "<<startTime<<endl;

	float *minutes = read1DimVar(data, "XTIME"); //XTIME is time in minutes

	int n = data.get_var("XTIME")->num_vals();

	for ( int i=0; i<n; i++ )
	{
		time.push_back( startTime + boost::posix_time::minutes(minutes[i]) );
		cout<<time[time.size()-1]<<endl;
	}

	delete [] minutes;
}


float *TrajectoryForecaster::read1DimVar(const NcFile &data, const string& name)
{
	int u_gr_p_data_1dim = data.get_var(name.c_str())->get_dim(0)->size();
	float *result = new float[u_gr_p_data_1dim];
	data.get_var(name.c_str())->get(result,u_gr_p_data_1dim);

	return result;
}


boost::const_multi_array_ref<float, 2> *TrajectoryForecaster::read2DimVar(const NcFile &data, const string& name, float* buffer)
{
	int u_gr_p_data_1dim = data.get_var(name.c_str())->get_dim(0)->size();
	int u_gr_p_data_2dim = data.get_var(name.c_str())->get_dim(1)->size();

	buffer = new float[u_gr_p_data_1dim*u_gr_p_data_2dim];

	data.get_var(name.c_str())->get(buffer,u_gr_p_data_1dim, u_gr_p_data_2dim);

	return new boost::const_multi_array_ref<float, 2>( buffer, boost::extents[u_gr_p_data_1dim][u_gr_p_data_2dim]);
}

boost::const_multi_array_ref<float, 3> *TrajectoryForecaster::read3DimVar(const NcFile &data, const string& name, float* buffer)
{
	int u_gr_p_data_1dim = data.get_var(name.c_str())->get_dim(0)->size();
	int u_gr_p_data_2dim = data.get_var(name.c_str())->get_dim(1)->size();
	int u_gr_p_data_3dim = data.get_var(name.c_str())->get_dim(2)->size();

	buffer = new float[u_gr_p_data_1dim*u_gr_p_data_2dim*u_gr_p_data_3dim];

	data.get_var(name.c_str())->get(buffer,u_gr_p_data_1dim, u_gr_p_data_2dim, u_gr_p_data_3dim  );

	return new boost::const_multi_array_ref<float, 3>( buffer, boost::extents[u_gr_p_data_1dim][u_gr_p_data_2dim][u_gr_p_data_3dim]);
}

boost::const_multi_array_ref<float, 4> *TrajectoryForecaster::read4DimVar(const NcFile &data, const string& name, float** buffer)
{
	int u_gr_p_data_1dim = data.get_var(name.c_str())->get_dim(0)->size();
	int u_gr_p_data_2dim = data.get_var(name.c_str())->get_dim(1)->size();
	int u_gr_p_data_3dim = data.get_var(name.c_str())->get_dim(2)->size();
	int u_gr_p_data_4dim = data.get_var(name.c_str())->get_dim(3)->size();

	*buffer = new float[u_gr_p_data_1dim*u_gr_p_data_2dim*u_gr_p_data_3dim*u_gr_p_data_4dim];

	data.get_var(name.c_str())->get(*buffer,u_gr_p_data_1dim, u_gr_p_data_2dim, u_gr_p_data_3dim, u_gr_p_data_4dim  );

	return new boost::const_multi_array_ref<float, 4>( *buffer, boost::extents[u_gr_p_data_1dim][u_gr_p_data_2dim][u_gr_p_data_3dim][u_gr_p_data_4dim]);
}


float TrajectoryForecaster::bilinearInterpolation(float latitude, float longitude, boost::const_multi_array_ref<float, 4> *data, int t, int h, int latIndex, int lonIndex){
	float latAlpha = ((*lat)[0][latIndex+1][lonIndex] - latitude) / ((*lat)[0][latIndex+1][lonIndex] - (*lat)[0][latIndex][lonIndex]);
	float lonAlpha = ((*lon)[0][latIndex][lonIndex+1] - longitude) / ((*lon)[0][latIndex][lonIndex+1] - (*lon)[0][latIndex][lonIndex]);

	//interpolate the wind in the lower layer
	float tmp1 = linearInterpolation(latAlpha, (*data)[t][h][latIndex][lonIndex], (*data)[t][h][latIndex+1][lonIndex]);
	float tmp2 = linearInterpolation(latAlpha, (*data)[t][h][latIndex][lonIndex+1], (*data)[t][h][latIndex+1][lonIndex+1]);
	return linearInterpolation(lonAlpha, tmp1, tmp2);
}



