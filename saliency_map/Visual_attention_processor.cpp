//
//  Visual_attention_processor.cpp
//  saliency_map
//
//  Created by myke on 1/22/14.
//  Copyright (c) 2014 myke. All rights reserved.
//

#include "Visual_attention_processor.h"

Visual_attention_processor::Visual_attention_processor() {
    saliency = Saliency_map();
    retinal = Saliency_map();
    endogenous_spatial = Saliency_map();
}

Visual_attention_processor::Visual_attention_processor(double angular_resolution, double periphery_radius) {
    saliency = Saliency_map(angular_resolution, periphery_radius);
    retinal = Saliency_map(angular_resolution, periphery_radius);
    endogenous_spatial = Saliency_map(angular_resolution, periphery_radius);
    
    retinal.retinal_distribution();
    saliency.flat_distribution();
    endogenous_spatial.flat_distribution();
}

void Visual_attention_processor::generate_saliency_map(double retinal_scalar, double endogenous_spatial_scalar) {
    saliency = add(retinal_scalar, endogenous_spatial_scalar, retinal, endogenous_spatial);
    saliency.normalize();
}

Saliency_map * Visual_attention_processor::saliency_ptr() {
    return &saliency;
}

Saliency_map * Visual_attention_processor::endogenous_spatial_ptr() {
    return &endogenous_spatial;
}

Saliency_map * Visual_attention_processor::retinal_ptr() {
    return &retinal;
}

static Saliency_map add(double scale_n, double scale_m, Saliency_map n, Saliency_map m) {
    Saliency_map ret = n;
    ret.scale(scale_n);
    ret.linear_combination(m,scale_m);
    return ret;
}