# PyNeRV
A Python wrapper of the dimensionality reduction algorithm Neighbor Retrieval Visualizer ([NeRV](http://research.cs.aalto.fi/pml/software/dredviz/))

## Compile
Set up the paths in `Makefile` then `make`.

## Usage example

    import numpy as np
    import matplotlib.pyplot as plt
    from nerv import NeRV
    
    data = np.genfromtxt('dredviz-1.0.2/spheredata.dat', skip_header=1)
    color = np.genfromtxt('dredviz-1.0.2/spheredata.col', skip_header=1)
    nerv_obj = NeRV()
    nerv_obj.fit(data)
    print('Final cost = %g' % nerv_obj.cost_)
    plt.scatter(*nerv_obj.embedding_.T, c=color)
    plt.show()
