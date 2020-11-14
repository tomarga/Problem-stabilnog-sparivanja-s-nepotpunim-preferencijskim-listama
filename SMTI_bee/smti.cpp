#include "smti.h"

#include <iostream>
#include "smti.h"
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <iomanip>
#include <algorithm>
#include <list>
#include <iterator>
#include <set>
#include <math.h>

using namespace std;

int smti::randfunc(int donji, int gornji) {
    int broj = (rand() % (gornji - donji + 1)) + donji;
    return broj;
}

double smti::fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

bool smti::member(list < int > v, int x) {
    std::list < int > ::iterator it;
    for (it = v.begin(); it != v.end(); ++it)
        if (*it == x) return true;
    return false;
}

void smti::generator(vector < vector < int > > & mpref, vector < vector < int > > & wpref, int N, int m[][pop], int w[][pop], double p1, double p2, vector < vector < int > > & mr, vector < vector < int > > & wr) {
    double p;
    int i, j, w1, d;
    for (i = 1; i < N + 1; ++i) {

        for (d = 1; d < N + 1; ++d) {

            p = fRand(0, 1);
            if (p < p1) {
                w1 = m[i - 1][d - 1];
                m[i - 1][d - 1] = 0;
                for (j = 1; j < N + 1; ++j)
                    if (w[w1 - 1][j - 1] == i) {
                        w[w1 - 1][j - 1] = 0;
                        break;
                    }
            }
        }
    }

    vector < int > v;

    for (i = N - 1; i >= 0; --i) {

        v.clear();

        for (d = N - 1; d >= 0; --d)
            if (m[i][d] != 0) {
                v.insert(v.begin(), m[i][d]);
            }

        mpref.insert(mpref.begin(), v);
    }

    for (i = N - 1; i >= 0; --i) {

        v.clear();

        for (d = N - 1; d >= 0; --d)
            if (w[i][d] != 0) {
                v.insert(v.begin(), w[i][d]);
            }

        wpref.insert(wpref.begin(), v);
    }

    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < N; ++d)
            cout << m[i][d] << " ";
    }

    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < N; ++d)
            cout << w[i][d] << " ";
    }


    // rang prvog elementra je uvijek 1, ties provjeravamo za j>=1
    for (i = 0; i < N; ++i) {

        v.clear();
        v.push_back(1);
        for (d = 1; d < mpref[i].size(); ++d) {
            p = fRand(0, 1);
            //cout << p;
            if (p < p2) {
                v.push_back(v[d - 1]);
            } // ties s prethodnim
            else {
                v.push_back(v[d - 1] + 1);
            }
        }
        mr.push_back(v);

    }

    for (i = 0; i < N; ++i) {

        v.clear();
        v.insert(v.begin(), 1);
        for (d = 1; d < wpref[i].size(); ++d) {
            p = fRand(0, 1);
            //cout << p;
            if (p < p2) {
                v.push_back(v[d - 1]);
            } // ties s prethodnim
            else {
                v.push_back(v[d - 1] + 1);
            }

        }
        wr.push_back(v);
    }

    cout << endl << endl << "Muski ties: " << endl;
    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < mpref[i].size(); ++d)
            cout << mr[i][d] << " ";
    }

    cout << endl << endl << "Zenski ties: " << endl;
    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < wpref[i].size(); ++d)
            cout << wr[i][d] << " ";
    }
}


int smti::BP_error(int m1, int w1, int m, int N, vector < vector < int > > men, vector < vector < int > > women, vector < vector < int > > mr, vector < vector < int > > wr) {

    //nadjimo rang od m1 u preferencijskoj listi od w1
    int ind = w1 - 1;
    int rank_m1 = women[ind].size() + 1, rank_m = women[ind].size() + 1, j, k;
    for (j = 0; j < women[ind].size(); ++j)
        if (women[ind][j] == m1) {
            rank_m1 = wr[ind][j];
            break;
        }

    for (k = 0; k < women[ind].size(); ++k)
        if (women[ind][k] == m) {
            rank_m = wr[ind][k];
            break;
        }

    if (rank_m == women[ind].size() + 1)
        return 0; // m nije na preferencijskoj listi od w1; ne mogu ciniti blokirajuci par;

    if (rank_m1 == women[ind].size() + 1)
        return 1; // w1 je slobodna (m1 nije na pref. listi od w1) , BP

    else return max(0, rank_m1 - rank_m);
}


