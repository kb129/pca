#include <iostream>

#include "Eigen/Core"
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

int main(void){
    MatrixXd A(2, 2);
    A << 1, 4, 1, 3;

    cout << "Here is the matrix A: \n" << A << endl;

    SelfAdjointEigenSolver<MatrixXd> eigensolver(A);
    if(eigensolver.info() != Success) abort();
    cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
    cout << "Here is a matrix whose columns are eigenvectors of A\n"
        << "coresponding to these eigenvalues:\n"
        << eigensolver.eigenvectors() << endl;

    return 0;
}
