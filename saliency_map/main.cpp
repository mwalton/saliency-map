//
//  main.cpp
//  saliency_map
//
//  Created by Michael Walton on 12/15/13.
//  Copyright (c) 2013 Michael Walton. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "Visual_attention_processor.h"

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
    Visual_attention_processor v_attn = Visual_attention_processor(1.0, 60);
    
    //insert features
    Saliency_map * spatial_ptr = v_attn.endogenous_spatial_ptr();
    
    spatial_ptr->insert_gaussian_cue(2, 2, 5, 5);
    spatial_ptr->insert_gaussian_cue(5, 2, 100, 100);
    spatial_ptr->insert_gaussian_cue(10, 2, 5, 100);
    spatial_ptr->normalize();
    
    spatial_cue_file << spatial_ptr->to_string();
    retina_file << v_attn.retinal_ptr() -> to_string();
    
    v_attn.generate_saliency_map(.005, 1);
    sum_file << v_attn.saliency_ptr() -> to_string();
    
    spatial_cue_file.close();
    retina_file.close();
    sum_file.close();
    
    return 0;
}