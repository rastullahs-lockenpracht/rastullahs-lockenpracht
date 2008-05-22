#include <QtGui>

#include "rldialog.h"
#include "helper.h"

// Hauptklasse von dem Feld... NAME Šndern
MainWindow::MainWindow()
{

	widget = new QWidget; //heir erstelle ich das Main Widget, in dem alles passiert
	setCentralWidget(widget);

/* ######################### ERSTELLUNG - CREATE ########################*/

	createActions();		// Actions erstellen
	createMenus();			// MenŸs erstellen
	
	//readSettings();			// die gespeicherten Einstellungen laden
	setWindowIcon(QIcon(":/images/icon.png"));
	IDcount = 1;  //die erste ID beginnt mit 1, dachte ich mir ;)
	
	
	
	
/* ##### Struktur erstellen */
	str_title = new QLabel(tr("Dialogstruktur")); //Titel des Strukturbaumes erstellen	

	str_structur = new QTreeWidget;	 //Strukturbaum erstellen
	str_structur -> setFixedWidth(250);
	str_structur -> setColumnCount(3);
	str_headerlabels << tr("Type") << tr("ID") << tr("Name");  //hier wird eine QStringlist erstellt, die alle Listenueberschriften erhaellt
	str_structur->setHeaderLabels(str_headerlabels);
	
	str_structur->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	str_structur->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	str_structur->header()->setResizeMode(2, QHeaderView::Stretch);
	//str_structur -> setHeaderLabels(str_headerlabels);
	
	str_addO = new QToolButton;
		str_addO->setText(tr("neue Option"));
	str_addR = new QToolButton;
		str_addR->setText(tr("neue Antwort"));

	str_rem = new QToolButton;
		str_rem->setText("-");
		str_rem->setFixedWidth(25);
	


/* ##### Bereich rechts erstellen: entweder helper oder editor */
modeWidget = new QStackedWidget;
	modeWidget->addWidget(new startMode);
	modeWidget->addWidget(new optionMode);
	modeWidget->addWidget(new responseMode);
	
	
//connections eingeben
	connect(str_structur, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(changePage(QTreeWidgetItem*, int))); 
	connect(str_addO, SIGNAL(clicked()), this, SLOT(newOption())); 
	connect(str_addR, SIGNAL(clicked()), this, SLOT(newResponse())); 
	connect(str_rem, SIGNAL(clicked()), this, SLOT(remItem())); 

	
//Layout bestimmen

	
	QHBoxLayout *str_buttons = new QHBoxLayout;
		str_buttons->addWidget(str_addO);
		str_buttons->addWidget(str_addR);
		str_buttons->addWidget(str_rem);
	
	QVBoxLayout *structLayout = new QVBoxLayout; //Strukturlayout -> links
		structLayout -> addWidget(str_title);
		structLayout -> addWidget(str_structur);
		structLayout->addLayout(str_buttons);
	

	QHBoxLayout *mainLayout = new QHBoxLayout; //hauptlayout
		mainLayout ->addLayout(structLayout);
		mainLayout ->addWidget(modeWidget, 0);
		widget->setLayout(mainLayout); //das Layout "installieren"
		
	setWindowTitle(tr("RL Dialog Editor"));
}

/* um zwischend en anzuzeigenden Modis zu schalten */
void MainWindow::changePage(QTreeWidgetItem*, int)
{
	if(isOption()) 
		{modeWidget->setCurrentIndex(1);}
		
	if(isOption()==2)
		{modeWidget->setCurrentIndex(2);}
}

// erstellt eine neue Option an der richtigen position, nd ruft den Options helper auf
void MainWindow::newOption()
{
	if(str_structur->currentItem())
	{ str_item = new QTreeWidgetItem(str_structur, str_structur->currentItem()); }
	else
	{ str_item = new QTreeWidgetItem(str_structur); }
	
		str_item->setText(0, "O");
		str_item->setText(1, QString::number(IDcount));
		IDcount++;		
		str_item->setSelected(true);
		str_structur->setCurrentItem(str_item);
	
	modeWidget->setCurrentIndex(1);

}

// das gleiche, nur mit response
void MainWindow::newResponse()
{
	
	if(str_structur->currentItem())
	{ str_item = new QTreeWidgetItem(str_structur, str_structur->currentItem()); }
	else
	{ str_item = new QTreeWidgetItem(str_structur); }
	
		str_item->setText(0, "R");
		str_item->setText(1, QString::number(IDcount));
		IDcount++;
		str_item->setSelected(true);
		str_structur->setCurrentItem(str_item);

	modeWidget->setCurrentIndex(2);
}


