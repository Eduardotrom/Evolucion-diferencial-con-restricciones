#ifndef DE_HPP
#define DE_HPP
#include<algorithm>
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include "var_types.hpp"
#include <tuple>

using namespace std;

class individuo{
    private:
        vec_db pos;
        t_ff fitness;
    public:
        individuo(){};
        individuo(int n);
        vec_db get_pos();
        void set_pos(vec_db &x);
        void set_fitness(t_ff &x);
        t_ff get_fitness();
};

class Diff_evolution{
    private:
        float p_mut,CR,f;
        int iter,iter_max,n_pob,dims;
        tuple<float,float> (*f_obj)(vec_db),ap_best;
        vec_db best;
        individuo best_ind;
        vector<tuple<float,float>>  aptitud,convergencia;
        mat_db limites;
        vector<individuo> poblacion;
    public:
        Diff_evolution(){};
        Diff_evolution(int pob,mat_db limits,int iter_max, double ff,double Cr, tuple<float,float> (*f)(vec_db));
        void evaluacion();
        void limits_chk(vec_db &hijos);
        vec_db cruzar(int r1,int r2,int r3);
        void primavera();
        void reemplazo(vector<individuo> &hijos);
        void pob_init();
        void optimiza(double tol=1e-6);
        void show_best();
        void show_best_adapt();
        vec_db get_best();
        tuple<float,float> get_error();
        void conv_print(string nom);
        vec_db current_to_pbest(int r1,int r2);
};

bool compare_ind(individuo &i1,individuo &i2);
#endif