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
    Matrix(unsigned long,unsigned long);
    Matrix(unsigned long);
    Matrix();
    //Matrix(const Matrix &);
    //~Matrix();
    
    //Mutators
    void set(unsigned long, unsigned long, double);
    void add(Matrix);
    void add_submatrix(Matrix, unsigned long, unsigned long, double);
    void to_gaussian(double);
    void normalize();
    Matrix convolution(Matrix);
    
    //Accessors
    unsigned long get_m_rows();
    unsigned long get_n_columns();
    double get(unsigned long,unsigned long);
    double sum();

    //void clean();
    
    std::string to_string();
    //void swap(Matrix &);

};


#endif
