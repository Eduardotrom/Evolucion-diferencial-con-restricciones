#include "De.hpp"
#include "var_types.hpp"
Diff_evolution::Diff_evolution(int pob,mat_db limits,int iter_max, double ff,double Cr, tuple<float,float> (*f)(vec_db)){
            this->n_pob=pob;
            this->iter_max=iter_max;
            this->limites=limits;
            this->dims=limits.size();
            this->poblacion=vector<individuo>(pob,individuo(dims));
            this->aptitud.resize(pob);
            this->f_obj=f;
            this->f=ff;
            this->convergencia.reserve(iter_max);
            this->CR=Cr;
            //cout<<"Se inicializo el metodo para "<<this->n_pob<<" pobladores de "<<this->dims<<" dimensiones"<<endl;
}


void Diff_evolution::pob_init(){
    vec_db temp(this->dims);
    double *p_pob=temp.data();
    for(int i=0;i<this->n_pob;i++){
        //cout<<" El poblador "<<i<<" es:"<<endl;
        for(int j=0;j<this->dims;j++){
            double *p_lims=this->limites[j].data();
            double rango=p_lims[1]-p_lims[0];
            double r=rand()/static_cast<double>(RAND_MAX);
            p_pob[j]=p_lims[0]+rango*r;
            //cout<<this->poblacion[i][j]<<" ";
        }//cout<<endl;
        this->poblacion[i].set_pos(temp);
    }
    this->best=poblacion[0].get_pos();
    this->ap_best=(this->f_obj)(best);
    poblacion[0].set_fitness(this->ap_best);
    this->best_ind=poblacion[0];
}


vec_db Diff_evolution::cruzar(int r1,int r2,int r3){
    vec_db v1=this->poblacion[r1].get_pos(),v2=this->poblacion[r2].get_pos(),v3=this->poblacion[r3].get_pos();
    vec_db hijo=v1,bst=this->best_ind.get_pos();
    
    for(int i=0;i<hijo.size();i++){
        hijo[i]+=(this->f)*(v2[i]-v3[i])+(this->f)*(bst[i]-v1[i]);
    }
    return hijo;
}

void Diff_evolution::limits_chk(vec_db &hijos){
    for(int i=0;i<this->dims;i++){
        double tmp=hijos[i];
        if(tmp>this->limites[i][1]||tmp<this->limites[i][0]){
            double rr=rand()/(double)RAND_MAX;
            double dist=this->limites[i][1]-this->limites[i][0];
            tmp=this->limites[i][0]+rr*dist;
            hijos[i]=tmp;
        }
    }
}

void Diff_evolution::primavera(){
    int n=this->n_pob;
    int parents_num=(n*0.2+1<3)?3:n*0.2+1;
    vector<individuo> hijos(n,this->poblacion[0]);
    std::sort(poblacion.begin(),poblacion.end(),compare_ind);
    for(int i=0;i<n;i++){
        int r1=rand()%(parents_num);
        while(r1==i)r1=rand()%(parents_num);
        int r2=rand()%(parents_num);
        while(r2==i||r2==r1)r2=rand()%(parents_num);
        int r3=rand()%(parents_num);
        while(r3==i||r3==r1||r3==r2)r3=rand()%(parents_num);
        if(rand()/(double)RAND_MAX<=this->CR){
            vec_db tmp=cruzar(r1,r2,r3);   
            limits_chk(tmp);
            hijos[i].set_pos(tmp);
            
        }else{
            hijos[i]=this->poblacion[i];
        }
        reemplazo(hijos);
    }
}


void Diff_evolution::reemplazo(vector<individuo> &hijos){
    for(int i=0;i<this->n_pob;i++){
        tuple<float,float> eval=this->f_obj(hijos[i].get_pos());
        hijos[i].set_fitness(eval);
        /*if(hijos[i].get_fitness()<this->poblacion[i].get_fitness()){
            this->poblacion[i]=hijos[i];*/
        this->poblacion[i]=hijos[i];
            if(eval<this->ap_best){
                ap_best=eval;
                this->best=hijos[i].get_pos();
                this->best_ind=hijos[i];
            }
        //}
    }
}

void Diff_evolution::evaluacion(){
    for(int i=0;i<this->n_pob;i++){
        auto tmp=(this->f_obj)(this->poblacion[i].get_pos());
        this->poblacion[i].set_fitness(tmp);
        if(this->poblacion[i].get_fitness()<this->best_ind.get_fitness()){
            this->ap_best=poblacion[i].get_fitness();
            this->best=poblacion[i].get_pos();
            this->best_ind=poblacion[i];
        }
    }
}

void Diff_evolution::optimiza(double tol){
    int cont=0;
    for(iter=0;iter<this->iter_max;iter++){
        evaluacion();
        this->convergencia.push_back(best_ind.get_fitness());
        if(get<1>(this->best_ind.get_fitness())<=tol)break;
        primavera();
        //cout<<iter<<" "<<this->ap_best<<endl;
    }
    
}

void Diff_evolution::show_best(){
    cout<<"El mejor hasta el momento es"<<endl;
    for(int i=0;i<this->dims;i++){
        cout<<this->best[i]<<endl;
    }
}

void Diff_evolution::show_best_adapt(){
    printf("El mejor tiene un error de %5f, %5f\n",get<0>(this->ap_best),get<1>(this->ap_best));
}



vec_db Diff_evolution::get_best(){
     return this->best_ind.get_pos();
}

tuple<float,float> Diff_evolution::get_error(){
    return this->ap_best;
}

void Diff_evolution:: conv_print(string nom){
    ofstream f(nom);
    for(int i=0;i<this->convergencia.size();i++)
        f<<i<<" "<<get<0>(this->convergencia[i])<<" "<<get<1>(this->convergencia[i])<<" "<<std::endl;
    f.close();
}

individuo:: individuo(int n){
    this->pos.resize(n);
    this->fitness=std::make_tuple(9999.99,9999.99);
}
vec_db individuo::get_pos(){
    return this->pos;
}
void individuo::set_pos(vec_db &x){
    this->pos=x;
}
t_ff individuo::get_fitness(){
    return this->fitness;
}
void individuo::set_fitness(t_ff &x){
    this->fitness=x;
}





bool compare_ind(individuo &i1,individuo &i2){
    return (i1.get_fitness()<i2.get_fitness());
}