#pragma once
#include <string>
#include <iostream>

using std::string;
using std::ostream;
class Proprietar {
	friend ostream& operator<<(ostream& out, const Proprietar& ex);
private:
	int apartament;
	double suprafata;
	string nume;
	string tip;

public:
	Proprietar() : apartament{ -1 }, suprafata{ 0 }, nume{ "" }, tip{ "" }{};

	Proprietar(int ap, double sup, const string n, const string t) : apartament{ ap }, suprafata{ sup }, nume{ n }, tip{ t }{
	}

	Proprietar(const Proprietar& ot) :apartament{ ot.apartament }, suprafata{ ot.suprafata }, nume{ ot.nume }, tip{ ot.tip }{
		//std::cout<<"Copy constructor used\n";
	}

	int get_apartament() const noexcept{
		return apartament;
	}

	double get_suprafata() const noexcept{
		return suprafata;
	}

	const string& get_nume() const {
		return nume;
	}

	const string& get_tip() const {
		return tip;
	}

	bool operator==(const Proprietar& ot) const noexcept {
		return apartament == ot.apartament;
	}

	bool operator!=(const Proprietar& ot) const noexcept {
		return apartament != ot.apartament;
	}


	void operator=(const Proprietar& ot) {
		apartament = ot.apartament;
		suprafata = ot.suprafata;
		nume = ot.nume;
		tip = ot.tip;
	}

	~Proprietar() = default;
};

ostream& operator<<(ostream& out, const Proprietar& ex);

