#ifndef IFDIALOG_H
#define IFDIALOG_H

#include <QtGui>
#include <QtXml>

/* Klasse von den Elementen angeben, die ich verwenden will
class QLabel;
class QLineEdit;
class QTextEdit;
*/


//Hauptklasse -> Der Dialog
class ifDialog : public QDialog
{
	Q_OBJECT
	
public:
	ifDialog(QWidget *parent = 0);
	QStackedWidget *typeWidget;
	
	QString getProperty();
	QString getValue();
	QString getTalent();
	QString getQName();
	
	
private slots:	//Hier kommen die slots rein, die ich verwenden will
	
		void changeType(int);
	
private: //hier werden die Elemente angegeben, die ich verwende


	QLabel *typeLabel;
	QComboBox *typeCombo;
	
	QGroupBox *typeBox;
	
	QString typeName;
	
	QPushButton *takeButton;
	QPushButton *cancleButton;

};

class DomParser
{
public:
	DomParser(QIODevice *device);
	
	QStringList get_categories();
	QStringList get_groups(QString currentCat);
	QStringList get_talents(QString currentGroup);

private:
	QString currentCatI;
	QString currentGroupI;
	
	QDomNodeList CatNodeList;
	QDomNode CatNode;	
	QDomNode GroupNode;
	QDomNode TalentNode;
	QDomNodeList GroupNodeList;
	QDomNodeList TalentNodeList;
	
	QStringList categories;
	QStringList groups;
	QStringList talents;
	
	unsigned int i;
};


// ****** DIE TYPEWIDGET KLASSEN *****
class quest : public QWidget
{
public:
	quest(QWidget *parent = 0);
	static QComboBox *nameCombo;
	static QComboBox *propertyCombo;
	static QComboBox *valueCombo;

private:

	QLabel *nameLabel;
	QLabel *propertyLabel;
	QLabel *valueLabel;
};

class charakter : public QWidget
{
	Q_OBJECT

public:
	charakter(QWidget *parent = 0);
	static QComboBox *talentsCombo;
	static QComboBox *isCombo;
	static QLineEdit *valueLEdit;
	
private slots:
	
	void setCategory(int);
	void setGroup(int);
	
private:
	
	DomParser *getTheDom;

	QLabel *catLabel;
		QComboBox *catCombo;
	QLabel *groupsLabel;
		QComboBox *groupsCombo;
	QLabel *talentsLabel;
	QLabel *isLabel;
};

class probe : public QWidget
{
public:
	probe(QWidget *parent = 0);
	static QComboBox *talentsCombo;
	static QComboBox *isCombo;
	static QLineEdit *valueLEdit;

private:

	QComboBox *catCombo;
	QComboBox *groupsCombo;
	QLabel *catLabel;
	QLabel *isLabel;

};


#endif