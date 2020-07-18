#pragma once

template <typename T>
struct Nod {
	T valoare;
	Nod* urm;
	Nod(const T& v, Nod* u) :valoare{ v },  urm { u }{
	}
};

template <typename T>
class Iterator;

template <typename T>
class Lista {
private:
	Nod<T>* head;

	/*Face o copie a listei inlantuite
	*/
	Nod<T>* copiaza(Nod<T>* current);

public:
	/*Elibereaza memoria de noduri
	*/
	void elibereazaNoduri() noexcept;

	Lista() noexcept : head{ nullptr }{} ;

	/*Constructor de copiere
	Rule of Three
	*/
	Lista(const Lista& ot);

	/*Operator de asigment
	Rule of three
	*/
	void operator=(const Lista& ot);
	/*Destructor
	Rule of three
	*/
	~Lista();

	/*Returneaza numarul de elemente
	*/
	size_t size() const noexcept;

	/*Adauga un element la sfarsit
	*/
	void push_back(const T& elem);

	/*Sterge elementul de pe pozitia poz
	*/
	void erase(size_t poz) noexcept;

	/*Modifica elementul Told cu Tnew
	*/
	void modify(const T& Told, const T& Tnew);

	/*Returneaza primul element
	*/
	T& first() const noexcept;

	/*Returneaza elementul de pe o pozitie
	*/
	T& operator[](size_t poz) noexcept;

	/*Returneaza un iterator la inceputul listei
	*/
	Iterator<T> begin() const noexcept;
	/*Retuneaza un iterator la sfarsitul listei
	*/
	Iterator<T> end() const noexcept;
};

template <typename T>
Nod<T>* Lista<T>::copiaza( Nod<T>* curent) {
	if (curent == nullptr)
		return nullptr;
	auto n = new Nod<T>{ curent->valoare, nullptr };
	n->urm = copiaza(curent->urm);
	return n;
}

template <typename T>
void Lista<T>::elibereazaNoduri() noexcept {
	auto curent = head;
	while (curent != nullptr) {
		auto aux = curent->urm;
		delete curent;
		curent = aux;
	}
	head = nullptr;
}

template <typename T>
Lista<T>::Lista(const Lista& ot) {
	head = copiaza(ot.head);
}

template <typename T>
void Lista<T>::operator=(const Lista& ot) {
	elibereazaNoduri();
	head = copiaza(ot.head);
}

template <typename T>
Lista<T>::~Lista() {
	elibereazaNoduri();
}

template <typename T>
size_t Lista<T>::size() const noexcept {
	auto curent = head;
	int lg = 0;
	while (curent != nullptr) {
		lg++;
		curent = curent->urm;
	}
	return lg;
}

template <typename T>
void Lista<T>::push_back(const T& elem) {
	auto curent = head;
	while (curent != nullptr && curent->urm != nullptr) {
		curent = curent->urm;
	}

	if (curent == nullptr)//lista e vida
		head = new Nod<T>{ elem, nullptr };
	else 
		curent->urm = new Nod<T>{ elem, nullptr };
}

template <typename T>
void Lista<T>::erase(size_t poz) noexcept {
	unsigned lg = 0;
	auto curent = head;
	Nod<T>* anterior=nullptr;
	while (lg < poz) {
		anterior = curent;
		curent = curent->urm;
		lg++;
	}
	if (anterior != nullptr) {
		anterior->urm = curent->urm;
		delete curent;
	}
	else {
		anterior = head;
		head = head->urm;
		delete anterior;
	}
	
}

template <typename T>
void Lista<T>::modify(const T& Told, const T& Tnew) {

	auto curent = head;
	Nod<T>* ant = nullptr;
	auto urm = curent->urm;
	while (curent->valoare != Told && urm != nullptr) {
		ant = curent;
		curent = urm;
		urm = urm->urm;
	}

	if (ant == nullptr) {//elementul e pe prima pozitie
		Nod<T>* newT = new Nod<T>{ Tnew, head->urm };
		delete head;
		head = newT;
	}
	else if (urm == nullptr) {
		Nod<T>* newT = new Nod<T>{ Tnew, nullptr };
		delete curent;
		ant->urm = newT;
	}
	else {
		Nod<T>* newT = new Nod<T>{ Tnew, curent->urm};
		delete curent;
		ant->urm = newT;
	}
}

template <typename T>
T& Lista<T>::first() const noexcept {
	return head->valoare;
}

template <typename T>
T& Lista<T>::operator[](size_t poz) noexcept{
	unsigned lg = 0;
	auto curent = head;
	while (lg < poz) {
		curent = curent->urm;
		lg++;
	}
	return curent->valoare;
}

template <typename T>
class Iterator {
	Nod<T>* curent;
public:
	Iterator(Nod<T>* c) noexcept :curent{ c } {
	}

	//pt range for
	bool operator!=(const Iterator& ot) noexcept ;

	//trece la urmatorul for din iteratie
	void operator++() noexcept ;

	//returneaza elementul curent
	T& operator*() noexcept ;
};

template <typename T>
Iterator<T> Lista<T>::begin() const noexcept {
	return Iterator<T>{ head };
}

template <typename T>
Iterator<T> Lista<T>::end() const noexcept {
	return Iterator<T>{ nullptr };
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator& ot) noexcept {
	return curent != ot.curent;
}

template <typename T>
void Iterator<T>::operator++() noexcept  {
	curent =curent->urm;
}

template <typename T>
T& Iterator<T>::operator*() noexcept  {
	return curent->valoare;
}