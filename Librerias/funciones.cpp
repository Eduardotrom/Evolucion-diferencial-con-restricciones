#include "funciones.hpp"
#include <cmath>
using namespace std;
//---------------------------------------Funciones principales a optimizar------------------------------------------------------ 
//------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------Función de himmelblau no linear--------------------------------------------------------
t_ff himmelblau_nl(std::vector<double> x){
    int n=x.size();
    if(n!=5)return std::make_tuple(999999.9,99999.9);
    float func=5.3578547*x[2]*x[2]+0.8356891*x[0]*x[4]+37.29323*x[0]-40792.141;
    //aqui se evaluaran las restricciones
    float rest=0,rt;
    //restriccion 1
    rt=85.33407+0.0056858*x[1]*x[4]+0.00026*x[0]*x[3]-2*0.0022053*x[2]*x[4];
    rest+=((0<=rt)&&(rt<=92))?0:rt;
    //restriccion 2
    rt=80.51249+0.0071317*x[1]*x[4]+0.0029955*x[0]*x[1]+0.0021813*x[2]*x[2]+ 0.0021813*x[2]*x[2];
    rest+=((90<=rt)&&(rt<=110))?0:rt;
    //restriccion 3
    rt=9.300961+0.0047026*x[2]*x[4]+0.0012547*x[0]*x[2]+0.0019085*x[2]*x[3];
    rest+=((20<=rt)&&(rt<=25))?0:rt;
    return std::make_tuple(abs(rest),func);
}
//---------------------------------------Minimizacion del peso de un resorte de Tension compresion------------------------------
//------------------------------------------------------------------------------------------------------------------------------
t_ff spring(std::vector<double> x){
    int n=x.size();
    if(n!=3)return std::make_tuple(999999.9,99999.9);
    float fun=(x[2]+2)*x[0]*x[0]*x[1],rest=0,rt;
    //restriccion 1
    rt=1-((pow(x[1],3)*x[2])/(pow(x[0],4)));
    rest+=(0>=rt)?0:rt;
    //Restriccion 2
    rt=(4*pow(x[2],2)-x[0]*x[1])/(12566*((pow(x[0],3)*x[1]-pow(x[0],4))));
    rt+=1/(5108*x[0]*x[0])-1;
    rest+=(0>=rt)?0:rt;
    //Restriccion 3
    rt=1-(140.45*x[0]/(pow(x[1],2)*x[2]));
    rest+=(0>=rt)?0:rt;
    //Restriccion 4
    rt=(x[0]+x[1])/1.5-1;
    rest+=(0>rt)?0:rt;
    return std::make_tuple(abs(rest),fun);
}

//---------------------------------------Diseño de un recipiente a presión------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
t_ff p_vessel(std::vector<double> x){
    int n=x.size();
    x[0]=int(x[0]+1)*0.0625
    x[1]=int(x[1]+1)*0.0625
    if(n!=4)return std::make_tuple(999999.9,99999.9);
    float fun=0.6224*x[0]*x[2]*x[3]+1.7781*x[1]*pow(x[2],2)+3.1661*x[0]*pow(x[3],2)+19.84*x[0]*pow(x[2],2);
    float rt,rest=0,pi=acos(-1.0);
    //restriccion 1
    rt=-x[0]+0.0193*x[2];
    rest+=(0>=rt)?0:rt;
    //restriccion 2
    rt=-x[1]+0.00954*x[2];
    rest+=(0>=rt)?0:rt;
    //restriccion 4
    rt=-pi*pow(x[2],2)*x[3]-(4.0/3.0)*pi*pow(x[2],3)+1296000;
    rest+=(0>=rt)?0:rt;
    //restriccion 4
    rt=x[3]-240;
    rest+=(0>=rt)?0:rt;
    return std::make_tuple(abs(rest),fun);
}
//--------------------------------------Diseño de una viga soldada--------------------------------------------------------------
t_ff beam(std::vector<double> x){
    //Esta funcion requiere de una gran cantidad de parametros
    float P=6e3,smax=3e4,dmax=0.25,tmax=13600,E=30e6,G=12*10e6,L=14;
    //Asi como de valores secundarios que se requiere evaluar en otras funciones.
    float M=M_f(P,L,x);
    float R=fr(x);
    float sig=sigma(P,L,x);
    float J=J_f(x);
    float delta=delta_f(P,L,E,x);
    float pc=P_c(E,L,G,x);
    float tau=tau_f(P,M,R,J,x);
    float fun=1.1047*x[0]*x[0]*x[1]+0.04811*x[2]*x[3]*(14+x[1]);
    float rt,rest=0;
    //Restriccion 1
    rt=tau-tmax;
    rest+=(0>rt)?0:rt;
    //Restriccion 2
    rt=sig-smax;
    rest+=(0>rt)?0:rt;
    //Restriccion 3
    rt=x[0]-x[3];
    rest+=(0>rt)?0:rt;
    //restriccion 4
    rt=0.125-x[1];
    rest+=(0>rt)?0:rt;
    //Restriccion 5
    rt=delta-dmax;
    rest+=(0>rt)?0:rt;
    //Restriccion 6
    rt=P-pc;
    rest+=(0>rt)?0:rt;
    //Restriccion 7
    rt=0.10471*x[0]*x[0]+0.04811*x[2]*x[3]*(14.0+x[1])-5.0;
    rest+=(0>rt)?0:rt;
    return std::make_tuple(abs(rest),fun);
}


