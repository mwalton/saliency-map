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

Saliency_map::Saliency_map(double ar, double periphery_VA)
    : Matrix() {
        angular_resolution = ar;
        int n = 2 * periphery_VA / angular_resolution;
        resize(n,n,1);
        normalize();
}

//Mutators
/*
 Generates a retinal map
 FWHM (full width at half maximum) is used to compute appropriate sigmas
 for the width of the fovea, parafovea and periphery
 
 three gaussians are generated and summed to create the retinal map
 */
void Saliency_map::retinal_distribution(double fv, double paraF, double perif) {
    double FWHM_constant = 2.355;
    double periphery_sigma = FWHM_constant * perif;
    double fovea_sigma = FWHM_constant * fv;
    double parafovea_sigma = FWHM_constant * paraF;
    
    to_gaussian(periphery_sigma);
    
    Saliency_map fovea = Saliency_map(get_angular_resolution());
    Saliency_map parafovea = Saliency_map(get_angular_resolution());
    
    fovea.to_gaussian(fovea_sigma);
    parafovea.to_gaussian(parafovea_sigma);
    
    //CHANGE ADD FUNCITONS TO INCLUDE SCALING
    //LINEAR COMBINATION INSTEAD OF BASIC ADDITION
    //WILL BE MORE EFFICIENT & CLEANER
    fovea.scale(.001);
    parafovea.scale(.05);
    
    add(fovea);
    add(parafovea);
    normalize();
}





//Accessors
double Saliency_map::get_angular_resolution() {
    return angular_resolution;
}