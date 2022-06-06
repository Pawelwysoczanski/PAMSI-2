#pragma once

#include "Graf.hpp"

struct s_Krawedz
{
    int zrodlo, cel, waga;
};

class ListaGraf : public Graf
{
    s_Krawedz* krawedz;

public:
    void WypelnijGraf(const bool Petla) const override;
    void WypiszGraf() const override;
    const int CzytajPlik() override;
    void StworzWejscie(const int t_startNode) const override;

    const s_Krawedz *GetStruktura() const { return krawedz; }
    void DodajKrawedz(int t_NumerKrawedzi, int t_liczba, int t_waga, int flaga) const;
    bool WykrywacJesliIstnieje(int t_guradNumber, int t_nodeNumber) const;

    explicit ListaGraf(int t_Wierzcholek, double t_Cel) : Graf(t_Wierzcholek, static_cast<int>(t_Cel * t_Wierzcholek * (t_Wierzcholek - 1)), t_Cel), krawedz(new s_Krawedz[static_cast<int>(t_Cel * t_Wierzcholek * (t_Wierzcholek - 1))]) {}
    ListaGraf() : Graf(){};
    ~ListaGraf() { delete krawedz; }
};