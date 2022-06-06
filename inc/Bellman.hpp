#pragma once

#include <chrono>
#include <string>
#include "Lista_Graf.hpp"
#include "Macierz_graf.hpp"

double Bellman(std::shared_ptr<ListaGraf>t_graf,int t_startNode,bool t_WypiszRozwiazanie);//Algorytm dla Listy sasiedztwa

double Bellman(std::shared_ptr<MacierzGraf>t_graf,int t_startNode,bool t_WypiszRozwiazanie);//Algorytm dla macierzy sasiedztwa