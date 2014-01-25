//
//  saliency_map.h
//  saliency_map
//
//  Created by myke on 1/15/14.
//  Copyright (c) 2014 myke. All rights reserved.
//

#ifndef SALIENCY_MAP_H
#define SALIENCY_MAP_H

#include <iostream>
#include "Matrix.h"
#include <string>

class Saliency_map: public Matrix {
private:
    double FWHM_constant = 2.355;
    
    double angular_resolution;
    double periphery_radius;
    
public:
    //Constructors
    Saliency_map();
    Saliency_map(double _angular_resolution, double _periphery_DVA);
    
    //Mutators
    void retinal_distribution(double fovea = 1.0, double paraF = 7.5, double perif = 60);
    void flat_distribution();
    
    void insert_gaussian_cue(double size, double intensity, int x_mean, int y_mean);
    //void insert_rect_cue(unsigned long, unsigned long, double, unsigned long, unsigned long);
    void set_volume(double);
    
    //Accessors
    double get_angular_resolution();
    int get_n_cues();
    //double get_volume();
};

#endif
