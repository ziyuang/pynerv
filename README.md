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
    nerv_obj = NeRV(random_state=0)
    nerv_obj.fit(data)
    print('Final cost = %g' % nerv_obj.cost_)
    plt.scatter(*nerv_obj.embedding_.T, c=color)
    plt.show()

which shows (right)

<img src="https://raw.githubusercontent.com/ziyuang/pynerv/master/spheredata.png" width="341px"></img>&nbsp;
<img src="https://raw.githubusercontent.com/ziyuang/pynerv/master/spheredata_nerv.png" width="341px"></img>

## Reference

    @article{venna2010information,
      title={Information retrieval perspective to nonlinear dimensionality reduction for data visualization},
      author={Venna, Jarkko and Peltonen, Jaakko and Nybo, Kristian and Aidos, Helena and Kaski, Samuel},
      journal={Journal of Machine Learning Research},
      volume={11},
      number={Feb},
      pages={451--490},
      year={2010}
    }
