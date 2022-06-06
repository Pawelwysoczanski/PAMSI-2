#include "../inc/Bellman.hpp"

static int nieskonczonosc = 1000000;      
static int neg_nieskonczonosc = -1000000; 
                                          

// Funkcja wyswietlajaca w terminalu rozwiazanie utworzone przez algorytm oraz tworzaca plik(WyjscieBellman.txt) w ktorym mozna znlezc rozwiazanie
void Rozwiazanie(std::string sciezka[], int dystans[], int t_n, int WezelStartowy)
{

    std::ofstream plik("WyjscieBellman.txt");

    std::cout << std::string(20, '-') << "\n   Rozwiazanie\n\n";
    std::cout << " Wewzel poczatkowy -> " << WezelStartowy << "\n\n";

    plik << std::string(20, '-') << "\n   Rozwiazanie\n\n";
    plik << "Wewzel poczatkowy -> " << WezelStartowy << "\n\n";

    for (int i = 0; i < t_n; ++i)
    {

        if (dystans[i] == neg_nieskonczonosc)
        {
            std::cout << i << "->"
                      << "-inf\n";
            plik << i << "->"
                 << "-inf\n";
            continue; // kontynuuj, aby program nie wypisywał ścieżki do tego węzła (spójrz na koniec pętli, aby wyświetlić rzeczywiste drukowanie)
        }
        else if (dystans[i] == nieskonczonosc)
        {
            std::cout << i << "->"
                      << "inf\n";
            plik << i << "->"
                 << "inf\n";
            continue; // tak jak wyżej
        }
        // Jesli odelglosc nie jest inf wtedy wypisuje najktrotsza sciezke
        else
        {
            std::cout << i << "->" << dystans[i];
            plik << i << "->" << dystans[i];
        }

        // następujące dwa bloki (ifs) drukowania najkrótszej ścieżki zapisanej w std::string obiekt

        // blok pierwszy: dotyczący numeru węzła zrodłowego
        if (i < 10)
        {
            std::cout << "   ";
            plik << "   ";
        }
        else if (i < 100)
        {
            std::cout << "  ";
            plik << "  ";
        }
        else
        {
            std::cout << " ";
            plik << " ";
        }

        // blok drugi: dotyczący najkrótszej odległości
        if ((dystans[i] >= 100 && dystans[i] < 1000) || (-100 > dystans[i] && dystans[i] <= -10))
        {

            std::cout << " Najkrotsza droga: " << sciezka[i] << i;
            plik << " Najkrotsza droga: " << sciezka[i] << i;
        }
        else if (0 <= dystans[i] && dystans[i] < 10)
        {

            std::cout << "   Najkrotsza droga: " << sciezka[i] << i;
            plik << "   Najkrotsza droga: " << sciezka[i] << i;
        }
        else if ((dystans[i] >= 10 && dystans[i] < 100) || (-10 < dystans[i] && dystans[i] < 0))
        {

            std::cout << "  Najkrotsza droga: " << sciezka[i] << i;
            plik << "  Najkrotsza droga: " << sciezka[i] << i;
        }
        else
        { // w razie niebezpieczeństwa dodaj kolejny przedział: (-1000,-100] || [1000,10000)

            std::cout << "Najkrotsza droga: " << sciezka[i] << i;
            plik << "Najkrotsza droga: " << sciezka[i] << i;
        }
        std::cout << std::endl;
        plik << std::endl;
    }
    std::cout << std::endl;
    plik.close();
}

