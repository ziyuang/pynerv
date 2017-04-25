import numpy as np
from scipy.spatial.distance import pdist, squareform
from sklearn.decomposition import PCA
from enum import Enum
from pynerv import nerv


class NeRV:
    class NeRVInit(Enum):
        PCA = 0
        RandInit = 1

    def __init__(self, n_components=2, lmbd=0.1, neighbors=20, init=NeRVInit.PCA, random_state=None, 
        metric='euclidean', iterations=10, weights=None, initrad=None, cg_steps=2, cg_steps_final=40, record_file_stem=None):
        """
        A Python wrapper for Neighbor Retrieval Visualizer (NeRV)
        :param n_components: 
            The desired dimension of the output data. 
            It should be a positive integer. The default value for `n_components` is 2. 
            Note that n_components is ignored if initialization data is passed via `init`, 
            because the dimension of the output data must have the same dimension as the initialization.
        :param lmbd: 
            Control the trustworthiness-continuity tradeoff. 
            A value of zero emphasizes trustworthiness only; 
            increasing `lmbd` shifts the emphasis towards continuity. 
            `lmbd` must be a floating point number between 0 and 1, 
            but in practice values higher than 0.5 are not recommended. 
            The default value for `lmbd` is 0.1.
        :param neighbors: 
            Set the number of neighbors that each point should have: 
            each point will consider the `neighbors` points closest to it its neighbors. 
            `neighbors` should be a positive integer. 
            The default value for `neighbors` is 20.
        :param init: 
            Initialization of embedding.
            `init` == NeRV.NeRVInit.PCA:
                Use PCA result of the input data to initialize the embedding when `metric` != "precomputed".
                If `metric` == "precomputed", fall back to NeRV.NeRVInit.RandInit;
            `init` == NeRV.NeRVInit.RandInit:
                Use a random layout to initialize the embedding;
            `init` can be converted to an instance of numpy.ndarray:
                Use the array to initialize the embedding, which also determines `n_components`.
        :param random_state: 
            When not None, it is passed to numpy.random.seed to initialize the pseudo number generator. 
        :param metric: 
            The way to calculate the pairwise distances between the input data points. 
            `metric` is either "precomputed" or a string/function allowed by scipy.spatial.distance.pdist, 
            which is used for distance calculation.
            When `metric` == "precomputed", the input data `X` is assumed to be a pairwise distance matrix. 
        :param iterations: 
            The number of iterations to perform. The default value is 10.
        :param weights: 
            Weights of each data point's contribution to the cost function. 
            `weights` should contain as many positive floating numbers as there are data points.
        :param initrad: 
            The initial value for the global radius parameter. 
            The default is 0.5 * the maximum distance in the original data.
        :param cg_steps: 
            The number of conjugate gradient steps to perform per iteration in NeRV's optimization scheme. 
            The default value is 2.
        :param cg_steps_final: 
            The number of conjugate gradient steps to perform on the final iteration in NeRV's optimization scheme. 
            The default value is 40.
        :param record_file_stem: 
            When `record_file_stem` is not None but a string, 
            NeRV will output the current embedding to a separate file after each optimization step. 
            The filenames will be of the form ${record_file_stem}_${step_number}.dat. 
        """
        self.n_components = n_components
        self.lmbd = lmbd
        self.neighbors = neighbors
        self.init = init
        self.random_state = random_state
        self.metric = metric
        self.iterations = iterations
        self.weights = weights
        self.initrad = initrad
        self.cg_steps = cg_steps
        self.cg_steps_final = cg_steps_final
        self.record_file_stem = record_file_stem
        self.cost_ = None
        self.embedding_ = None

        if random_state is not None:
            print('Using {seed} as RNG seed.'.format(seed=random_state))
            np.random.seed(random_state)
        print('lambda = %g' % self.lmbd)
        print('Effective number of neighbors = %d' % self.neighbors)

    def fit(self, X):
        X = np.asarray(X)
        n = X.shape[0]

        if self.metric == 'precomputed':
            D = X
        else:
            D = squareform(pdist(X, metric=self.metric))

        if self.init != NeRV.NeRVInit.PCA or self.metric == 'precomputed':
            if self.init == NeRV.NeRVInit.RandInit:
                embedding_init = np.random.rand(n, self.n_components)
            else:
                embedding_init = np.asarray(self.init)
        else:
            pca = PCA(n_components=self.n_components)
            embedding_init = pca.fit_transform(X)
        self.n_components = embedding_init.shape[1]

        if self.weights is None:
            self.weights = np.ones(n)
        else:
            self.weights = np.asarray(self.weights)
        assert self.weights.shape == (n,)

        diameter = np.max(D)
        if self.initrad is None:
            self.initrad = diameter / 2.0
            
        print('Initial radius = %g' % self.initrad)
        print('Diameter of input data set = %g' % diameter)
        print('Number of iterations = %d' % self.iterations)
        print('Conjugate gradient steps per iteration = %d' % self.cg_steps)
        print('Conjugate gradient steps on final iteration = %d' % self.cg_steps_final)

        self.embedding_, self.cost_ = \
            nerv(D, self.lmbd, self.neighbors, embedding_init, self.n_components, self.weights,
                 self.initrad, self.iterations, self.cg_steps, self.cg_steps_final, self.record_file_stem)

    def fit_transform(self, X):
        self.fit(X)
        return self.embedding_


if __name__ == '__main__':
    import matplotlib.pyplot as plt
    data = np.genfromtxt('dredviz-1.0.2/spheredata.dat', skip_header=1)
    color = np.genfromtxt('dredviz-1.0.2/spheredata.col', skip_header=1)
    nerv_obj = NeRV()
    nerv_obj.fit(data)
    print('Final cost = %g' % nerv_obj.cost_)
    plt.scatter(*nerv_obj.embedding_.T, c=color)
    plt.show()