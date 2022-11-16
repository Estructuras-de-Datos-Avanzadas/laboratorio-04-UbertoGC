#include <iostream>
using namespace std;
struct colores
{
    long double red = -1;
    long double green = -1;
    long double blue = -1;
};
class octree_nodo
{
private:
    int cuadrante;
    int referencias;
    colores color_propio;
    octree_nodo *hijos;

public:
    octree_nodo();
    octree_nodo(int, int, int);
    void anadir(colores, int, int, octree_nodo*,int&);
    void reducir(octree_nodo*);
    friend ostream& operator << (ostream &,const octree_nodo &);
    ~octree_nodo();
};
octree_nodo::octree_nodo(){
    this->cuadrante = -1;
    this->referencias = 0;
    this->hijos = nullptr;
}
octree_nodo::octree_nodo(int red, int green, int blue){
    this->color_propio.red = red;
    this->color_propio.green = green;
    this->color_propio.blue = blue;
    this->cuadrante = 3;
    this->referencias = 0;
    this->hijos = new octree_nodo[8];
}
void octree_nodo::anadir(colores nuevo, int cuad, int maximo, octree_nodo *padre, int& hojas){
    if (this->hijos == nullptr){
        if (this->cuadrante == -1){
            this->cuadrante = cuad;
            this->color_propio.red = nuevo.red;
            this->color_propio.green = nuevo.green;
            this->color_propio.blue = nuevo.blue;
            this->referencias++;
            hojas++;
            return;
        }
        if(this->color_propio.red == nuevo.red && this->color_propio.blue == nuevo.blue && this->color_propio.green == nuevo.green){
            this->referencias++;
            return;
        }
        this->hijos = new octree_nodo[8];
        colores bajar_v;
        bajar_v.red = this->color_propio.red;
        bajar_v.green = this->color_propio.green;
        bajar_v.blue = this->color_propio.blue;
        if(cuad<4)
            this->color_propio.red = padre->color_propio.red - maximo;
        else
            this->color_propio.red = padre->color_propio.red + maximo;
        if(cuad < 2 || (cuad < 6 && cuad > 3))
            this->color_propio.blue = padre->color_propio.blue - maximo;
        else
            this->color_propio.blue = padre->color_propio.blue + maximo;
        if(cuad % 2 == 0)
            this->color_propio.green = padre->color_propio.green - maximo;
        else
            this->color_propio.green = padre->color_propio.green + maximo;
        int valor_1;
        if (bajar_v.red < this->color_propio.red){
            if (bajar_v.blue < this->color_propio.blue)
                if (bajar_v.green < this->color_propio.green)
                    valor_1 = 0;
                else
                    valor_1 = 1;
            else if (bajar_v.green < this->color_propio.green)
                valor_1 = 2;
            else
                valor_1 = 3;
        }
        else{
            if (bajar_v.blue < this->color_propio.blue)
                if (bajar_v.green < this->color_propio.green)
                    valor_1 = 4;
                else
                    valor_1 = 5;
            else if (bajar_v.green < this->color_propio.green)
                valor_1 = 6;
            else
                valor_1 = 7;
        }
        this->hijos[valor_1].anadir(bajar_v, valor_1, maximo/2, this,hojas);
        this->hijos[valor_1].referencias = this->referencias;
        this->referencias = 0;
    }
    int valor_2;
    if (nuevo.red < this->color_propio.red){
        if (nuevo.blue < this->color_propio.blue)
            if (nuevo.green < this->color_propio.green)
                valor_2 = 0;
            else
                valor_2 = 1;
        else if (nuevo.green < this->color_propio.green)
            valor_2 = 2;
        else
            valor_2 = 3;
    }
    else{
        if (nuevo.blue < this->color_propio.blue)
            if (nuevo.green < this->color_propio.green)
                valor_2 = 4;
            else
                valor_2 = 5;
        else if (nuevo.green < this->color_propio.green)
            valor_2 = 6;
        else
            valor_2 = 7;
    }
    this->hijos[valor_2].anadir(nuevo, valor_2, maximo/2, this,hojas);
}
void octree_nodo::reducir(octree_nodo*currentnode){
    for (int n = 0; n<8; n++){
        if (currentnode->hijos[n].cuadrante != -1) {
            currentnode->reference += currentnode->child[n].reference;
            currentnode->color_propio.red += currentnode->hijos[n].color_propio.red;
            currentnode->color_propio.green += currentnode->hijos[n].color_propio.green;
            currentnode->color_propio.blue += currentnode->hijos[n].color_propio.blue;
            delete (currentnode->hijos[n]);
        }
    }
}
ostream& operator << (ostream &o,const octree_nodo &a){
    if(a.hijos!=nullptr){
        for (int i = 0; i < 8; i++){
            o<<char(34)<<a.color_propio.red<<" - "<<a.color_propio.green<<" - "<<a.color_propio.blue<<"_nl"<<char(34)<<" -> ";
            if(a.hijos[i].cuadrante == -1)
                o<<"void"<<i<<"_"<<a.color_propio.red<<a.color_propio.green<<a.color_propio.blue<<"; ";
            else{
                o<<char(34)<<a.hijos[i].color_propio.red<<" - "<<a.hijos[i].color_propio.green<<" - "<<a.hijos[i].color_propio.blue;
                if(a.hijos[i].hijos != nullptr)
                    o<<"_nl"<<char(34)<<"; ";
                else
                    o<<char(34)<<"; ";
            }
        }
        o<<endl;
        for (int i = 0; i < 4; i++)
            if(a.hijos[i].hijos!=nullptr)
                o<<a.hijos[i];
    }
    return o;
}
octree_nodo::~octree_nodo(){}

class octree{
private:
    octree_nodo *raiz;
    colores inicio;
    int hojas;
public:
    octree(long double,long double,long double);
    void anadir(long double,long double,long double);
    void paleta();
    friend ostream& operator<<(ostream&, const octree &);
    ~octree();
};
octree::octree(long double val_1, long double val_2, long double val_3){
    hojas = 0;
    raiz = new octree_nodo(val_1/2,val_2/2,val_3/2);
}
void octree::anadir(long double red, long double green, long double blue){
    colores nuevo;
    nuevo.red = red;
    nuevo.green = green;
    nuevo.blue = blue;
    this->raiz->anadir(nuevo,3,128,this->raiz,this->hojas);
}
void octree::paleta(){
    int minima_ref;
    octree_nodo * tmp_buscador,tmp_determinador;
    while (hojas>256){
        /* code */
    }
}
ostream& operator<< (ostream &o,const octree &a){
    o << *(a.raiz);
    o << "------------------------------------------------------"<<endl;
    return o;
}
octree::~octree()
{
}

int main(){
    octree alfa(128,128,128);

    return 0;
}