/*
 *  Matrix.cpp
 *  Visual Attention Processor
 *
 *  Created by Michael Walton on 12/15/13.
 *  Copyright (c) 2013 Michael Walton. All rights reserved.
 */

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
    max = 1;
}

//NxN with initial value
Matrix::Matrix(size_t n, double init_val)
:height(n), width(n) {
    elements.resize( width , vector<double> ( height , init_val ));
    for(int i = 0; i < n; i++) {
        elements[i][i] = 1;
    }
    max = 1;
}

Matrix::Matrix(size_t x, size_t y, double init_val)
:width(x), height(y) {
    elements.resize( width , vector<double> ( height , init_val ));
    max = 1;
}

//------------------------------------------------------------------
// Mutators
//------------------------------------------------------------------
void Matrix::resize(size_t x, size_t y, double inti_val) {
    width = x;
    height = y;
    
    elements.assign( width , vector<double>( height , inti_val ) );
}

void Matrix::set(unsigned long x, unsigned long y, double val) {
    elements[x][y] = val;
    if (elements[x][y] > max) max = elements[x][y];
}

void Matrix::linear_combination(Matrix m, double scalar) {
    assert(m.get_height() == get_height());
    assert(m.get_width() == get_width());
    
    max = 0;
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] += scalar * m.get_value(x, y);
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::scale(double scalar) {
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] *= scalar;
    }
    max = max * scalar;
}

void Matrix::convolution(Matrix kernel) {
    double sum;
    max = 0;
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
            if (sum > max) max = sum;
        }
    }
    elements = tmp.elements;
}

void Matrix::normalize(double sum) {
    max = 0;
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        //std::cout << elements[x][y] << " / " << sum <<std::endl;
        elements[x][y] /= sum;
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::normalize() {
    double sum = this->sum();
    max = 0;
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] /= sum;
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::to_gaussian(int x_mean, int y_mean, double sigma) {
    max = 0;
    double sum = 0;
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] = exp( -0.5 * (pow((x-x_mean)/sigma, 2.0) + pow((y-y_mean)/sigma,2.0)) )
        / (2 * M_PI * sigma * sigma);
        
        sum += elements[x][y];
        if (elements[x][y] > max) max = elements[x][y];
    }
    //normalize(sum);
}

void Matrix::to_linear(double m) {
    max = 0;
    double sum = 0;
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] = m * x;
        sum += elements[x][y];
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::to_positive_sigmoid(double upper_asymptote) {
    max = 0;
    double sum = 0;
    double x_origin = width / 2;
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] = (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((x -x_origin)))));
        sum += elements[x][y];
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::to_negative_sigmoid(double upper_asymptote) {
    max = 0;
    double sum = 0;
    double x_origin = width / 2;
    
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        elements[x][y] = (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((x -x_origin)))));
        sum += elements[x][y];
        if (elements[x][y] > max) max = elements[x][y];
    }
}

void Matrix::to_yaxis_sigmoid(double upper_asymptote, bool positive) {
    max = 0;
    double sum = 0;
    double y_origin = width / 2;
    
    if (positive) {
        for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
            elements[x][y] = (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((y -y_origin)))));
            sum += elements[x][y];
            if (elements[x][y] > max) max = elements[x][y];
        }
    } else {
        for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
            elements[x][y] = (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((y -y_origin)))));
            sum += elements[x][y];
            if (elements[x][y] > max) max = elements[x][y];
        }
    }
}

void Matrix::to_multivariable_sigmoid(double upper_asymptote, int quadrant) {
    max = 0;
    double sum = 0;
    double x_origin = width / 2;
    double y_origin = height / 2;
    
    upper_asymptote /= 2;
    
    switch (quadrant) {
        case 1:
            for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
                elements[x][y] =
                (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((x - x_origin)))))
                + (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((y - y_origin)))));
                sum += elements[x][y];
                if (elements[x][y] > max) max = elements[x][y];
            }
            break;
        case 2:
            for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
                elements[x][y] =
                (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((x - x_origin)))))
                + (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((y - y_origin)))));
                sum += elements[x][y];
                if (elements[x][y] > max) max = elements[x][y];
            }
            break;
        case 3:
            for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
                elements[x][y] =
                (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((x - x_origin)))))
                + (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((y - y_origin)))));
                sum += elements[x][y];
                if (elements[x][y] > max) max = elements[x][y];
            }
            break;
        case 4:
            for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
                elements[x][y] =
                (double)upper_asymptote/((double)1+exp(double(-8.0 / double(width) * double((x - x_origin)))))
                + (double)upper_asymptote/((double)1+exp(double(8.0 / double(width) * double((y - y_origin)))));
                sum += elements[x][y];
                if (elements[x][y] > max) max = elements[x][y];
            }
            break;
        default:
            break;
    }
}

//------------------------------------------------------------------
// Accessors
//------------------------------------------------------------------

unsigned long Matrix::get_height() {
    return height;
}

unsigned long Matrix::get_width() {
    return width;
}

double Matrix::get_value(int x, int y) {
    return elements[x][y];
}

double Matrix::sum() {
    double sum = 0;
    for (int x = 0; x < width; ++x) for (int y = 0; y < height; ++y) {
        sum += elements[x][y];
    }
    return sum;
}

double Matrix::get_max() {
    return max;
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
