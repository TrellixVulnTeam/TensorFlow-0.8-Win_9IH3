#include <Eigen/Eigen>
#include <iostream>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


using namespace Eigen;
using namespace std;

int main(int, char**)
{
  cout.precision(3);
  Array4d v(0.5,10,0,-1);
cout << v.lgamma() << endl;
  return 0;
}
