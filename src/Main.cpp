#include <time.h>

#include "../inc/Bellman.hpp"

// funkcja przeprowadzająca test
// ma szablon z reprezentacją graf. Przekazywane argumenty:
//@powtorz - ile razy algorytm rozwiąże obiekty o tych samych parametrach (im więcej tym dokładniejszy wynik)
//@TablicWierz,@TablicaGesto - parametry obiektów, które zostaną utworzone
template <typename T>
std::ostream &tests(std::ostream &plik, int (&TablicaWierz)[5], double (&TablicaGesto)[4], int powtorz)
{

	for (double gestosc : TablicaGesto)
	{
		for (int wierzcholek : TablicaWierz)
		{
			double suma = 0;
			for (int ipowtorz = 0; ipowtorz < powtorz; ++ipowtorz)
			{ // tyle razy ile ustawiono zmienna powtorz

				std::shared_ptr<T> graf = std::make_shared<T>(wierzcholek, gestosc); // tworzenie odpowiedniego obiektu
				int WezwlStarowy = rand() % wierzcholek;							 // generowanie losowe wezla startowego

				graf->WypelnijGraf(true);

				suma += Bellman(std::move(graf), WezwlStarowy, false); // zwrot czasu realizacji
				std::cout << (ipowtorz * 100 / powtorz) + 1 << "%"
						  << "\r" << std::flush;
			}
			plik << suma / powtorz << " "; // srednia czasow
			std::cout << wierzcholek << " Wierzcholek Gotowy!" << std::endl;
		}
		std::cout << "\n"
				  << gestosc << " Gestosc Gotowa!\n"
				  << std::endl;
	}
	plik << "\n";
	return plik;
}

int main()
{

	srand(static_cast<unsigned int>(time(NULL)));

	bool testZone = true;

	// TEST //

	if (testZone)
	{

		/////////////////////////
		// Zmienne do modyfikacji//
		int TablicaWierz[5] = {5, 15, 30, 50, 100};	   // ilość wierzchołków do przetestowania
		double TablicaGesto[4] = {0.25, 0.5, 0.75, 1}; // badane gęstości
		int powtorz = 100;							   // ile razy algorytm rozwiąże obiekty o tych samych parametrach
		/////////////////////////

		std::ofstream plik("WyjscieCzas.txt");
		if (!plik.is_open())
		{

			std::cerr << "WYJSCIE CZASU NIE OTWARTE!" << std::flush;
			return 1;
		}

		tests<ListaGraf>(plik, TablicaWierz, TablicaGesto, powtorz); // test dla listy
		std::cout << "LISTA GOTOWA \n"
				  << std::endl;
		tests<MacierzGraf>(plik, TablicaWierz, TablicaGesto, powtorz); // test dla macierzy
		std::cout << "MACIERZ GOTOWA" << std::endl;

		plik.close();
		return 0;
	}

	/////////////////////////
	// Zmienne do modyfikacji//
	// typedef ListaGraf GRAF; // Rezprezentacja grafu
	typedef MacierzGraf GRAF;
	bool uzyjplik = true;			// wejscie do pliku(Wejscie.txt)
	bool mozliwoscpowtorzen = true; // ponowne połączenie między węzłami
	int wierzcholek = 7;			// ilosc wierzcholkow
	double gestosc = 1;				// Gestosc grafu
	int WezwlStarowy = 4;			// wezel startowy
	/////////////////////////

	std::shared_ptr<GRAF> graf;

	if (uzyjplik)
	{ // jeśli dane wejściowe z pliku
		std::shared_ptr<GRAF> tmp = std::make_shared<GRAF>();
		graf = tmp;
		try
		{
			WezwlStarowy = tmp->CzytajPlik(); // jeśli wprowadzone poprawnie, nie będzie wyjątku
		}
		catch (const char *msg)
		{
			std::cerr << msg << std::flush;
			return 1;
		}
	}
	else
	{ // jeśli losowy graf
		std::shared_ptr<GRAF> tmp = std::make_shared<GRAF>(wierzcholek, gestosc);
		graf = tmp;
		tmp->WypelnijGraf(mozliwoscpowtorzen); // losowe wypełnienie
	}

	graf->WypiszGraf();
	try
	{
		if (!uzyjplik)
			graf->StworzWejscie(WezwlStarowy); // jeśli utworzono pozytywnie
	}
	catch (const char *msg)
	{
		std::cerr << msg << std::flush;
		return 1;
	}
	Bellman(std::move(graf), WezwlStarowy, true);

	return 0;
}