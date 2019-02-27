#ifndef VARIANCE_REDUCTION_H
#define VARIANCE_REDUCTION_H 1

#include <map>
#include "hdf5.h"

#include "openmc/xml_interface.h"
#include "openmc/error.h"

namespace openmc {
namespace variance_reduction {

  extern bool importance_splitting;
  extern bool weight_splitting;
  extern std::map<int,double> importances;

  void read_variance_reduction(pugi::xml_node *node);

  void read_importances(pugi::xml_node *node);

  void read_weight_windows(pugi::xml_node *node);

} // end variance_reduction namespace
} // end openmc namespace

#endif