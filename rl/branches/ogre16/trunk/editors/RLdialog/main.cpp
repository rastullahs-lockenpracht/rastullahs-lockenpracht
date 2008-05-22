    /****************************************************************
    **
    ** Franz WIlding 2007
	** RL Dialog Editor
    **
    ****************************************************************/

    #include <QApplication>
    #include "rldialog.h"

    int main(int argc, char *argv[])
    {
        QApplication app(argc, argv);

		MainWindow mainWin;
		mainWin.resize(800, 600);
		mainWin.show();

        return app.exec();
    }

