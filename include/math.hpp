# pragma once

# include <eigen3/Eigen/Core>
# include <eigen3/Eigen/Dense>

# include <cmath>

typedef double distance_type;

template< typename T, unsigned int Dim >
using vec_type = Eigen::Matrix< T, Dim, 1 >;

typedef vec_type< double, 3 > vec3_type;

template< typename T, unsigned int DimM, unsigned int DimN >
using mat_type = Eigen::Matrix< T, DimM, DimN >;

typedef mat_type< double, 3, 3 > mat33_type;
typedef mat_type< double, 4, 4 > mat44_type;