//
//  saliency_map.cpp
//  saliency_map
//
//  Created by myke on 1/15/14.
//  Copyright (c) 2014 myke. All rights reserved.
//

#include "saliency_map.h"
#include <math.h>

/*
 An empty map is defined by its angular resolution in degrees visual angle
 The calculation for determining the size n of the map is given by:
 n = 2 * periphery_VA / angular_resolution
 */

Saliency_map::Saliency_map(double _angular_resolution, double _weight, double max_eccentricity)
    : Matrix() {
        full_width_half_max = _weight;
        angular_resolution = _angular_resolution;
        int n = 2 * max_eccentricity / angular_resolution;
        resize(n,n,1);
        normalize();
}

//Mutators
/*
 Generates a retinal map
 FWHM (full width at half maximum) is used to compute appropriate sigmas
 for the width of the fovea, parafovea and periphery
 
 three gaussians are generated and combined to create the retinal map
 */
void Saliency_map::retinal_distribution(double fv, double paraF, double perif) {
    double periphery_sigma = FWHM_constant * perif;
    double fovea_sigma = FWHM_constant * fv;
    double parafovea_sigma = FWHM_constant * paraF;
    
    Saliency_map fovea = Saliency_map(angular_resolution);
    Saliency_map parafovea = Saliency_map(angular_resolution);
    
    to_gaussian(periphery_sigma);
    fovea.to_gaussian(fovea_sigma);
    parafovea.to_gaussian(parafovea_sigma);
    
    scale(120);
    linear_combination(fovea, 1);
    linear_combination(parafovea, 56.25);
    normalize();
}

void Saliency_map::insert_cue( double size, double intensity, unsigned long x, unsigned long y ) {
    Saliency_map cue = Saliency_map(angular_resolution);
    double cue_sigma = FWHM_constant * size;
    double scaled_intensity = intensity * size;
    
    cue.to_gaussian(cue_sigma, x, y);
    
    linear_combination(cue, scaled_intensity);
}

//Accessors
double Saliency_map::get_angular_resolution() {
    return angular_resolution;
}