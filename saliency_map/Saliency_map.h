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
    double angular_resolution;
    double periphery_DVA;
    int n_cues = 0;
    
    const double FWHM_constant = 2.355;
    
public:
    //Constructors
    Saliency_map(double _angular_resolution, double _periphery_DVA = 60);
    
    //Mutators
    void retinal_distribution(double fovea = 1.0, double paraF = 7.5, double perif = 60);
    void insert_gaussian_cue(double size, int x_mean, int y_mean);
    //void insert_rect_cue(unsigned long, unsigned long, double, unsigned long, unsigned long);
    void set_volume(double);
    
    //Accessors
    double get_angular_resolution();
    int get_n_cues();
    //double get_volume();
};

#endif
