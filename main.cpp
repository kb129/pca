#include <iostream>
#include <vector>

#include "Eigen/Core"
#include "Eigen/Dense"

extern "C"
{
    #include "MNISTread.h"
}

using namespace std;
using namespace Eigen;

int main(int argc, char **argv)
{
    FILE *fp;
    int nimg;
    int ndim;
    int nrow;
    int ncol;

    /* 引数チェック */
    if (argc < 2)
        exit(-1);

    /* ファイルオープン */
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        exit(-1);
    }

    read_header_MNISTimage(fp, &nimg, &nrow, &ncol);
    ndim = nrow * ncol;

    /* 行列を定義 */
    MatrixXd img_matrix(nimg, ndim); // 画像データを格納する配列

    /* 画像読み込み */
    cout << "Read images..." << endl;
    for (int i = 0; i < nimg; i++)
    {
        for (int j = 0; j < ndim; j++)
        {
            img_matrix(i, j) = read_grayscale_pixel(fp);

            if (img_matrix(i, j) < 0.0 || img_matrix(i, j) > 1.0)
            {
                cerr << "WARNING(image " << i << "/" << nimg
                     << ", dim " << j << "/" << ndim << "): value = " << img_matrix(i, j) << endl;
            }
        }
    }
    cout << "Done." << endl;
    
    /* 平均ベクトルを求める */
    VectorXd v_mean(ndim); // 平均ベクトル
    for(int i=0; i<ndim; i++){
        int sum = 0;
        for(int j=0; j<nimg; j++){
            sum += img_matrix(j, i);
        }
        v_mean(i);
    }

    /* データ行列から平均ベクトルを引く */
    MatrixXd img_matrix_bar(nimg, ndim);
    for(int i=0; i<nimg; i++){
        for(int j=0; j<ndim; j++){
            img_matrix_bar(i, j) = img_matrix(i, j) - v_mean(j);
        }
    }
    
    /* 共分散行列を求める */
    MatrixXd sigma(ndim, ndim);
    sigma = img_matrix_bar.transpose() * img_matrix_bar / (double)nimg;
    

    /* 固有値を求める */
    SelfAdjointEigenSolver<MatrixXd> eigensolver(sigma);
    VectorXd values(ndim);
    if(eigensolver.info()!=Success) abort();
    values = eigensolver.eigenvalues();
    cout << values << endl;


    /* 固有値をソート */
    vector<double> vec;
    for(int i=0; i<ndim; i++){
        vec.push_back(values(i));
    }
    sort(begin(vec), end(vec), greater<double>());

    for(int i=0; i<ndim; i++){
        //fprintf(stderr, "%e\n", vec[i]);
    }

    /* 全分散量を計算 */
    double v = 0;
    for(int i=0; i<ndim; i++){
        v += vec[i];
    }

    /* 累積寄与率を設定 */
    double r;
    int m; // 累積寄与率rを満たすために必要な固有値の個数
    scanf("%d", &r);
    double sum = 0;
    while(sum < r){

        sum += vec[m];
        m++;
    }



    return 0;
}
#if 0 
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

#endif