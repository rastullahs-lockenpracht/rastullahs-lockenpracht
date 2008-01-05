#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QToolButton>

/* Klasse von den Elementen angeben, die ich verwenden will */
class QLabel;
class QWidget;
class QGroupBox;
class QString;
class QListWidget;
class QStackedWidget;

//Hauptklasse -> Der Dialog
class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow();


private slots:	//Hier kommen die slots rein, die ich verwenden will
	void newFile();
	void open();
	bool save();
	bool saveAs();
	//void about();
	//void help();
	void openRecentFile();
	//void updateStatusBar();
	void changePage(QTreeWidgetItem*, int); //um zwischen Start, Option und response herumzuschalten
	void newOption();
	void newResponse();
	void remItem(); // löscht das selectete item
	
private:

	void createActions();
	void createMenus();
	//void readSettings();
	//void writeSettings();
	bool okToContinue();
	bool loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	
	QString strippedName(const QString &fullFileName);
	QStringList recentFiles;
	QString curFile;
	QString optOrRes;
	
	enum { MaxRecentFiles = 5};
	QAction *recentFileActions[MaxRecentFiles];
	QAction *seperatorAction;
	
	QMenu *fileMenu;
	QMenu *helpMenu;
	
	QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
	QAction *helpAction;
	QAction *aboutAction;
	
	QWidget *widget; // Hauptfenster
	
	QTreeWidget *str_structur; //Strukturliste
	
	QLabel *str_title; // Titel der Strukturliste
	QToolButton *str_addO;
	QToolButton *str_addR;
	QToolButton *str_rem;
	
	QLabel *cnt_title; // Titel des Contents
	QGroupBox *cnt_content; //Content
	QStringList str_headerlabels; //Die Spaltenueberschriften der Strukturliste
	
	QTreeWidgetItem *str_item; //die beiden Typen von Listeneinträgen
	QStackedWidget *modeWidget; //dieses tolle Widget enthällt die Widgets: Start, option, response!
	bool okToDelete(); // fragt nach, ob wirklich löschen
	int isOption(); // checkt, ob das gerade mackierte Element eine Option ist (true), oder eine Response (false)
	int IDcount;
	
protected:
	void closeEvent(QCloseEvent *event); // allgemiene Funktion, die aufgerufen wird wenn das Programm geschlossen wird, um ändeurngen zu speichern
};

#endif