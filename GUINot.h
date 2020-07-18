#pragma once
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
#include <qpainter.h>

class GUINotificare : public QWidget, public Observer {
public:
	GUINotificare(ServicePropri& srv) : srv{ srv } {
		init();
		loadData();
		initconnect();
		srv.lista.addObs(this);
	}

	void update() override {
		loadData();
	}

	void closeEvent(QCloseEvent* event) override {
		srv.lista.removeObs(this);
	}

	~GUINotificare() {
		srv.lista.removeObs(this);
	}

private:
	ServicePropri& srv;
	QListWidget* lst = new QListWidget;
	QLabel* afis = new QLabel;
	QLineEdit* lineAdd = new QLineEdit;
	QPushButton* btAdd = new QPushButton{ "Adauga" };

	QLineEdit* lineExp = new QLineEdit;
	QPushButton* btExp = new QPushButton{ "Export" };

	QLineEdit* linePop = new QLineEdit;
	QPushButton* btPop = new QPushButton{ "Populeaza lista" };

	QPushButton* btDel = new QPushButton{ "Goleste lista" };

	void init();
	void loadData();
	void initconnect();
	void showError(string ex);
};


class GuiCosFiguri: public QWidget, public Observer {
public:
	GuiCosFiguri(ServicePropri& srv) : srv{ srv } {
		srv.lista.addObs(this);
	}
	~GuiCosFiguri() {
		srv.lista.removeObs(this);
	}
	void closeEvent(QCloseEvent* event) override {
		srv.lista.removeObs(this);
	}

private:
	ServicePropri& srv;
	void paintEvent(QPaintEvent* ev) override;
	void update() override {
		repaint();
	}

};