#include <QtGui>

#include "ifdialog.h"

QComboBox* quest::propertyCombo = 0;
QComboBox* quest::valueCombo = 0;
QComboBox* quest::nameCombo = 0;

QLineEdit* probe::valueLEdit = 0;
QComboBox* probe::talentsCombo = 0;
QComboBox* probe::isCombo = 0;

QLineEdit* charakter::valueLEdit = 0;
QComboBox* charakter::talentsCombo = 0;
QComboBox* charakter::isCombo = 0;

// Hauptklasse von dem Feld... NAME ändern
ifDialog::ifDialog(QWidget* parent)
    : QDialog(parent)
{

    typeLabel = new QLabel(tr("Type: "));
    typeCombo = new QComboBox();
    typeLabel->setBuddy(typeCombo);
    typeCombo->addItem(tr("Quest"));
    typeCombo->addItem(tr("Charakter"));
    typeCombo->addItem(tr("probe"));

    typeWidget = new QStackedWidget;

    typeWidget->addWidget(new quest);
    typeWidget->addWidget(new charakter);
    typeWidget->addWidget(new probe);

    typeBox = new QGroupBox(typeName);

    takeButton = new QPushButton(QString::fromUtf8("Übernehmen"));
    takeButton->setDefault(true);
    cancleButton = new QPushButton(tr("abbrechen"));

    // connect
    connect(typeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));
    connect(takeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancleButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Layout bestimmen

    QHBoxLayout* typeLayout = new QHBoxLayout; //  type layout (Lanel + selectionbox)
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);
    typeLayout->addStretch();

    QHBoxLayout* takeLayout = new QHBoxLayout; //  type layout (Lanel + selectionbox)
    takeLayout->addStretch();
    takeLayout->addWidget(cancleButton);
    takeLayout->addWidget(takeButton);

    QVBoxLayout* contentLayout = new QVBoxLayout;
    contentLayout->addWidget(typeWidget);
    typeBox->setLayout(contentLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout; // hauptlayout
    mainLayout->addLayout(typeLayout);
    mainLayout->addWidget(typeBox);
    mainLayout->addLayout(takeLayout);
    setLayout(mainLayout); // das Layout "installieren"

    setWindowTitle(tr("Vorbedingung"));
}

QString ifDialog::getProperty()
{
    switch (typeCombo->currentIndex())
    {
    case 1:
        return quest::propertyCombo->currentText();
        break;
    case 2:
        return charakter::isCombo->currentText();
        break;
    case 3:
        return probe::isCombo->currentText();
        break;
    }
    return false;
}

QString ifDialog::getValue()
{
    switch (typeCombo->currentIndex())
    {
    case 1:
        return quest::valueCombo->currentText();
        break;
    case 2:
        return charakter::valueLEdit->text();
        break;
    case 3:
        return probe::valueLEdit->text();
        break;
    }
    return false;
}

QString ifDialog::getTalent()
{
    switch (typeCombo->currentIndex())
    {
    case 2:
        return charakter::talentsCombo->currentText();
        break;
    case 3:
        return probe::talentsCombo->currentText();
        break;
    }
    return false;
}

QString ifDialog::getQName()
{
    return quest::nameCombo->currentText();
}

void ifDialog::changeType(int)
{
    typeWidget->setCurrentIndex(typeCombo->currentIndex());
}

