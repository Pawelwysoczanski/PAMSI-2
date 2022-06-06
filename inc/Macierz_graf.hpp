#pragma once

#include "Graf.hpp"
#include <cstdlib>
#include <memory>

class MacierzGraf : public Graf
{
    std::unique_ptr<std::unique_ptr<int[]>[]> Macierz;

public:
    void WypelnijGraf(const bool Petla) const override;
    void WypiszGraf() const override;
    const int CzytajPlik() override;
    void StworzWejscie(const int WezelStartowy) const override;
    const int &GetWaga(int t_Wiersz, int t_Kolumna) const
    {
        return Macierz[t_Wiersz][t_Kolumna];
    }
    explicit MacierzGraf(int t_Wierzcholek, double t_Cel);
    MacierzGraf() : Graf(){};
};
