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
    const double FWHM_constant = 2.355;
    const double fovea_scalar = .001;
    const double parafovea_scalar = .05;
    
public:
    //Constructors
    Saliency_map(double, double periphery_VA = 60);
    
    //Mutators
    void retinal_distribution(double fovea = 1.0, double paraF = 7.5, double perif = 60);
    
    //Accessors
    double get_angular_resolution();
};

#endif /* defined(__saliency_map__saliency_map__) */
