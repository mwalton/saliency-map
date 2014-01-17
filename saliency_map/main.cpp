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
#include "saliency_map.h"

int main(int argc, const char * argv[])
{
    //Open file for output to matlab
    std::ofstream spatial_cue_file;
    spatial_cue_file.open("spatial.csv");
    
    std::ofstream retina_file;
    retina_file.open("retina.csv");
    
    std::ofstream sum_file;
    sum_file.open("sum.csv");
    
    //Make the base map
    Saliency_map map = Saliency_map(1.0);
    map.retinal_distribution();
    
    //Make a feature map
    Saliency_map feature_map = Saliency_map(1.0);
    feature_map.insert_cue(1, 1, 10, 10);
    feature_map.normalize();
    
    spatial_cue_file << feature_map.to_string();
    retina_file << map.to_string();
    
    map.linear_combination(feature_map, 1);
    map.normalize();
    sum_file << map.to_string();
    
    spatial_cue_file.close();
    retina_file.close();
    sum_file.close();
    
    return 0;
}

/*Matrix tests
 int main(int argc, const char * argv[])
 {
 //Open file for output to matlab
 std::ofstream matlab_file;
 matlab_file.open("map_test.csv");
 
 //Make the base map
 Matrix saliency_map = Matrix(50,50);
 saliency_map.to_gaussian(7);
 
 //Make the feature map
 Matrix feature_map = Matrix(50, 50);
 
 //Generate fovea
 Matrix fovea = Matrix(14);
 fovea.to_gaussian(2.7);  //represent overt attention/foveal topography
 
 //generate an arbitrary matrix to represent a square stimulus cue
 Matrix stim_cue = Matrix(7,7,1);
 stim_cue.to_gaussian(3);
 
 //add the stimulus cues to the feature map
 feature_map.add_submatrix(stim_cue, 30, 2);
 feature_map.add_submatrix(stim_cue, 15, 15);
 
 
 
 //add the foveal overt attention bias to the map
 //saliency_map.add_submatrix(fovea, 18, 18);
 
 
 //make a gaussian kernel
 Matrix gauss_kernel = Matrix(3,3);
 gauss_kernel.to_gaussian(5);
 
 //do gaussian blur convolution
 feature_map.scale(.1);
 saliency_map.add(feature_map);
 Matrix blurred_map = saliency_map.convolution(gauss_kernel);
 blurred_map.normalize();
 
 //DoG test
 Matrix left = Matrix(20,20);
 left.to_gaussian(4);
 
 Matrix right = Matrix(20,20);
 right.to_gaussian(2);
 
 right.scale(-1);
 left.add(right);
 
 matlab_file << blurred_map.to_string();
 matlab_file.close();
 
 return 0;
 }
 */

