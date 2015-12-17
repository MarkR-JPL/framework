#ifndef MERRA_AEROSOL_H
#define MERRA_AEROSOL_H
#include "aerosol.h"
#include "hdf_file.h"
#include "pressure.h"
#include "composite_initial_guess.h"

namespace FullPhysics {
/****************************************************************//**
  This class is used to create the Aerosol from a Merra climatology 
  file.
*******************************************************************/
class MerraAerosol: public Printable<MerraAerosol> {
public:
  MerraAerosol(const HdfFile& Merra_climatology,
	       const HdfFile& Aerosol_property,
	       DoubleWithUnit Latitude, DoubleWithUnit Longitude,
	       const boost::shared_ptr< Pressure > &Press,
	       const blitz::Array<double, 2>& Aerosol_cov,
	       double Max_aod = 0.2,
	       double Exp_aod = 0.8,
	       int Min_types = 2,
	       int Max_types = 4,
	       double Max_residual = 0.005,
	       double Reference_wn=1e4/0.755);
  virtual ~MerraAerosol() {}
  boost::shared_ptr<Aerosol> aerosol() const;
  void add_aerosol(const boost::shared_ptr<AerosolExtinction>& Aext,
		   const boost::shared_ptr<AerosolProperty>& Aprop,
		   const boost::shared_ptr<InitialGuessBuilder>& Ig);

//-----------------------------------------------------------------------
/// Return IntitialGuessBuilder for the Aerosol.
//-----------------------------------------------------------------------

  boost::shared_ptr<InitialGuessBuilder> initial_guess() const { return ig;}

//-----------------------------------------------------------------------
/// Number of merra particles.
//-----------------------------------------------------------------------

  int number_merra_particle() const {return number_merra_particle_;}

  virtual void print(std::ostream& Os) const;
private:
  mutable boost::shared_ptr<Aerosol> aerosol_;
  int number_merra_particle_;
  std::vector<boost::shared_ptr<AerosolExtinction> > aext;
  std::vector<boost::shared_ptr<AerosolProperty> > aprop;
  boost::shared_ptr<CompositeInitialGuess> ig;
  boost::shared_ptr<Pressure> press;
  double ref_wn;
  std::string merra_fname, prop_fname;
  DoubleWithUnit lat, lon;
  int file_index(const HdfFile& Merra_climatology, 
		 const std::string& Field_name,
		 DoubleWithUnit& V, bool Wrap_around = false);
};
}
#endif
