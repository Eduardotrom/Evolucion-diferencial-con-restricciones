#ifndef VAR_TYPES_HPP
#define VAR_TYPES_HPP
#include<tuple>
#include <vector>
typedef std::vector<int> vec_int;
typedef std::vector<double> vec_db;
typedef std::vector<vec_int> mat_int;
typedef std::vector<vec_db> mat_db;
typedef std::tuple<float,float> t_ff;
#endif