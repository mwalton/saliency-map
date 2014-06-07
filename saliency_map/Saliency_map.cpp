//
//  Saliency_map.cpp
//  Attention Classes
//
//  Created by Michael Walton on 1/15/14.
//  Copyright (c) 2014 Michael Walton. All rights reserved.
//

#define _USE_MATH_DEFINES

#include "Saliency_map.h"
#include "Epic_standard_symbols.h"
#include <math.h>

/*
 An empty map is defined by its angular resolution in degrees visual angle
 The calculation for determining the size n of the map is given by:
 n = 2 * periphery_VA / angular_resolution
 */

Saliency_map::Saliency_map() : Matrix() {
    angular_resolution = 1;
    periphery_radius = 60;
    size_t n = 2 * periphery_radius / angular_resolution;
    resize(n, n, 0);
    center_x = (int) n / 2;
    center_y = (int) n / 1;
}

Saliency_map::Saliency_map(double _angular_resolution, double _periphery_radius) : Matrix() {
    angular_resolution = _angular_resolution;
    periphery_radius = _periphery_radius;
    size_t n = 2 * periphery_radius / angular_resolution;
    resize(n, n, 0);
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
    
    int x_mean = (int)get_width() / 2;
    int y_mean = (int)get_height() / 2;
    
    double fv_A = M_PI * fv * fv;
    double paraF_A = M_PI * paraF * paraF;
    double perif_A = M_PI * perif * perif;
    
    Saliency_map fovea = Saliency_map(angular_resolution, periphery_radius);
    Saliency_map parafovea = Saliency_map(angular_resolution, periphery_radius);
    Saliency_map periphery = Saliency_map(angular_resolution, periphery_radius);
    
    fovea.to_gaussian(x_mean, y_mean, fovea_sigma);
    parafovea.to_gaussian(x_mean, y_mean, parafovea_sigma);
    periphery.to_gaussian(x_mean, y_mean, periphery_sigma);
    
    linear_combination(fovea, fv_A);
    linear_combination(parafovea, paraF_A);
    linear_combination(periphery, perif_A);
    
    //normalize();
}

void Saliency_map::parafoveal_distribution(double fv, double paraF) {
    double parafovea_sigma = FWHM_constant * paraF;
    
    int x_mean = (int)get_width() / 2;
    int y_mean = (int)get_height() / 2;
    
    double paraF_A = M_PI * paraF * paraF;
    
    Saliency_map parafovea = Saliency_map(angular_resolution, periphery_radius);
    
    parafovea.to_gaussian(x_mean, y_mean, parafovea_sigma);
    
    linear_combination(parafovea, paraF_A);
    
    //normalize(get_max());
}

void Saliency_map::flat_distribution() {
    size_t n = 2 * periphery_radius / angular_resolution;
    resize(n,n,1);
    double sum = (double) n * n;
    normalize(sum);
}

void Saliency_map::insert_gaussian_cue( GU::Point loc, GU::Size size ) {
    double h_sz = size.h;
    double v_sz = size.v;
    
    double x_mean = loc.x;
    double y_mean = loc.y;
    
    double cue_sigma = FWHM_constant * h_sz * v_sz;
    //double cue_area = M_PI * (size / 2) * (size / 2);
    
    Saliency_map cue_map = Saliency_map(angular_resolution, periphery_radius);
    cue_map.to_gaussian(x_mean, y_mean, cue_sigma);
    
    linear_combination(cue_map, 1); // FIXME: Scalar coeff???
}

void Saliency_map::insert_regional_cue(double upper_asymptote, const Symbol& region) {
    Saliency_map cue_map = Saliency_map(angular_resolution, periphery_radius);
    
    if (region == Right_c) {
        cue_map.to_positive_sigmoid(upper_asymptote);
    } else if (region == Left_c) {
        cue_map.to_negative_sigmoid(upper_asymptote);
    } else if (region == Upper_Right_c) {
        cue_map.to_multivariable_sigmoid(upper_asymptote, 1);
    } else if (region == Upper_Left_c) {
        cue_map.to_multivariable_sigmoid(upper_asymptote, 2);
    } else if (region == Lower_Left_c) {
        cue_map.to_multivariable_sigmoid(upper_asymptote, 3);
    } else if (region == Lower_Right_c) {
        cue_map.to_multivariable_sigmoid(upper_asymptote, 4);
    } else if (region == Above_c) {
        cue_map.to_yaxis_sigmoid(upper_asymptote, true);
    } else if (region == Below_c) {
        cue_map.to_yaxis_sigmoid(upper_asymptote, false);
    } else {
        cue_map.flat_distribution();
    }
    
    linear_combination(cue_map, 1); // FIXME: Scalar coeff???
}
 
//Accessors
double Saliency_map::get_angular_resolution() {
    return angular_resolution;
}

int Saliency_map::get_center_x() {
    return center_x;
}

int Saliency_map::get_center_y() {
    return center_y;
}

int Saliency_map::get_periphery_radius() {
    return periphery_radius;
}