float M_f(float P,float L,std::vector<double> &x){
    float m=P*(L+(x[2]/2));
}
float fr(std::vector<double>&x){
    float r1=(pow(x[1],2)/4+pow(((x[0]+x[2]),2)/2,2));
    return sqrt(r1);
    
}
float sigma(float P,float L,std::vector<double>& x){
    return 6*P*L/(x[3]*pow(x[2],2));
    
}
float J_f(std::vector<double> &x){
    float j1=sqrt(2)*x[0]*x[1];
    float j0=pow(x[1],2)/12.0+pow(((x[0]+x[2])/2.0),2.0);
    return 2.0*j1*j0;
}
float P_c(float E,float L,float G,vec_f & x){
    float p1=pow(x[2],2)*pow(x[3],6)/36.0;
    p1=4.013*E*sqrt(p1)/(L*L);
    float p2=1-(x[2]/(2*L))*sqrt(E/(4*G));
    return p1*p2;
}
float delta_f(float P,float L,float E,vec_f &x){
    return 4*P*L*L*L/(E*pow(x[2],3)*x[3]);
}
float tau_f(float P,float M,float R,float J,vec_f &x){
    float tp=P/(sqrt(2)*x[0]*x[1]);
    float tpp=M*R/J;
    float rs=tp*tp+((2*tp*x[1])/(2.0*R))+tpp*tpp;
    return sqrt(rs);
}



//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------Estas son las funciones tipicas para optimizacion con restricciones-----------------
//------------------------------------------Se programaron unicamente 2 ya que eso se habia mencionado al prinicipio------------
//------------------------------------------Se deja para futura referencia------------------------------------------------------
tuple<float,float>g1(vector<double> x){  
    int n=x.size();
    if(n!=13){
        cout<<"El vector de entrada no tiene dimensiones correctas "<<endl;
        exit(0);
    }
    float ff=0;
    for(int i=0;i<4;i++){
        ff+=5*x[i]-5*x[i]*x[i];
    }
    for(int i=4;i<n;i++){
        ff-=x[i];
    }
    //aqui empiezan las restricciones
    float stv=0;
    stv+=(2*x[0]+2*x[1]+x[9]+x[10]-10>0);
    stv+=(2*x[0]+2*x[2]+x[9]+x[11]-10>0);
    stv+=(2*x[1]+2*x[2]+x[10]+x[11]-10>0);
    stv+=(-8*x[0]+x[9]>0);
    stv+=(-8*x[1]+x[10]>0);
    stv+=(-8*x[2]+x[11]>0);
    stv+=(-8*x[0]+x[9]>0);
    stv+=(-2*x[3]-x[4]+x[9]>0);
    stv+=(-2*x[5]-x[6]+x[10]>0);
    stv+=(-2*x[7]-x[8]+x[11]>0);
    return std::make_tuple(stv,ff);
}
std::tuple<float,float>g2(vector<double> x){  
    int n=x.size();
    if(n!=20){
        cout<<"El vector de entrada no tiene dimensiones correctas "<<endl;
        exit(0);
    }
    float num=0,den=0,prod=1,sumg=0,prodg=1;
    for(int i=0;i<n;i++){
        num+=pow(cos(x[i]),4);
        prod*=pow(cos(x[i]),2);
        den=i*x[i]*x[i];
        sumg+=x[i];
        prodg*=x[i];
    }
    float f=abs((num-2*prod)/sqrt(den));
    float stv=0;
    stv+=(0.75-prodg>0);
    stv+=(sumg-7.5*20>0);
    return std::make_tuple(stv,f);
}
std::tuple<float,float> g3(vector<double> x){  
    int n=x.size();
    float prod=1,sum=0;
    for(int i=0;i<n;i++){
        prod*=x[i];
        sum+=x[i]*x[i];
    }
    float f=pow(sqrt((float)n),n)*prod;
    float stv=(sum-1!=0);
    return std::make_tuple(stv,f);

}
