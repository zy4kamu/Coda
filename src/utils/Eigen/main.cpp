#include "eigen.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
//#include "Tools.h"

using std::wcout;
using std::wcin;
using std::locale;
using std::string;

int main()
{
	Eigen::setNbThreads(3);
//	struct timeval start, end;
//	srand(time(NULL));
//gettimeofday(&start, NULL);

	eigentools::DenseMat a(10000, 1000);
	a.setRandom();
	MatrixXd b = a.transpose();
	eigentools::DenseMat c = b*a;

//	gettimeofday(&end, NULL);
//	double delta = ((end.tv_sec - start.tv_sec) * 1000000u +
//		end.tv_usec - start.tv_usec) / 1.e6;
//	std::cout << delta << std::endl;

//    std::vector<float> v;
//    eigentools::eigenVec2StlVec(a,v);
//    for (size_t i = 0; i < v.size(); i++)
//        std::cout<<v[i]<<std::endl;
//	std::cout << c << std::endl;
	getchar();
}
