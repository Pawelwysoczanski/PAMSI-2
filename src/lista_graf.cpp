#include "../inc/Lista_Graf.hpp"

static int rownowaga_interwalow = -1;

void ListaGraf::DodajKrawedz(int NumerKrawedzi, int t_numer, int t_waga, int t_flaga) const
{

	krawedz[t_flaga].cel = t_numer;
	krawedz[t_flaga].zrodlo = NumerKrawedzi;
	krawedz[t_flaga].waga = t_waga;
}

bool ListaGraf::WykrywacJesliIstnieje(int NumerKrawedzi, int t_NumerWezla) const
{

	for (int i = 0; i < Krawedz; ++i)
	{ // sprawdza cala liste krawedzi

		if (krawedz[i].cel == t_NumerWezla && krawedz[i].zrodlo == NumerKrawedzi)
			return true;
	}
	return false;
}

void ListaGraf::WypelnijGraf(const bool t_allowLoops) const
{

	if (Gestosc == 1)
	{

		int f_flaga = 0;
		for (int iZrodlo = 0; iZrodlo < Wierzcholek; ++iZrodlo)
		{
			for (int iCel = 0; iCel < Wierzcholek; ++iCel)
			{

				if (iZrodlo != iCel)
				{ // unikaj tworzenia petli

					int f_waga = rand() % 20 + rownowaga_interwalow;
					while (f_waga == 0)
					{ // wpisz wage inna niz 0

						f_waga = rand() % 20 + rownowaga_interwalow;
					}
					DodajKrawedz(iZrodlo, iCel, f_waga, f_flaga++);
				}
			}
		}
	}
	else
	{ // inne gęstości; całkiem taki sam algorytm jak w macierz_graf

		int f_krawedz = 0;
		while (f_krawedz < Krawedz)
		{

			int guardNode = rand() % Wierzcholek;
			int nodeNode = rand() % Wierzcholek;
			if (!WykrywacJesliIstnieje(guardNode, nodeNode))
			{

				int f_waga = rand() % 20 + rownowaga_interwalow;
				while (f_waga == 0)
				{

					f_waga = rand() % 20 + rownowaga_interwalow;
				}

				if (guardNode != nodeNode)
					DodajKrawedz(guardNode, nodeNode, f_waga, f_krawedz++);
				else if (t_allowLoops)
					DodajKrawedz(guardNode, nodeNode, f_waga, f_krawedz++);
			}
		}
	}
}

void ListaGraf::WypiszGraf() const
{

	std::cout << std::string(30, '-') << "\n  Reprezentacja Listy sasiedztwa\n\n";
	for (int iZrodlo = 0; iZrodlo < Wierzcholek; ++iZrodlo)
	{

		// numer zrodla
		std::cout << iZrodlo;
		for (int ikrawedz = 0; ikrawedz < Krawedz; ++ikrawedz)
		{

			// jeśli znaleziono faktycznie iterowane zrodlo w tablicy, wypisz to
			if (krawedz[ikrawedz].zrodlo == iZrodlo)
				std::cout << "->"
						  << "[" << krawedz[ikrawedz].cel << "|" << krawedz[ikrawedz].waga << "]";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

const int ListaGraf::CzytajPlik()
{

	std::ifstream plik("Wejscie.txt");
	if (!plik.is_open())
	{
		throw "WEJSCIE MACIERZY NIE OTWARTE!";
		return 1;
	}

	// atrybuty i inicjalizacja tablicy (poza start -> wezel startowy, który nie jest atrybutem)
	int start, zrodlo, cel, waga;
	plik >> Krawedz >> Wierzcholek >> start;
	krawedz = new s_Krawedz[Krawedz];

	// dodaj wszystkie poloczenia
	for (int ikrawedz = 0; ikrawedz < Krawedz; ++ikrawedz)
	{

		plik >> zrodlo >> cel >> waga;
		DodajKrawedz(zrodlo, cel, waga, ikrawedz);
	}
	plik.close();
	return start; // zwroc wezel startowy
}

// tak samo jak w macierzy
void ListaGraf::StworzWejscie(const int WezelStartowy) const
{

	std::ofstream plik("StworzWejscie.txt");
	if (!plik.is_open())
	{
		throw "UTWORZONE WEJSCIE MACIERZY NIE OTWARTY!";
		return;
	}

	plik << Krawedz << " " << Wierzcholek << " " << WezelStartowy << "\n";
	for (int ikrawedz = 0; ikrawedz < Krawedz; ++ikrawedz)
	{

		plik << krawedz[ikrawedz].zrodlo << " ";
		plik << krawedz[ikrawedz].cel << " ";
		plik << krawedz[ikrawedz].waga << "\n";
	}
	plik.close();
}
