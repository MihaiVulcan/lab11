#include "teste.h"
#include "domain.h"
#include "repo.h"
#include "service.h"
#include "lista.h"
#include <assert.h>

void test_domain() {
	Proprietar p1(1, 55.5, "Andrei", "apartament");
	Proprietar p2(1, 60, "Maria", "studio");
	assert(p1.get_apartament() == 1);
	assert(p1.get_suprafata() == 55.5);
	assert(p1.get_nume() == "Andrei");
	assert(p1.get_tip() == "apartament");

	assert(p1 == p2);
}

void test_repo() {
	Proprietar p1(1, 55.5, "Andrei", "apartament");
	Proprietar p2(5, 60, "Maria", "studio");
	Proprietar p3(1, 70, "Andrei", "apartament");

	//test adauga
	Proprietari propri;
	propri.store(p1);
	assert(propri.size() == 1);
	try {
		propri.store(p3);
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}
	assert(propri.size() == 1);
	propri.store(p2);
	assert(propri.size() == 2);

	//test stergere
	try {
		propri.deleteP(2);
		assert(false);
	}
	catch (const RepoException&){
		assert(true);
	}
	assert(propri.size() == 2);
	propri.deleteP(1);
	assert(propri.size() == 1);

	//test modifica
	Proprietar p4(5, 70, "Andrei", "apartament");
	propri.modify(p4);
	vector<Proprietar> aux = propri.getAll();
	assert(aux[0].get_suprafata()==70);

	Proprietar p5(100, 70, "Andrei", "apartament");
	try {
		propri.modify(p5);
		assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}
}

void test_srv() {
	Proprietari propri;
	Validator vali;
	ServicePropri srv{ propri, vali };

	//test adaugare
	srv.adauga(1, 55.5, "Andrei", "Studio");
	assert(srv.size() == 1);
	try {
		srv.adauga(1, 55.5, "Andrei", "Studio");
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}
	assert(srv.size() == 1);
	srv.adauga(2, 120, "Maria", "Penthouse");
	assert(srv.size() == 2);

	//test stergere
	try {
		srv.sterge(5);
		assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}
	assert(srv.size() == 2);
	srv.sterge(1);
	assert(srv.size() == 1);
	vector<Proprietar> aux1 = srv.getAll();
	assert(aux1[0].get_apartament() == 2);

	//test modifica
	srv.modifica(2, 90.7, "Alexandru", "Garsoniera");
	vector<Proprietar> aux2 = srv.getAll();
	assert(aux2[0].get_suprafata() == 90.7);

	//test cautare
	const Proprietar& p = srv.cauta(2);
	assert(p.get_suprafata() == 90.7);
	try {
		srv.cauta(99);
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}
}

void test_vali() {
	Validator vali;
	Proprietar p1(1, 55.5, "Andrei", "Apartament");
	Proprietar p2(-1, 60, "Maria", "Studio");
	Proprietar p3(1, -60, "Maria", "Stud");
	Proprietar p4(1, 60, "Maria", "");
	Proprietar p5(1, 60, "", "Studio");

	vali.validate(p1);
	try{
		vali.validate(p2);
		assert(false);
	}
	catch (ValidateException) {
		assert(true);
	}

	try {
		vali.validate(p3);
		assert(false);
	}
	catch (ValidateException) {
		assert(true);
	}

	try {
		vali.validate(p4);
		assert(false);
	}
	catch (ValidateException) {
		assert(true);
	}

	try {
		vali.validate(p5);
		assert(false);
	}
	catch (ValidateException) {
		assert(true);
	}
}

void test_lista() {
	Lista<Proprietar> prop;
	Proprietar p1(1, 55.5, "Andrei", "Apartament");
	Proprietar p2(2, 60, "Maria", "Studio");
	Proprietar p3(3, 100, "Andrei", "Garsoniera");
	Proprietar p4(4, 70, "Maria", "Apartament");
	Proprietar p5(5, 80, "Alex", "Studio");

	prop.push_back(p1);
	assert(prop.size() == 1);
	prop.push_back(p2);
	assert(prop.size() == 2);
	prop.push_back(p3);
	assert(prop.size() == 3);
	prop.push_back(p4);
	assert(prop.size() == 4);
	prop.push_back(p5);
	assert(prop.size() == 5);

	assert(prop.first() == p1);
	assert(prop[0] == p1);
	assert(prop[1] == p2);
	assert(prop[2] == p3);
	assert(prop[3] == p4);
	assert(prop[4] == p5);

	int i = 1;
	for (const auto& e : prop)
		assert(e.get_apartament() == i++);

	prop.erase(1);
	assert(prop.size() == 4);

	Proprietar p1New(1, 60, "Andrei", "Apartament");
	prop.modify(p1, p1New);
	assert(prop[0].get_suprafata() == 60);

	Proprietar p3New(3, 120, "Andrei", "Garsoniera");
	prop.modify(p3, p3New);
	assert(prop[1].get_suprafata() == 120);

	Proprietar p5New(5, 100, "Alex", "Studio");
	prop.modify(p5, p5New);
	assert(prop[3].get_suprafata() == 100);

	prop.erase(0);
	assert(prop.size() == 3);

	prop.erase(2);
	assert(prop.size() == 2);
}