// Algorytm dla Listy sasiedztwa
double Bellman(std::shared_ptr<ListaGraf> graf, int WezelStartowy, bool WklejRozwiazanie)
{

    std::string *PrzechowajSciezke = new std::string[graf->GetWierzcholek()]; // miejsce do przechowywania ciągów ścieżek

    auto t_start = std::chrono::high_resolution_clock::now(); // start zegara

    int *PrzechowajOdleglosc = new int[graf->GetWierzcholek()]; // miejsce do przechowywania najkrotszej trasy

    for (int iCell = 0; iCell < graf->GetWierzcholek(); ++iCell)
    {

        PrzechowajOdleglosc[iCell] = nieskonczonosc; // inicjuj wszystkie odleglosci jako inf
    }

    PrzechowajOdleglosc[WezelStartowy] = 0; // inicjuj zrodlo jako 0

    for (int i = 1; i < graf->GetWierzcholek(); ++i)
    {
        for (int j = 0; j < graf->GetKrawedz(); ++j)
        {

            int u = graf->GetStruktura()[j].zrodlo;
            int v = graf->GetStruktura()[j].cel;
            int waga = graf->GetStruktura()[j].waga;

            if (PrzechowajOdleglosc[u] + waga < PrzechowajOdleglosc[v])
            { // Sprawdz czy znaleziono najkrotsza trase
                PrzechowajOdleglosc[v] = PrzechowajOdleglosc[u] + waga;

                if (WklejRozwiazanie)
                { // Jesli tak wklej rozwiazanie

                    PrzechowajSciezke[v].clear();
                    PrzechowajSciezke[v].append(PrzechowajSciezke[u] + std::to_string(u) + "->");
                }
            }
        }
    }

    // sprawdzam teraz pod kątem cykli ujemnych
    for (int i = 1; i < graf->GetWierzcholek(); ++i)
    {
        for (int j = 0; j < graf->GetKrawedz(); ++j)
        {

            int u = graf->GetStruktura()[j].zrodlo;
            int v = graf->GetStruktura()[j].cel;
            int waga = graf->GetStruktura()[j].waga;
            if (PrzechowajOdleglosc[u] + waga < PrzechowajOdleglosc[v])
            {

                if (PrzechowajOdleglosc[u] > nieskonczonosc / 2)
                    PrzechowajOdleglosc[u] = nieskonczonosc; // ochrona przed dodaniem ujemnych inf-
                else
                PrzechowajOdleglosc[v] = neg_nieskonczonosc; //-jeśli węzeł nie jest w żaden sposób połączony
            }
            else if (PrzechowajOdleglosc[u] > nieskonczonosc / 2)
                PrzechowajOdleglosc[u] = nieskonczonosc;
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now(); // zatrzymaj zegar

    // wypisz rozwiąanie jeśli użytkownik chce
    if (WklejRozwiazanie)
        Rozwiazanie(std::move(PrzechowajSciezke), std::move(PrzechowajOdleglosc), graf->GetWierzcholek(), WezelStartowy);
    delete[] PrzechowajOdleglosc;
    delete[] PrzechowajSciezke;
    return std::chrono::duration<double, std::milli>(t_end - t_start).count(); // zwróć różnicę czasu
}

// Algorytm dla Mcierzy sasiedztwa(ten sam algorytm co dla listy)
double Bellman(std::shared_ptr<MacierzGraf> graf, int WezelStartowy, bool WklejRozwiazanie)
{

    std::string *PrzechowajSciezke = new std::string[graf->GetWierzcholek()];

    auto t_start = std::chrono::high_resolution_clock::now();

    int *PrzechowajOdleglosc = new int[graf->GetWierzcholek()];

    for (int iCell = 0; iCell < graf->GetWierzcholek(); ++iCell)
    {

        PrzechowajOdleglosc[iCell] = nieskonczonosc;
    }

    PrzechowajOdleglosc[WezelStartowy] = 0;

    for (int i = 1; i < graf->GetWierzcholek(); ++i)
    {
        for (int j = 0; j < graf->GetWierzcholek(); ++j)
        {
            for (int w = 0; w < graf->GetWierzcholek(); ++w)
            {

                int u = j;
                int v = w;
                int waga = graf->GetWaga(j, w);
                if (PrzechowajOdleglosc[u] + waga < PrzechowajOdleglosc[v])
                {

                    PrzechowajOdleglosc[v] = PrzechowajOdleglosc[u] + waga;
                    if (WklejRozwiazanie)
                    {

                        PrzechowajSciezke[v].clear();
                        PrzechowajSciezke[v].append(PrzechowajSciezke[u] + std::to_string(u) + "->");
                    }
                }
            }
        }
    }
    for (int i = 1; i < graf->GetWierzcholek(); ++i)
    {
        for (int j = 0; j < graf->GetWierzcholek(); ++j)
        {
            for (int w = 0; w < graf->GetWierzcholek(); ++w)
            {

                int u = j;
                int v = w;
                int waga = graf->GetWaga(j, w);
                if (PrzechowajOdleglosc[u] + waga < PrzechowajOdleglosc[v])
                {

                    if (PrzechowajOdleglosc[u] > nieskonczonosc / 2)
                        PrzechowajOdleglosc[u] = nieskonczonosc;
                    else if (waga == nieskonczonosc)
                        continue; // jeśli ten węzeł(j) nie ma połączenia z węzłem(w)
                    else
                        PrzechowajOdleglosc[v] = neg_nieskonczonosc;
                }
                else if (PrzechowajOdleglosc[u] > nieskonczonosc / 2)
                    PrzechowajOdleglosc[u] = nieskonczonosc;
            }
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    if (WklejRozwiazanie)
        Rozwiazanie(std::move(PrzechowajSciezke), std::move(PrzechowajOdleglosc), graf->GetWierzcholek(), WezelStartowy);
    delete[] PrzechowajOdleglosc;
    delete[] PrzechowajSciezke;
    return std::chrono::duration<double, std::milli>(t_end - t_start).count();
}
