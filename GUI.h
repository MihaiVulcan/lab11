#pragma once
#include "lab11.h"
#include "service.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qcombobox.h>
#include "GUINot.h"

class GUI : public QWidget, public Observer {
public:
	GUI(ServicePropri& srv) : srv{ srv } {
		initGUI();
		load_data();
		initConect();
		srv.lista.addObs(this);
	}

	void update() override{
		load_cos();
	}

	~GUI() {
		srv.lista.removeObs(this);
	}

private:
	ServicePropri& srv;
	QListWidget* lst = new QListWidget;
	QLineEdit* lineAp = new QLineEdit;
	QLineEdit* lineName = new QLineEdit;
	QLineEdit* lineSup = new QLineEdit;
	QLineEdit* lineTip = new QLineEdit;

	QPushButton* btAdd = new QPushButton{ "&Adauga" };
	QPushButton* btDel = new QPushButton{ "&Sterge" };
	QPushButton* btMod = new QPushButton{ "&Modifica" };
	QPushButton* btUndo = new QPushButton{ "&Undo" };
	QPushButton* btSrc = new QPushButton{ "&Cauta" };
	QPushButton* btSupTot = new QPushButton{ "&Suprafata Totala" };
	QPushButton* btRap = new QPushButton{ "Raport" };
	QPushButton* btLst = new QPushButton{ "Lista Notificare" };
	QPushButton* btLstFig = new QPushButton{ "Lista Notificare Figuri" };

	QComboBox* cboxSort = new QComboBox;
	QComboBox* cboxFiltru = new QComboBox;

	QSlider* slPr = new QSlider{Qt::Horizontal };
	QLabel* lbSup = new QLabel();

	QHBoxLayout* lyMain = new QHBoxLayout{};
	QVBoxLayout* lyBtdy = new QVBoxLayout{};

	QLabel* afis = new QLabel;
	QLineEdit* lineAdd = new QLineEdit;
	QPushButton* btAddCos = new QPushButton{ "Adauga" };

	QLineEdit* lineExp = new QLineEdit;
	QPushButton* btExp = new QPushButton{ "Export" };

	QLineEdit* linePop = new QLineEdit;
	QPushButton* btPop = new QPushButton{ "Populeaza lista" };

	QPushButton* btDelCos = new QPushButton{ "Goleste lista" };

	void showError(string ex);

	void initConect();

	void load_data();

	void load_cos();

	void clear_data();

	void initGUI();

	void adaugaButoane(vector<Proprietar> prop);

	void stergeButoane();
};