//
//  Visual_attention_processor.h
//  saliency_map
//
//  Created by Michael Walton on 1/22/14.
//  Copyright (c) 2014 Michael Walton. All rights reserved.
//

#ifndef VISUAL_ATTENTION_PROCESSOR_H
#define VISUAL_ATTENTION_PROCESSOR_H

//#include <iostream>
#include "Saliency_map.h"

class Visual_attention_processor {
private:
    Saliency_map saliency;
    Saliency_map retinal;
    Saliency_map endogenous_spatial;
    
public:
    //Constructors
    Visual_attention_processor();
    Visual_attention_processor(double angular_resolution, double periphery_radius);
    
    //Mutators
    void generate_saliency_map(double retinal_scalar, double endogenous_spatial_scalar);
    
    //Accessors
    Saliency_map* saliency_ptr();
    Saliency_map* retinal_ptr();
    Saliency_map* endogenous_spatial_ptr();
};

static Saliency_map add(double scale_n, double scale_m, Saliency_map n, Saliency_map m);

#endif
