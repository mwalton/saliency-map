//
//  saliency_map.h
//  saliency_map
//
//  Created by myke on 1/15/14.
//  Copyright (c) 2014 myke. All rights reserved.
//

#ifndef __saliency_map__saliency_map__
#define __saliency_map__saliency_map__

#include <iostream>
#include "linear_algebra.h"
#include <string>

class Saliency_map: public Matrix {
private:
    double angular_resolution;
    double full_width_half_max;
    double volume;
    const double FWHM_constant = 2.355;
    
public:
    //Constructors
    Saliency_map(double, double weight = 1, double periphery_VA = 60);
    
    //Mutators
    void retinal_distribution(double fovea = 1.0, double paraF = 7.5, double perif = 60);
    void insert_gaussian_cue(double, double, unsigned long, unsigned long);
    void insert_rect_cue(unsigned long, unsigned long, double, unsigned long, unsigned long);
    void set_volume(double);
    
    //Accessors
    double get_angular_resolution();
    double get_volume();
};

#endif /* defined(__saliency_map__saliency_map__) */
