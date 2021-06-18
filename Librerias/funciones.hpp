#include <vector>
#include <iostream>
#include <tuple>

typedef std::tuple<float,float> t_ff;
typedef std::vector<double> vec_f;


t_ff himmelblau_nl(std::vector<double> x);
t_ff spring(std::vector<double> x);
t_ff p_vessel(std::vector<double>x);
t_ff beam(std::vector<double> x);
float M_f(float P,float L,std::vector<double> &x);
float fr(std::vector<double>&x);
float sigma(float P,float L,std::vector<double>& x);
float J_f(std::vector<double> &x);
float P_c(float E,float L,float G,vec_f & x);
float delta_f(float P,float L,float E,vec_f &x);
float tau_f(float P,float M,float R,float J,vec_f &x);


t_ff g1(vec_f x);
t_ff g2(vec_f x);
t_ff g3(vec_f x);