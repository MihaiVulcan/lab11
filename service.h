#pragma once
#include <functional>
#include <random>
#include <numeric>
#include <algorithm> 
#include "repo.h"
#include "validator.h"
#include <map>
#include <fstream>
#include "undo.h"

using namespace std;

class EntityCountDTO {
public:
	string nume;
	string tip;
	int count;
	EntityCountDTO(string n, string t, int c);
	EntityCountDTO() : count{ 0 }, nume{ "" }, tip{ "" } {};
};


class ServicePropri {
private:
	Repo& propri;
	Validator& vali;
	vector<unique_ptr<ActiuneUndo>> undoActions;


	//functie generala de filtrare care primeste o functie filtru
	//retureaza o Lista de proprietari
	const vector<Proprietar> filtreaza(function<bool(const Proprietar&)> fct);
	/*Functie generala de sortare a Proprietarilor
		cmp-functie lambda care primeste 2 Propriatri si returneaza bool
	*/
	const vector<Proprietar> sort(function<bool(const Proprietar& p1, const Proprietar& p2)> cmp);
public:
	ListaNotificare lista;
	ServicePropri(Repo& propri, Validator& vali)noexcept :propri{ propri }, vali{ vali }, lista{}{
	}
	~ServicePropri() = default;

	//nu permitem copierea
	ServicePropri(const ServicePropri& ot) = delete;
	
	/*adauga un proprietar nou
	input: ap-nr aparatament, s-suprafata, n-nume, t-tipul apratamentului*/
	void adauga(int ap, double s, const string& n, const string& t);

	/*sterge un proprietar
	ap-numar apartament*/
	void sterge(int ap);

	/*modifica un proprietar 
	input: ap-nr aparatament, s-suprafata, n-nume, t-tipul apratamentului*/
	void modifica(int ap, double s, const string& n, const string& t);


	const Proprietar& cauta(int ap);

	/*filtreaza dupa tip
	tip:-string
	*/
	const vector<Proprietar> filtruTip(string tip);

	/*filtreaza dupa interval de suprafata
	min max- unsigned
	*/
	const vector<Proprietar> filtruSp(int min, int max);

	/*sortare dupa nume
	*/
	const vector<Proprietar> sortNume();

	//sortare dupa suprafata
	const vector<Proprietar> sortSp();

	//sortare dupa suprafata si tip
	const vector<Proprietar> sortSpTip();

	vector<EntityCountDTO> raport();

	//returneaza toti proprietarii
	const vector<Proprietar> getAll() const {
		return propri.getAll();
	}

	//returneaza numarul de proprietari
	int size() const{
		return propri.size();
	}

	//adauga un apartament in lista de notificare
	//daca apartamenutl nu exita RepoException
	void adaugaLista(int ap);

	//goleste lista de notificare
	void goleste();

	//adauga aparatente aleator
	//nr numar de apartamente intoduse
	void aleator(int nr);

	//returneaza lungimea listei
	int listSize();

	//exporta lista intr-un fiser cu denumirea filename
	void exporta(string filename);

	//suprafata totala
	double supTotal();

	//undo prentru adaugare stergere si modificare
	void undo();
};

