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
    
    //Make an identity matrix
    Matrix saliency_map = Matrix(50,50);
    saliency_map.to_gaussian(10);  //represent overt attention/foveal topography
    
    //generate an arbitrary matrix to represent a square stimulus cue
    //vector< vector<double> > elem = {{1,1,1},{1,1,1},{1,1,1}};
    Matrix stim_cue = Matrix(20);
    stim_cue.to_gaussian(1.5);
    
    
    //add the stimulus cues to the saliency map
    saliency_map.add_submatrix(stim_cue, 30, 2, .01);
    saliency_map.add_submatrix(stim_cue, 7, 7, .01);
    
    //make a gaussian kernel
    Matrix gauss_kernel = Matrix(3,3);
    gauss_kernel.to_gaussian(5);
    
    //do gaussian blur convolution
    Matrix blurred_map = saliency_map.convolution(gauss_kernel);
    
    std::cout << blurred_map.to_string() << std::endl;
    matlab_file << blurred_map.to_string();
    matlab_file.close();
    
    return 0;
}

