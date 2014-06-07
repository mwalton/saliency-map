//
//  Visual_attention_processor.cpp
//  Attention Classes
//
//  Created by Michael Walton on 1/22/14.
//  Copyright (c) 2014 Michael Walton. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "Visual_attention_processor.h"


#include "Epic_standard_symbols.h"
#include "Numeric_utilities.h"
#include "Assert_throw.h"
#include "Visual_perceptual_store.h"
#include "Eye_processor.h"

using std::pow;
using std::string;
using std::ostringstream;
using std::istringstream;

/******************************* PARAM-SPEC >> CONSTRUCTOR ******************************************
 ****************************************************************************************************/

Visual_attention_processor * Visual_attention_processor::create
    (const Visual_physical_store& physical_store, const Parameter_specification& param_spec) {
        istringstream iss(param_spec.specification);
        double angular_resolution;
        double weight;
        double delay_deviation_magnitude;
        double radius;
        std::string debugstr;
        bool debug;
        
        std::string overt_param;
        Symbol overt_distribution;
        
        iss >> overt_param;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read overt attention distribution in parameter specification", param_spec);
        
        if (overt_param == "Complex_Gaussian") overt_distribution = GaussianComplex_c;
        else if (overt_param == "Gaussian") overt_distribution = Gaussian_c;
        else if (overt_param == "Flat") overt_distribution = Flat_c;
        else Parameter::throw_parameter_error("Invalid overt attention distribution", param_spec);
        
        iss >> angular_resolution;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read angular resolution in parameter specification", param_spec);
        
        iss >> radius;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read map radius in parameter specification", param_spec);
        
        iss >> weight;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read map weight in parameter specification", param_spec);
        
        iss >> delay_deviation_magnitude;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read delay deviation celiling in parameter specification", param_spec);
        
        iss >> debugstr;
        debug = iss;
        
        
        return new Visual_attention_processor(overt_distribution,
                                              angular_resolution,
                                              radius,
                                              weight,
                                              delay_deviation_magnitude,
                                              debug);
}

/******************************* CONSTRUCTORS *******************************************************
 ****************************************************************************************************/

Visual_attention_processor::Visual_attention_processor() {
    saliency = Saliency_map();
    retinal = Saliency_map();
    endogenous_spatial = Saliency_map();
    
    angular_resolution = 1;
    periphery_radius = 60;
    central_coordinate = 60;
    map_size = central_coordinate * 2;
}

Visual_attention_processor::Visual_attention_processor(Symbol overt_dist, double _angular_resolution, double _periphery_radius, double _weight, double _delay_deviation_magnitude, int _debug) {
    angular_resolution = _angular_resolution;
    periphery_radius = _periphery_radius;
    weight = _weight;
    delay_deviation_magnitude = _delay_deviation_magnitude;
    
    central_coordinate = periphery_radius / angular_resolution;
    map_size = central_coordinate * 2;
    
    saliency = Saliency_map(angular_resolution, periphery_radius);
    retinal = Saliency_map(angular_resolution, periphery_radius);
    endogenous_spatial = Saliency_map(angular_resolution, periphery_radius);
    
    if (overt_dist == GaussianComplex_c) retinal.retinal_distribution();
    else if (overt_dist == Gaussian_c) retinal.parafoveal_distribution();
    else retinal.flat_distribution();
    
    saliency.flat_distribution();
    endogenous_spatial.flat_distribution();
    
    debug = (_debug > 0) ? true : false;
}

/******************************* MUTATORS ***********************************************************
 ****************************************************************************************************/

void Visual_attention_processor::generate_saliency_map(double retinal_scalar, double endogenous_spatial_scalar) {
    saliency = add(retinal_scalar, endogenous_spatial_scalar, retinal, endogenous_spatial);
    
    saliency.normalize();
    //saliency = endogenous_spatial;
}

void Visual_attention_processor::spatial_cue(Human_processor * const human_ptr, const Symbol& objname) {
    Symbol phys_name = human_ptr->get_Eye_processor_ptr()->get_name_map().get_physical_name(objname);
    Visual_perceptual_store * vis_percept_ptr = human_ptr->get_Visual_perceptual_store_ptr();
    
    Smart_Pointer<Visual_store_object> percept_ptr = vis_percept_ptr->get_object_ptr(objname);
    
    GU::Point eye_location = human_ptr->get_Eye_processor_ptr()->get_location();
    GU::Point spatial_coordinate = percept_ptr->get_location();
    GU::Size cue_size = percept_ptr->get_size();
    
    last_known_cue_coordinate = spatial_coordinate;
    last_known_cue_size = cue_size;
    
    GU::Point retinotopic_loc = get_retinotopic_coordinate(eye_location, spatial_coordinate);
    
    if(retinotopic_loc.x < map_size && retinotopic_loc.y < map_size &&
        retinotopic_loc.x >= 0 && retinotopic_loc.y >= 0) {
        endogenous_spatial.insert_gaussian_cue(retinotopic_loc, cue_size);
    }
}

