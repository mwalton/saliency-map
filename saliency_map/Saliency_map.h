//
//  Saliency_map.h
//  Attention Classes
//
//  Created by Michael Walton on 1/15/14.
//  Copyright (c) 2014 Michael Walton. All rights reserved.
//

#ifndef SALIENCY_MAP_H
#define SALIENCY_MAP_H

#include <iostream>
#include <Geometry.h>
namespace GU = Geometry_Utilities;
#include <string>
#include "Symbol.h"

#include "Matrix.h"

class Saliency_map: public Matrix {
private:
    double FWHM_constant = 2.355;
    
    double angular_resolution;
    double periphery_radius;
    
    int center_x;
    int center_y;
    
public:
    //Constructors
    Saliency_map();
    Saliency_map(double _angular_resolution, double _periphery_DVA);
    
    //Mutators
    void retinal_distribution(double fovea = 1.0, double paraF = 7.5, double perif = 60);
    void parafoveal_distribution(double fovea = 1.0, double paraF = 7.5);
    void flat_distribution();
    
    void insert_gaussian_cue(GU::Point loc, GU::Size size);
    void insert_regional_cue(double upper_asymptote, const Symbol& region);
    void insert_rectangular_cue(GU::Point loc, GU::Size size);
    //void insert_rect_cue(unsigned long, unsigned long, double, unsigned long, unsigned long);
    void set_volume(double);
    
    //Accessors
    double get_angular_resolution();
    int get_n_cues();
    int get_center_x();
    int get_center_y();
    int get_periphery_radius();
    //double get_volume();
};

#endif
