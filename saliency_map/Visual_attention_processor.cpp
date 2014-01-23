//
//  Visual_attention_processor.cpp
//  saliency_map
//
//  Created by myke on 1/22/14.
//  Copyright (c) 2014 myke. All rights reserved.
//

#include "Visual_attention_processor.h"
#include "Saliency_map.h"

class Visual_attention_processor {
private:
    Saliency_map saliency;
    Saliency_map endogenous_retinal;
    Saliency_map endogenous_spatial;
    
public:
    //Constructors
    Visual_attention_processor();
    
    //Mutators
    
    //Accessors
    Saliency_map* get_retinal_map();
    Saliency_map* get_spatial_map();
};