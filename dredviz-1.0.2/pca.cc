#include "pca.hh"

namespace dredviz {
PCA::PCA(const MatrixType& inputdata)
  : data(inputData.getCols(), inputData.getRows()),
  components(inputdata.getCols(), inputdata.getCols())
{
  //The data vectors are row vectors in inputdata, but
  //here we'd rather have them be column vectors.

  for(size_t i = 0; i < inputdata.getRows(); i++)
    for(size_t j = 0; j < inputdata.getCols(); j++)
      //nm::Matrix indices start from 1
      data(j+1, i+1) = inputdata(i,j);

  compute_pca();
}

template <typename MatrixType> void PCA<MatrixType>::compute_pca()
{

  /* Make sure that the mean of the data vectors is zero. */

  ColumnVector mean = data.getCol(1);

  for(size_t i = 2; i <= data.NCols(); i++)
  {
    mean += data.getCol(i);
  }

  mean /= data.NCols();

  for(size_t i = 1; i <= data.NCols(); i++)
  {
    data.getCol(i) -= mean;
  }


  /* Compute covariance matrix and store eigenvalues and -vectors. */

  nm::SymmetricMatrix cov;
  cov << data * data.t() / data.NCols();
  EigenValues(cov, eigenvalues, components);
}
