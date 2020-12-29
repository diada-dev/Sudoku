#include <iostream>

using namespace std;

//change calc to precalc
/*
uint16_t returnPower(int8_t val){
    if (val==0) return 0;
    int16_t rez = 1;
    return rez << --val;
}
*/
uint16_t returnPower[10];
void calcPower(){
    uint16_t x,y;
    returnPower[0] = 0;
    y = 1;
    for (x=1;x<10;x++){
        returnPower[x]=y;
        y = y << 1;
    }
}

uint8_t calcBitNum(uint16_t n){
    uint16_t rez,x;
    rez = 0;
    for (x=0;x<9;x++){
        if ((n & 1) == 0) rez++;
        n = n >> 1;
    }
    return rez;
}
uint8_t calcBitPlace(uint16_t n){
    uint8_t x;
    for (x=0;x<9;x++){
        if ((n & 1) == 0) return (x+1);
        n = n >> 1;
    }
    return 0;
}

class matrix{
private:
    uint8_t m[9][9];
    uint16_t mx[9], my[9], mz[9];

public:
    matrix(uint8_t* n){
        uint8_t x,y;
        for (y=0;y<9;y++){
            for (x=0;x<9;x++, n++) m[y][x] = *n;
        }
    }

    void print(){
        uint8_t x,y;
        for (y=0;y<9;y++){
            for (x=0;x<9;x++)
                cout<<static_cast<int16_t>(m[y][x])<<" ";
            cout<<endl;
        }
    }

    void setn(uint8_t y, uint8_t x, uint8_t val){
        m[y][x] = val;
    }

    uint8_t retn(uint8_t y, uint8_t x){
        return m[y][x];
    }

    uint8_t calculate(){
        uint8_t x,y,z,dz,r,nc,x1,y1;
        uint16_t t,dt,rP,t1;
        bool flag;

        for (y=0;y<9;y++) {my[y] = 0; mx[y] = 0; mz[y] = 0;};

        for (y=0;y<9;y++){
            for (x=0;x<9;x++){
              rP = returnPower[(m[y][x])];

              my[x] = my[x] + rP;
              mx[y] = mx[y] + rP;

              z = (y/3) * 3 + (x/3);
              mz[z] = mz[z] + rP;
            }
        }

        //place num
        flag = true;
        while(flag){
            flag = false;
            nc = 0;
            for (y=0;y<9;y++){
                for (x=0;x<9;x++){
                    if (m[y][x] == 0){
                        t = mx[y] | my[x];
                        dz = (y/3) * 3 + (x/3);
                        t = t | mz[dz];

                        r = calcBitNum(t);
                        switch(r){
                            case 0: return 1;
                            case 1:
                                m[y][x] = calcBitPlace(t);
                                dt = 511 - t;
                                my[x] = my[x] + dt;
                                mx[y] = mx[y] + dt;
                                mz[dz] = mz[dz] + dt;
                                flag=true;
                                break;
                            default:
                                nc++;
                                x1 = x;
                                y1 = y;
                                t1 = t;
                        }
                    }
                }
            }
        }

        if (nc==0) return 0;

        for (x=0;x<9;x++){
            if ((t1 & 1) == 0){
                matrix* md = new matrix(&m[0][0]);
                md->setn(y1,x1,x+1);
                r = md->calculate();
                if (r==0){
                    for (y=0;y<9;y++)
                        for (x=0;x<9;x++) if (m[y][x]==0) m[y][x] = md->retn(y,x);
                    delete (md);
                    return 0;
                }
                delete (md);
            }
            t1 = t1 >> 1;
        }
        return 1;
    }
};



int main(){
    calcPower();
    uint8_t rez;
    //insert sudoku matrix
    uint8_t a[81] = {
                      0,1,0,5,2,3,9,0,4,
                      4,2,3,0,7,0,1,0,0,
                      7,0,9,4,0,6,0,3,0,
                      5,7,8,0,0,1,0,4,0,
                      0,0,0,9,4,0,5,2,7,
                      2,0,4,0,5,7,0,1,3,
                      0,0,7,1,9,2,4,0,0,
                      9,4,0,7,6,0,3,0,1,
                      0,6,5,0,0,0,7,9,2
                     };

    matrix* m = new matrix(&a[0]);
    cout<<"Source matrix"<<endl;
    m->print();
    cout<<endl;
    rez = m->calculate();

    cout<<"Result:"<<endl;
    if (rez==0)
        m->print();
    else
        cout<<"Error";


    delete(m);

    return 0;
}
