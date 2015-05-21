#ifndef SRC_NMIE_WRAPPER_H_
#define SRC_NMIE_WRAPPER_H_
//**********************************************************************************//
//    Copyright (C) 2009-2015  Ovidio Pena <ovidio@bytesfall.com>                   //
//    Copyright (C) 2013-2015  Konstantin Ladutenko <kostyfisik@gmail.com>          //
//                                                                                  //
//    This file is part of scattnlay                                                //
//                                                                                  //
//    This program is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by          //
//    the Free Software Foundation, either version 3 of the License, or             //
//    (at your option) any later version.                                           //
//                                                                                  //
//    This program is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 //
//    GNU General Public License for more details.                                  //
//                                                                                  //
//    The only additional remark is that we expect that all publications            //
//    describing work using this software, or all commercial products               //
//    using it, cite the following reference:                                       //
//    [1] O. Pena and U. Pal, "Scattering of electromagnetic radiation by           //
//        a multilayered sphere," Computer Physics Communications,                  //
//        vol. 180, Nov. 2009, pp. 2348-2354.                                       //
//                                                                                  //
//    You should have received a copy of the GNU General Public License             //
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.         //
//**********************************************************************************//

#include <array>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>



namespace nmie {


  class MultiLayerMie {
    // Will throw for any error!
    // SP stands for size parameter units.
   public:
    void GetFailed();
    long iformat = 0;
    bool output = true;
    void prn(double var) {
      do {
	if (!output) break;
	++iformat;
	printf("%23.13e",var);	     
	if (iformat%4 == 0) printf("\n");
      } while (false);
    }
    // Set parameters in applied units 
    void SetWavelength(double wavelength) {wavelength_ = wavelength;};
    // It is possible to set only a multilayer target to run calculaitons.
    // For many runs it can be convenient to separate target and coating layers.
    // Per layer
    void AddTargetLayer(double layer_width, std::complex<double> layer_index);
    void AddCoatingLayer(double layer_width, std::complex<double> layer_index);
    // For all layers
    void SetTargetWidth(std::vector<double> width);
    void SetTargetIndex(std::vector< std::complex<double> > index);
    void SetTargetPEC(double radius);
    void SetCoatingWidth(std::vector<double> width);
    void SetCoatingIndex(std::vector< std::complex<double> > index);
    void SetFieldPoints(std::vector< std::array<double,3> > coords);

    //Set parameters in size parameter units
    void SetWidthSP(const std::vector<double>& width);
    void SetIndexSP(const std::vector< std::complex<double> >& index);
    void SetFieldPointsSP(const std::vector< std::vector<double> >& coords_sp);

    // Set common parameters
    void SetAnglesForPattern(double from_angle, double to_angle, int samples);
    void SetAngles(const std::vector<double>& angles);
    std::vector<double> GetAngles();
    void SetPEC(int layer_position = 0);  // By default set PEC layer to be the first one
    
    void SetMaxTermsNumber(int nmax);
    int GetMaxTermsUsed() {return nmax_used_;};
    
    void ClearTarget();
    void ClearCoating();
    void ClearLayers();
    void ClearAllDesign(); //Layers + SP + index_

    // Applied units requests
    double GetTotalRadius();
    double GetTargetRadius();
    double GetCoatingWidth();
    std::vector<double>                  GetTargetLayersWidth();
    std::vector< std::complex<double> >  GetTargetLayersIndex();
    std::vector<double>                  GetCoatingLayersWidth();
    std::vector< std::complex<double> >  GetCoatingLayersIndex();
    std::vector< std::vector<double> >   GetFieldPoints();
    std::vector<std::vector< std::complex<double> > >
      GetFieldE(){return E_field_;};   // {X[], Y[], Z[]}
    std::vector<std::vector< std::complex<double> > >
      GetFieldH(){return H_field_;};
    std::vector< std::vector<double> > GetSpectra(double from_WL, double to_WL, int samples);  // ext, sca, abs, bk
    double GetRCSext();
    double GetRCSsca();
    double GetRCSabs();
    double GetRCSbk();
    std::vector<double> GetPatternEk();
    std::vector<double> GetPatternHk();
    std::vector<double> GetPatternUnpolarized();

    // Size parameter units
    std::vector<double> GetLayerWidthSP();
    // Same as to get target and coating index
    std::vector< std::complex<double> > GetLayerIndex();  
    std::vector< std::array<double,3> > GetFieldPointsSP();
    // Do we need normalize field to size parameter?
    /* std::vector<std::vector<std::complex<double> > >  GetFieldESP(); */
    /* std::vector<std::vector<std::complex<double> > >  GetFieldHSP(); */
    std::vector< std::array<double,5> > GetSpectraSP(double from_SP, double to_SP, int samples);  // WL,ext, sca, abs, bk

    std::vector<double> GetQsca_channel();
    std::vector<double> GetQabs_channel();
    std::vector<double> GetQsca_channel_normalized();
    std::vector<double> GetQabs_channel_normalized();
    std::vector<std::complex<double> > GetAn(){return an_;};
    std::vector<std::complex<double> > GetBn(){return bn_;}; 

    double GetAsymmetryFactor();
    double GetAlbedo();
    std::vector<std::complex<double> > GetS1();
    std::vector<std::complex<double> > GetS2();
    std::vector<double> GetPatternEkSP();
    std::vector<double> GetPatternHkSP();
    std::vector<double> GetPatternUnpolarizedSP();
    

    // Output results (data file + python script to plot it with matplotlib)
    void PlotSpectra();
    void PlotSpectraSP();
    void PlotField();
    void PlotFieldSP();
    void PlotPattern();
    void PlotPatternSP();

  private:
    void ConvertToSP();
    void GenerateSizeParameter();
    void GenerateIndex();
    void InitMieCalculations();

    void sbesjh(std::complex<double> z, std::vector<std::complex<double> >& jn,
	            std::vector<std::complex<double> >& jnp, std::vector<std::complex<double> >& h1n,
	            std::vector<std::complex<double> >& h1np);
    void sphericalBessel(std::complex<double> z, std::vector<std::complex<double> >& bj,
			             std::vector<std::complex<double> >& by, std::vector<std::complex<double> >& bd);
    std::complex<double> calcD1confra(int N, const std::complex<double> z);
    
    double wavelength_ = 1.0;
    double total_radius_ = 0.0;
    /// Width and index for each layer of the structure
    std::vector<double> target_width_, coating_width_;
    std::vector< std::complex<double> > target_index_, coating_index_;

    // Scattering coefficients
    std::vector<std::complex<double> > an_, bn_;
    std::vector< std::vector<double> > coords_sp_;
    /// Store result
    // Mie efficinecy from each multipole channel.
    std::vector<double> Qsca_ch_, Qext_ch_, Qabs_ch_, Qbk_ch_, Qpr_ch_;
    std::vector<double> Qsca_ch_norm_, Qext_ch_norm_, Qabs_ch_norm_, Qbk_ch_norm_, Qpr_ch_norm_;




  };  // end of class MultiLayerMie

}  // end of namespace nmie
#endif  // SRC_NMIE_WRAPPER_H