int smti::func_fit(int bPos[][pop], vector < vector < int > > men, vector < vector < int > > women, int N, int i, vector < vector < int > > & bp, list < int > & single_m, list < int > & single_z, vector < vector < int > > mr, vector < vector < int > > wr) {

    // imamo jednu pcelu, pa sada gledamo funkciju fit u tom stanju
    int slobodni = 0, blok_par = 0;
    bp.clear();
    vector < int > v;
    v.clear();
    single_m.clear();
    single_z.clear();

    int m, w, w1, rank_w, j, k, l, m1, error, min_error, rank_m1, rank_m;
    for (m = 0; m < N; ++m) {
        v.clear();
        error = 0;
        min_error = pow(2, 31) - 1; //  za svakog muskarca u i-tom stanju trazimo dominirajuci blokirajuci par
        rank_w = men[m].size() + 1;


        w = bPos[i][m]; // zena s kojom je m-ti muskarac u braku

        rank_m = women[w - 1].size() + 1; // je li na ref. listi od w m? ako nije, m je slobodan

        for (j = 0; j < men[m].size(); ++j) // u polju men na 0. mjestu je muskarac ciju preferencijsku listu u nastavnku navodimo.
            if (men[m][j] == w) {
                rank_w = mr[m][j];
                break;
            } // rank od w u pref. listi od m

        for (j = 0; j < women[w - 1].size(); ++j)
            if (women[w - 1][j] == m + 1) {
                rank_m = wr[w - 1][j];
                break;
            }

        if (rank_m == women[w - 1].size() + 1) {
            if (!member(single_m, m + 1)) {
                single_m.insert(single_m.begin(), m + 1);
                ++slobodni;
            }
            bp.insert(bp.begin() + m, v);
            continue;
        }

        if (rank_w == 1) {
            error = 0;
            bp.insert(bp.begin() + m, v);
        }

        if (rank_w == men[m].size() + 1) {
            if (!member(single_m, m + 1)) {
                single_m.insert(single_m.begin(), m + 1);
                ++slobodni;
            }
            bp.insert(bp.begin() + m, v);
            continue;

        } // m slobodan
        // po svim w1 za koje je rank_w1<rank_w
        k = 0;
        while (mr[m][k] < rank_w) {
            error = 0;
            // nadjemo m1 s kojim je w1 u braku
            // m1 je takav. ako je w1 slobodna onda je m1=N+1;
            w1 = men[m][k];
            // trazim je li w1 i s kim u braku (i-ta pcela)
            m1 = N + 1;

            rank_m1 = women[w1 - 1].size() + 1;

            for (l = 0; l < N; ++l)
                if (bPos[i][l] == w1) {
                    m1 = l + 1;
                    break;
                } // u i-tom sparivanju je w1 u braku s m1

            for (l = 0; l < women[w1 - 1].size(); ++l)
                if (women[w1 - 1][l] == m1) {
                    rank_m1 = l + 1;
                    break;
                }

            if (rank_m1 < women[w1 - 1].size() + 1) {
                error = 0;
                // provjeri cine li (m, w1) blokirajuci par
                error = BP_error(m1, w1, m, N, men, women, mr, wr);

                if (error > 0) {
                    ++blok_par;

                    if (error < min_error) {
                        min_error = error;
                        v.clear();
                        v.insert(v.begin(), m);
                        v.insert(v.begin(), w1);
                        v.insert(v.begin(), m1);
                    }
                }

                ++k;
                continue;
            } else if (rank_m1 == women[w1 - 1].size() + 1 || m1 == N + 1) {

                if (!member(single_z, w1)) single_z.insert(single_z.begin(), w1);
                ++k;
                continue;
            }
        }

        bp.insert(bp.begin() + m, v);
    }

    return blok_par * N + slobodni;

}


