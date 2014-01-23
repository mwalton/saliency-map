//
//  linear_algebra.cpp
//  saliency_map
//
//  Created by myke on 12/15/13.
//  Copyright (c) 2013 myke. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <sstream>

#include "Matrix.h"

//Constructors

//No-arg
Matrix::Matrix()
:height(0), width(0) {
    std::vector< std::vector<double> > elements;
}

//NxN with initial value
Matrix::Matrix(size_t n, double init_val)
:height(n), width(n) {
    elements.resize( width , vector<double> ( height , init_val ));
    for(int i = 0; i < n; i++) {
        elements[i][i] = 1;
    }
}

Matrix::Matrix(size_t x, size_t y, double init_val)
:width(x), height(y) {
    elements.resize( width , vector<double> ( height , init_val ));
}

//------------------------------------------------------------------
// Mutators
//------------------------------------------------------------------
void Matrix::resize(size_t x, size_t y, double inti_val) {
    width = x;
    height = y;
    
    elements.resize( width , vector<double>( height , inti_val ) );
}

void Matrix::set(unsigned long x, unsigned long y, double val) {
    elements[x][y] = val;
}

void Matrix::linear_combination(Matrix m, double scalar) {
    assert(m.get_height() == get_height());
    assert(m.get_width() == get_width());
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] += scalar * m.get_value(x, y);
    }
}

void Matrix::scale(double scalar) {
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] *= scalar;
    }
}

void Matrix::convolution(Matrix kernel) {
    double sum;
    Matrix tmp = Matrix(get_height(), get_width());
    for(int y = 1; y < height - 1; y++){
        for(int x = 1; x < width - 1; x++){
            sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                    sum = sum + kernel.get_value(j+1, k+1) * get_value(y - j, x - k);
                }
            }
            tmp.set(x, y, sum);
        }
    }
    elements = tmp.elements;
}

void Matrix::normalize(double sum) {
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] /= sum;
    }
}

void Matrix::normalize() {
    double sum = this->sum();
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] /= sum;
    }
}

void Matrix::to_gaussian(int x_mean, int y_mean, double sigma) {
    double sum = 0;
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] = exp( -0.5 * (pow((x-x_mean)/sigma, 2.0) + pow((y-y_mean)/sigma,2.0)) )
        / (2 * M_PI * sigma * sigma);
        sum += elements[x][y];
    }
    normalize(sum);
}
/*
double Matrix::approximate_eccentricity(unsigned long x, unsigned long y) {
    double center_x = get_width() / 2;
    double center_y = get_height() / 2;
    
    double x_dist_squared = (center_x - x) * (center_x - x);
    double y_dist_squared = (center_y - y) * (center_y - y);
    
    return sqrt( x_dist_squared + y_dist_squared );
    
}*/
//------------------------------------------------------------------
// Accessors
//------------------------------------------------------------------

unsigned long Matrix::get_height() {
    return height;
}

unsigned long Matrix::get_width() {
    return width;
}

double Matrix::get_value(size_t x, size_t y) {
    return elements[x][y];
}

double Matrix::sum() {
    double sum = 0;
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        sum += elements[x][y];
    }
    return sum;
}

//------------------------------------------------------------------
// Others
//------------------------------------------------------------------

std::string Matrix::to_string() {
    std::stringstream result;
    
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            std::cout.width(10);
            if (j != 0) result << ",";
            result << std::setprecision(10) << elements[i][j];
        }
        result << std::endl;
    }
    return result.str();
}
