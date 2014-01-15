//
//  linear_algebra.h
//  saliency_map
//
//  Created by myke on 12/15/13.
//  Copyright (c) 2013 myke. All rights reserved.
//

#ifndef linear_algebra_h
#define linear_algebra_h

#include <vector>
#include <string>

using std::vector;

class Matrix {
private:
    unsigned long m_rows;
    unsigned long n_columns;
    vector< vector<double> > elements;
    
    void normalize(double);
    
public:
    //Constructors
    Matrix(vector< vector<double> >);
    Matrix(unsigned long, unsigned long);
    Matrix(unsigned long, unsigned long, double);
    Matrix(unsigned long);
    Matrix();
    //Matrix(const Matrix &);
    //~Matrix();
    
    //Mutators
    void resize(unsigned long, unsigned long, double initial_value = 0);
    void set(unsigned long, unsigned long, double);
    void add(Matrix);
    void scale(double);
    void add_submatrix(Matrix, unsigned long, unsigned long, double weight = 1);
    void to_gaussian(double);
    void normalize();
    void rectify_beyond_eccentricity(double,double);
    Matrix convolution(Matrix);
    
    //Accessors
    unsigned long get_m_rows();
    unsigned long get_n_columns();
    double get(unsigned long,unsigned long);
    double sum();
    double approximate_eccentricity(unsigned long,unsigned long);

    //void clean();
    
    std::string to_string();
    //void swap(Matrix &);

};


#endif
