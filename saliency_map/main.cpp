//
//  main.cpp
//  saliency_map
//
//  Created by myke on 12/15/13.
//  Copyright (c) 2013 myke. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "linear_algebra.h"
//#include "engine.h"

int main(int argc, const char * argv[])
{
    //Open file for output to matlab
    std::ofstream matlab_file;
    matlab_file.open("mat_test.txt");
    
    //Make the base map
    Matrix saliency_map = Matrix(50,50);
    
    //Make the feature map
    Matrix feature_map = Matrix(50, 50);
    
    //Generate fovea
    Matrix fovea = Matrix(14);
    fovea.to_gaussian(2);  //represent overt attention/foveal topography
    
    //generate an arbitrary matrix to represent a square stimulus cue
    Matrix stim_cue = Matrix(7,7,1);
    stim_cue.to_gaussian(3);
    
    //add the stimulus cues to the feature map
    feature_map.add_submatrix(stim_cue, 30, 2);
    feature_map.add_submatrix(stim_cue, 15, 15);
    
    feature_map.normalize();
    
    //add the foveal overt attention bias to the map
    saliency_map.add_submatrix(fovea, 18, 18);
    saliency_map.add(feature_map);
    
    //make a gaussian kernel
    Matrix gauss_kernel = Matrix(3,3);
    gauss_kernel.to_gaussian(5);
    
    //do gaussian blur convolution
    Matrix blurred_map = saliency_map.convolution(gauss_kernel);
    blurred_map.normalize();
    
    //DoG test
    Matrix left = Matrix(20,20);
    left.to_gaussian(4);
    
    Matrix right = Matrix(20,20);
    right.to_gaussian(2);
    
    right.scale(-1);
    left.add(right);
    
    std::cout << left.to_string() << std::endl;
    matlab_file << left.to_string();
    matlab_file.close();
    
    return 0;
}
