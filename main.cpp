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

int need_num_of_contrib_ratio(vector<double> v, double v_total, double r, int ndim)
{
    int m = 0; // 累積寄与率rを満たすために必要な固有値の個数
    double sum = 0;

    while (sum < r * v_total)
    {
        sum += v[m++];
    }
    return m;
}

int main(int argc, char **argv)
{
    FILE *fp;
    int nimg;
    int ndim;
    int nrow;
    int ncol;

    /* 引数チェック */
    if (argc < 2) exit(-1);

    /* ファイルオープン */
    if ((fp = fopen(argv[1], "rb")) == NULL) exit(-1);

    /* ヘッダ読み込み */
    read_header_MNISTimage(fp, &nimg, &nrow, &ncol);
    ndim = nrow * ncol;

    /* 行列を定義 */
    MatrixXd img_matrix(nimg, ndim); // 画像データを格納する配列

    /* 画像読み込み */
    cerr << "Read images..." << endl;
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

    /* 平均ベクトルを求める */
    VectorXd v_mean(ndim); // 平均ベクトル
    for (int i = 0; i < ndim; i++)
    {
        int sum = 0;
        for (int j = 0; j < nimg; j++)
        {
            sum += img_matrix(j, i);
        }
        v_mean(i) = sum;
    }

    /* データ行列から平均ベクトルを引く */
    MatrixXd img_matrix_bar(nimg, ndim);
    for (int i = 0; i < nimg; i++)
    {
        for (int j = 0; j < ndim; j++)
        {
            img_matrix_bar(i, j) = (double)img_matrix(i, j) - (double)v_mean(0);
        }
    }

    /* 共分散行列を求める */
    MatrixXd sigma = img_matrix_bar.transpose() * img_matrix_bar / (double)nimg;

    /* 固有値を求める */
    SelfAdjointEigenSolver<MatrixXd> eigensolver(sigma);
    VectorXd values(ndim);
    if (eigensolver.info() != Success)
        abort();
    values = eigensolver.eigenvalues();

    /* 固有値をソート */
    vector<double> val;
    for (int i = 0; i < ndim; i++)
    {
        val.push_back((double)values(i));
    }
    sort(begin(val), end(val), greater<double>());

    /* 全分散量を計算 */
    double v_total = 0;
    for (int i = 0; i < ndim; i++)
    {
        v_total += val[i];
    }

    /* 累積寄与率を設定 */
    int n;
    for (int i = 0; i < 1000; i++)
    {
        n = need_num_of_contrib_ratio(val, v_total, i / 1000.0, ndim);
        cout << n << " " << i / 1000.0 << endl;
    }

    return 0;
}
