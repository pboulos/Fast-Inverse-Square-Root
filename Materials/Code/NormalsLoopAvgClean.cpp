#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>
#include <math.h>
#include <windows.h>

using namespace std;
using namespace cv;

double PCFreq = 0.0;
__int64 CounterStart = 0;

float qrsqrt(float number) {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
     
        x2 = number * 0.5F;
        y = number;
        i = *(long *) &y;
        i = 0x5f3759df - (i >> 1);
        y = *(float *) &i;
        y = y * (threehalfs - (x2 * y * y));
        // y = y * (threehalfs - (x2 * y * y));
           
        return y;
}

float slowsqrt(float number)
{
        /* naive method */
        float output = 1.0f / sqrtf(number);
		return output;
}

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    printf("QueryPerformanceFrequency failed!\n");

	// Returns result in ns.
     
    PCFreq = double(li.QuadPart)/1000000.0;
     
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

int main()
{
	int framenumber = 0;

	// Create logfile to put framenumber, roots calculated,
	// and the time in ns taken to compute. 

	std::ofstream datafile;
	datafile.open("Log.txt");
    
	std::ifstream infile("pnglist.txt");
	std::string line;

	double tick = 0;

	while (std::getline(infile, line))
	{
		// Load frame corresponding to line in pnglist.txt.

		Mat depth = cv::imread(line);
		cvtColor(depth, depth, CV_RGB2GRAY);
		depth.convertTo(depth, CV_32FC1);

		Mat normals(depth.size(), CV_32FC3);

		int counter = 0;

		float comptime = 0;
		float summed;
		float sq0;
		float sq1;
		float sq2;
		float mult;
		long i;
		float x2;
		const float threehalfs = 1.5F;

		double tick = 0;

		// Loop over each normal calculation 100 times in order
		// to account for variability in processor calculations.

		for(int c = 0; c < 100; ++c)
		{
			StartCounter();

			for(int x = 1; x < depth.cols; ++x)
			{
				for(int y = 1; y < depth.rows; ++y)
				{

					Vec3f n;
			
					if (depth.at<float>(y,x) == 0)
					{
						// Skip computation if pixel is black.
						// Produces black background as opposed to red.

						Vec3f an(1,1,1);

						n = an;

					}
					else
					{

						// 3d pixels, think (x,y, depth)
						/* * * * *
						 * * t * *
						 * l c * *
						 * * * * */

						Vec3f t(x,y-1,depth.at<float>(y-1, x));
						Vec3f l(x-1,y,depth.at<float>(y, x-1));
						Vec3f c(x,y,depth.at<float>(y, x));

						Vec3f d = (l-c).cross(t-c);

						// OpenCV has a built-in normalisation function.
						// We'll avoid using it, and instead explicitly
						// compute the normal so we can substitute our desired
						// InvSqrt function in to compare computation speed.

						//Vec3f n = normalize(d);

						sq0 = pow(d(0), 2);
						sq1 = pow(d(1), 2);
						sq2 = pow(d(2), 2);

						summed = (sq0 + sq1 + sq2);

						// The constant 0.6489 is the mean of all the summed pixels
						// upon which the normalisation will be performed.
						// This is used as a standard on which to compare which method
						// is fastest while neglecting other computational resources used
						// for other steps during the normalisation process.
						// Since this is for testing purposes, commenting and uncommenting
						// the desired method will yield a log file with the computation times
						// for the respective InvSqrt method.

						// mult = 0.6489;

						// mult = slowsqrt(summed);

						mult = qrsqrt(summed);

						Vec3f an(d(0), d(1), d(2));

						n = (an * mult);

						counter +=1;
					
					}

					normals.at<Vec3f>(y,x) = n;
				}
			}

			tick += GetCounter();

		}

		tick = tick/100;
		counter = counter/100;

		normals *= 255;
		normals.convertTo(normals, CV_8UC3);

		datafile << framenumber << "," << counter << "," << tick << "\n";

		string filename(line);
		filename.erase(filename.length()-4);
		filename += "_normalised.png";
		imwrite(filename, normals);

		framenumber += 1;

	}

	cout << tick;
  
	datafile.close();

    waitKey(0);

    return 0;
}