void test_filtre() {
	Proprietari propri;
	Validator vali;
	ServicePropri srv{ propri, vali };

	srv.adauga(1, 123, "ana", "Apartament");
	srv.adauga(2, 100, "ana", "Studio");
	assert(srv.size() == 2);

	const vector<Proprietar> p = srv.filtruTip("Apartament");
	assert(p.size() == 1);

	const vector<Proprietar> p1 = srv.filtruSp(80, 120);
	assert(p1.size()==1);
}

void test_sort() {
	Proprietari propri;
	Validator vali;
	ServicePropri srv{ propri, vali };

	srv.adauga(1, 123, "maria", "Apartament");
	srv.adauga(2, 100, "ana", "Studio");

	vector<Proprietar> p1 = srv.sortNume();
	assert(p1[0].get_nume() == "ana");

	vector<Proprietar> p2 = srv.sortSp();
	assert(p2[0].get_nume() == "ana");

	srv.adauga(3, 200, "alex", "Apartament");
	srv.adauga(4, 50, "andrei", "Studio");

	vector<Proprietar> p3 = srv.sortSpTip();
	assert(p3[0].get_nume() == "maria");
	assert(p3[1].get_nume() == "alex");
	assert(p3[2].get_nume() == "andrei");
	assert(p3[3].get_nume() == "ana");
}

void test_listaNotificare() {
	Proprietari propri;
	Validator vali;
	ServicePropri srv{ propri, vali };

	srv.adauga(1, 55.5, "Andrei", "Apartament");
	srv.adauga(2, 60, "Maria", "Studio");
	srv.adauga(3, 160, "Ana", "Studio");
	srv.adauga(4, 20, "Alex", "Garsoniera");
	srv.adauga(5, 80, "Bogdan", "Penthouse");


	srv.adaugaLista(1);
	assert(srv.listSize() == 1);

	try {
		srv.adaugaLista(7);
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}

	try {
		srv.adaugaLista(1);
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}

	srv.adaugaLista(3);
	assert(srv.listSize() == 2);

	srv.goleste();
	assert(srv.listSize() == 0);

	srv.adaugaLista(5);
	assert(srv.listSize() == 1);
	
	srv.goleste();
	srv.aleator(3);
	srv.exporta("test");
}

void test_undo() {
	Proprietari propri;
	Validator vali;
	ServicePropri srv{ propri, vali };

	srv.adauga(1, 55.5, "Andrei", "Apartament");
	srv.adauga(2, 60, "Maria", "Studio");
	srv.adauga(3, 160, "Ana", "Studio");
	
	srv.undo();
	assert(srv.size() == 2);

	srv.sterge(1);
	assert(srv.size() == 1);

	srv.undo();
	assert(srv.size() == 2);

	srv.modifica(2, 90.7, "Alexandru", "Garsoniera");
	vector<Proprietar> aux = srv.getAll();
	assert(aux[1].get_suprafata() == 90.7);

	srv.undo();
	aux = srv.getAll();
	assert(aux[1].get_suprafata() == 60);

	srv.adaugaLista(1);

	srv.goleste();

	srv.undo();
}

void test_repoFisiere() {
	ProprietariFisiere repo{ "teste.txt" };

	repo.store(Proprietar{ 1, 123, "ana", "Apartament" });
	repo.store(Proprietar{ 2, 123, "ana", "Apartament" });

	repo.deleteP(1);
	assert(repo.size() == 1);
	vector<Proprietar> aux = repo.getAll();
	assert(aux[0].get_apartament() == 2);
	repo.deleteP(2);
}

void test_repoChaos() {
	Proprietar p1(1, 55.5, "Andrei", "apartament");
	Proprietar p2(5, 60, "Maria", "studio");
	Proprietar p3(1, 70, "Andrei", "apartament");

	//test adauga
	RepoChaos propri{ 0 };
	propri.store(p1);
	assert(propri.size() == 1);
	try {
		propri.store(p3);
		assert(false);
	}
	catch (RepoException) {
		assert(true);
	}
	assert(propri.size() == 1);
	propri.store(p2);
	assert(propri.size() == 2);

	//test stergere
	try {
		propri.deleteP(2);
		assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}
	assert(propri.size() == 2);
	propri.deleteP(1);
	assert(propri.size() == 1);

	//test modifica
	Proprietar p4(5, 70, "Andrei", "apartament");
	propri.modify(p4);
	vector<Proprietar> aux = propri.getAll();
	assert(aux[0].get_suprafata() == 70);

	Proprietar p5(100, 70, "Andrei", "apartament");
	try {
		propri.modify(p5);
		assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}
}

void test_all() {
	test_domain();
	test_repo();
	test_srv();
	test_vali();
	test_lista();
	test_filtre();
	test_sort();
	test_listaNotificare();
	test_undo();
	test_repoFisiere();
	test_repoChaos();
}