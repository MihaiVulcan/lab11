#pragma once
#include <functional>
#include <random>
#include <numeric>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "domain.h"
#include "lista.h"
#include "observer.h"
#include <fstream>
#include <fstream>
using std::vector;
using std::map;
using std::ostream;
using std::pair;
using namespace std;

class ListaNotificare : public Observable {
private:
	vector<Proprietar> lista;
public:
	ListaNotificare();
	//adauga un apartament in lista de notificare
	//daca apartamenutl nu exita RepoException
	void adaugaLista(Proprietar p);

	void stergeLista(Proprietar p);

	//goleste lista de notificare
	void goleste();

	//returneaza lungimea listei
	int listSize();

	//exporta lista intr-un fiser cu denumirea filename
	void exporta(string filename);

	vector<Proprietar> getAll() {
		return lista;
	}
};

class Repo {
public:

	Repo() noexcept = default;

	virtual void store(const Proprietar& p) = 0;

	/*sterge un proprietar
	input:ap-numar apartament*/
	virtual void deleteP(int ap) = 0;

	/*modifica un proprietar
	input p-Propritar valid*/
	virtual void modify(const Proprietar& p) = 0;

	virtual const Proprietar& search(const Proprietar& ot) = 0;

	//returneaza numar de proprietari
	virtual int size() = 0;

	//returneaza toti proprietarii
	virtual vector<Proprietar> getAll() = 0;

	/*map<int, Proprietar>::iterator begin() {
		return propri.begin();
	}

	map<int, Proprietar>::iterator end() {
		return propri.end();
	}
	
	virtual Proprietar operator[](int poz) = 0;*/

	virtual ~Repo() = default;
};

class Proprietari: public Repo {
private:
	map<int, Proprietar> propri;
	bool exista(const Proprietar& p) const noexcept;
public:
	Proprietari() noexcept {};
	Proprietari(const Proprietari& ot) = delete;

	/*adauga un proprietar
	input:p-Propritar*/
	 void store(const Proprietar& p) override;

	/*sterge un proprietar
	input:ap-numar apartament*/
	 void deleteP(int ap) override;

	/*modifica un proprietar
	input p-Propritar valid*/
	 void modify(const Proprietar& p)override;

	 const Proprietar& search(const Proprietar& ot)override;

	//returneaza numar de proprietari
	 int size() override;

	//returneaza toti proprietarii
	 vector<Proprietar> getAll() override;

	 ~Proprietari() = default;
};

class RepoException {
	string msg;
public:
	RepoException(string m) :msg{ m } {}
	//functie friend (vreau sa folosesc membru privat msg)
	string toString() {
		return msg;
	}
	friend ostream& operator<<(ostream& out, const RepoException& ex);
};

ostream& operator<<(ostream& out, const RepoException& ex);

class ProprietariFisiere: public Repo {
private:
	string filename;

	void read_from_file();
	void write_to_file();

	map<int, Proprietar> propri;
	bool exista(const Proprietar& p) const noexcept;

public:
	ProprietariFisiere(string filename) : Repo(), filename{ filename } {
		read_from_file();
	}
	
	/*adauga un proprietar
	input:p-Propritar*/
	void store(const Proprietar& p) override;

	/*sterge un proprietar
	input:ap-numar apartament*/
	void deleteP(int ap) override;

	/*modifica un proprietar
	input p-Propritar valid*/
	void modify(const Proprietar& p)override;

	const Proprietar& search(const Proprietar& ot)override;

	//returneaza numar de proprietari
	int size() override;

	//returneaza toti proprietarii
	vector<Proprietar> getAll() override;

	~ProprietariFisiere() = default;

};

class RepoChaos : public Repo{
private:
	double probabilitate;
	void crapa() const;
	map<int, Proprietar> propri;
	bool exista(const Proprietar& p) const noexcept;
public:
	RepoChaos(double p) : Repo(), probabilitate{ p } {};

	/*adauga un proprietar
	input:p-Propritar*/
	void store(const Proprietar& p) override;

	/*sterge un proprietar
	input:ap-numar apartament*/
	void deleteP(int ap) override;

	/*modifica un proprietar
	input p-Propritar valid*/
	void modify(const Proprietar& p)override;

	const Proprietar& search(const Proprietar& ot)override;

	//returneaza numar de proprietari
	int size() override;

	//returneaza toti proprietarii
	vector<Proprietar> getAll() override;

	~RepoChaos() = default;
};