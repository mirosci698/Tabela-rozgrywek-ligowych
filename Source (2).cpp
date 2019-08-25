#include <iostream>		//Pliki naglowkowe (do czego potrzebne) - obsluga wejscia/wyjscia.
#include <string>		//Obsluga stringow.
#include <fstream>		//Obsluga plikow.
#include <sstream>		//Obsluga strumieni istringstream i ofstringstream, parsowanie informacji z plikow.
#include <windows.h>	//Funkcje systemu operacyjnego - system("cls") i Sleep(int x).
#include <conio.h>		//Funkcja _getch() do pobrania 1 znaku klawiatury.
using namespace std;
const int wygrana = 3;		// Stale okreslajace ilosc punktow
const int remis = 1;		// za dany wynik, za przegrana nigdy nie przyznaje sie punktow.
const int dlugosckontrolowanastringu = 1000;	// Obszar czyszczenia bufora w cin.ignore(int x, '\n').
const int przesuniecieASCII = 32;	//Stala potrzebna do zabezpieczen na duze/male litery (przerwa miedzy dana litera duza i mala w kodowaniu ASCII).
const int poczateknumeracji = 1;	//Poczatkowy identyfikator druzyny, bedzie inkrementowany o 1.
const int poczatkowakolejka = 1;	//Stala uzywana potrzebna jako numer pierwszej kolejki, ktory bedzie inkrementowany o 1. 
const int okresoczekiwania = 2000;	//Stala uzywana w Sleep(int x) - opoznienie programu w danych miejscach o dana liczbe milisekund.
const int przedkolejkami = poczatkowakolejka - 1;		//Stala potrzebna jako informacja jako stan przed dodaniem jakichkolwiek kolejek.
const int poczatekzakresuznakow = 65;	//Stala pokazujaca poczatek zakresu znakow przy wczytywaniu nazwy (duze litery).
const int konieczakresuznakow = 90;	//Stala pokazujaca koniec zakresu znakow przy wczytywaniu nazwy (duze litery).
const int separatornazwy = 95;	//Stala pokazujaca separator uzywany w nazwie.
const int poczatekstringa = 0;	//Stala dajaca indeks pierwszego elementu stringa.
const int poprzedni = 1;		//Stala dajaca mozliwosc przesuniecia na wczesniejsza pozycje.
const int poczatkowepunkty = 0;			//Stale opisujace
const int poczatkowebramkizdobyte = 0;	//stan poczatkowy 
const int poczatkowebramkistracone = 0;	//tabeli
const int poczatkowailoscmeczow = 0;	//(stan przedsezonowy).
const int zliczaniemeczow = 0;			//Stala potrzebna jako wyzerowanie przy zliczniu spotkan.
const int poczatkowapozycja = 0;		//Stala potrzebna do wyswietlania tabeli.
const int napol = 2;					//Stala uzywana przy dzieleniu na pol.
const int poczatekwyboru = 49;			//Poczatek zakresu wyboru w funkcjach codalej, dzialanieprogramu, rodzajtabeli.
const int koniecwyboru_dzialanie = 51;	//Koniec zakresu wyboru dla funkcji dzialanieprogramu.
const int koniecwyboru = 53;			//Koniec zakresu wyboru dla funkcji codalej i rodzajtabeli.
const bool dodawanie = true;		// Stale rozrozniajace
const bool wczytywanie = false;		//	wersje funkcji
const bool ustalanierundy = false;	// komunikatwyboru.
const bool usiebie = true;			// Stale rozrozniajace
const bool nawyjezdzie = false;		//	typ
const bool jesieñ = false;			//	wyswietlanej
const bool wiosna = true;			//	tabeli.
const bool przedrozgrywkami = false;	// Stala pozwalajaca ustawic pierwsza kolejke jako jesienna.
const string komunikat1 = "Podaj identyfikator gospodarza: ";	// Komunikaty
const string komunikat2 = "Podaj identyfikator goscia: ";		//	wykorzystywane
const string komunikat3 = "Podaj wynik gospodarza: ";			//	przy
const string komunikat4 = "Podaj wynik goscia: ";				//	pobieraniu.
const string nazwapliku = "plik.txt";		//Nazwa pliku przechowujacego wyniki.
const string informacje = "informacje.txt";	//Nazwa pliku przechowujacego informacje (numer kolejki, runde, liczbe druzyn).
const string plikzdruzynami = "druzyny.txt";//Nazwa pliku przechowujacego stan przedsezonowy potrzebny do budowy tabeli.
const char separatorplikowy = ' ';	//Znak uzywany jako separator przy zapisie do pliku.
struct spotkanie {		//Struktura ktora opisuje 1 mecz.
	int gospodarz;		//Identyfikator gospodarza.
	int gosc;			//Identyfikator goscia.
	int pktgospodarz;	//Bramki zdobyte w meczu przez gospodarza.
	int pktgosc;		//Bramki zdobyte w meczu przez goscia.
	spotkanie* nast; // Wskaznik na nastepny element - lista jednokierunkowa.
};
struct kolejka {	// Struktura opisujaca kolejke.
	int nr;			// Numer kolejki.
	bool runda;		// Runda kolejki.
	kolejka* nast;	//Wskaznik na nastepny - lista jednokierunkowa.
	spotkanie* listaspotkan; //Wskaznik na podwieszana liste spotkan.
};
struct poletabeli {	//Struktura opisujaca pole tabeli.
	string nazwa;	//Nazwa druzyny.
	int identyfikator;	//Identyfikator druzyny.
	int punkty;			//Liczba punktow zdobytych przez druzyne.
	int bramkizdobyte;	//Bramki zdobyte przez druzyne.
	int bramkistracone;	//Bramki stracone przez druzyne.
	int iloscmeczow;	//Ilosc meczow rozegrana przez druzyne.
	poletabeli* nast;	//Wskaznik na nastepne pole - lista jednokierunkowa.
};
void komunikatpoczatkowy()	//Funkcja interfejsowa wyswietlajaca opis dzialania programu, nie przyjmuje ani nie zwraca zadnych wartosci.
{
	cout << "Witaj w tabeli rozgrywek ligowych." << endl; //Opis programu wyswietlany na ekranie.
	cout << "Na poczatku wczytujesz sezon, rozpoczynasz nowy lub od razu konczysz program." << endl;
	cout << "Rozpoczecie nowego sezonu oznacza porzucenie ewentualnie zapisanego." << endl;
	cout << "(Nie mozna jednoczesnie rozgrywac sezonu poprzedniego i nastepnego.)" << endl;
	cout << "Przy tworzeniu sezonu na poczatku wpisujesz wszystkie druzyny do tabeli." << endl;
	cout << "Druzyn w trakcie sezonu nie mozna dopisywac." << endl;
	cout << "Przypisane przez program identyfikatory posluza do wpisywania spotkan." << endl;
	cout << "Ten sam zespol nie moze rozegrac w jednej kolejce 2 spotkan." << endl;
	cout << "Pozniej bedziesz mogl dopisywac kolejki, spotkania do konkretnych kolejek lub" << endl;
	cout << "usuwac wczesniej wprowadzone mecze albo wyswietlac tabele." << endl;
	cout << "Maksymalna ilosc meczow w kolejce to polowa ilosci druzyn zaokraglona w dol." << endl;
	cout << "Tabele sortuje sie z priorytetem: punkty, bramki zdobyte, bramki stracone." << endl;
	cout << "Wcisnij dowolny przycisk. ";
	_getch();	//Pobieranie 1 znaku - VS wymaga "_" na poczatku.
	system("cls");	//Odswiezanie ekranu.
}
void dobranazwa(bool &zly, string &nazwa) //Funkcja interfejsowa sprawdzajaca czy nazwa spelnia dane warunki.
{												//Przyjmuje informacje o powtorzeniu (przez referencje - uzupelni ja) i string do wczytu nazwy,
	do {										//nie zwraca nic (void).
		zly = false;		//Czyszczenie informacji o powtorzeniu.
		cout << "Podaj nazwe druzyny" << endl;	//Informacje dla uzytkownika.
		cout << "(ciagi DUZYCH liter rozdzielonych JEDNYM znakiem podkreslenia," << endl;
		cout << "musi zaczynac sie i konczyc litera): ";
		cin >> nazwa;					//Pierwszy wczyt.
		for (int i = poczatekstringa; i < nazwa.size(); i++)	// Zmienna i jest iteratorem potrzebnym tylko w petli, jest krotka bo uzywana
		{														// jako indeks stringa.
			if (((i == poczatekstringa) && (nazwa[i] > konieczakresuznakow || nazwa[i] < poczatekzakresuznakow)) || ((i > poczatekstringa) && (nazwa[i] == separatornazwy) && (nazwa[i - poprzedni] == separatornazwy)) || ((nazwa[i] != separatornazwy) && (nazwa[i] > konieczakresuznakow || nazwa[i] < poczatekzakresuznakow)) || ((i == nazwa.size() - poprzedni) && (nazwa[i] == separatornazwy)))
			{							//Sprawdzamy czy string spelnia warunki (dlatego if musi byc tak dlugi) na calej swej dlugosci (petla for).
				zly = true;		//Jezeli nie spelnil (na poczatku/koncu separator, kilka separatorow obok siebie, niepozadane znaki)
				system("cls");			//Odnotowanie w zmiennej zly, odswiezenie ekranu.
				cin.ignore(dlugosckontrolowanastringu, '\n');	//Wyczyszczenie bufora
				cout << "Zla nazwa. Podaj inna nazwe.";	//Informacja dla uzytkownika.
				Sleep(okresoczekiwania);	//Opoznienie programu o staly czas.
				system("cls");	//Ponowne odswiezenie.
				break;	//Przerwanie petli.
			}
		}
	} while (zly); //Powtarzanie dopoki wyraz jest zly.
}
void sprawdznazwe(string &nazwa, poletabeli* glowatabela)	//Funkcja interfejsowa sprawdzajaca czy nazwa druzyny sie powtorzyla i jest odpowiednia. 
{															//Otrzymuje do zmiany (przez referencje) stringa z nazwa i wskaznik
	bool powtorzony;										//na poczatek tabeli (bez referencji - po wyjsciu taki sam).
	do {
		dobranazwa(powtorzony, nazwa);		//Sprawdzanie czy sama nazwa spelnia zalozenia.
		poletabeli* licznik = glowatabela;	//Nie mozemy stracic glowy tabeli.
		while (licznik)						//Szukanie do konca lub dopoki nie znajdzie.
		{
			if (nazwa.length() == licznik->nazwa.length()) //Podejrzewamy powtorzenie nazwy gdy maja ta sama dlugosc.
			{
				powtorzony = true;
				for (int i = poczatekstringa; i < nazwa.length(); i++) //Iterator potrzebny jako indeks - krotka nazwa.
				{
					if (nazwa[i] != licznik->nazwa[i]) //Wystarczy ze jeden znak nie bedzie sie zgadzal aby byl rozne
					{
						powtorzony = false;	//Zerowanie informacji o powtorzeniu
						break;				//i wyjscie z petli.
					}
				}
				if (powtorzony)
				{					//Prosba o ponowne wprowadzenie w przypadku powtorzenia.
					system("cls"); //Odswiezenie ekranu.
					cout << "Istnieje juz druzyna o takiej nazwie. Podaj inna nazwe."; //Informacja dla uzytkownika.
					Sleep(okresoczekiwania);	//Opoznienie programu o staly czas.
					system("cls");		//Odswiezenie ekranu.
					break;			//Wyjscie z petli.
				}
			}
			licznik = licznik->nast; //Przejscie przez cala tabele az do spotkania powtorki.
		}
	} while (powtorzony); //Powtarzanie w przypadku powtorzenia.
}
void komunikatzopoznieniem() //Funkcja interfejsowa, wyswietla dany komunikat i czeka okreslony czas.
{							//Nie przyjmuje ani nie zwraca zadnych wartosci.
	cout << "Rozpoczela sie juz runda wiosenna. Mozesz dodawac tylko kolejki wiosenne. Sproboj jeszcze raz. "; //Informacja dla uzytkownika
	Sleep(okresoczekiwania);	//Oczekiwanie.
}
void komunikatspotkania()//Funkcja interfejsowa, wyswietla zadany komunikat, czeka i odswieza ekran.
{							//Nie przyjmuje ani nie zwraca zadnych wartosci.
	system("cls");			//Odswiezenie ekranu.
	cout << "Nie mozesz wprowadzic wiecej spotkan. Pytanie o kolejke. "; //Informacja dla uzytkownika.
	Sleep(okresoczekiwania);	//Oczekiwanie.
	system("cls");			//Odswiezenie ekranu.
}
bool komunikatwyboru(bool wersja)	//Funkcja zwracajaca informacje o wyborze uzytkownika (bool), otrzymuje bool opisujacy wersje komunikatu.
{
	string komunikat;	//Wyswietlany komunikat w zaleznosci od wersji.
	char dobre, zle;	//Mozliwosci wyboru.
	if (wersja)			//W zaleznosci od wersji co mamy robic, przypisujemy komunikat i mozliwosci wyboru.
	{
		komunikat = "Czy chcesz jeszcze dodawac? (T/N) ";
		dobre = 't';
		zle = 'n';
	}
	else
	{
		komunikat = "Runda jesienna czy wiosenna? (J/W) ";
		dobre = 'w';
		zle = 'j';
	}
	char klawisz;
	system("cls"); //Odswiezenie ekranu.
	cout << komunikat; //Informacja dla uzytkownika.
	do {
		klawisz = _getch();	//Powtarzaj dopuki nie wybierzesz jednej z mozliwosci.
	} while (klawisz != dobre && klawisz != dobre - przesuniecieASCII && klawisz != zle && klawisz != zle - przesuniecieASCII); //Zabezpieczenie - wielkosc liter.
	system("cls"); //Odswiezenie ekranu.
	if (klawisz == dobre || klawisz == dobre - przesuniecieASCII) // Zabezpieczenie - wielkosc liter.
		return true;
	else
		return false; //Wartosci zwracane w zaleznosci od wyboru.
}
void komunikatnazwy()		//Funkcja interfejsowa, wyswietla zadany komunikat, czeka i odswieza ekran.
{							//Nie przyjmuje ani nie zwraca zadnych wartosci.
	system("cls");			//Odswiezenie ekranu.
	cout << "Druzyna nie moze grac sama ze soba.";	//Informacja dla uzytkownika.
	Sleep(okresoczekiwania);		//Oczekiwanie.		
	system("cls");				//Odswiezenie ekranu.
}
void wypisztabele(poletabeli* glowatabela)	//Funkcja interfejsowa wypisujaca pola tabeli i miejsce. Nie zwraca nic, otrzymuje wskaznik na glowetabeli
{											//(bez referencji - po wyjsciu ta sama).
	int pozycja = poczatkowapozycja;	//Zmienna pomocnicza do wyswietlania miejsca.
	cout << "poz	" << "id	" << "nazwa	" << "lm	" << "b+	" << "b-	" << "b+/-	" << "pkt" << endl; //Opis wyswietlanej informacji;
	while (glowatabela)	//Odczyt calej tabeli.
	{
		pozycja++;									//Wyswietlanie informacji o druzynie.
		cout << pozycja << '	';
		cout << glowatabela->identyfikator << '	'; //Rozne odstepy - tabulacja (dlatego zostawiam to tu).
		cout << glowatabela->nazwa << '	';
		cout << glowatabela->iloscmeczow << '	';
		cout << glowatabela->bramkizdobyte << '	';
		cout << glowatabela->bramkistracone << '	';
		cout << glowatabela->bramkizdobyte - glowatabela->bramkistracone << '	';
		cout << glowatabela->punkty << endl;
		glowatabela = glowatabela->nast;	//Przejscie na nastepna pozycje.
	}
	cout << endl << "Wcisnij dowolny klawisz."; //Informacja dla uzytkownika.
	_getch();	//Zczyt klawisza aby przejsc dalej.
}
void zapiszstanpoczatkowy(poletabeli* glowatabela)	//Funkcja zapisujaca do pliku stan poczatkowy tabeli. Otrzymuje wskaznik na glowetabeli
{													//(bez referencji brak zmiany po wyjsciu) i nie zwraca zadnych wartosci.
	ofstream plik;
	plik.open(plikzdruzynami);	//Utworzenie pliku.
	while (glowatabela)		//Przejscie do konca tabeli.
	{
		plik << glowatabela->nazwa << separatorplikowy;		//Zapis odseparowanych wartosci w pliku.
		plik << glowatabela->identyfikator << separatorplikowy;
		plik << glowatabela->bramkizdobyte << separatorplikowy;
		plik << glowatabela->bramkistracone << separatorplikowy;
		plik << glowatabela->iloscmeczow << separatorplikowy;
		plik << glowatabela->punkty;
		if (glowatabela->nast) //Przejscie do nastepnej linii tylko gdy mamy jeszcze cos do wpisania.
			plik << endl;
		glowatabela = glowatabela->nast;	//Przejscie na nastepna pozycje.
	}
}
poletabeli* stworzsezon(int &liczbadruzyn) //Funkcja tworzaca tabele druzyn (wpisujaca je na poczatku). Zwraca glowe tej tabeli, otrzymuje zmienna
{											//liczbadruzyn przez referencje aby tam wpisac ilosc wpisanych druzyn.
	int identyfikator = poczateknumeracji;	//Zmienna pomocnicza do nadawania identyfikatorow.
	poletabeli* glowatabela = NULL;			//Tabela jest pusta.
	do {
		poletabeli* tymczasowy = new poletabeli;	//Alokacja nowego pola tabeli.
		sprawdznazwe(tymczasowy->nazwa, glowatabela);	//Sprawdzanie czy mozna nadac nazwe druzynie.
		tymczasowy->identyfikator = identyfikator;	//Nadanie identyfikatora.
		identyfikator++;			// Nastepna druzyna ma kolejny.
		tymczasowy->punkty = poczatkowepunkty;			//Wpis wartosci poczatkowych.
		tymczasowy->bramkizdobyte = poczatkowebramkizdobyte;		
		tymczasowy->bramkistracone = poczatkowebramkistracone;
		tymczasowy->iloscmeczow = poczatkowailoscmeczow;
		tymczasowy->nast = glowatabela;	//Tworzenie listy jednokierunkowej
		glowatabela = tymczasowy;		//z dodawaniem na gore.
	} while (komunikatwyboru(dodawanie)); //Uzytkownik wpisuje dopoki chce.
	wypisztabele(glowatabela);	//Wypisywanie stanu przedsezonowego (uzytkownik moze poznac identyfikatory druzyn).
	liczbadruzyn = identyfikator - poczateknumeracji; //Okreslenie ilosci druzyn.
	return glowatabela;	//Zwrot glowy tabeli.
}
bool sprawdzwkolejce(spotkanie* listaspotkan, int id)	//Funkcja zwracajaca informacje, czy dana druzyna rozgrywala mecz w danej kolejce.
{														//Otrzymuje wskaznik glowy listyspotkan i identyfikator podanej druzyny.
	while (listaspotkan)
	{
		if (listaspotkan->gospodarz == id || listaspotkan->gosc == id)	//Sprawdzanie czy druzyna grala mecz, jesli tak - zwroc informacje.
			return true;
		listaspotkan = listaspotkan->nast;	//Sprawdzamy dopoki nie znajdziemy lub do konca.
	}
	return false;	//Informacja o braku znalezienia meczu z dana druzyna.
}
int pobierzzesprawdzeniem(string komunikat, kolejka* danakolejka)	//Funkcja interfejsowa zwracajaca pobrana liczbe calkowita ze sprawdzaniem bledow
{																	//i powtorzenia. Otrzymuje string z komunikatem i kolejke w ktorej sprawdza.
	system("cls");													//Odswiezenie ekranu.
	int liczba;														//Zmienna pomocnicza do przechowywania pobranej wartosci.
	cout << komunikat;												//Wyswietlenie komunikatu.
	cin >> liczba;													//Pierwszy wczyt.
	while (cin.fail() || liczba < 0 || sprawdzwkolejce(danakolejka->listaspotkan, liczba)) //Sprawdzanie bledow wejscia/wyjscia i powtorzenia.
	{
		cin.clear();									//Czyszczenie flagi bledu.
		cin.ignore(dlugosckontrolowanastringu, '\n'); //Czyszczenie bufora klawiatury.
		cout << "Blad. Sproboj jeszcze raz. ";			//Informacja dla uzytkownika.
		Sleep(okresoczekiwania);						//Oczekiwanie.
		system("cls");									//Odswiezenie ekranu.
		cout << komunikat;								//Informacja dla uzytkownika.
		cin >> liczba;									//Ponowny wczyt.
	}
	return liczba;										//Zwrot wczytanej wartosci.
}
int pobierz(string komunikat)			//Funkcja interfejsowa zwracajaca pobrana liczbe calkowita ze sprawdzaniem bledow.
{										//Otrzymuje string z komunikatem.
	system("cls");						//Odswiezenie ekranu.
	int liczba;							//Zmienna pomocnicza do przechowywania pobranej wartosci.
	cout << komunikat;					//Wyswietlenie komunikatu.
	cin >> liczba;						//Pierwszy wczyt.
	while (cin.fail() || liczba < 0 )			//Sprawdzanie bledow wejscia/wyjscia.
	{
		cin.clear();							//Czyszczenie flagi bledu.
		cin.ignore(dlugosckontrolowanastringu, '\n');	//Czyszczenie bufora klawiatury.
		cout << "Blad. Sproboj jeszcze raz. ";		//Informacja dla uzytkownika.
		Sleep(okresoczekiwania);					//Oczekiwanie.
		system("cls");								//Odswiezenie ekranu.
		cout << komunikat;							//Informacja dla uzytkownika.
		cin >> liczba;								//Ponowny wczyt.
	}
	return liczba;									//Zwrot wczytanej wartosci.
}
void pobierzwynik(int &gospodarz, int &gosc, int &pktgospodarz, int &pktgosc, kolejka* danakolejka)
{//Funkcja pobierajaca wynik spotkania, otrzymuje wszystkie wartosci liczbowe opisujace spotkanie przez referencje (otrzymaja zadane wartosci) i kolejke w ktorej wpisujemy przez wskaznik (wyjdzie niezmieniona).
	gospodarz = pobierzzesprawdzeniem(komunikat1, danakolejka);	// Pobieranie - identyfikatory ze sprawdzaniem powtorzen, wyniki bez.
	do {
		gosc = pobierzzesprawdzeniem(komunikat2, danakolejka);
		if (gospodarz == gosc)
			komunikatnazwy();	//Informacja dla uzytkownika - nie mozna grac sam ze soba.
	} while (gospodarz == gosc);	//Wpis dopoki nie beda rozne.
	pktgospodarz = pobierz(komunikat3);
	pktgosc = pobierz(komunikat4);
}
void dodajspotkanie(kolejka* danakolejka)	//Funkcja dodajaca spotkanie do kolejki, otrzymuje dana kolejke przez wskaznik, nie zwraca nic.
{											
	spotkanie* tymczasowy = new spotkanie;		//Pobiera wyniki do wartosci zaalokowanego elementu.
	pobierzwynik(tymczasowy->gospodarz, tymczasowy->gosc, tymczasowy->pktgospodarz, tymczasowy->pktgosc, danakolejka);
	tymczasowy->nast = danakolejka->listaspotkan;	//Lista jednokierunkowa - dodawanie na gore.
	danakolejka->listaspotkan = tymczasowy;
}
void dodajkolejke(kolejka*& glowakolejka, int& nr, bool &rundapop, int liczbadruzyn) //nic nie zwraca
{	//Funkcja dodajaca kolejki do listy kolejek, otrzymuje glowe i wszystkie wartosci liczbowe opisujace kolejke przez referencje (do zmiany) i liczbe druzyn do kontroli liczby spotkan.
	kolejka* tymczasowy = new kolejka; //Alokacja nowej kolejki.
	tymczasowy->nr = nr;	//Nadawanie kolejnych numerow kolejkom.
	nr++;
	bool runda = komunikatwyboru(ustalanierundy);	//Ustalanie rundy kolejki.
	while ((!runda) && (rundapop))		//Sprawdzanie czy po kolejce wiosennej nie ma jesiennej (system jesien - wiosna).
	{
		komunikatzopoznieniem(); //Informacja dla uzytkownika.
		runda = komunikatwyboru(ustalanierundy); //Ponowny wczyt.
	}
	tymczasowy->runda = runda;	//Wpis rundy.
	rundapop = runda;	//Wpisywanie stanu poprzedniego dla przejscia nastepnego.
	tymczasowy->nast = NULL;	//Wskaznik nastepnego ma nie wskazywac na nic.
	tymczasowy->listaspotkan = NULL;	//Lista podwieszana ma byc pusta.
	int iloscmeczow = zliczaniemeczow; //Zmienna pomocnicza do zliczania spotkan.
	while (iloscmeczow < (liczbadruzyn - liczbadruzyn%napol)/napol) //Sprawdzanie przekroczenia dopuszczalnej ilosci meczow.
	{	
		dodajspotkanie(tymczasowy);	//Jezeli nie mozemy dodac spotkanie.
		iloscmeczow++;	//Zwiekszenie ilosci spotkan.
		if (iloscmeczow >= (liczbadruzyn - liczbadruzyn % napol) / napol || !komunikatwyboru(dodawanie))	//Dodawanie spotkan gdy ich dopuszczalna liczba
			break;																					// nie jest przekroczona i uzytkownik chce.
	}
	komunikatspotkania(); //Komunikat o koncu mozliwosci dodawania spotkan.
	if (!glowakolejka)		//lista jednokierunkowa - dodawanie na koniec. ( W pliku bedzie wtedy kolejnosc chronologiczna.)
	{
		glowakolejka = tymczasowy;
		return;
	}
	kolejka* licznik = glowakolejka;
	while (licznik->nast != NULL)
		licznik = licznik->nast;
	licznik->nast = tymczasowy;
}
void dodawaniekolejek(kolejka*& glowakolejka, int& nr, bool &rundapop, int liczbadruzyn)
{	//Funkcja dodajaca kolejne kolejki (dopoki uzytkownik chce), otrzymuje glowe listy kolejek przez referencje (mozna zmienic), wartosci opisujace kolejke przez referencje (do zmiany).
	do {		//i liczbe druzyny dla kontroli, nie zwraca nic.
		dodajkolejke(glowakolejka, nr, rundapop, liczbadruzyn);
	} while (komunikatwyboru(dodawanie)); //Sprawdzanie czy chce dalej dodawac.
}
void przeszukajiwpisz(poletabeli* glowatabela, int id, int bramkizdobyte, int bramkistracone)
{	//Funkcja szukajaca odpowiedniej druzyny w tabeli i wpisujaca wartosci opisujace druzyne w meczu, otrzymuje te wartosci, glowe tabeli przez wskaznik (bez referencji - brak zmian), nie zwraca nic.
	while ((glowatabela) && (glowatabela->identyfikator != id))	//Wyszukiwanie druzyny
		glowatabela = glowatabela->nast;
	if (glowatabela)
	{
		glowatabela->iloscmeczow++;	//Zmiana wartosci jezeli znaleziono
		glowatabela->bramkizdobyte += bramkizdobyte;
		glowatabela->bramkistracone += bramkistracone;
		if (bramkizdobyte > bramkistracone)	//W zaleznosci od sukcesu - dodawanie punktow.
			glowatabela->punkty += wygrana;
		else
			if (bramkizdobyte == bramkistracone)
				glowatabela->punkty += remis;
	}										//Za przegrana nic nie dopisujemy.
}
void tabelasezon(kolejka *glowakolejka, poletabeli* glowatabela)
{	//Funkcja tworzaca tabele dla calego sezonu. Nie zwraca nic, dostaje glowy kolejki i tabeli przez wskaznik (bez referencj - nie zmieniamy).
	while (glowakolejka)	//Przeszukanie kolejek i ich meczow i wpis dla kazdego meczu.
	{
		spotkanie* licznik = glowakolejka->listaspotkan; //Nie mozemy zniszczyc liczbyspotkan.
		while (licznik)	
		{
			przeszukajiwpisz(glowatabela, licznik->gospodarz, licznik->pktgospodarz, licznik->pktgosc); //Wpisywanie dla gospodarza kazdego meczu.
			przeszukajiwpisz(glowatabela, licznik->gosc, licznik->pktgosc, licznik->pktgospodarz);		//Wpisywanie dla goscia kazdego meczu.
			licznik = licznik->nast; //Przejscie po wszystkich meczach.
		}
		glowakolejka = glowakolejka->nast;		//Przejscie po wszystkich kolejkach.
	}
}
void tabelarunda(kolejka *glowakolejka, poletabeli* glowatabela, bool runda)
{	//Funkcja tworzaca tabele konkretnej rundy, nie zwraca nic, dostaje glowy kolejki i tabeli przez wskaznik i informacje o rundzie (bool).
	if (!runda) //czy jesien czy wiosna
	{
		while ((glowakolejka) && (glowakolejka->runda == runda)) //Przejscie tylko po kolejkach jesiennych.
		{
			spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
			while (licznik)
			{
				przeszukajiwpisz(glowatabela, licznik->gospodarz, licznik->pktgospodarz, licznik->pktgosc);	//Wpisywanie dla gospodarza kazdego meczu.
				przeszukajiwpisz(glowatabela, licznik->gosc, licznik->pktgosc, licznik->pktgospodarz);	//Wpisywanie dla goscia kazdego meczu.
				licznik = licznik->nast;	//Przejscie po wszystkich meczach.
			}
			glowakolejka = glowakolejka->nast;	//Przejscie po wszystkich kolejkach.
		}
		return; //Zakonczenie dzialania.
	}
	while ((glowakolejka) && (glowakolejka->runda != runda)) //Pominiecie kolejek jesiennych - sa przed wiosennymi.
		glowakolejka = glowakolejka->nast;
	while (glowakolejka)
	{							//Wpisy dla wszystkich kolejek do konca - same wiosenne.
		spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
		while (licznik)
		{
			przeszukajiwpisz(glowatabela, licznik->gospodarz, licznik->pktgospodarz, licznik->pktgosc);	//Wpisywanie dla gospodarza kazdego meczu.
			przeszukajiwpisz(glowatabela, licznik->gosc, licznik->pktgosc, licznik->pktgospodarz);	//Wpisywanie dla goscia kazdego meczu.
			licznik = licznik->nast;	//Przejscie po wszystkich meczach.
		}
		glowakolejka = glowakolejka->nast;	//Przejscie po wszystkich kolejkach.
	}
}
void tabelamiejsce(kolejka *glowakolejka, poletabeli* glowatabela, bool miejsce)
{	//Funkcja tworzaca tabele konkretnego miejsca. Nie zwraca nic, dostaje glowy kolejki i tabeli przez wskaznik i informacje o miejscu (bool).
	if (miejsce)	//W zaleznosci od miejsca tylko wyniki gospodarza lub goscia.
	{
		while (glowakolejka) //przejscie po wszystkich kolejkach
		{
			spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
			while (licznik)
			{
				przeszukajiwpisz(glowatabela, licznik->gospodarz, licznik->pktgospodarz, licznik->pktgosc);	//Wpisywanie dla gospodarza kazdego meczu.
				licznik = licznik->nast;	//Przejscie po wszystkich spotkaniach.
			}
			glowakolejka = glowakolejka->nast;	//Przejscie po wszystkich kolejkach.
		}
	}
	else
	{
		while (glowakolejka)
		{
			spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
			while (licznik)
			{
				przeszukajiwpisz(glowatabela, licznik->gosc, licznik->pktgosc, licznik->pktgospodarz);	//Wpisywanie dla goscia kazdego meczu.
				licznik = licznik->nast;		//Przejscie po wszystkich spotkaniach.
			}
			glowakolejka = glowakolejka->nast;	//Przejscie po wszystkich kolejkach.
		}
	}
}
poletabeli* ukladaniedruzyn(poletabeli* glowaposortowana, poletabeli* glowatabela)
{	//Funkcja ukladajaca dany element w kolejnosci, zwraca glowe po posortowaniu, ktora dostaje przez wskaznik i glowetabeli przez wskaznik.
	poletabeli* tymczasowy = new poletabeli;
	tymczasowy->nazwa = glowatabela->nazwa;	//Przepisywanie pol tabeli.
	tymczasowy->identyfikator = glowatabela->identyfikator;
	tymczasowy->punkty = glowatabela->punkty;
	tymczasowy->bramkizdobyte = glowatabela->bramkizdobyte;
	tymczasowy->bramkistracone = glowatabela->bramkistracone;
	tymczasowy->iloscmeczow = glowatabela->iloscmeczow;
	tymczasowy->nast = NULL;
	if (!glowaposortowana)	//Ustawianie w zaleznosci od punktow (malejaco), bramek zdobytych (malejaco), bramek straconych (rosnaco).
		return tymczasowy;
	if ((glowaposortowana->punkty < tymczasowy->punkty) || ((glowaposortowana->punkty == tymczasowy->punkty) && (glowaposortowana->bramkizdobyte < tymczasowy->bramkizdobyte)) || ((glowaposortowana->punkty == tymczasowy->punkty) && (glowaposortowana->bramkizdobyte == tymczasowy->bramkizdobyte) && (glowaposortowana->bramkistracone > tymczasowy->bramkistracone)))
	{		//Dlugie warunki bo duzo sprawdzaja.
		tymczasowy->nast = glowaposortowana;
		glowaposortowana = tymczasowy;
		return glowaposortowana;
	}
	poletabeli* licznik = glowaposortowana;	//Przejscie po tabeli.
	while ((licznik->nast) && ((licznik->nast->punkty > tymczasowy->punkty) || ((licznik->nast->punkty == tymczasowy->punkty) && (licznik->nast->bramkizdobyte > tymczasowy->bramkizdobyte)) || ((licznik->nast->punkty == tymczasowy->punkty) && (licznik->nast->bramkizdobyte == tymczasowy->bramkizdobyte) && (licznik->nast->bramkistracone < tymczasowy->bramkistracone))))
		licznik = licznik->nast;	//Dlugi warunek - duzo warunkow do sprawdzenia.
	if (!licznik->nast)
		licznik->nast = tymczasowy;
	else
	{
		tymczasowy->nast = licznik->nast;
		licznik->nast = tymczasowy;
	}
	return glowaposortowana;
}						//Dodawanie na koniec
void usuwanietabeli(poletabeli *&glowatabela)	//Funkcja usuwajaca pola tabeli. Nie zwraca nic, Otrzymuje glowe tabeli przez wskaznik z referencja (do zmiany).
{
	poletabeli* tymczasowy = glowatabela;	//Przechowanie, przejscie na nastepny i usuniecie.
	glowatabela = glowatabela->nast;
	delete tymczasowy;
}
poletabeli* sortujtabele(poletabeli*& glowatabela)
{ //Funkcja sortujaca tabele. Zwraca glowe tabeli posortowanej i otrzymuje przez wskaznik z referencja (zmiana) glowe starej tabeli.
	poletabeli* glowaposortowana = NULL;	//Poczatkowo posortowana jest pusta. 
	while (glowatabela) // Sortuje dopoki istnieje stara tabela.
	{
		glowaposortowana = ukladaniedruzyn(glowaposortowana, glowatabela); //Ukladamy pozycja po pozycji
		usuwanietabeli(glowatabela);	//Usuwamy pozycja po pozycji.
	}
	return glowaposortowana;	//Zwrot posortowanej.
}
void usuntabele(poletabeli *&glowatabela) //Funkcja usuwajaca tabele. Otrzymuje wskaznik na glowe tabeli (z referencja - zmiana), nie zwraca nic. 
{
	while (glowatabela)		//Dopoki tabela istnieje - usuwamy ja pozycja po pozycji.
		usuwanietabeli(glowatabela);
}
bool odczytajstanpoczatkowy(poletabeli *& glowatabela)	//Funkcja odczytujaca poczatkowa tabele z pliku. Otrzymuje wskaznik na glowe tabeli (z referencja - zmiana).
{														//Zwraca informacje czy sie udalo (bool).
	ifstream plik;				
	plik.open(plikzdruzynami);	//Otwarcie pliku.
	if (!plik.is_open())		//Jezeli nie uda sie otworzyc - zwroc informacje z nie powiodlo sie.
		return false;
	string informacje;	//string pomocniczy do wczytu.
	while (!plik.eof())	//Do konca pliku.
	{
		getline(plik, informacje);	//Wczytywanie linii z pliku do stringa.
		istringstream pomocniczy(informacje);	//Zmienna do parsowania.
		poletabeli* tymczasowy = new poletabeli;
		pomocniczy >> tymczasowy->nazwa >> tymczasowy->identyfikator >> tymczasowy->bramkizdobyte >> tymczasowy->bramkistracone >> tymczasowy->iloscmeczow >> tymczasowy->punkty;	//Parsowanie - dlugie nazwy dlatego wychodzi.
		if (pomocniczy.fail())
			return false;
		if (tymczasowy->identyfikator < poczateknumeracji - poprzedni || tymczasowy->bramkizdobyte != poczatkowebramkizdobyte || tymczasowy->bramkistracone != poczatkowebramkistracone || tymczasowy->iloscmeczow != poczatkowailoscmeczow || tymczasowy->punkty != poczatkowepunkty)
			return false;	//Skomplikowany warunek, dlatego wychodzi poza ekran.
		for (int i = poczatekstringa; i < tymczasowy->nazwa.size(); i++) //i - iterator jako indeks - musi byc krotki.
			if (((i == poczatekstringa) && (tymczasowy->nazwa[i] > konieczakresuznakow || tymczasowy->nazwa[i] < poczatekzakresuznakow)) || ((i> poczatekstringa) && (tymczasowy->nazwa[i] == separatornazwy) && (tymczasowy->nazwa[i - poprzedni] == separatornazwy)) || ((tymczasowy->nazwa[i] != separatornazwy) && (tymczasowy->nazwa[i] > konieczakresuznakow || tymczasowy->nazwa[i] < poczatekzakresuznakow)) || ((i == tymczasowy->nazwa.size() - poprzedni) && (tymczasowy->nazwa[i] == separatornazwy)))
				return false;	//Skomplikowany warunek dlatego wychodzi poza ekran.
		poletabeli* licznik = glowatabela;
		while (licznik)
		{
			if (licznik->identyfikator == tymczasowy->identyfikator)	//Powtorki identyfikatorow.
				return false;
			if (tymczasowy->nazwa.size() == licznik->nazwa.size())	//Podejrzenie powtorzenia przy tej samej dlugosci
			{
				bool blad = true;
				for (int i = poczatekstringa; i < tymczasowy->nazwa.size(); i++)
					if (tymczasowy->nazwa[i] != licznik->nazwa[i]) //Zabezpieczenie na powtorzenie nazwy.
						blad = false;
				if (blad)
					return false;
			}
			licznik = licznik->nast;
		}
		tymczasowy->nast = glowatabela;
		glowatabela = tymczasowy;	//Lista jednokierunkowa - wpis na gore.
	}
	return true;
}	//Funkcji nie mozna bardzie skrocic - musi sprawdzic wiele warunkow.
void rodzajtabeli(kolejka* glowakolejka, poletabeli*&glowatabela)
{ //Funkcja interfejsowa wybierajaca rodzaj tabeli. Nie zwraca nic, otrzymuje glowe kolejki i glowe tabeli - przez wskaznik (z referencja - zmiany).
	char klawisz; //Zmienna pomocnicza do wybierania.
	do
	{
		cout << "Wybierz rodzaj tabeli:" << endl;	//Informacja.
		cout << "1 - caly sezon" << endl;
		cout << "2 - runda jesienna" << endl;
		cout << "3 - runda wiosenna" << endl;
		cout << "4 - domowe" << endl;
		cout << "5 - wyjazdowe ";
		klawisz = _getch();
		system("cls");
	} while (klawisz < poczatekwyboru || klawisz > koniecwyboru); //zabezpieczenie na wybrane znaki
	usuntabele(glowatabela);		//Usuwanie starej tabeli.
	odczytajstanpoczatkowy(glowatabela);	//Odczyt wzorca.
	switch (klawisz)
	{
		case '1':								//Sposob dzialania w zaleznosci od wyboru klawisza. Zostawilem jako znaki dla lepszej przejrzystosci.
		tabelasezon(glowakolejka, glowatabela);
		break;
		case '2':
		tabelarunda(glowakolejka, glowatabela, jesieñ);
		break;
		case '3':
		tabelarunda(glowakolejka, glowatabela, wiosna);
		break;
		case '4':
		tabelamiejsce(glowakolejka, glowatabela, usiebie);
		break;
		default:
		tabelamiejsce(glowakolejka, glowatabela, nawyjezdzie);
	}
}
bool edytuj(kolejka* glowakolejka, int nr, int id)
{ // Funkcja usuwajaca spotkania w wybranej kolejce. Zwraca informacje czy sie udalo (bool), otrzymuje glowe kolejek, dany nr kolejki i id uczestnika spotkania.
	bool usun = false;	//Zmienna pomocnicza okreslajaca czy mamy usunac.
	while (glowakolejka)
	{
		if (glowakolejka->nr == nr)
		{
			spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
			spotkanie* poprzedni = NULL;	//Do pomijanie spotkan.
			while (licznik)
			{
				if (licznik->gospodarz == id || licznik->gosc == id)	//Jezeli znajdzie do do usuniecia.
				{
					usun = true;
					break;
				}
				poprzedni = licznik;
				licznik = licznik->nast;	//Szuka az znajdzie lub do konca.
			}
			if (usun)	//Jezeli znalazlo to usuwa.
			{
				spotkanie* tymczasowy;
				if (poprzedni)			//Jezeli na poczatku listy.
				{
					tymczasowy = licznik;
					poprzedni->nast = licznik->nast;
				}
				else
				{
					tymczasowy = glowakolejka->listaspotkan;	//Jezeli w srodku.
					glowakolejka->listaspotkan = glowakolejka->listaspotkan->nast;
				}
				delete tymczasowy;
				break;
			}
		}
		glowakolejka = glowakolejka->nast;
	}
	if (glowakolejka)
		return true;
	return false;	//Jezeli przeszlo po wszystkich kolejkach bez odnalezienia to blad.
}
void edycja(kolejka* glowakolejka)
{//Funkcja interfejsowa do obslugi usuniec, otrzymuje glowe kolejek przez wskaznik i nie zwraca nic
	int nr, id; //Zmienne pomocnicze.
	system("cls");
	nr = pobierz("Podaj numer kolejki: "); //Wczyt wartosci pomocniczy.ch
	id = pobierz("Podaj ktoregos z uczestnikow spotkania: ");
	if (edytuj(glowakolejka, nr, id))	// Wyswietlanie informacji w zaleznosci od powodzenia edycji.
		cout << "Znaleziono. " << endl;
	else
		cout << "Blad. " << endl;;
	cout << "Wcisnij dowolny klawisz. ";	//Informacja i zczyt klawisza.
	_getch();
	system("cls");			//Odswiezenie.
}
bool dopisywanie(kolejka* glowakolejka, int nr, int liczbadruzyn)
{// Funkcja dopisujaca spotkanie w wybranej kolejce. Zwraca informacje czy sie udalo (bool), otrzymuje glowe kolejek przez wskaznik, dany numer kolejki i liczbe druzyn do kontroli.
	int mecze = zliczaniemeczow; // Zmienna pomocnicza, musi byc zaczete zerem.
	while (glowakolejka)
	{
		if (glowakolejka->nr == nr)	//Przejscia az znajdze dana kolejke.
		{	
			spotkanie* licznik = glowakolejka->listaspotkan;	//Nie mozemy zniszczyc listy spotkan.
			while (licznik)
			{
				mecze++;							//Zliczanie liczby spotkan.
				licznik = licznik->nast;
			}
			if (mecze >= (liczbadruzyn - liczbadruzyn % napol)/napol)	//Czy mozna dopisac mecz.
				return false;
			dodajspotkanie(glowakolejka);	//Dodawanie spotkan.
			return true;
		}
		glowakolejka = glowakolejka->nast;
	}
	return false;	//Jezeli przeszedl do konca - blad.
}
void dopis(kolejka* glowakolejka, int liczbadruzyn)
{//Funkcja interfejsowa do obslugi dopisu. Otrzymuje glowe kolejek przez wskaznik i liczbe druzyn dla kontroli, nie zwraca nic.
	int nr;	//Pomocnicza do okreslania numeru kolejki.
	system("cls");	//Odswiezenie.
	nr = pobierz("Podaj numer kolejki: "); //Wczyt pomocniczej.
	if (dopisywanie(glowakolejka, nr, liczbadruzyn))
		cout << "Dopisano. " << endl;	//Wyswietlanie informacji w zaleznosci od powodzenia dopisu.
	else
		cout << "Blad. " << endl;;
	cout << "Wcisnij dowolny klawisz. ";	//Informacja i oczekiwanie na klawisz.
	_getch();
	system("cls");	//Odswiezenie.
}
void przechowanieinformacji(int nr, int liczbadruzyn, bool rundapop)
{//Funkcja wpisujaca do pliku aktualny stan programu (nr kolejki, liczbe druzyn, runde poprzednia). Otrzymuje te wartosci, nie zwraca nic.
	ofstream plik;
	plik.open(informacje);	//Utworzenie pliku.
	plik << nr << separatorplikowy << liczbadruzyn << separatorplikowy << rundapop;	//Wpis.
	plik.close();	//Zamkniecie.
}
void usuwaniekolejek(kolejka *&glowakolejka)
{	//Funkcja usuwajaca kolejke. Otrzymuje glowe kolejek przez wskaznik (z referencja - do zmiany), nie zwraca nic.
	kolejka* tymczasowy = glowakolejka;	//Przechowanie, przesuniecie na nastepny i usuniecie.
	glowakolejka = glowakolejka->nast;
	delete tymczasowy;
}
void usuwaniespotkan(spotkanie* &glowaspotkanie)
{	//Funkcja usuwajaca spotkanie. Otrzymuje glowe listy spotkan przez wskaznik (z referencja - do zmiany), nie zwraca nic.
	spotkanie* tymczasowy = glowaspotkanie;		//Przechowanie, przesuniecie na nastepny i usuniecie.
	glowaspotkanie = glowaspotkanie->nast;
	delete tymczasowy;
}
void zapisdopliku(kolejka *&glowakolejka)
{	//Funkcja zapisujaca do pliku wyniki spotkan. Otrzymuje glowe kolejek przez wskaznik (z referencja - do zmiany), nie zwraca nic.
	ofstream plik;
	plik.open(nazwapliku);		//Stworzenie pliku.
	while (glowakolejka)	//Przejscie przez wszystkie kolejki i spotkania w nich.
	{
		while (glowakolejka->listaspotkan)
		{
			plik << glowakolejka->nr << separatorplikowy;	//Wpis do pliku.
			plik << glowakolejka->runda << separatorplikowy;
			plik << glowakolejka->listaspotkan->gospodarz << separatorplikowy;
			plik << glowakolejka->listaspotkan->gosc << separatorplikowy;
			plik << glowakolejka->listaspotkan->pktgospodarz << separatorplikowy;
			plik << glowakolejka->listaspotkan->pktgosc;
			if ((glowakolejka->nast) || (glowakolejka->listaspotkan->nast))
				plik << endl;
			usuwaniespotkan(glowakolejka->listaspotkan);
		}									//Usuwanie odczytanego elementu.
		usuwaniekolejek(glowakolejka);
	}
	plik.close();	//Zamkniecie pliku.
}
void pytanie(char &klawisz)
{	//Funkcja odczytujaca wybrany przez uzytkownika klawisz. Otrzymuje przez referencje znak, ktory bedzie zmieniac. Nie zwraca nic.
	do {
		system("cls");				//Odswiezenie i wyswietlenie informacji.
		cout << "Co robic dalej?" << endl;
		cout << "1 - usun dany mecz z podanej kolejki" << endl;
		cout << "2 - dopisz kolejny mecz do danej kolejki" << endl;
		cout << "3 - dodaj kolejki" << endl;
		cout << "4 - wyswietl tabele" << endl;
		cout << "5 - zapisz informacje i zakoncz ";
		klawisz = _getch();		//Zczyt klawisza.
		system("cls");			//Odswiezenie.
	} while (klawisz < poczatekwyboru || klawisz > koniecwyboru); //Dopoki nie wczyta mozliwego klawisza.
}
void codalej(kolejka* glowakolejka, poletabeli*&glowatabela,int &nr, bool &rundapop, int liczbadruzyn)
{ //Funkcja wykonujaca mozliwe dzialania programu. Otrzymuje wskaznik na glowe kolejki, wskaznik na glowe tabeli (z referencja - do zmiany), nr kolejki przez referencje aby go zmienic, runde poprzednia przez referencje aby ja zmienic i liczbe druzyn do kontroli.
	char klawisz; // Zmienna pomocnicza do przechowywania wyboru.
	do {
		pytanie(klawisz); //Odczyt wyboru.
		switch (klawisz)
		{									//W zaleznosci od wyboru zadane dzialanie.
			case '1':						//Zostawilem znaki aby bylo lepiej widac.
			edycja(glowakolejka);
			break;
			case '2':
			dopis(glowakolejka, liczbadruzyn);
			break;
			case '3':
			dodawaniekolejek(glowakolejka, nr, rundapop, liczbadruzyn);
			break;
			case '4':
			rodzajtabeli(glowakolejka, glowatabela);
			glowatabela = sortujtabele(glowatabela);
			wypisztabele(glowatabela);
			break;
			default:
			przechowanieinformacji(nr, liczbadruzyn, rundapop);
			zapisdopliku(glowakolejka);
			usuntabele(glowatabela);
		}
	} while (klawisz != koniecwyboru);	//Dopoki nie chce zakonczyc.
}
void wczytajspotkanie(kolejka* danakolejka, int gospodarz, int gosc, int pktgospodarz, int pktgosc)
{	//Funkcja wczytujaca spotkania do kolejek. Otrzymuje wskaznik na dana kolejke i wielkosci charakteryzujace spotkanie.
	spotkanie* tymczasowy = new spotkanie;	//Alokacja nowego spotkania.
	tymczasowy->gospodarz = gospodarz;		//Przepisanie wartosci.
	tymczasowy->gosc = gosc;
	tymczasowy->pktgospodarz = pktgospodarz;
	tymczasowy->pktgosc = pktgosc;
	tymczasowy->nast = danakolejka->listaspotkan;	//Lista jednokierunkowa - dodawanie na gore.
	danakolejka->listaspotkan = tymczasowy;
}
kolejka* wczytajkolejke(kolejka*& glowakolejka, int& nr, bool &runda, bool &rundapop)
{	//Funkcja wczytujaca kolejke. Otrzymuje wskaznik na glowe kolejki (z referencja - zmiany), numer kolejki przez referencje (zmiany) oraz runde i runde poprzedni przez referencje (zmiany).
	kolejka* tymczasowy = new kolejka;	//Alokacja nowej kolejki.
	tymczasowy->nr = nr;		//Uzupelnienie wartosci.
	tymczasowy->runda = runda;
	tymczasowy->nast = NULL;
	tymczasowy->listaspotkan = NULL;
	if (!glowakolejka)		//Lista jednokierunkowa - dodawanie na koniec.
	{
		glowakolejka = tymczasowy;
		return glowakolejka;
	}
	kolejka* licznik = glowakolejka;
	while (licznik->nast != NULL)
		licznik = licznik->nast;
	licznik->nast = tymczasowy;
	return licznik->nast;
}
bool odczytzpliku(kolejka *&glowakolejka, int liczbadruzyn)
{	//Funkcja odczytujaca wyniki meczow z pliku. Otrzymuje glowe kolejki przez wskaznik (z referencja - zmiany) i liczbe druzyn.
	ifstream plik;
	plik.open(nazwapliku);	//Otwarcie pliku.
	if (!plik.is_open())	//Jezeli sie nie udalo - zwroc informacje o tym.
		return false;
	kolejka* aktualnakolejka = NULL;	//Na poczatku lista kolejek pusta.
	bool poprzedniarunda = przedrozgrywkami;	//Zmienne pomocnicze do wczytu.
	int poprzedniakolejka = przedkolejkami;
	int licznikspotkan = zliczaniemeczow;
	string mecz;
	int nr, gospodarz, gosc, pktgospodarz, pktgosc;
	bool runda;
	getline(plik, mecz);				//Jezeli pusty - zwroc informacje o tym.
	if (mecz.size() == poczatekstringa)
		return true;
	plik.close();
	plik.open(nazwapliku);	//Zamkniecie i ponowne otwarcie aby wszystko odczytac.
	while (!plik.eof())	//Do konca pliku.
	{
		getline(plik, mecz);	//Wczyt linii informacji.
		istringstream pomocniczy(mecz);
		pomocniczy >> nr >> runda >> gospodarz >> gosc >> pktgospodarz >> pktgosc;	//Parsowanie informacji.
		if (pomocniczy.fail())	//Zbezpieczenie - zly format.
			return false;
		else
		{
			if ((poprzedniarunda) && (!runda))	//Zabezpieczenie rund.
				return false;
			if (nr < poprzedniakolejka)			//Zabezpieczenie - numer kolejki.
				return false;
			if (nr != poprzedniakolejka)
			{
				aktualnakolejka = wczytajkolejke(glowakolejka, nr, runda, poprzedniarunda);
				licznikspotkan = zliczaniemeczow;
			}
			licznikspotkan++;
			if (licznikspotkan > (liczbadruzyn - liczbadruzyn % napol) / napol)	//Zabezpiecenie - liczba spotkan.
				return false;
			wczytajspotkanie(aktualnakolejka, gospodarz, gosc, pktgospodarz, pktgosc);
			poprzedniarunda = runda;
			poprzedniakolejka = nr;
		}
	}
	return true;
}	//Wiele warunkow wczytu - nie mozna krocej.
bool odczytywanieinformacji(int &nr, int &liczbadruzyn, bool &rundapop)
{	//Funkcja odczytujaca informacje o programie (nr, liczbadruzyn, runda poprzednia). Otrzymuje te wartosci przez referencje (do zmiany), zwraca informacje czy sie udalo (bool).
	ifstream plik;
	plik.open(informacje);	//Powiazanie pliku.
	if (!plik.is_open())	//Otwarcie pliku. Jezeli sie nie uda - zwroc informacje.
		return false;
	string inf;	//String pomocniczy dla wczytu.
	getline(plik, inf);	//Zczyt - wszystko ma byc w jednej linii.
	istringstream pomocniczy(inf);
	pomocniczy >> nr >> liczbadruzyn >> rundapop;	//Parsowanie informacji.
	if (pomocniczy.fail())	//Jezeli nie udalo sie parsowac - zwroc informacje.
		return false;
	return true;	//Jezeli sie powiodlo - zwroc informacje.
}
void dzialanieprogramu(kolejka*& glowakolejka, poletabeli*& glowatabela, int &nr, int &liczbadruzyn, bool &rundapop)
{	//Funkcja interfejsowa wykonujaca algorytm dzialania programu. Otrzymuje wskaniki na glowe kolejek i tabeli, a takze numer kolejki, liczbe druzyn, a takze runde poprzednia (wszystko przez referencje - do zmiany), nie zwraca nic.
	komunikatpoczatkowy();	//Informacja dla uzytkownika.
	char klawisz;	//Zmienna pomocnicza do odczytu wyboru.
	do {
		system("cls");		//Odswiezenie ekranu.
		cout << "Co chcesz robic?" << endl;	//Informacje dla uzytkownika.
		cout << "1 - wczytaj zapisany sezon" << endl;
		cout << "2 - rozpocznij nowy " << endl;
		cout << "3 - zakoncz ";
		klawisz = _getch();	//Wczyt wyboru.
		system("cls");	//Odswiezenie.
	} while (klawisz < poczatekwyboru || klawisz > koniecwyboru_dzialanie);	//Dopoki nie bedzie to jedna z mozliwych.
	switch (klawisz)
	{					//Dzialanie w zaleznosci od wyboru.
		case '1':
		if (!odczytywanieinformacji(nr, liczbadruzyn, rundapop) || !odczytajstanpoczatkowy(glowatabela) || !odczytzpliku(glowakolejka, liczbadruzyn))
		{
			cout << "Blad";
			_getch();
		}
		break;
		case '2':		//Znaki zostawione aby bylo lepiej widac dzialanie.
		glowatabela = stworzsezon(liczbadruzyn);
		zapiszstanpoczatkowy(glowatabela);
		break;
	}
	if (klawisz != koniecwyboru_dzialanie)		//Po poczatkowym wyborze - dalsze dzialania programu.
		codalej(glowakolejka, glowatabela, nr, rundapop, liczbadruzyn);
}
void programglowny()
{ //Funkcja przechowujaca inicjalizujaca wartosci poczatkowe i wywolujaca dzialanie programu. Nie przyjmuje niec i nic nie zwraca.
	kolejka* glowakolejka = NULL;	//Inicjalizacja wartosci programu.
	poletabeli* glowatabela = NULL;
	int nr = poczatkowakolejka;
	int liczbadruzyn;
	bool rundapop = przedrozgrywkami;
	dzialanieprogramu(glowakolejka, glowatabela, nr, liczbadruzyn, rundapop);	//Wywolanie dzialania.
}
int main()
{
	programglowny();
	return 1;
}