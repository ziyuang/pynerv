#ifndef PCA_HH
#define PCA_HH
#include <newmat.h>
#include <newmatap.h>

namespace nm = NEWMAT;

/* This class performs Principal Components Analysis on a given data set.
 * The PCA is computed in the constructor of the class. Any subset of the
 * principal components can be retrieved in a variety of formats. */

namespace dredviz {
template <typename MatrixType>
class PCA
{
  public:
  /* The constructor expects the data in a matrix form: data(i,j) should return
   * the jth element of the ith data vector as a double. */
    PCA(const MatrixType& inputdata) 
    : data(inputdata.getRows(), inputdata.getCols()),
      components(inputdata.getCols(), inputdata.getCols())
    {
      for(size_t i = 0; i < inputdata.getRows(); i++)
        for(size_t j = 0; j < inputdata.getCols(); j++)
          //Matrix indices start from 1
          data(i+1, j+1) = inputdata(i,j);

      compute_pca();
    }

  /* Copies n_c first principal components into the matrix mat, which should 
   * the same syntax as the template parameter for the constructor above.
   * The matrix is assumed to be of the appropriate size. */
    template <typename MatType> 
      void getComponents(MatType& mat) const
    {
      int N = components.Ncols();
      int n_c = mat.getCols();

      //std::cout << "data " << data.Nrows() << " " << data.Ncols() << ", comp "
      //  << components.Nrows() << " " << n_c << " "
      //  << components.Ncols()  << std::endl;
      
      //nm::Matrix projection = data * components.Columns(1,mat.getCols());

      /* The first n_c principal components are the n_c last columns of
       * 'components' */

      nm::Matrix projection = data * components.Columns(N - n_c + 1, N);
      
      for(int i = 0; i < projection.Nrows(); i++)
        for(int j = n_c; j > 0; j--)
          mat(i,n_c - j) = projection(i+1,j);
    }

  protected:
    void compute_pca()
    {

      /* Make sure that the mean of the data vectors is zero. */

      nm::RowVector mean = data.Row(1);

      for(int i = 2; i <= data.Nrows(); i++)
      {
        mean += data.Row(i);
      }

      mean /= data.Nrows();

      for(int i = 1; i <= data.Nrows(); i++)
      {
        data.Row(i) -= mean;
      }


      /* Compute covariance matrix and store eigenvalues and -vectors. */

      nm::SymmetricMatrix cov;
      cov << data.t() * data / data.Ncols();

      /* Note that eigenvalues are returned in ASCENDING order, so the
       * first principal component will be the last column of components. */
      nm::EigenValues(cov, eigenvalues, components);
    }

  private:
  nm::Matrix data;
  nm::Matrix components;
  nm::DiagonalMatrix eigenvalues;
};
}
#endif

