#include "helper.h"
#include <QtGui>

startMode::startMode(QWidget* parent)
    : QWidget(parent)
{
}

optionMode::optionMode(QWidget* parent)
    : QWidget(parent)
{

    /**** ALLE ELEMENTE ERSTELLEN ****/

    /* Name */
    opt_nameLabel = new QLabel(tr("Name:"));
    opt_nameLEdit = new QLineEdit;
    opt_nameLabel->setBuddy(opt_nameLEdit);
    opt_nameLabel->setAlignment(Qt::AlignRight);
    opt_nameLabel->setFixedWidth(95);

    /* ID */
    opt_IDLabel = new QLabel(tr("ID: 23"));

    /* Text */
    opt_textLabel = new QLabel(tr("Text:"));
    opt_textTEdit = new QTextEdit;
    opt_textLabel->setBuddy(opt_textTEdit);
    opt_textLabel->setAlignment(Qt::AlignRight);

    /* Sound */
    opt_soundLabel = new QLabel(tr("Sound:"));
    opt_soundLEdit = new QLineEdit;
    opt_soundLabel->setBuddy(opt_soundLEdit);
    opt_soundLabel->setAlignment(Qt::AlignRight);

    /* Vorbedingung ALIAS if */
    opt_ifLabel = new QLabel(tr("Vorbedingung:"));
    opt_ifTree = new QTreeWidget;
    opt_ifTree->setColumnCount(4);
    opt_ifHeaderlabels << tr("Type") << tr("Name / Talent") << tr("Property")
                       << tr("Value"); // hier wird eine QStringlist erstellt, die alle Listenueberschriften erhaellt
    opt_ifTree->setHeaderLabels(opt_ifHeaderlabels);
    opt_ifLabel->setBuddy(opt_ifTree);
    opt_ifLabel->setAlignment(Qt::AlignRight);

    /* Mehrfach */
    opt_mehrfachCheck = new QCheckBox(tr("Mehrfach"));
    opt_mehrfachBox = new QGroupBox();
    opt_mehrfachBox->setDisabled(true);

    opt_wennLabel = new QLabel(tr("Wenn..."));
    opt_wennLabel->setFixedWidth(95);
    opt_wennLabel->setAlignment(Qt::AlignRight);

    opt_wennTree = new QTreeWidget;
    opt_caseLabel = new QLabel(tr("cases"));
    opt_caseLabel->setAlignment(Qt::AlignRight);

    opt_caseTree = new QTreeWidget;

    /* die coolen kleinen Tool Buttons */
    opt_ifAdd = new QToolButton;
    opt_ifAdd->setText("+");
    opt_ifAdd->setFixedWidth(25);
    opt_ifRem = new QToolButton;
    opt_ifRem->setText("-");
    opt_ifRem->setFixedWidth(25);
    opt_wenn = new QToolButton;
    opt_wenn->setText("...");
    opt_wenn->setFixedWidth(25);

    opt_caseAdd = new QToolButton;
    opt_caseAdd->setText("+");
    opt_caseAdd->setFixedWidth(25);
    opt_caseRem = new QToolButton;
    opt_caseRem->setText("-");
    opt_caseRem->setFixedWidth(25);

    /* Ãœbernehmen Button */
    opt_takeButton = new QPushButton(QString::fromUtf8("Ãœbernehmen"));

    /* connections */
    connect(opt_ifAdd, SIGNAL(clicked()), this, SLOT(openIfDialog()));

    /* LAXOUT BESTIMMEN */

    QGridLayout* mehrfachLayout = new QGridLayout;
    mehrfachLayout->addWidget(opt_wennLabel, 0, 0, 1, 1);
    mehrfachLayout->addWidget(opt_wennTree, 0, 1, 2, 1);
    mehrfachLayout->addWidget(opt_wenn, 0, 2);

    mehrfachLayout->addWidget(opt_caseLabel, 2, 0, 1, 1);
    mehrfachLayout->addWidget(opt_caseTree, 2, 1, 2, 1);

    mehrfachLayout->addWidget(opt_caseAdd, 2, 2);
    mehrfachLayout->addWidget(opt_caseRem, 3, 2);
    opt_mehrfachBox->setLayout(mehrfachLayout);

    QGridLayout* baseLayout = new QGridLayout;
    baseLayout->addWidget(opt_nameLabel, 0, 0);
    baseLayout->addWidget(opt_nameLEdit, 0, 1);
    baseLayout->addWidget(opt_IDLabel, 0, 2);

    baseLayout->addWidget(opt_textLabel, 1, 0, 1, 1);
    baseLayout->addWidget(opt_textTEdit, 1, 1, 2, 1);

    baseLayout->addWidget(opt_soundLabel, 3, 0);
    baseLayout->addWidget(opt_soundLEdit, 3, 1);

    baseLayout->addWidget(opt_ifLabel, 4, 0, 1, 1);
    baseLayout->addWidget(opt_ifTree, 4, 1, 2, 1);

    baseLayout->addWidget(opt_ifAdd, 4, 2);
    baseLayout->addWidget(opt_ifRem, 5, 2);

    baseLayout->addWidget(opt_mehrfachCheck, 6, 0);
    baseLayout->addWidget(opt_mehrfachBox, 7, 0, 4, 3);
    baseLayout->addWidget(opt_takeButton, 11, 1);

    setLayout(baseLayout);
}

