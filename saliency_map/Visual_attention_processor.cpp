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
        double periphery_radius;
        std::string overt_param;
        Symbol overt_distribution;
        
        iss >> overt_param;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read overt attention distribution in parameter specification", param_spec);
        
        if (overt_param == "Retinal") overt_distribution = Retinal_c;
        else if (overt_param == "Parafoveal") overt_distribution = Parafoveal_c;
        else if (overt_param == "None") overt_distribution = Flat_c;
        else Parameter::throw_parameter_error("Invalid overt attention distribution", param_spec);
        
        iss >> angular_resolution;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read angular resolution in parameter specification", param_spec);
        
        iss >> periphery_radius;
        if(!iss)
            Parameter::throw_parameter_error("Unable to read periphery radius in parameter specification", param_spec);
        
        return new Visual_attention_processor(overt_distribution, angular_resolution, periphery_radius);
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

Visual_attention_processor::Visual_attention_processor(Symbol overt_dist, double _angular_resolution, double _periphery_radius) {
    angular_resolution = _angular_resolution;
    periphery_radius = _periphery_radius;
    
    central_coordinate = periphery_radius / angular_resolution;
    map_size = central_coordinate * 2;
    
    saliency = Saliency_map(angular_resolution, periphery_radius);
    retinal = Saliency_map(angular_resolution, periphery_radius);
    endogenous_spatial = Saliency_map(angular_resolution, periphery_radius);
    
    double fv = 1 / angular_resolution;
    double para_f = 7.5 / angular_resolution;
    double perif = 60 / angular_resolution;
    
    if (overt_dist == Retinal_c) retinal.retinal_distribution();
    else if (overt_dist == Parafoveal_c) retinal.parafoveal_distribution();
    else retinal.flat_distribution();
    
    saliency.flat_distribution();
    endogenous_spatial.flat_distribution();
}

/******************************* MUTATORS ***********************************************************
 ****************************************************************************************************/

void Visual_attention_processor::generate_saliency_map(double retinal_scalar, double endogenous_spatial_scalar) {
    saliency = add(retinal_scalar, endogenous_spatial_scalar, retinal, endogenous_spatial);
    
    saliency.normalize();
}

void Visual_attention_processor::spatial_cue(Human_processor * const human_ptr, const Symbol& objname) {
    Symbol cue = human_ptr->get_Eye_processor_ptr()->get_name_map().get_physical_name(objname);
    Visual_perceptual_store * vis_store_ptr = human_ptr->get_Visual_perceptual_store_ptr();
    Smart_Pointer<Visual_store_object> cue_ptr = vis_store_ptr->get_object_ptr(objname);
    
    GU::Point eye_location = human_ptr->get_Eye_processor_ptr()->get_location();
    GU::Point spatial_coordinate = cue_ptr->get_location();
    GU::Size cue_size = cue_ptr->get_size();
    
    GU::Point retinotopic_loc = get_retinotopic_coordinate(eye_location, spatial_coordinate);
    
    if(retinotopic_loc.x < map_size && retinotopic_loc.y < map_size &&
       retinotopic_loc.x >= 0 && retinotopic_loc.y >= 0) {
        endogenous_spatial.insert_gaussian_cue(retinotopic_loc, cue_size);
    }
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
    
    //std::cout << "cue at: (" << retinal_x << ", " << retinal_y << ")" << std::endl; //DEBUG
    
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

long Visual_attention_processor::get_delay_range() {
    return delay_range;
}

long Visual_attention_processor::delay(Smart_Pointer<Visual_store_object> physobj_ptr,
                                       GU::Point eye_loc, double time_fluctuation) {
    if (physobj_ptr->get_eccentricity() > periphery_radius) return delay_range;
    
    generate_saliency_map(1, 1);
    to_csv();  //DEBUG OUTPUT
    
    GU::Point obj_loc = physobj_ptr->get_location();
    GU::Point retinotopic_coordinate = get_retinotopic_coordinate(eye_loc, obj_loc);
    
    double scalar = saliency.get_value(retinotopic_coordinate.x, retinotopic_coordinate.y);
    
    long delay = delay_range - (scalar * 50 * delay_range) * time_fluctuation;  //FIXME: arbitrary value
    
    std::cout << "f(" << obj_loc.x << ", " << obj_loc.y << ") = " << scalar
    << " Delay = " << delay << std::endl; //DEBUG
    
    return delay;
}
/************************************** SYMBOLS *****************************************************
 ****************************************************************************************************/
const Symbol Flat_c("Overt_flat");
const Symbol Retinal_c("Overt_retinal");
const Symbol Parafoveal_c("Overt_parafoveal");

/******************************* STATIC METHODS *****************************************************
 ****************************************************************************************************/

static Saliency_map add(double scale_n, double scale_m, Saliency_map n, Saliency_map m) {
    Saliency_map ret = n;
    ret.scale(scale_n);
    ret.linear_combination(m,scale_m);
    return ret;
}