void Visual_attention_processor::spatial_cue(Human_processor * const human_ptr) {
    GU::Point eye_location = human_ptr->get_Eye_processor_ptr()->get_location();
    GU::Point spatial_coordinate = last_known_cue_coordinate;
    GU::Size cue_size = last_known_cue_size;
    
    GU::Point retinotopic_loc = get_retinotopic_coordinate(eye_location, spatial_coordinate);
    
    if(retinotopic_loc.x < map_size && retinotopic_loc.y < map_size &&
       retinotopic_loc.x >= 0 && retinotopic_loc.y >= 0) {
        endogenous_spatial.insert_gaussian_cue(retinotopic_loc, cue_size);
    }
}

void Visual_attention_processor::regional_cue(const Symbol& region) {
    //double upper_bound = retinal.get_max();
    double upper_bound = 1;

    endogenous_spatial.insert_regional_cue(upper_bound, region);
}

void Visual_attention_processor::clear_endogenous_spatial() {
    endogenous_spatial.flat_distribution();
}


/******************************* ACCESSORS ***********************************************************
 ****************************************************************************************************/
Saliency_map * Visual_attention_processor::saliency_ptr() {
    return &saliency;
}

Saliency_map * Visual_attention_processor::endogenous_spatial_ptr() {
    return &endogenous_spatial;
}

Saliency_map * Visual_attention_processor::retinal_ptr() {
    return &retinal;
}

GU::Point Visual_attention_processor::get_retinotopic_coordinate(GU::Point eye_loc, GU::Point obj_loc) {
    double eye_x = eye_loc.x / angular_resolution;
    double eye_y = eye_loc.y / angular_resolution;
    
    double obj_x = obj_loc.x / angular_resolution;
    double obj_y  = obj_loc.y / angular_resolution;
    
    double retinal_x = (central_coordinate + obj_x - eye_x);
    double retinal_y = (central_coordinate - obj_y + eye_y);
    
    Assert(retinal_y > 0);
    Assert(retinal_x > 0);
    
    //std::cout << "Spatial: (" << obj_loc.x << "," << obj_loc.y <<  ") Retinotopic: (" << retinal_x << ", " << retinal_y << ")" << std::endl; //DEBUG
    
    GU::Point coordinate = GU::Point(retinal_x, retinal_y);
    return coordinate;
}

string Visual_attention_processor::get_description() const
{
	ostringstream oss;
	oss << "Angular Resolution: " << angular_resolution << "; Periphery Radius: " << periphery_radius;
	return oss.str();
}

//Creates csv files of all maps for MATLAB visualization
void Visual_attention_processor::to_csv() {
    std::ofstream spatial_cue_file;
    spatial_cue_file.open("spatial.csv");
    
    std::ofstream retina_file;
    retina_file.open("retina.csv");
    
    std::ofstream saliency_file;
    saliency_file.open("saliency.csv");
    
    spatial_cue_file << endogenous_spatial.to_string();
    retina_file << retinal.to_string();
    
    saliency_file << saliency.to_string();
    
    spatial_cue_file.close();
    retina_file.close();
    saliency_file.close();
}

long Visual_attention_processor::delay(double min_delay, Smart_Pointer<Visual_store_object> physobj_ptr,
                                       GU::Point eye_loc, double time_fluctuation) {
    
    generate_saliency_map(1, weight);
    
    GU::Point obj_loc = physobj_ptr->get_location();
    GU::Point retinotopic_coordinate = get_retinotopic_coordinate(eye_loc, obj_loc);
    
    double max_delay = min_delay + min_delay * delay_deviation_magnitude;
    
    double delay_range = max_delay - min_delay;
    double map_value = saliency.get_value(retinotopic_coordinate.x, retinotopic_coordinate.y);
    
    double max = saliency.get_max();
    double scalar =  (1 - map_value / max);
    
    if (physobj_ptr->get_eccentricity() > periphery_radius) return max_delay;
    if (map_value == 0 || max == 0) return min_delay; //this should never happen
    if (map_value == 1) return min_delay; //this should never happen
    
    long delay = (min_delay + scalar * delay_range) * time_fluctuation;
    //if (delay < min_delay) delay = min_delay;
    //if (delay > max_delay) delay = max_delay;
    
    if (debug) {
        std::cout << "f(" << obj_loc.x << ", " << obj_loc.y << ") = {" << retinotopic_coordinate.x << "," << retinotopic_coordinate.y << "} " << scalar << " Delay = " << delay << std::endl; //DEBUG
    
        to_csv();  //DEBUG OUTPUT
    }
    
    return delay;
}
/************************************** SYMBOLS *****************************************************
 ****************************************************************************************************/
const Symbol Flat_c("Flat");
const Symbol GaussianComplex_c("Complex_Gaussian");
const Symbol Gaussian_c("Gaussian");

/******************************* STATIC METHODS *****************************************************
 ****************************************************************************************************/

static Saliency_map add(double scale_n, double scale_m, Saliency_map n, Saliency_map m) {
    Saliency_map ret = n;
    ret.scale(scale_n);
    ret.linear_combination(m,scale_m);
    return ret;
}
