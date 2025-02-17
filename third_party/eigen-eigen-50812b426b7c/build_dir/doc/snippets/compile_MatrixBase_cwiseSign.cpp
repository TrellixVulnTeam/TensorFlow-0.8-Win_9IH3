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
  MatrixXd m(2,3);
m <<  2, -4, 6,
     -5,  1, 0;
cout << m.cwiseSign() << endl;

  return 0;
}
