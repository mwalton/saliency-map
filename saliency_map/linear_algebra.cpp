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

#include "linear_algebra.h"

//Constructor
Matrix::Matrix(unsigned long m,unsigned long n)
:m_rows(m), n_columns(n) {
    elements.resize( n_columns , vector<double>( m_rows , 0 ) );
}

Matrix::Matrix(unsigned long m,unsigned long n, double initial_val)
:m_rows(m), n_columns(n) {
    elements.resize( n_columns , vector<double>( m_rows , initial_val ) );
}

Matrix::Matrix(unsigned long n)
:m_rows(n), n_columns(n) {
    elements.resize(n_columns, vector<double> (m_rows, 0));
    for(int i = 0; i < n; i++) {
        elements[i][i] = 1;
    }
}

Matrix::Matrix(vector< vector<double> > matrix_elems)
:elements(matrix_elems) {
    m_rows = matrix_elems.size();
    n_columns = matrix_elems[0].size();
}

//Empty constructor
Matrix::Matrix()
:m_rows(0), n_columns(0) {
    std::vector< std::vector<double> > elements;
}
/*
//Copy constructor
Matrix::Matrix(const Matrix &A)
:L(A.L), M(A.M), N(A.N)
{
    G = new double ** [L];
    int i,j,k;
    for (i=0;i<L;i++){
        G[i] = new double * [M];
        for (j=0;j<M;j++){
            G[i][j] = new double [N];
            for (k=0;k<N;k++){
                G[i][j][k] = A.G[i][j][k];
            }
        }
    }
}

//Destructor
Matrix::~Matrix()
{
    // Free memory
    for (int i=0;i<L;i++){
        for (int j=0;j<M;j++){
            delete [] G[i][j];
            G[i][j] = NULL;
        }
        delete [] G[i];
        G[i] = NULL;
    }
    delete G;
    G = NULL;
}

//------------------------------------------------------------------
// Mutators
//------------------------------------------------------------------

 
 
//Access operator ():
double& Matrix::operator()(int i,int j,int k)
{
    assert(i >= 0 && i < L);
    assert(j >= 0 && j < M);
    assert(k >= 0 && k < N);
    return G[i][j][k];
}

//Assignment operator
Matrix& Matrix::operator = (Matrix A)
{
    swap(A);
    return *this;
}
*/

void Matrix::resize(unsigned long m, unsigned long n, double initial_value) {
    m_rows = m;
    n_columns = n;
    elements.resize( n_columns , vector<double>( m_rows , initial_value ) );
}

void Matrix::set(unsigned long x, unsigned long y, double val) {
    elements[x][y] = val;
}

void Matrix::linear_combination(Matrix m, double scalar) {
    assert(m.get_m_rows() == get_m_rows());
    assert(m.get_n_columns() == get_n_columns());
    
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] += scalar * m.get(x, y);
    }
}

void Matrix::scale(double scalar) {
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] *= scalar;
    }
}

void Matrix::convolution(Matrix kernel) {
    double sum;
    Matrix tmp = Matrix(get_m_rows(),get_n_columns());
    for(int y = 1; y < m_rows - 1; y++){
        for(int x = 1; x < n_columns - 1; x++){
            sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                    sum = sum + kernel.get(j+1, k+1) * get(y - j, x - k);
                }
            }
            tmp.set(x, y, sum);
        }
    }
    elements = tmp.elements;
}

void Matrix::normalize(double sum) {
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] /= sum;
    }
}

void Matrix::normalize() {
    double sum = this->sum();
    
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] /= sum;
    }
}

void Matrix::to_gaussian(double sigma) {
    double mean = n_columns / 2;
    double sum = 0;
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
        / (2 * M_PI * sigma * sigma);
        sum += elements[x][y];
    }
    normalize(sum);
}

void Matrix::to_gaussian(double sigma, double x_mean, double y_mean) {
    double sum = 0;
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        elements[x][y] = exp( -0.5 * (pow((x-x_mean)/sigma, 2.0) + pow((y-y_mean)/sigma,2.0)) )
        / (2 * M_PI * sigma * sigma);
        sum += elements[x][y];
    }
    normalize(sum);
}

double Matrix::approximate_eccentricity(unsigned long x, unsigned long y) {
    double center_x = get_n_columns() / 2;
    double center_y = get_m_rows() / 2;
    
    double x_dist_squared = (center_x - x) * (center_x - x);
    double y_dist_squared = (center_y - y) * (center_y - y);
    
    return sqrt( x_dist_squared + y_dist_squared );
    
}
//------------------------------------------------------------------
// Accessors
//------------------------------------------------------------------

unsigned long Matrix::get_m_rows() {
    return m_rows;
}

unsigned long Matrix::get_n_columns() {
    return n_columns;
}

double Matrix::get(unsigned long i,unsigned long j) {
    return elements[i][j];
}

double Matrix::sum() {
    double sum = 0;
    for (int x = 0; x < n_columns; ++x) for (int y = 0; y < m_rows; ++y) {
        sum += elements[x][y];
    }
    return sum;
}


/*
void Matrix::clean()
{
    int i,j,k;
    for (i=0;i<L;i++){
        for (j=0;j<M;j++){
            for (k=0;k<N;k++){
                G[i][j][k] = NAN;
            }
        }
    }
}

//------------------------------------------------------------------
// Others
//------------------------------------------------------------------

//Swap
void Matrix::swap(Matrix & A)
{
    std::swap(L, A.L);
    std::swap(M, A.M);
    std::swap(N, A.N);
    std::swap(G, A.G);
}
*/
std::string Matrix::to_string() {
    std::stringstream result;
    
    for (int i=0; i < m_rows; i++){
        for (int j=0; j < n_columns; j++){
            std::cout.width(10);
            if (j != 0) result << ",";
            result << std::setprecision(10) << elements[i][j];
        }
        result << std::endl;
    }
    return result.str();
}

//cout
//std::ostream& operator<<(std::ostream& o, Matrix& m) { return m.to_string(o); }