// wird aufgerufen, wenn ein Listenelement entfernt wird, um sicherzugehen...
bool MainWindow::okToDelete()
{
	if(!isOption())
	{ return false; }
		if(isOption()) 
			{optOrRes = tr("Willst du diese Option wirklich lšschen?") ;}
		
		if(isOption() == 2)
			{optOrRes = tr("Willst du diese Antwort wirklich lšschen?") ;}
		
			int r = QMessageBox::warning(this, tr("RL-Dialog"), optOrRes,
					QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
		
			if (r == QMessageBox::Yes)
				{return true; }
			else if (r == QMessageBox::Cancel)
				{ return false; }

		return false;
}

// lšscht ein ListenItem
void MainWindow::remItem()
{
if(okToDelete())
	{ delete str_structur->currentItem(); }
}

// checkt ob es items gibt, und wenn ja, welches
int MainWindow::isOption()
{
	if(str_structur->topLevelItemCount() == 0)
		{ return false; }
	if(str_structur->currentItem()->text(0) == "O") 
		{ return true ;}
	if(str_structur->currentItem()->text(0) == "R") 
		{ return 2;}
		
	return false;
}















// KLasse, oder auslagern
void MainWindow::createActions()
{

/* New */
	newAction = new QAction(tr("&Neu"), this);
	newAction->setIcon(QIcon(":/images/new.png"));
	newAction->setShortcut(tr("Ctrl+N"));
	newAction->setStatusTip(tr("einen neuen Dialog erstellen"));
	
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

/* Oeffnen */
	openAction = new QAction(tr("&…ffnen..."), this);
	openAction->setIcon(QIcon(":/images/open.png"));
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("einen Dialog šffnen"));
	
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

/* Save */
	saveAction = new QAction(tr("&Speichern"), this);
	saveAction->setIcon(QIcon(":/images/save.png"));
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("diesen Dialog speichern"));
	
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

/* Save AS */
	saveAsAction = new QAction(tr("&Speichern unter..."), this);
	saveAsAction->setIcon(QIcon(":/images/saveas.png"));
	saveAsAction->setStatusTip(tr("diesen Dialog speichern unter..."));
	
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));


/* Recent Documents */
	for(int i = 0; i< MaxRecentFiles; i++)
	{
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]-> setVisible(false);
		
		connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

/* Exit */
	exitAction = new QAction(tr("&Beenden"), this);
	exitAction->setIcon(QIcon(":/images/exit.png"));
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("RL-Dialog beenden"));

/* Help */
	helpAction = new QAction(tr("&Hilfe"), this);
	helpAction->setIcon(QIcon(":/images/help.png"));
	helpAction->setShortcut(tr("F1"));
	helpAction->setStatusTip(tr("Dokumentation zu RL-Dialog"));

	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

/* About */
	aboutAction = new QAction(tr("&†ber RL-Dialog"), this);
	aboutAction->setStatusTip(tr("Informationen zu RL-Dialog"));

	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	
}

void MainWindow::createMenus()
{

	//hier wird as File/Datei MenŸ erstellt, und mit den Actions gefŸllt. 
	fileMenu = menuBar()->addMenu(tr("&Datei"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	seperatorAction = fileMenu->addSeparator();
	
	// um die letzten 5 Dateien anzuzeigen
	for(int i = 0; i<MaxRecentFiles; i++)
	{
		fileMenu->addAction(recentFileActions[i]);
	}
	
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	
	//und hier das Hilfe MenŸ (vorher noch ein Trennzeichen)
	menuBar()->addSeparator();
	
	helpMenu = menuBar()->addMenu(tr("&Hilfe"));
	helpMenu->addAction(helpAction);
	helpMenu->addAction(aboutAction);
	
}

void MainWindow::newFile()
{
	if(okToContinue())
	{
	//	widget->clear();
		setCurrentFile("");
	}
}

bool MainWindow::okToContinue()
{
	if(isWindowModified())
	{
		int r = QMessageBox::warning(this, tr("RL-Dialog"), tr("Dieser Dialog wurde noch nicht gespeichert!.\n Willst du ihn jetzt speichern?"),
							QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
		
		if (r == QMessageBox::Yes)
			{return save(); }
		else if (r == QMessageBox::Cancel)
			{ return false; }
	}
	
	return true;
}

void MainWindow::open()
{
	if(okToContinue())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Dialog …fnnen"), ".", tr("Dialog (*.dialog)"));
		
		if(!fileName.isEmpty())
			{ loadFile(fileName); }
	}
}

bool MainWindow::loadFile(const QString &fileName)
{
	//if(!widget->readFile(fileName))
	//	{ return false;	}
	
	setCurrentFile(fileName);
	return true;
}

bool MainWindow::save()
{
	if(curFile.isEmpty())
		{ return saveAs(); }
	
	else
		{ return saveFile(curFile); }
}

bool MainWindow::saveFile(const QString &fileName)
{
	//if(!widget->writeFile(fileName))
	//	{ return false; }
	
	setCurrentFile(fileName);
	return true;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Dialog Speichern"), ".", tr("Dialog (*.dialog)"));
	
	if(fileName.isEmpty())
		{ return false; }
	
	return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(okToContinue())
	{
		//writeSettings();
		event->accept();
	}
	
	else
		{ event->ignore(); }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	setWindowModified(false);
	
	QString shownName = "neuer Dialog";
	if(!curFile.isEmpty())
	{
		shownName = strippedName(curFile);
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}
	
	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Dialog")));
}

QString MainWindow::strippedName(const QString &fullFileName)
	{ return QFileInfo(fullFileName).fileName(); }
	
void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);
	while (i.hasNext())
	{
		if(!QFile::exists(i.next()))
			{ i.remove(); }
	}
	
	for(int j = 0; j< MaxRecentFiles; j++)
	{
		if(j < recentFiles.count())
		{
			QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(recentFiles[j]));
			recentFileActions[j]->setText(text);
			recentFileActions[j]->setData(recentFiles[j]);
			recentFileActions[j]->setVisible(true);
		}
		
		else
			{ recentFileActions[j]->setVisible(false); }
		
		seperatorAction->setVisible(!recentFiles.isEmpty());
	}
}

void MainWindow::openRecentFile()
{
	if(okToContinue())
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if(action)
			{ loadFile(action->data().toString()); }
	}
}

