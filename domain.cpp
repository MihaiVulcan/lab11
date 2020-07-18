#include "domain.h"

ostream& operator<<(ostream& out, const Proprietar& p) {
	out << p.apartament << " " << p.nume << " " << p.suprafata << " " << p.tip;
	return out;
}