#include "service.h"

ListaNotificare::ListaNotificare() {}

void ServicePropri::adauga(int ap, double s, const string& n, const string& t) {
	Proprietar p{ ap, s, n, t };
	vali.validate(p);
	propri.store(p);
	undoActions.push_back(std::make_unique<UndoAdauga>(propri, p));
}

void ServicePropri::sterge(int ap) {
	const Proprietar& p{ ap, 0, "", "" };
	undoActions.push_back(std::make_unique<UndoSterge>(propri, propri.search(p)));
	propri.deleteP(ap);
}

void ServicePropri::modifica(int ap, double s, const string& n, const string& t) {
	Proprietar p{ ap, s, n, t };
	Proprietar aux = propri.search(p);
	vali.validate(p);
	propri.modify(p);
	undoActions.push_back(std::make_unique<UndoModifica>(propri, aux));
}

const Proprietar& ServicePropri::cauta(int ap) {
	const Proprietar& p{ ap, 0, "", "" };
	return propri.search(p);
}

const vector<Proprietar> ServicePropri::filtreaza(function<bool(const Proprietar&)> fct) {
	vector<Proprietar> rez;
	for (const auto& elem : propri.getAll())
		if (fct(elem))
			rez.push_back(elem);
	return rez;
}

const vector<Proprietar> ServicePropri::filtruTip(string tip) {

	vector<Proprietar> rez(propri.size());
	vector<Proprietar> aux = propri.getAll();
	auto it = copy_if(aux.begin(), aux.end(), rez.begin(), [tip](const Proprietar& p) { return p.get_tip() == tip; });
	rez.resize(distance(rez.begin(), it));
	return rez;

	/*return filtreaza([tip](const Proprietar& p)  {
		return p.get_tip() == tip;
		});*/
}

const vector<Proprietar> ServicePropri::filtruSp(int min, int max) {

	vector<Proprietar> rez(propri.size());
	vector<Proprietar> aux = propri.getAll();
	auto it = copy_if(aux.begin(), aux.end(), rez.begin(), [min, max](const Proprietar& p) { return p.get_suprafata() >= min && p.get_suprafata() <= max; });
	rez.resize(distance(rez.begin(), it));
	return rez;

	/*return filtreaza([min, max](const Proprietar& p) noexcept {
		return p.get_suprafata() >= min && p.get_suprafata() <= max;
		});*/
}

const vector<Proprietar> ServicePropri::sort(function<bool(const Proprietar& p1, const Proprietar& p2)> cmp) {
	vector<Proprietar> v;
	vector<Proprietar> p = propri.getAll() ;//facem copie
	bool sch = true;
	while (sch) {
		sch = false;
		for(size_t i=0; i<p.size()-1; i++ )
			if (cmp(p[i], p[i + 1])) {
				Proprietar aux = p[i];
				p[i] = p[i + 1];
				p[i + 1] = aux;
				sch = true;
			}
	}
	return p;
}

const vector<Proprietar> ServicePropri::sortNume() {
	vector<Proprietar> rez = propri.getAll();
	std::sort(rez.begin(), rez.end(), [](const Proprietar& p1, const Proprietar& p2) {return p1.get_nume() < p2.get_nume(); });
	return rez;
	/*return sort([](const Proprietar& p1, const Proprietar& p2)  {
		return p1.get_nume() > p2.get_nume();
		});*/
}

const vector<Proprietar> ServicePropri::sortSp() {
	vector<Proprietar> rez = propri.getAll();
	std::sort(rez.begin(), rez.end(), [](const Proprietar& p1, const Proprietar& p2) noexcept {
		return p1.get_suprafata() < p2.get_suprafata(); });
	return rez;
	/*return sort([](const Proprietar& p1, const Proprietar& p2) noexcept {
		return p1.get_suprafata() > p2.get_suprafata();
		});*/
}

const vector<Proprietar> ServicePropri::sortSpTip() {
	vector<Proprietar> rez = propri.getAll();
	std::sort(rez.begin(), rez.end(), [](const Proprietar& p1, const Proprietar& p2) {
		if (p1.get_tip() == p2.get_tip())
			return p1.get_suprafata() < p2.get_suprafata();
		return p1.get_tip() < p2.get_tip(); });
	return rez;
	/*return sort([](const Proprietar& p1, const Proprietar& p2)  {
		if (p1.get_tip() == p2.get_tip())
			return p1.get_suprafata() > p2.get_suprafata();
		return p1.get_tip() > p2.get_tip();
		});*/
}

EntityCountDTO::EntityCountDTO(string n, string t, int c) : nume{ n }, tip{ t }, count{ c }{
}

vector<EntityCountDTO> ServicePropri::raport() {

	EntityCountDTO p{ "ana", "ana", 1 };

	std::map<string, EntityCountDTO> dic;
	std::pair<std::map<string, EntityCountDTO>::iterator, bool> ret;
	for (const auto& e : propri.getAll()){
		string n = "ana";
		string tip = e.get_tip();
		EntityCountDTO en{ n, tip, 1 };
		ret = dic.emplace(tip, en);
		if (ret.second == false) {
			dic[tip].count++;
		}

	}
	vector<EntityCountDTO> rez;

	for (const auto& e : propri.getAll()) {
		EntityCountDTO en{ e.get_nume(), e.get_tip(), dic[e.get_tip()].count };
		rez.push_back(en);
	}
	return rez;
}

void ServicePropri::adaugaLista(int ap){
	auto p = cauta(ap);
	lista.adaugaLista(p);
	undoActions.push_back(std::make_unique<UndoAdaugaLst>(lista, p));
}

void ServicePropri::goleste() {
	lista.goleste();
	auto it = undoActions.begin();
	while(it!=undoActions.end()) {
		if ((*it)->identity() == "UndoAdaugaLst") {
			undoActions.erase(it);
			it = undoActions.begin();
		}
		else
			it++;
	}
}

int ServicePropri::listSize() {
	return lista.listSize();
}

void ServicePropri::exporta(string filename) {
	lista.exporta(filename);
}

void ServicePropri::aleator(int nr) {
	if (nr > size())
		throw RepoException("Nu sunt atatea aparatamente in lista");

	vector<Proprietar> aux = propri.getAll();
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> const dist(0, propri.size() - 1);
	while (nr > 0) {
		Proprietar p = aux[dist(mt)];
		try {
			lista.adaugaLista(p);
			nr--;
		}
		catch (RepoException) {
		}
	}
}

double ServicePropri::supTotal() {
	vector<Proprietar> v;
	vector<Proprietar> aux{ propri.getAll() };
	double rez = std::accumulate(aux.begin(), aux.end(), 0.0, [](double x, const Proprietar& p) {return x+p.get_suprafata(); });
	return rez;
}

void ServicePropri::undo() {
	if (undoActions.empty())
		throw RepoException("Nu mai exista operatii!");
	undoActions.back()->doUndo();
	undoActions.pop_back();
}