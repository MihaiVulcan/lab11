#include "GUI.h"

void GUI::showError(string ex) {
	QMessageBox msgbox;
	msgbox.setWindowTitle("Eroare");
	msgbox.setText(QString::fromStdString(ex));
	msgbox.exec();
}

void GUI::initConect() {
	QObject::connect(btAdd, &QPushButton::clicked, [&]() {
		int ap = lineAp->text().toInt();
		string nume = lineName->text().toStdString();
		double sup = lineSup->text().toDouble();
		string tip = lineTip->text().toStdString();
		try {
			srv.adauga(ap, sup, nume, tip);
			load_data();
			clear_data();
		}
		catch (ValidateException(ex)) {
			showError(ex.ToString());
		}
		catch (RepoException(ex)) {
			showError(ex.toString());
		}
		});

	QObject::connect(btDel, &QPushButton::clicked, [&]() {
		auto rez = lst->selectedItems();
		if (rez.isEmpty())
			showError("Selectati un element");
		else {
			auto selItem = rez.at(0);
			int ap = selItem->data(Qt::UserRole).toInt();
			try {
				srv.sterge(ap);
				load_data();
				clear_data();
			}
			catch (RepoException(ex)) {
				showError(ex.toString());
			}
		}
		});

	QObject::connect(btMod, &QPushButton::clicked, [&]() {
		int ap = lineAp->text().toInt();
		string nume = lineName->text().toStdString();
		double sup = lineSup->text().toDouble();
		string tip = lineTip->text().toStdString();
		try {
			srv.modifica(ap, sup, nume, tip);
			load_data();
			clear_data();
		}
		catch (ValidateException(ex)) {
			showError(ex.ToString());
		}
		catch (RepoException(ex)) {
			showError(ex.toString());
		}
		});

	QObject::connect(btUndo, &QPushButton::clicked, [&]() {
		try {
			srv.undo();
			load_data();
		}
		catch (RepoException(ex)) {
			showError(ex.toString());
		}
		});

	QObject::connect(cboxSort, &QComboBox::currentTextChanged, [&]() {
		load_data();
		});

	QObject::connect(btSrc, &QPushButton::clicked, [&]() {
		int ap = lineAp->text().toInt();
		try {
			Proprietar p = srv.cauta(ap);
			QMessageBox msg;
			msg.setWindowTitle("Apartament cautat");
			msg.setText(QString::number(p.get_apartament()) + " " + QString::fromStdString(p.get_nume()) + " " + QString::number(p.get_suprafata()) + " " + QString::fromStdString(p.get_tip()));
			msg.exec();
			clear_data();
		}
		catch (RepoException(ex)) {
			showError(ex.toString());
		}
		});

	QObject::connect(btSupTot, &QPushButton::clicked, [&]() {
		double sup = srv.supTotal();
		QMessageBox msg;
		msg.setWindowTitle("Suprafata totala");
		msg.setText("Suprafata totala este: " + QString::number(sup));
		msg.exec();
		});

	QObject::connect(btRap, &QPushButton::clicked, [&]() {
		vector<EntityCountDTO> rap = srv.raport();
		string raport;
		for (auto p : rap) {
			raport += p.nume + " " + p.tip + " " + to_string(p.count) + "\n";
		}
		QMessageBox msg;
		msg.setWindowTitle("Raport");
		msg.setText("Raport: \n" + QString::fromStdString(raport));
		msg.exec();
		});

	QObject::connect(btLst, &QPushButton::clicked, [&]() {
		auto not = new GUINotificare{srv};
		not->show(); 
		});

	QObject::connect(slPr, &QSlider::valueChanged, [&]() {
		lbSup->setText(QString::number(slPr->value()));
		load_data();
		});

	QObject::connect(cboxFiltru, &QComboBox::currentTextChanged, [&]() {
		load_data();
		});

	QObject::connect(btAddCos, &QPushButton::clicked, [&]() {
		int nr = lineAdd->text().toInt();
		if (nr == 0) {
			showError("Intoduceti un numar");
		}
		else {
			try {
				srv.adaugaLista(nr);
			}
			catch (RepoException(ex)) {
				showError(ex.toString());
			}
			load_cos();
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
		if (nr != 0) {
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
		load_cos();
		});

	QObject::connect(btDelCos, &QPushButton::clicked, [&]() {
		srv.goleste();
		load_cos();
		});

	QObject::connect(btLstFig, &QPushButton::clicked, [&]() {
		auto not = new GuiCosFiguri{ srv };
		not->show();
		});

	QObject::connect(lst->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
		if (lst->selectionModel()->selectedIndexes().isEmpty())
			return;
		auto sel = lst->selectedItems().at(0);
		int id = sel->data(Qt::UserRole).toInt();
		lineAp->setText(QString::number(id));
		});
}

void GUI::load_cos() {
	int nr = srv.listSize();
	QString a = QString::number(nr);
	afis->setText(a);
}

void GUI::load_data() {
	stergeButoane();
	lst->clear();
	vector<Proprietar> propri;
	if (cboxSort->currentIndex() == 0)	propri = srv.getAll();
	else if (cboxSort->currentIndex() == 1) propri = srv.sortNume();
	else if (cboxSort->currentIndex() == 2) propri = srv.sortSp();
	else if (cboxSort->currentIndex() == 3) propri = srv.sortSpTip();
	adaugaButoane(propri);

	int value = slPr->value();
	string tip = cboxFiltru->currentText().toStdString();
	for (const auto& p : propri)
		if (p.get_suprafata() > value && (tip == "none" || tip == p.get_tip())) {
			auto item = new QListWidgetItem(QString::number(p.get_apartament()) + " " + QString::fromStdString(p.get_nume()) + " " + QString::number(p.get_suprafata()) + " " + QString::fromStdString(p.get_tip()));
			item->setData(Qt::UserRole, QString::number(p.get_apartament()));
			lst->addItem(item);
		}
}

void GUI::clear_data() {
	lineAp->clear();
	lineName->clear();
	lineSup->clear();
	lineTip->clear();
}

void GUI::adaugaButoane(vector<Proprietar> prop) {
	for (auto& p : prop) {
		auto btn = new QPushButton{ QString::number(p.get_apartament()) };
		lyBtdy->addWidget(btn);
		QObject::connect(btn, &QPushButton::clicked, [this, btn, p]() {
			srv.sterge(p.get_apartament());
			//delete btn;
			load_data();
			});
	}
	lyMain->addLayout(lyBtdy);
}

void GUI::stergeButoane() {
	QLayoutItem* child;
	while (lyBtdy->count() != 0) {
		child = lyBtdy->takeAt(0);
		if (child->widget() != 0) {
			delete child->widget();
		}
		delete child;
	}
}

void GUI::initGUI() {
	setLayout(lyMain);

	lyMain->addWidget(lst);

	auto lyRight = new QVBoxLayout;

	auto lyForm = new QFormLayout;
	lyForm->addRow("Apartament", lineAp);
	lyForm->addRow("Nume", lineName);
	lyForm->addRow("Suprafata", lineSup);
	lyForm->addRow("Tip", lineTip);
	lyRight->addLayout(lyForm);

	auto lyBtn1 = new QHBoxLayout;
	lyBtn1->addWidget(btAdd);
	lyBtn1->addWidget(btDel);
	lyBtn1->addWidget(btMod);
	lyBtn1->addWidget(btUndo);
	lyRight->addLayout(lyBtn1);

	auto lyBtn2 = new QHBoxLayout;
	lyBtn2->addWidget(btSrc);
	lyBtn2->addWidget(btSupTot);
	lyBtn2->addWidget(btRap);
	lyRight->addLayout(lyBtn2);

	auto lyBtn3 = new QGridLayout;
	cboxSort->addItem("none");
	cboxSort->addItem("sortare nume");
	cboxSort->addItem("sortare suprafata");
	cboxSort->addItem("sortare suprafata si tip");
	lyBtn3->addWidget(new QLabel{ "Optiune soratare" }, 0, 0);
	lyBtn3->addWidget(cboxSort, 0, 1);
	lyRight->addLayout(lyBtn3);

	auto lyBtn4 = new QGridLayout;
	lyBtn4->addWidget(new QLabel{ "Filtru Suprafata" }, 0, 0);

	slPr->setMinimum(0);
	slPr->setMaximum(1000);
	slPr->setTickInterval(100);

	lyBtn4->addWidget(lbSup, 0, 1);
	lyBtn4->addWidget(slPr,0 ,2);
	lyRight->addLayout(lyBtn4);

	auto lyBtn5 = new QGridLayout;
	cboxFiltru->addItem("none");
	cboxFiltru->addItem("Apartament");
	cboxFiltru->addItem("Garsoniera");
	cboxFiltru->addItem("Penthouse");
	cboxFiltru->addItem("Studio");
	lyBtn5->addWidget(new QLabel{ "Optiune filtrare tip" }, 0, 0);
	lyBtn5->addWidget(cboxFiltru, 0, 1);
	lyRight->addLayout(lyBtn5);

	QGridLayout* lyCos = new QGridLayout;
	setLayout(lyCos);

	lyCos->addWidget(new QLabel("Numar apartamente in lista: "), 0, 0);
	lyCos->addWidget(afis, 0, 1);

	lyCos->addWidget(new QLabel("Numar aparatament: "), 1, 0);
	lyCos->addWidget(lineAdd, 1, 1);
	lyCos->addWidget(btAddCos, 1, 2);

	lyCos->addWidget(new QLabel("Nume Fisier: "), 2, 0);
	lyCos->addWidget(lineExp, 2, 1);
	lyCos->addWidget(btExp, 2, 2);

	lyCos->addWidget(new QLabel("Adauga aparatamente: "), 3, 0);
	lyCos->addWidget(linePop, 3, 1);
	lyCos->addWidget(btPop, 3, 2);

	lyCos->addWidget(btDelCos, 4, 0);
	lyCos->addWidget(btLst, 4, 1);
	lyCos->addWidget(btLstFig, 4, 2);
	lyRight->addLayout(lyCos);

	lyMain->addLayout(lyRight);
}