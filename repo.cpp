#include "repo.h"

void ListaNotificare::adaugaLista(Proprietar p) {
	auto it = find(lista.begin(), lista.end(), p);
	if (it == lista.end()) {
		lista.push_back(p);
	}
	else
		throw RepoException("Apartamentul " + to_string(p.get_apartament()) + "a fost adaugat deja la lista");
	notify();
}

void ListaNotificare::stergeLista(Proprietar p) {
	auto it = find(lista.begin(), lista.end(), p);
	lista.erase(it);
	notify();
}

void ListaNotificare::goleste() {
	while (lista.empty());
	lista.resize(0);
	notify();
}

int ListaNotificare::listSize() {
	return lista.size();
}

void ListaNotificare::exporta(string filename) {
	filename += ".csv";
	std::ofstream fout(filename, std::ofstream::out | std::ofstream::trunc);
	for (const auto& e : lista) {
		fout << e.get_apartament() << "," << e.get_nume() << "," << e.get_suprafata() << "," << e.get_tip() << endl;
	}
	fout.close();
}

bool Proprietari::exista(const Proprietar& p) const noexcept {
	/*for (const Proprietar& prop : propri) {
		if (prop == p)
			return true;
	}
	return false;*/
	auto it = propri.find(p.get_apartament());
	if (it == propri.end())
		return false;
	return true;
}

void Proprietari::store(const Proprietar& p) {
	if (!exista(p))
		propri.emplace(p.get_apartament(), p);
	else
		throw RepoException("Exista deja un proprietar la apartamentul " + std::to_string(p.get_apartament()));
}

void Proprietari::deleteP(int ap) {
	/*bool gasit = false;
	vector<Proprietar>::iterator poz = propri.begin();
	for(auto& e:propri){
		if (e.get_apartament() == ap){
			propri.erase(poz);
			gasit = true;
			break;
		}
		poz++;
	}
	if(!gasit)
		throw  RepoException("Apartamentul " + std::to_string(ap) + " nu exista ");*/

	auto it = propri.find(ap);
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ap) + " nu exista ");
	propri.erase(it);
}


void Proprietari::modify(const Proprietar& p) {
	/*int poz = 0;
	bool gasit = false;
	for (auto& aux : propri)
	{
		if (aux == p) {
			aux = p;
			gasit = true;
			break;
		}
		poz++;
	}

	if (!gasit)
		throw  RepoException("Apartamentul " + std::to_string(p.get_apartament()) + " nu exista ");*/
	std::map<int , Proprietar>::iterator it = propri.find(p.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(p.get_apartament()) + " nu exista ");
	it->second = p;
}

const Proprietar& Proprietari::search(const Proprietar& ot) {
	/*for (const Proprietar& p : propri)
		if (p == ot)
			return p;
	throw RepoException("Apartamentul " + std::to_string(ot.get_apartament()) + " nu exista ");*/
	std::map<int, Proprietar>::iterator it = propri.find(ot.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ot.get_apartament()) + " nu exista ");
	return it->second;
}

int Proprietari::size()  {
	return propri.size();
}

vector<Proprietar> Proprietari::getAll()  {
	vector<Proprietar> v;
	std::transform(propri.begin(), propri.end(), std::back_inserter(v), []( pair<int, Proprietar> p) {return p.second;});
	return v;
}

void RepoChaos::crapa() const {
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> const dist(0, 100);
	double p = dist(mt);
	p /= 100;
	if (p < probabilitate)
		throw RepoException("eroare");
}

bool RepoChaos::exista(const Proprietar& p) const noexcept{
	auto it = propri.find(p.get_apartament());
	if (it == propri.end())
		return false;
	return true;
}

void RepoChaos::store(const Proprietar& p) {
	crapa();
	if (!exista(p))
		propri.emplace(p.get_apartament(), p);
	else
		throw RepoException("Exista deja un proprietar la apartamentul " + std::to_string(p.get_apartament()));
}

ostream& operator<<(ostream& out, const RepoException& ex) {
	out << ex.msg;
	return out;
}

void RepoChaos::deleteP(int ap) {
	crapa();
	auto it = propri.find(ap);
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ap) + " nu exista ");
	propri.erase(it);
}


void RepoChaos::modify(const Proprietar& p) {
	crapa();
	std::map<int, Proprietar>::iterator it = propri.find(p.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(p.get_apartament()) + " nu exista ");
	it->second = p;
}

const Proprietar& RepoChaos::search(const Proprietar& ot) {
	crapa();
	std::map<int, Proprietar>::iterator it = propri.find(ot.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ot.get_apartament()) + " nu exista ");
	return it->second;
}

int RepoChaos::size() {
	crapa();
	return propri.size();
}

vector<Proprietar> RepoChaos::getAll() {
	crapa();
	vector<Proprietar> v;
	std::transform(propri.begin(), propri.end(), std::back_inserter(v), [](pair<int, Proprietar> p) {return p.second; });
	return v;
}


void ProprietariFisiere::read_from_file() {
	std::ifstream fin(filename);

	map<int, Proprietar> aux;
	int ap;
	double sup;
	string nume, tip;
	while (fin >> ap >> nume >> sup >> tip) {
		try {
			Proprietar p{ ap, sup, nume, tip };
			if(!exista(p))
				propri.emplace(p.get_apartament(), p);
		}
		catch (RepoException) {
		}
	}

	fin.close();
}

void ProprietariFisiere::write_to_file() {
	std::ofstream fout(filename);
	for (auto p : getAll()) {
		fout << p.get_apartament() << " " << p.get_nume() << " " << p.get_suprafata() << " " << p.get_tip() << std::endl;
	}

	fout.close();
}

bool ProprietariFisiere::exista(const Proprietar& p) const noexcept {
	auto it = propri.find(p.get_apartament());
	if (it == propri.end())
		return false;
	return true;
}

void ProprietariFisiere::store(const Proprietar& p) {
	read_from_file();
	if (!exista(p))
		propri.emplace(p.get_apartament(), p);
	else
		throw RepoException("Exista deja un proprietar la apartamentul " + std::to_string(p.get_apartament()));
	write_to_file();
}

void ProprietariFisiere::deleteP(int ap) {
	read_from_file();
	auto it = propri.find(ap);
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ap) + " nu exista ");
	propri.erase(it);
	write_to_file();
}


void ProprietariFisiere::modify(const Proprietar& p) {
	read_from_file();
	std::map<int, Proprietar>::iterator it = propri.find(p.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(p.get_apartament()) + " nu exista ");
	it->second = p;
	write_to_file();
}

const Proprietar& ProprietariFisiere::search(const Proprietar& ot) {
	read_from_file();
	std::map<int, Proprietar>::iterator it = propri.find(ot.get_apartament());
	if (it == propri.end())
		throw  RepoException("Apartamentul " + std::to_string(ot.get_apartament()) + " nu exista ");
	return it->second;
}

int ProprietariFisiere::size() {
	read_from_file();
	return propri.size();
}

vector<Proprietar> ProprietariFisiere::getAll() {
	read_from_file();
	vector<Proprietar> v;
	std::transform(propri.begin(), propri.end(), std::back_inserter(v), [](pair<int, Proprietar> p) {return p.second; });
	return v;
}
