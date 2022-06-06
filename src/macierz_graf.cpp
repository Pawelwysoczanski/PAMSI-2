#include "../inc/Macierz_graf.hpp"

static int nieskonczonosc = 1000000; 

static int rownowaga_interwalow  = -1; 

//Konstruktor
MacierzGraf::MacierzGraf(int t_Wierzcholek, double t_gestosc) 
	: Graf(t_Wierzcholek, static_cast<int>(t_gestosc* t_Wierzcholek* (t_Wierzcholek - 1)), t_gestosc), 
	  Macierz(std::make_unique<std::unique_ptr<int[]>[]>(t_Wierzcholek)) {

	//inicjalizacja wszystkich tablic wewnątrz głównej tablicy Macierza
	for (int komorka = 0; komorka < Wierzcholek; ++komorka) {

		Macierz[komorka] = std::make_unique<int[]>(t_Wierzcholek); 
		for (int kolumna = 0; kolumna < Wierzcholek; ++kolumna) {
			
			if (komorka == kolumna) Macierz[komorka][kolumna] = 0; //przekątna zer
			else Macierz[komorka][kolumna] = nieskonczonosc; //pozostałe komórki wypełnij inf
		}
	}
}


void MacierzGraf::WypelnijGraf(const bool mozliwoscpowtorzen) const {

	if (Gestosc == 1) { 
		for (int wiersz = 0; wiersz < Wierzcholek; ++wiersz) { 
			for (int kolumna = 0; kolumna < Wierzcholek; ++kolumna) {  
				if (wiersz != kolumna) 
				{
					int f_waga = rand() % 20  + rownowaga_interwalow ;
					while (f_waga == 0) { 

						f_waga = rand() % 20 + rownowaga_interwalow ;
					}
					Macierz[wiersz][kolumna] = f_waga;
				}
			}
		}
	}
	else {

		int f_krawedz = Krawedz;

		while (f_krawedz) { //tak długo, jak stworzy odpowiednią liczbę krawędzi

			int WierszWezla = rand() % Wierzcholek; //losowo wybierz liczbę węzłów, pomiędzy którymi zostanie utworzona krawędź
			int KolumnaWezla = rand() % Wierzcholek;

			//jeśli krawędź jeszcze nie istnieje, co oznacza, że ​​nadal jest ustawiona na 0 lub nieskończoność
			if (Macierz[WierszWezla][KolumnaWezla] == 0|| Macierz[WierszWezla][KolumnaWezla]== nieskonczonosc) { 

				int f_waga = rand() % 20 + rownowaga_interwalow ;
				while (f_waga == 0) { //losuj losowo inny niż 0

					f_waga = rand() % 20 + rownowaga_interwalow ;
				}

				//po (jeśli) wykrywa, czy program chce utworzyć krawędź pętli do tego samego węzła
                //jeśli użytkownik określił mozliwoscpowtorzen = true, zostanie utworzony, jeśli false,
                //iteracja zostanie pominięta i nie stworzy żadnej krawędzi
				if (WierszWezla != KolumnaWezla) {

					Macierz[WierszWezla][KolumnaWezla] = f_waga;
					--f_krawedz;
				}
				else if (mozliwoscpowtorzen) {

					Macierz[WierszWezla][KolumnaWezla] = f_waga;
					--f_krawedz;
				}
			}
		}
	}
}


