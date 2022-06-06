#pragma once

#include <stdlib.h>
#include <iostream>
#include <fstream>

class Graf
{

protected:
    int Wierzcholek, Krawedz;
    double Gestosc;

public:
    const int &GetWierzcholek() const { return Wierzcholek; }
    const int &GetKrawedz() const { return Krawedz; }

    virtual void WypelnijGraf(const bool Petla) const = 0;
    virtual void WypiszGraf() const = 0;
    virtual const int CzytajPlik()  = 0;
    virtual void StworzWejscie(const int WezelStartowy) const = 0;
    virtual ~Graf(){};
    explicit Graf(int t_Wierzcholek, int t_Krawedz, double t_Gestosc) : Wierzcholek(t_Wierzcholek), Krawedz(t_Krawedz), Gestosc(t_Gestosc) {}
    Graf(){};
};