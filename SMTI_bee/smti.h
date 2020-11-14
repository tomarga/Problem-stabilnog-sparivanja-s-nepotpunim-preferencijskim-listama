#ifndef SMTI_H
#define SMTI_H

#include <iostream>
#include "smti.h"
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>
#include <limits.h>
#include <iomanip>
#include <algorithm>
#include <list>
#include <iterator>
#include <set>
#include <math.h>
#include <QString>


#define pop 100
using namespace std;

class smti
{
    int N, n, m, e, nsp, nep;
    double p1, p2;

public:
    smti( int n_, int scouts, int m_best, int e_best, int nsp_num, int nep_num, double p, double r) { N=n_; n=scouts; m=m_best; e=e_best; nsp=nsp_num; nep=nep_num; p1=p; p2=r;};

    int randfunc( int donji, int gornji);

    double fRand( double fMin, double fMax);

    bool member(list<int> v, int x);

    void generator(vector<vector<int> > &mpref, vector<vector<int> > &wpref, int N, int m[][pop], int w[][pop], double p1, double p2,
                    vector<vector<int> > &mr, vector<vector<int> > &wr);

    int BP_error(int m1, int w1, int m, int N, vector<vector<int> > men, vector<vector<int> > women, vector<vector<int> > mr, vector<vector<int> > wr);

    int func_fit(int bPos[][pop], vector<vector<int> > men, vector<vector<int> > women, int N, int i, vector<vector<int> > &bp, list<int> &single_m, list<int> &single_z,
                 vector<vector<int> > mr, vector<vector<int> > wr);
    void sortiraj(int fit[], int n, int bPos[][pop], vector<vector<int> > men, vector<vector<int> > women, int N, int ind[], int m);

    //moze se staviti max vrijeme kao argument fje?

    void find_marriage ( QString &A, QString &B );

    //treba li destruktor?


    friend class MainWindow;

};

#endif // KLASA_H
