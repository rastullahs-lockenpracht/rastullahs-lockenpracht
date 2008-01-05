#ifndef HELPER_H
#define HELPER_H

#include <QWidget>
#include "ifDialog.h"

class startMode : public QWidget
{
	
public:
	startMode(QWidget *parent = 0);
};

class optionMode : public QWidget
{
	Q_OBJECT
	
public:
	optionMode(QWidget *parent = 0);

private slots:	//Hier kommen die slots rein, die ich verwenden will

	void openIfDialog();
private:

	QLabel *opt_nameLabel;
	QLabel *opt_IDLabel;
	QLabel *opt_textLabel;
	QLabel *opt_soundLabel;
	QLabel *opt_ifLabel;
	QLabel *opt_mehrfachLabel;
	QLabel *opt_wennLabel;
	QLabel *opt_caseLabel;
	
	QLineEdit *opt_nameLEdit;
	QLineEdit *opt_soundLEdit;
	
	QTextEdit *opt_textTEdit;
	
	QTreeWidget *opt_ifTree;
	QStringList opt_ifHeaderlabels;
	QTreeWidgetItem *if_item;
	
	QTreeWidget *opt_wennTree;
	QTreeWidget *opt_caseTree;
	
	QToolButton *opt_ifAdd;
	QToolButton *opt_ifRem;
	QToolButton *opt_wenn;
	QToolButton *opt_caseAdd;
	QToolButton *opt_caseRem;
	
	QPushButton *opt_takeButton;
	
	QCheckBox *opt_mehrfachCheck;
	QGroupBox *opt_mehrfachBox;

/* alle Strings, die ich vom ifDialog bekomme */
	QString ifType; //bei allen varianten wird aber nicht aus dem ifDialog genommen
	QString ifProperty; //bei allen varianten
	QString ifValue; //bei allen varianten
	QString ifName; //bei Quest IST AUCH DAS TALENT!!

};

class responseMode : public QWidget
{
public:
	responseMode(QWidget *parent = 0);
private:
	QLabel *res_nameLabel;
	QLabel *res_IDLabel;
	QLabel *res_textLabel;
	QLabel *res_soundLabel;
	QLabel *res_effektLabel;
	QLabel *res_gotoLabel;
	
	QLineEdit *res_nameLEdit;
	QLineEdit *res_soundLEdit;
	
	QTextEdit *res_textTEdit;
	
	QTreeWidget *res_effektTree;
	
	QToolButton *res_effektAdd;
	QToolButton *res_effektRem;
	
	QPushButton *res_takeButton;
	
	QComboBox *res_gotoCombo;
};

#endif