// BnB_Algorytm_Bruteforce.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

double PCFreq = 0.0;
int licznik = 0;

struct Item
{
	double weight;
	double value;
};

vector < Item > plecak;

struct Node
{
	int level, profit, bound;
	float weight;
};



void Generuj_plik(int rozmiar,const char* nazwa){
	fstream plik;
	if (plik.fail())
		cout << "Blad wczytywania pliku" << endl;
	plik.open(nazwa, ios::out);
	plik << rozmiar;
	plik << " ";
	plik << rand() % 50 + 10;
	plik << "\n";
	for (int i = 0; i < rozmiar; i++)
	{
		plik << rand() % 50 + 1; 
		plik << " ";
		plik << rand() % 100 + 1;
		plik << "\n";
	}
	
	plik.close();
}

int Rozmiar_pliku(const char* nazwa) {
	int rozmiar = 0;
	string plik;

	fstream file(nazwa, ios::in);

	if (file.is_open())
	{
		file >> rozmiar;
		if (file.fail())
			cout << "Blad wczytywania pliku" << endl;

	}

	file.close();
	return rozmiar;
};

int Limit_Wagi(const char* nazwa) {
	int limit = 0;
	string plik;

	fstream file(nazwa, ios::in);

	if (file.is_open())
	{
		file >> limit;
		file >> limit;
		if (file.fail())
			cout << "Blad wczytywania pliku" << endl;

	}

	file.close();
	return limit;
};

void Wczytaj_z_pliku(const char* plik) {
	string line;
	Item x;
	int size = 0;
	int limit = 0;

	fstream file(plik, ios::in);

	if (file.is_open())
	{
		file >> size;
		file >> limit;
		if (file.fail())
			cout << "Blad wczytywania pliku" << endl;
		for (size_t i = 0; i < size; i++)
		{
			file >> x.value;
			file >> x.weight;
			plecak.push_back(x);
			if (file.fail())
				cout << "Blad wczytywania pliku" << endl;
		}
		
		
		file.close();
	}
	else
		cout << "blad wczytywania" << endl;

}

void Wyswietl(const char* plik) {
	Wczytaj_z_pliku(plik);
	int size = Rozmiar_pliku(plik);
	for (int i = 0; i < size; i++) {
		cout << plecak[i].weight << " ";
		cout << plecak[i].value << " ";
		cout << "\n";
	}
}

bool Porownaj(Item a, Item b) {
	double przed1 = (double)a.value / a.weight;
	double przed2 = (double)b.value / b.weight;
	return przed1 > przed2;
}

int Bound(Node u, int n, int W, Item items[])
{
	if (u.weight >= W)
		return 0;

	int profit_bound = u.profit;

	int j = u.level + 1;
	int totweight = u.weight;

	while ((j < n) && (totweight + items[j].weight <= W)) {
		totweight += items[j].weight;
		profit_bound += items[j].value;
		j++;
	}

	if (j < n)
		profit_bound += (W - totweight) * items[j].value /
		items[j].weight;

	return profit_bound;
}

int Algorytm(int W, Item items[], int n){
	
	sort(items, items + n, Porownaj);

	queue<Node> Q;
	Node u, v;

	u.level = -1;
	u.profit = u.weight = 0;
	Q.push(u);

	int maxprofit = 0;
	while (!Q.empty())
	{
		u = Q.front();
		Q.pop();

		if (u.level == -1)
			v.level = 0;

		if (u.level == n - 1)
			continue;

		v.level = u.level + 1;

		v.weight = u.weight + items[v.level].weight;
		v.profit = u.profit + items[v.level].value;

		if (v.weight <= W && v.profit > maxprofit)
			maxprofit = v.profit;

		v.bound = Bound(v, n, W, items);

		if (v.bound > maxprofit)
			Q.push(v);

		v.weight = u.weight;
		v.profit = u.profit;
		v.bound = Bound(v, n, W, items);
		if (v.bound > maxprofit)
			Q.push(v);
	}

	return maxprofit;
}

int max(int a, int b){
	if(a>=b)
		return a;
	else
		return b;	
}

int Algorytm_Bruteforce(int W, Item items[], int n){
    if (n == 0 || W == 0)
        return 0;
 
 
    if (items[n - 1].weight > W)
        return Algorytm_Bruteforce(W, items, n - 1);
 
 
    else return max(items[n - 1].value + Algorytm_Bruteforce(W - items[n - 1].weight, items , n - 1), Algorytm_Bruteforce(W, items, n - 1));

}

void MENU() {
	int wybor = 0; // zmienna potrzebna do poruszania sie po switch-case menu
	string plik;
	cout << "Podaj nazwe pliku" << endl;
	cin >> plik;
	Wczytaj_z_pliku(plik.c_str());
	Wyswietl(plik.c_str());
	do
	{
		cout << " \nWybierz opcje: " << endl;
		cout << " 1. Wczytaj dane i wyswietl" << endl;
		cout << " 2. Wyswietl " << endl;
		cout << " 3. Uruchom algorytm podzialu i ograniczen " << endl;
		cout << " 4. Uruchom zachlanny " << endl;
		cout << " 5. Generuj plik" << endl;
		cout << " 0. Wyjdz" << endl;
		cin >> wybor;
		switch (wybor)
		{
		case 1:
		{
			plecak.clear();
			cout << "Podaj nazwe pliku" << endl;
			cin >> plik; 
			Wczytaj_z_pliku(plik.c_str());
			Wyswietl(plik.c_str());
			break;
		}
		case 2:
		{
			
			Wyswietl(plik.c_str());
			break;
		}
		case 3:
		{
		
			int W = Limit_Wagi(plik.c_str());
			const int s = plecak.size() / 2;
			Item przedmiot[s];
		
			int n = plecak.size()/2;
			for (int i = 0; i < n; i++) 
			{
				przedmiot[i] = plecak[i];
			}

			cout << " Wynik algorytmu " << Algorytm(W, przedmiot, n)<< endl;

		break;
		}
		case 4:
		{		
			int W = Limit_Wagi(plik.c_str());
			const int s = plecak.size() / 2;
			Item przedmiot[s];
		
			int n = plecak.size()/2;
			for (int i = 0; i < n; i++) 
			{
				przedmiot[i] = plecak[i];
			}

			cout << " Wynik algorytmu " << Algorytm_Bruteforce(W, przedmiot, n)<< endl;
		break;

		}
		case 5:
		{
			int wartosc = 1;
			string plik2;

			cout << "Podaj rozmiar generowanego plecaka " << endl;
			cin >> wartosc;

			cout << "Podaj nazwe generowanego pliku " << endl;
			cin >> plik2;
			cout << plik << endl;	
			Generuj_plik(wartosc,plik2.c_str());
		break;
		}
		case 0:
		{
			wybor = 0;
			break;
		}
		default:
			cout << "Niema takiej komendy" << endl;
			MENU();
			break;
		}
	} while (wybor != 0);
}


int main()
{

	MENU();
	getchar();

    return 0;
}
