#include "GUINot.h"

void GUINotificare::init() {
	auto lyMain = new QHBoxLayout;
	lyMain->addWidget(lst);
	setLayout(lyMain);

	QGridLayout* lyRight = new QGridLayout;

	lyRight->addWidget(new QLabel("Numar aparatament: "), 1, 0);
	lyRight->addWidget(lineAdd, 1, 1);
	lyRight->addWidget(btAdd, 1, 2);

	lyRight->addWidget(new QLabel("Nume Fisier: "), 2, 0);
	lyRight->addWidget(lineExp, 2, 1);
	lyRight->addWidget(btExp, 2, 2);

	lyRight->addWidget(new QLabel("Adauga aparatamente: "), 3, 0);
	lyRight->addWidget(linePop, 3, 1);
	lyRight->addWidget(btPop, 3, 2);

	lyRight->addWidget(btDel, 4, 0);

	lyMain->addLayout(lyRight);
}

void GUINotificare::loadData() {
	lst->clear();
	auto propri = srv.lista.getAll();
	for (const auto& p : propri) {
			auto item = new QListWidgetItem(QString::number(p.get_apartament()) + " " + QString::fromStdString(p.get_nume()) + " " + QString::number(p.get_suprafata()) + " " + QString::fromStdString(p.get_tip()));
			item->setData(Qt::UserRole, QString::number(p.get_apartament()));
			lst->addItem(item);
	}
}

void GUINotificare::showError(string ex) {
	QMessageBox msgbox;
	msgbox.setWindowTitle("Eroare");
	msgbox.setText(QString::fromStdString(ex));
	msgbox.exec();
}

void GUINotificare::initconnect() {
	QObject::connect(btAdd, &QPushButton::clicked, [&]() {
		int nr = lineAdd->text().toInt();
		if (nr == 0) {
			showError("Intoduceti un numar");
		}
		else {
			try {
				srv.adaugaLista(nr);
			}
			catch(RepoException(ex)){
				showError(ex.toString());
			}
			loadData();
		}
		lineAdd->clear();
		});

	QObject::connect(btExp, &QPushButton::clicked, [&]() {
		string filename = lineExp->text().toStdString();
		if (filename != "") {
			srv.exporta(filename);
		}
		else {
			showError("Introduceti un nume de fisier");
		}
		lineExp->clear();
		});

	QObject::connect(btPop, &QPushButton::clicked, [&]() {
		int nr = linePop->text().toInt();
		if (nr !=  0) {
			try {
				srv.aleator(nr);
			}
			catch (RepoException(ex)) {
				showError(ex.toString());
			}
		}
		else {
			showError("Introduceti un numar natural");
		}
		linePop->clear();
		loadData();
		});

	QObject::connect(btDel, &QPushButton::clicked, [&]() {
		srv.goleste();
		loadData();
		});
}

void GuiCosFiguri::paintEvent(QPaintEvent* ev){
	QPainter p{ this };
	int lungime = srv.listSize();
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> const dist(0, 200);
	for (int i = 0; i < lungime; i++) {
		int poz = dist(mt);
		p.drawRect(poz, poz, poz+20, poz+20);
	}
}