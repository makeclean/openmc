//! \file mgxs_interface.h
//! A collection of C interfaces to the C++ Mgxs class

#ifndef OPENMC_MGXS_INTERFACE_H
#define OPENMC_MGXS_INTERFACE_H

#include "hdf5_interface.h"
#include "mgxs.h"

#include <vector>

namespace openmc {

//==============================================================================
// Global MGXS data container structure
//==============================================================================

struct MgxsInterface
{
  int num_energy_groups;
  int num_delayed_groups;

  // List of available names in the HDF5 file
  std::vector<std::string> xs_names;
  std::vector<std::string> xs_to_read;
  std::vector<std::vector<double>> xs_temps_to_read;

  // Name of the HDF5 file which contains mgxs
  std::string cross_sections_path;

  std::vector<Mgxs> nuclides_MG;
  std::vector<Mgxs> macro_xs;

  std::vector<double> energy_bins;
  std::vector<double> energy_bin_avg;
  std::vector<double> rev_energy_bins;

  // temperatues of each available nuclide
  std::vector<std::vector<double>> nuc_temps;

  MgxsInterface() = default;

  // Construct from path to cross sections file, as well as a list
  // of XS to read and the corresponding temperatures for each XS
  MgxsInterface(const std::string& path_cross_sections,
                const std::vector<std::string> xs_to_read,
                const std::vector<std::vector<double>> xs_temps);
  void set_nuclides_to_read(std::vector<std::string> arg_xs_to_read);
  void set_nuclide_temperatures_to_read(std::vector<std::vector<double>> xs_temps);

  void init();

  void add_mgxs(hid_t file_id, const std::string& name,
         const std::vector<double>& temperature);

  void create_macro_xs();

  std::vector<std::vector<double>> get_mat_kTs();

  // Reads just the header of the cross sections file, to find
  // min & max energies as well as the available XS
  void read_header(const std::string& path_cross_sections);
};

namespace data {
  extern MgxsInterface mgInterface;
}

// Puts available XS in MGXS file to globals so that when
// materials are read, the MGXS specified in a material can
// be ensured to be present in the available data.
void put_mgxs_header_data_to_globals();

// Set which nuclides and temperatures are to be read on
// mgInterface through global data
void set_mg_interface_nuclides_and_temps();

// After macro XS have been read, materials can be marked as fissionable
void mark_fissionable_mgxs_materials();

//==============================================================================
// Mgxs tracking/transport/tallying interface methods
//==============================================================================

extern "C" void
calculate_xs_c(int i_mat, int gin, double sqrtkT, Direction u,
     double& total_xs, double& abs_xs, double& nu_fiss_xs);

double
get_nuclide_xs(int index, int xstype, int gin, const int* gout,
  const double* mu, const int* dg);

inline double
get_nuclide_xs(int index, int xstype, int gin)
{return get_nuclide_xs(index, xstype, gin, nullptr, nullptr, nullptr);}

double
get_macro_xs(int index, int xstype, int gin, const int* gout,
  const double* mu, const int* dg);

inline double
get_macro_xs(int index, int xstype, int gin)
{return get_macro_xs(index, xstype, gin, nullptr, nullptr, nullptr);}

//==============================================================================
// General Mgxs methods
//==============================================================================

extern "C" void
get_name_c(int index, int name_len, char* name);

extern "C" double
get_awr_c(int index);

} // namespace openmc
#endif // OPENMC_MGXS_INTERFACE_H
