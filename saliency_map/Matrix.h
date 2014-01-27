//
//  linear_algebra.h
//  saliency_map
//
//  Created by Michael Walton on 12/15/13.
//  Copyright (c) 2013 Michael Walton. All rights reserved.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

using std::vector;

class Matrix {
private:
    size_t width;
    size_t height;
    vector< vector<double> > elements;
    
public:
    //Constructors
    Matrix();
    Matrix(size_t n, double init_val);
    Matrix(size_t x, size_t y, double init_val);
    
    //Mutators
    void resize(size_t x, size_t y, double init_val);
    void set(size_t x, size_t y, double val);
    void set_all(double val);
    void linear_combination(Matrix m, double scalar);
    void scale(double scalar);
    void to_gaussian(int x_mean, int y_mean, double sigma);
    void normalize();
    void normalize(double sum);
    void convolution(Matrix m);
    
    //Accessors
    unsigned long get_height();
    unsigned long get_width();
    double get_value(size_t x, size_t y);
    double sum();
    
    //void clean();
    
    std::string to_string();
    //void swap(Matrix &);

};


#endif