//metoda zmienia swoje zachowanie w zależności od długości cyfr; jest również w stanie wykryć symbol minus (-)
void MacierzGraf::WypiszGraf() const {

	std::cout << std::string(33, '-') << "\n  Reprezentacj Macierzy sasiedztwa\n\n ";

	//Wypisuje numery krawedzi na górze 
	for (int i = 0; i < Wierzcholek; ++i) {
		
		if (i <= 10) std::cout << "    " << i;
		else if (i <= 100) std::cout << "   " << i;
		else std::cout << "  " << i;
	}
	std::cout << "\n\n";

	//uzupełnij pozostałe wiersze
	for (int wiersz = 0; wiersz < Wierzcholek; ++wiersz) {	
		
		//pierwsza sciana macierzy
		if (wiersz < 10) std::cout << wiersz << "   |";
		else if (wiersz < 100) std::cout << wiersz << "  |";
		else std::cout << wiersz << " |";
		
		//wszystkie połączenia w obecnym wierszu
		for (int kolumna = 0; kolumna < Wierzcholek; ++kolumna) {

			int foo = Macierz[wiersz][kolumna];

			if (foo == nieskonczonosc) std::cout << "*"; // jeśli węzeł jest ustawiony na "nieskończoność", to wypisz gwiazdkę
			else std::cout << foo;					     // jako symbol braku połączenia

			if (abs(foo) < 10 || abs(foo) == nieskonczonosc) std::cout << "    "; //1000000 liczy się jako gwiazda, więc spacja jednocyfrowa
			else if (abs(foo) < 100) std::cout << "   ";
			else std::cout << "  ";

			if (foo < 0) std::cout << '\b'; //ponieważ był znak minus powrotny wózek
		}
		//druga sciana
		std::cout << "|\n";
	}
	std::cout << std::endl;
}


//Metoda inicjalizująca Graf
const int MacierzGraf::CzytajPlik() {

	std::ifstream plik("Wejscie.txt"); 
	if (!plik.is_open()) {
		throw "WEJSCIE MACIERZY NIE OTWARTE!";
		return 1;
	}

	//atrybuty i inicjalizacja tablicy Macierz; start -> węzeł początkowy (nie atrybut)
	int start, zrodlo, cel, waga;
	plik >> Krawedz >> Wierzcholek >> start;
	Macierz = std::make_unique<std::unique_ptr<int[]>[]>(Wierzcholek);

	//inicjalizacja zagnieżdżonych tablic (tak samo jak w konstruktorze)
	for (int iWiersz = 0; iWiersz < Wierzcholek; ++iWiersz) {

		Macierz[iWiersz] = std::move(std::make_unique<int[]>(Wierzcholek));
		for (int kolumna = 0; kolumna < Wierzcholek; ++kolumna) {

			if (iWiersz == kolumna) Macierz[iWiersz][kolumna] = 0;
			else Macierz[iWiersz][kolumna] = nieskonczonosc;
		}
	}

	//wypełnianie tablic
	for (int iKrawedz = 0; iKrawedz < Krawedz; ++iKrawedz) {

		plik >> zrodlo >> cel >> waga;
		Macierz[zrodlo][cel] = waga;;
	}
	plik.close();
	return start; //zwraca węzeł początkowy (informacje wymagane przez algorytm bellmanForda)
}


//zapisuje informacje w następującej kolejności:
//
// ilość krawędzi | ilość wierzchołków | węzeł początkowy
//
// #krawędzie podążające#
//
// numer węzła źródłowego | numer węzła docelowego | waga krawędzi, aby dostać się do tego węzła ze źródła
//...
void MacierzGraf::StworzWejscie(const int WezelStartowy) const {
	
	std::ofstream plik("StwoezWejscie.txt");
	if (!plik.is_open()) {
		throw  "UTWORZONE WEJSCIE MACIERZY NIE OTWARTE!";
		return;
	}

	plik << Krawedz << " " << Wierzcholek << " " << WezelStartowy << "\n";
	for (int iWiersz = 0; iWiersz < Wierzcholek; ++iWiersz) {
		for (int kolumna = 0; kolumna < Wierzcholek; ++kolumna) {

			if (Macierz[iWiersz][kolumna] != nieskonczonosc && Macierz[iWiersz][kolumna] != 0) {

				plik << iWiersz << " ";
				plik << kolumna << " ";
				plik << Macierz[iWiersz][kolumna] << "\n";
			}
		}
	}
	plik.close();
}