//…ffnet nun den ifDialog, der eine Bedingung auswŠhlen lŠsst
void optionMode::openIfDialog()
{
    ifDialog ifdialog(this);
    if (ifdialog.exec())
    {

        ifProperty = ifdialog.getProperty();
        ifValue = ifdialog.getValue();

        switch (ifdialog.typeWidget->currentIndex())
        {
        case 0:
            ifType = "quest";
            ifName = ifdialog.getQName();
            break;
        case 1:
            ifType = "character";
            ifName = ifdialog.getTalent();
            break;
        case 2:
            ifType = "probe";
            ifName = ifdialog.getTalent();
            break;
        }

        if_item = new QTreeWidgetItem(opt_ifTree);
        if_item->setText(0, ifType);
        if_item->setText(1, ifName);
        if_item->setText(2, ifProperty);
        if_item->setText(3, ifValue);
        if_item->setSelected(true);
        opt_ifTree->setCurrentItem(if_item);
    }
}

responseMode::responseMode(QWidget* parent)
    : QWidget(parent)
{

    /**** ALLE ELEMENTE ERSTELLEN ****/

    /* Name */
    res_nameLabel = new QLabel(tr("Name:"));
    res_nameLEdit = new QLineEdit;
    res_nameLabel->setBuddy(res_nameLEdit);
    res_nameLabel->setAlignment(Qt::AlignRight);
    res_nameLabel->setFixedWidth(95);

    /* ID */
    res_IDLabel = new QLabel(tr("ID: 152"));

    /* Text */
    res_textLabel = new QLabel(tr("Text:"));
    res_textTEdit = new QTextEdit;
    res_textLabel->setBuddy(res_textTEdit);
    res_textLabel->setAlignment(Qt::AlignRight);

    /* Sound */
    res_soundLabel = new QLabel(tr("Sound:"));
    res_soundLEdit = new QLineEdit;
    res_soundLabel->setBuddy(res_soundLEdit);
    res_soundLabel->setAlignment(Qt::AlignRight);

    /* Effekte */
    res_effektLabel = new QLabel(tr("Effekt:"));
    res_effektTree = new QTreeWidget;
    res_effektLabel->setBuddy(res_effektTree);
    res_effektLabel->setAlignment(Qt::AlignRight);

    /* Goto */
    res_gotoLabel = new QLabel(tr("Goto:"));
    res_gotoCombo = new QComboBox();
    res_gotoLabel->setBuddy(res_gotoCombo);
    res_gotoLabel->setAlignment(Qt::AlignRight);
    res_gotoCombo->addItem(tr("Response 1"));
    res_gotoCombo->addItem(tr("Response 2"));
    res_gotoCombo->addItem(tr("Response 3"));
    res_gotoCombo->addItem(tr("Response 4"));
    res_gotoCombo->addItem(tr("Response 5"));

    /* die coolen kleinen Tool Buttons */
    res_effektAdd = new QToolButton;
    res_effektAdd->setText("+");
    res_effektAdd->setFixedWidth(25);
    res_effektRem = new QToolButton;
    res_effektRem->setText("-");
    res_effektRem->setFixedWidth(25);

    /* Ãœbernehmen Button */
    res_takeButton = new QPushButton(QString::fromUtf8("Ãœbernehmen"));

    /* LAXOUT BESTIMMEN */

    QGridLayout* baseLayout = new QGridLayout;
    baseLayout->addWidget(res_nameLabel, 0, 0);
    baseLayout->addWidget(res_nameLEdit, 0, 1);
    baseLayout->addWidget(res_IDLabel, 0, 2);

    baseLayout->addWidget(res_textLabel, 1, 0, 1, 1);
    baseLayout->addWidget(res_textTEdit, 1, 1, 2, 1);

    baseLayout->addWidget(res_soundLabel, 3, 0);
    baseLayout->addWidget(res_soundLEdit, 3, 1);

    baseLayout->addWidget(res_effektLabel, 4, 0, 1, 1);
    baseLayout->addWidget(res_effektTree, 4, 1, 2, 1);

    baseLayout->addWidget(res_effektAdd, 4, 2);
    baseLayout->addWidget(res_effektRem, 5, 2);

    baseLayout->addWidget(res_gotoLabel, 6, 0);
    baseLayout->addWidget(res_gotoCombo, 6, 1);

    baseLayout->addWidget(res_takeButton, 7, 1);

    setLayout(baseLayout);
}