DomParser::DomParser(QIODevice* device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox::warning(0, QObject::tr("DOM Parser"),
            QObject::tr("Parser Error Zeile %1, "
                        "Spalte %2: \n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorStr));
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "XDIML")
        return;

    CatNode = root.firstChild();
    CatNodeList = CatNode.childNodes();

    /*while (node)
    {
        if(node.toElement().tagName() == "Inhalt")
        {talents = talents << node.toElement().attribute("ID");
        talents << "1" << "2" << "4";
        }

        node = node.nextSibling();
    }*/
}

QStringList DomParser::get_categories()
{

    for (i = 0; i < CatNodeList.length(); i++)
    {
        CatNode = CatNodeList.item(i);
        categories << CatNode.toElement().tagName();
    }

    return categories;
}

QStringList DomParser::get_groups(QString currentCat)
{
    groups.clear();

    for (i = 0; i < CatNodeList.length(); i++)
    {
        CatNode = CatNodeList.item(i);
        categories << CatNode.toElement().tagName();
        if (CatNode.toElement().tagName() == currentCat)
            GroupNode = CatNode;
    }

    GroupNodeList = GroupNode.childNodes();

    for (i = 0; i < GroupNodeList.length(); i++)
    {
        GroupNode = GroupNodeList.item(i);
        groups << GroupNode.toElement().attribute("ID");
    }

    return groups;
}

QStringList DomParser::get_talents(QString currentGroup)
{
    talents.clear();

    for (i = 0; i < GroupNodeList.length(); i++)
    {
        GroupNode = GroupNodeList.item(i);
        groups << GroupNode.toElement().attribute("ID");

        if (GroupNode.toElement().attribute("ID") == currentGroup)
            TalentNode = GroupNode;
    }

    TalentNodeList = TalentNode.childNodes();

    for (i = 0; i < TalentNodeList.length(); i++)
    {
        TalentNode = TalentNodeList.item(i);
        talents << TalentNode.toElement().attribute("ID");
    }

    return talents;
}

void charakter::setCategory(int)
{
    groupsCombo->clear();
    groupsCombo->addItems(getTheDom->get_groups(catCombo->currentText()));
}

void charakter::setGroup(int)
{
    talentsCombo->clear();
    talentsCombo->addItems(getTheDom->get_talents(groupsCombo->currentText()));
}

quest::quest(QWidget* parent)
    : QWidget(parent)
{
    nameLabel = new QLabel(tr("Quest:"));
    nameLabel->setAlignment(Qt::AlignRight);
    nameCombo = new QComboBox();
    nameCombo->setEditable(true);
    nameLabel->setBuddy(nameCombo);

    propertyLabel = new QLabel(tr("Property:"));
    propertyLabel->setAlignment(Qt::AlignRight);
    propertyCombo = new QComboBox();
    propertyCombo->addItem("state");
    propertyCombo->addItem("known");
    propertyCombo->addItem("...");
    propertyLabel->setBuddy(propertyCombo);

    valueLabel = new QLabel(tr("Value:"));
    valueLabel->setAlignment(Qt::AlignRight);
    valueCombo = new QComboBox();
    valueCombo->addItem("completet");
    valueCombo->addItem("true");
    valueCombo->addItem("false");
    valueLabel->setBuddy(valueCombo);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameCombo, 0, 1);
    mainLayout->addWidget(propertyLabel, 1, 0);
    mainLayout->addWidget(propertyCombo, 1, 1);
    mainLayout->addWidget(valueLabel, 2, 0);
    mainLayout->addWidget(valueCombo, 2, 1);
    setLayout(mainLayout);
}

charakter::charakter(QWidget* parent)
    : QWidget(parent)
{
    getTheDom = new DomParser(&QFile("basis.xdi")); // liest die basis.xdi Datei aus

    catLabel = new QLabel(tr("Kategorie:"));
    catLabel->setAlignment(Qt::AlignRight);
    catCombo = new QComboBox();
    catCombo->addItems(getTheDom->get_categories());
    catLabel->setBuddy(catCombo);

    groupsLabel = new QLabel(tr("Gruppe:"));
    groupsLabel->setAlignment(Qt::AlignRight);
    groupsCombo = new QComboBox();
    groupsLabel->setBuddy(groupsCombo);

    talentsLabel = new QLabel(tr("Talent:"));
    talentsLabel->setAlignment(Qt::AlignRight);
    talentsCombo = new QComboBox();
    talentsLabel->setBuddy(talentsCombo);

    isLabel = new QLabel(tr("Ist:"));
    isLabel->setAlignment(Qt::AlignRight);
    isCombo = new QComboBox();
    isCombo->addItem(">");
    isCombo->addItem("<");
    isCombo->addItem("=");
    isLabel->setBuddy(isCombo);

    valueLEdit = new QLineEdit;
    valueLEdit->setFixedWidth(50);

    connect(catCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCategory(int)));
    connect(groupsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setGroup(int)));

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(catLabel, 0, 0);
    mainLayout->addWidget(catCombo, 0, 1, 1, 2);
    mainLayout->addWidget(groupsLabel, 1, 0);
    mainLayout->addWidget(groupsCombo, 1, 1, 1, 2);
    mainLayout->addWidget(talentsLabel, 2, 0);
    mainLayout->addWidget(talentsCombo, 2, 1, 1, 2);
    mainLayout->addWidget(isLabel, 3, 0);
    mainLayout->addWidget(isCombo, 3, 1);
    mainLayout->addWidget(valueLEdit, 3, 2);
    setLayout(mainLayout);
}

probe::probe(QWidget* parent)
    : QWidget(parent)
{
    catLabel = new QLabel(tr("Kategorie:"));
    catLabel->setAlignment(Qt::AlignRight);
    catCombo = new QComboBox();
    catCombo->addItem(tr("Attribut"));
    catCombo->addItem(tr("Talent"));
    catCombo->addItem(tr("Zauber"));
    catCombo->addItem(tr("Liturgien"));
    catLabel->setBuddy(catCombo);

    isLabel = new QLabel(tr("Ist:"));
    isLabel->setAlignment(Qt::AlignRight);
    isCombo = new QComboBox();
    isCombo->addItem(">");
    isCombo->addItem("<");
    isCombo->addItem("=");
    isLabel->setBuddy(isCombo);

    valueLEdit = new QLineEdit;
    valueLEdit->setFixedWidth(50);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(catLabel, 0, 0);
    mainLayout->addWidget(catCombo, 0, 1, 1, 2);
    mainLayout->addWidget(isLabel, 1, 0);
    mainLayout->addWidget(isCombo, 1, 1);
    mainLayout->addWidget(valueLEdit, 1, 2);
    setLayout(mainLayout);
}
