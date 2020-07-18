#include "lab11.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qlabel.h>
#include "GUI.h"
#include "UI.h"
#include "teste.h"

int main(int argc, char *argv[])
{
	test_all();
	QApplication a(argc, argv);
	ProprietariFisiere propri{"proprietari.txt"};
	Validator vali{};
	ServicePropri srv{ propri, vali };
	UI ui{srv};
	//ui.run();
	GUI gui{ srv };
	gui.show();
	return a.exec();
}
