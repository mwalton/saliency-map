//
//  Visual_attention_processor.h
//  Attention Classes
//
//  Created by Michael Walton on 1/22/14.
//  Copyright (c) 2014 Michael Walton. All rights reserved.
//

#ifndef VISUAL_ATTENTION_PROCESSOR_H
#define VISUAL_ATTENTION_PROCESSOR_H

#include "Saliency_map.h"

#include "Smart_Pointer.h"
#include "Visual_physical_store.h"
#include "Visual_constants.h"
#include "Symbol.h"

#include "Geometry.h"
namespace GU = Geometry_Utilities;

class Visual_attention_processor : public Reference_Counted_Object {
private:
    Saliency_map saliency;
    Saliency_map retinal;
    Saliency_map endogenous_spatial;
    
    double weight;
    double delay_deviation_magnitude;
    
    double angular_resolution;
    double periphery_radius;
    int central_coordinate;
    int map_size;
    
    GU::Point last_known_cue_coordinate;
    GU::Size last_known_cue_size;
    
    bool debug;
    
public:
    //Constructors
    Visual_attention_processor();
    Visual_attention_processor(Symbol overt_dist, double _angular_resolution, double _periphery_radius, double _weight, double _delay_deviation_magnitude, int _debug);
    
    //"pre-constructor" takes param specs and returns a new VAP
    static Visual_attention_processor * create(const Visual_physical_store& physical_store, const Parameter_specification& param_spec);
    
    //Mutators
    void generate_saliency_map(double retinal_scalar, double endogenous_spatial_scalar);
    void spatial_cue(Human_processor * const human_ptr, const Symbol& objname);
    void spatial_cue(Human_processor * const human_ptr);
    void regional_cue(const Symbol& region);
    void clear_endogenous_spatial();
    
    //Accessors
    Saliency_map* saliency_ptr();
    Saliency_map* retinal_ptr();
    Saliency_map* endogenous_spatial_ptr();
    GU::Point get_retinotopic_coordinate(GU::Point eye_loc, GU::Point obj_loc);
    
    long get_delay_range();
    long delay(double min_delay, Smart_Pointer<Visual_store_object> physobj_ptr,
               GU::Point eye_loc, double time_fluctuation);
    
    virtual std::string get_description() const;
    void to_csv();
};

//Symbols
extern const Symbol Flat_c;
extern const Symbol GaussianComplex_c;
extern const Symbol Gaussian_c;

static Saliency_map add(double scale_n, double scale_m, Saliency_map n, Saliency_map m);

#endif
