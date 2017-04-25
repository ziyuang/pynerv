/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include <iostream>
#include <string>
#include <fstream>

#include "dataset.hh"
#include "distancematrix.hh"
#include "nervcostfunction.hh"
#include "nervoptstrat.hh"
#include "goldensectionsearch.hh"
#include "conjugategradientopt.hh"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace dredviz;
namespace py = pybind11;


// Interface flexibility moves to nerv.py
py::tuple nerv(py::array_t<double>& pyInputDist,
               double lambda,
               size_t lastNeighbor,
               py::array_t<double>& init,
               size_t outputDimension,
               py::array_t<double>& pyWeights,
               double initrad,
               size_t iterations,
               size_t stepsPerRound,
               size_t stepsOnLastRound,
               py::object& pyFilenameStem
  ) {
  size_t n = pyInputDist.shape(0);
  std::vector<size_t> embedding_shape = {n, outputDimension};
  py::array_t<double> embedding(embedding_shape);
  ConjugateGradientOpt *optStep = NULL;

  try {
    DistanceMatrix origDataDist(n);
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        origDataDist(i, j) = *pyInputDist.data(i, j);
      }
    }

    std::vector<double> weights(n);
    for (size_t i = 0; i < n; i++) {
      weights[i] = *pyWeights.data(i);
    }

    DataSet projData(n, outputDimension);
    for (size_t i = 0; i < n; i++) {
      for (size_t d = 0; d < outputDimension; d++) {
        projData(i, d) = *init.data(i, d);
      }
    }
    
    origDataDist.scale (1.0 / origDataDist.getAverage ());
    DynamicDouble initialRadius (initrad, 0);
    GoldenSectionSearch linesearch;
    NeRVCostFunction costFunc (origDataDist, projData, linesearch,
                               initialRadius, lambda, lastNeighbor,
                               weights, std::cout);

    if (pyFilenameStem.is_none()) {
      optStep = new ConjugateGradientOpt(costFunc, linesearch, std::cerr);
    }
    else {
      std::string filename_stem = py::str(pyFilenameStem);
      optStep = new ConjugateGradientOpt(costFunc, linesearch, std::cerr,
                                         filename_stem);
    }

    std::string tmpoutputfile;
    NeRVOptStrat optStrategy(iterations, stepsPerRound, stepsOnLastRound, tmpoutputfile);
    optStrategy.optimize(origDataDist, projData, *optStep, costFunc, std::cerr);
    
    for (size_t i = 0; i < n; i++) {
      for (size_t d = 0; d < outputDimension; d++) {
        *embedding.mutable_data(i, d) = projData(i, d);
      }
    }
  }
  catch (Exception & exc)
  {
    std::cerr << "Error: " << exc.getErrMsg () << std::endl;
  }

  double finalCost = optStep->getFinalCost();
  return py::make_tuple(embedding, finalCost);
  delete optStep;
}


PYBIND11_PLUGIN(pynerv) {
    py::module m("pynerv", "A Python wrapper of Neighbor Retrieval Visualizer (NeRV)");
    m.def("nerv", &nerv, "NeRV's Python wrapper");
    return m.ptr();
}