void smti::sortiraj(int fit[], int n, int bPos[][pop], vector < vector < int > > men, vector < vector < int > > women, int N, int ind[], int m) {

    // ide od najmanjeg prema najvecem, minimiziramo funkciju cilja, zelimo da je BP_error sto manja
    int pamti_ind[n], i, j;
    for (i = 0; i < n; ++i) {
        pamti_ind[i] = fit[i];
    }

    list < int > posjeceni;
    posjeceni.clear();

    sort(fit, fit + n);

    //implementiraj binarno trazenje, za sada neka stoji ovako, nije mi sada bitna slozenost
    for (i = 0; i < n; ++i)

        for (j = 0; j < n; ++j)
            if (fit[i] == pamti_ind[j]) {
                //if( member(posjeceni, j) ) continue;
                if (!member(posjeceni, j)) {
                    *(ind + i) = j;
                    posjeceni.insert(posjeceni.begin(), j);
                    break;
                }
            }

    return;
}


void smti::find_marriage(QString & A, QString & B) {

    A.clear();
    B.clear();
    int i, d, s;
    list < int > l;

    srand(time(NULL));

    int mp[N][pop] = {
        0
    }, wp[N][pop] = {
        0
    };

    vector < vector < int > > men, women;
    srand(time(NULL));

    cout << "Muska pref. lista na pocetku:" << endl;
    for (i = 0; i < N; ++i) {
        l.clear();
        cout << endl;

        for (d = 0; d < N; ++d) {
            s = randfunc(1, N);
            while (member(l, s)) {
                s = randfunc(1, N);

            }

            mp[i][d] = s;
            cout << mp[i][d] << " ";
            l.insert(l.begin(), s);
        }
    }

    cout << endl << endl << "Zenska pref. lista na pocetku:" << endl;

    for (i = 0; i < N; ++i) {
        l.clear();
        cout << endl;
        for (d = 0; d < N; ++d) {
            s = randfunc(1, N);
            while (member(l, s)) {
                s = randfunc(1, N);
            }
            wp[i][d] = s;
            l.insert(l.begin(), s);
            cout << wp[i][d] << " ";
        }
    }

    vector < vector < int > > mr, wr;
    generator(men, women, N, mp, wp, p1, p2, mr, wr);


    cout << endl << endl << "Nova muska lista:" << endl;
    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < men[i].size(); ++d)
            cout << men[i][d] << " ";
    }

    cout << endl << endl << "Nova zenska lista: " << endl;

    for (i = 0; i < N; ++i) {
        cout << endl;
        for (d = 0; d < women[i].size(); ++d)
            cout << women[i][d] << " ";
    }

    int j;
    int bPos[n][pop] = {
        0
    };
    int fit[n];

    srand((unsigned) time(NULL)); // Razlicit broj svaki put

    //test parametri
    int R = 1; // broj testova
    int imax = 100000; // broj iteracija
    int iter, brojac; // broj iteracija
    int najbolji[n][N] = {
        0
    };


    brojac = 0;
    int stani = 0, najmanje = pow(2, 31) - 1;

    // na pocetku generiram pocetno stanje

    for (i = 0; i < n; ++i) {
        l.clear();
        for (d = 0; d < N; ++d) {
            s = randfunc(1, N);
            while (member(l, s)) {
                s = randfunc(1, N);
            }
            bPos[i][d] = s;
            l.insert(l.begin(), s);
        }
    }

    for (iter = 0; iter < imax; ++iter) {

        ++brojac;

        vector < vector < int > > bp;
        list < int > single_m, single_z;

        for (i = 0; i < n; ++i) {

            bp.clear();
            single_m.clear();
            single_z.clear();
            fit[i] = func_fit(bPos, men, women, N, i, bp, single_m, single_z, mr, wr);
            if (fit[i] == 0) {
                A.clear();
                cout << "Pronasli smo stabilan brak, brojac= " << brojac << " .";
                for (d = 0; d < N; ++d) {
                    A = A + QString::number(bPos[i][d]) + " ";
                    cout << bPos[i][d] << " ";
                }
                cout << endl;
                B = "0";

            }
        }

        // sortiranje po fit funkciji (funkciji dobrote)

        // izaberi najboljih m

        int ind[m], bee[m];
        sortiraj(fit, n, bPos, men, women, N, ind, m);

        ++stani;

        if (fit[0] < najmanje) {
            najmanje = fit[0];
            stani = 0;
        }
        if (stani == 30) {
            cout << "Nasli smo maksimalan stabilan brak!";
            list < int > single_m, single_z;
            int f = func_fit(bPos, men, women, N, ind[0], bp, single_m, single_z, mr, wr);

            list < int > ::iterator it = single_m.begin();

            int Brojslobodih = 0;
            A.clear();
            for (d = 0; d < N; ++d) {
                if (!member(single_m, d + 1)) {
                    cout << bPos[ind[0]][d] << " ";
                    A = A + QString::number(bPos[ind[0]][d]) + " ";
                } else {
                    cout << "0 ";
                    A += "- ";
                    Brojslobodih++;
                }
            }
            B = QString::number(Brojslobodih);
            cout << endl;
            cout << "Slobodni m su:";
            for (list < int > ::iterator it = single_m.begin(); it != single_m.end(); ++it)
                cout << * it << " ";
            cout << endl;
            return;

        }

        int k;

        for (k = 0; k < m; ++k) {
            if (k < e) bee[k] = nep; // koliko saljemo na elitno podrucje
            else bee[k] = nsp; // na preostalih m podrucja
        }

        // Sada pretražujemo  susjedstva odabraih lokacija;
        int fit[m]; // samo m najboljih, gledam njihove funkcije.

        int novi[n][N];

        for (k = 0; k < m; ++k) {

            bp.clear();
            single_m.clear();
            single_z.clear();
            list < int > single1_m, single1_z;

            fit[k] = func_fit(bPos, men, women, N, ind[k], bp, single_m, single_z, mr, wr);

            if (fit[k] == 0) {
                A.clear();
                cout << endl << "Pronasli smo jedan stabilan brak, brojac= " << brojac << " ." << endl;
                for (int l = 0; l < N; ++l) {
                    cout << bPos[k][l] << " ";
                    A = A + QString::number(bPos[k][l]) + " ";
                }
                B = "0";
                return;

            }
            // u bp mi se sada nalaze svi blokirajuci parovi

            vector < vector < int > > bpn; // zaista svi blok. parovi, bez praznih vektora
            j = 0;

            // u bpn su sada svi blokirajuci parovi za k-to stanje
            //if(bpn.size()<bee[k]) bee[k]=bpn.size();
            int bb = bpn.size(); // broj blokirajucih parova;

            int fit[bee[k]];

            int okolina[bee[k]][pop];

            int ind_najboljeg = k;

            int min = fit[k];
            int promjena = 0;
            int t;

            for (j = 0; j < bee[k]; ++j) {
                t = 0;
                // pretrazujemo okolinu k-tog rjesenja
                // j-to stanje bit ce ono u kojem razrjesavamo problem j-tog blokirajuceg para
                // rand funkcijom biram koji blokirajuci par popravljam

                if (!bb && single_m.size() != 0) {

                    // postoje neki slobodni clanovi
                    // izaberi jednog random iz single_m, jednu zenu iz single_z i spari ih
                    bp.clear();
                    single1_m.clear();
                    single1_z.clear();

                    int br_m = randfunc(1, single_m.size());
                    int br_z = randfunc(1, N);

                    int m, w = br_z, it = 0;


                    std::list < int > ::iterator it_m;
                    for (it_m = single_m.begin(); it_m != single_m.end(); ++it_m) {
                        ++it;
                        m = * it_m;
                        if (it == br_m) break;
                    }

                    it = 0;

                    std::list < int > ::iterator it_z;

                    for (int l = 0; l < N; ++l)
                        okolina[j][l] = bPos[ind[k]][l];

                    // m pridruzim w
                    int w1 = okolina[j][m - 1];
                    int m1;

                    for (int l = 0; l < N; ++l)
                        if (bPos[ind[k]][l] == w) {
                            m1 = l + 1;
                            break;
                        }

                    okolina[j][m - 1] = w;
                    okolina[j][m1 - 1] = w1;

                    it_m = single_m.begin();
                    it_z = single_z.begin();
                    //it_z=single_z.begin();

                    bp.clear();
                    single1_m.clear();
                    single1_z.clear();

                    int f = func_fit(okolina, men, women, N, j, bp, single1_m, single1_z, mr, wr);

                    if (!f) {
                        A.clear();
                        cout << endl << "Pronasli smo stabilan brak, brojac= " << brojac << " ." << endl;
                        for (int l = 0; l < N; ++l) {
                            A = A + QString::number(okolina[j][l]) + " ";

                            cout << okolina[j][l] << " ";
                        }
                        B = "0";
                        return;
                    }
                    if (f && f < min) {
                        ind_najboljeg = j;
                        min = f;
                        promjena = 1;
                    }

                    // zelim ukloniti w i m iz liste slobodnih jer sam to moguce ''novo'' stanje vec pretrazila
                    for (it_m = single_m.begin(); it_m != single_m.end(); ++it_m)
                        if ( * it_m == m) {
                            break;
                        }

                    t = 1;
                }

                if (bb > 0) {
                    // random biranje blokirajucih! implementiraj
                    srand(time(NULL));
                    int br = randfunc(0, bb - 1);

                    // provjeri je li dobar poredak;
                    int m1_, w1_, m_;

                    m1_ = bpn[br][0];
                    w1_ = bpn[br][1];
                    m_ = bpn[br][2];

                    for (int l = 0; l < N; ++l)
                        okolina[j][l] = bPos[ind[k]][l];

                    okolina[j][m1_ - 1] = okolina[j][m_ - 1];
                    okolina[j][m_ - 1] = w1_;

                    bp.clear();
                    single1_m.clear();
                    single1_z.clear();

                    int f = func_fit(okolina, men, women, N, j, bp, single1_m, single1_z, mr, wr);

                    if (!f) {
                        A.clear();
                        cout << "Pronasli smo stabilan brak,  brojac= " << brojac << " ." << endl;
                        for (int l = 0; l < N; ++l) {
                            A = A + QString::number(okolina[j][l]) + " ";

                            cout << okolina[j][l] << " ";
                        }
                        B = "0";
                        return;

                    }
                    if (f && f <= min) {
                        ind_najboljeg = j;
                        min = f;
                        promjena = 1;
                    }

                    // uklanjam taj blokirajuci iz liste
                    bpn.erase(bpn.begin() + br);

                    --bb;
                }

                if (j == bee[k] - 1 && promjena == 1) {

                    for (int s = 0; s < N; ++s)
                        novi[k][s] = okolina[ind_najboljeg][s];

                    //fit_novi[j]=fit(bee[][pop], men[][pop], women[][pop], N, j, bp);
                }

                if (j == bee[k] - 1 && !promjena) {
                    // cout << "odredi koji je najbolji " << endl;
                    for (int s = 0; s < N; ++s)
                        novi[k][s] = bPos[ind[k]][s];

                    //fit_novi[j]=fit(bee[][pop], men[][pop], women[][pop], N, j, bp);
                }
            }
        }

        for (k = 0; k < m; ++k)
            for (int l = 0; l < N; ++l)
                bPos[k][l] = novi[k][l];

        for (k = m; k < n; ++k) {
            l.clear();
            for (d = 0; d < N; ++d) {
                s = randfunc(1, N);
                while (member(l, s))
                    s = randfunc(1, N);

                bPos[k][d] = s;
                l.insert(l.begin(), s);
            }
        }
    }
}
