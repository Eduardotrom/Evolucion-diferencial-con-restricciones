#include "De.hpp"
#include "var_types.hpp"
#include <iostream>
#include "funciones.hpp"
#include <cstring>
using namespace std;

void help_message();


int main (int argc, char*argv[]){
    //Revisar los  limites para cada funcion
    int pob=100,iters=1000,func=0;
    mat_db lims;
    string nom="convergencia_himmelblau.txt";
    for(int i=0;i<argc;i++){
		if(0==strcmp(argv[i],"-f")){
			func=atoi(argv[i+1]);
		}
        if(0==strcmp(argv[i],"-i")){
		    iters=atoi(argv[i+1]);
        }
        if(0==strcmp(argv[i],"-p")){
		    pob=atoi(argv[i+1]);
        }
        if(0==strcmp(argv[i],"-h")){
		    help_message();
            exit(0);
		}
	}


    std::vector<t_ff(*)(vec_db)>funciones(4);
    //aqui se asignan las funciones al vector de punteros a funcion.
    funciones[0]=&himmelblau_nl;
    funciones[1]=&spring;
    funciones[2]=&p_vessel;
    funciones[3]=&beam;
    int dims;
    vec_db rango(2);
    switch(func){
        case 0:
            //Aqui se definen los rangos  para himmelblau
            dims=5;
            rango[0]=78;
            rango[1]=100;
            lims.reserve(dims);
            lims.push_back(rango);
            rango[0]=33;
            rango[1]=45;
            lims.push_back(rango);
            rango[0]=27;
            rango[1]=45;
            lims.push_back(rango);
            lims.push_back(rango);
            lims.push_back(rango);
        break;
    //aqui se definen los rangos para spring
        case 1:
            dims=3;
            rango[0]=0;
            rango[1]=15;
            lims.reserve(dims);
            lims.push_back(rango);
            lims.push_back(rango);
            lims.push_back(rango);
            nom="convergencia_resorte.txt";
            break;
        //aqui se definen los rangos para la olla a presion
        case 2:
            dims=4;
            rango[0]=0;
            rango[1]=100;
            lims.reserve(dims);
            lims.push_back(rango);
            rango[0]=0;
            rango[1]=100;
            lims.push_back(rango);
            rango[0]=0;
            rango[1]=100;
            lims.push_back(rango);
            rango[0]=0;
            rango[1]=250;
            lims.push_back(rango);
            nom="convergencia_olla.txt";
            break;
        // En este espacio se definen los rangos para la viga
        case 3:
            dims=4;
            rango[0]=0;
            rango[1]=15;
            lims.reserve(dims);
            lims.push_back(rango);
            lims.push_back(rango);
            lims.push_back(rango);
            lims.push_back(rango);
            nom="convergencia_viga.txt";
            break;
        default:
         cout<<"Funcion no valida"<<endl;
         exit(0);
    }
    //Aqui se termina con la definicion de los rangos y se ejecuta el optimizador
    srand(time(NULL));
    Diff_evolution prueba(pob,lims,iters,1.5,0.8,funciones[func]);
    prueba.pob_init();
    prueba.optimiza(-1e5);
    prueba.conv_print(nom);
    auto res=prueba.get_best();
    for(double x:res)
        cout<<x<<endl;

    return 0;
}





void help_message(){
    cout<<"----------------------------------------------------------------------------------------------------"<<endl;
    cout<<"Programa que utiliza un algoritmo genetico para resolver instancias del reto RTE                   "<<endl;
    cout<<"El programa recibe desde terminal la siguiente informacion precedida por las siguientes banderas   "<<endl;
    cout<<"             -f    Numero de funcion a optimizar:                                                  "<<endl;
    cout<<"                   1-Funcion de Himmelblau.                                                        "<<endl;
    cout<<"                   2-Funcion del resorte.                                                          "<<endl;
    cout<<"                   3-Optimizacion de la olla a presion.                                            "<<endl;
    cout<<"                   4-Optimizacion de la viga soldada.                                              "<<endl;
    cout<<"             -i    Numero maximo permitido de iteraciones en la optimizacion                       "<<endl;
    cout<<"             -p    Numero de pobladores                                                            "<<endl;
    cout<<"             -h    Imprime este mensaje de ayuda                                                   "<<endl;
    cout<<"----------------------------------------------------------------------------------------------------"<<endl;
}