/********************************************************************************
** Form generated from reading UI file 'wordbooktab.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDBOOKTAB_H
#define UI_WORDBOOKTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordBookTab
{
public:
    QGridLayout *gridLayout;
    QLabel *labelGrade;
    QLineEdit *lineEditGrade;
    QLineEdit *editWord;
    QPushButton *btnAddWord;
    QPushButton *btnplayer;
    QLabel *labelWord;
    QGroupBox *groupTable;
    QGridLayout *gridLayout_3;
    QTableView *tableView;
    QLabel *statusLabel;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *editDef;
    QLineEdit *editPhonetic;
    QLabel *labelDef;
    QLabel *labelPhonetic;
    QLineEdit *editExample;
    QLabel *labelExample;
    QPushButton *btnSearch;

    void setupUi(QWidget *WordBookTab)
    {
        if (WordBookTab->objectName().isEmpty())
            WordBookTab->setObjectName("WordBookTab");
        WordBookTab->resize(828, 633);
        gridLayout = new QGridLayout(WordBookTab);
        gridLayout->setObjectName("gridLayout");
        labelGrade = new QLabel(WordBookTab);
        labelGrade->setObjectName("labelGrade");

        gridLayout->addWidget(labelGrade, 0, 0, 1, 1);

        lineEditGrade = new QLineEdit(WordBookTab);
        lineEditGrade->setObjectName("lineEditGrade");
        lineEditGrade->setFrame(false);
        lineEditGrade->setReadOnly(true);

        gridLayout->addWidget(lineEditGrade, 0, 1, 1, 1);

        editWord = new QLineEdit(WordBookTab);
        editWord->setObjectName("editWord");

        gridLayout->addWidget(editWord, 1, 1, 1, 1);

        btnAddWord = new QPushButton(WordBookTab);
        btnAddWord->setObjectName("btnAddWord");

        gridLayout->addWidget(btnAddWord, 2, 3, 1, 1);

        btnplayer = new QPushButton(WordBookTab);
        btnplayer->setObjectName("btnplayer");

        gridLayout->addWidget(btnplayer, 1, 3, 1, 1);

        labelWord = new QLabel(WordBookTab);
        labelWord->setObjectName("labelWord");

        gridLayout->addWidget(labelWord, 1, 0, 1, 1);

        groupTable = new QGroupBox(WordBookTab);
        groupTable->setObjectName("groupTable");
        gridLayout_3 = new QGridLayout(groupTable);
        gridLayout_3->setObjectName("gridLayout_3");
        tableView = new QTableView(groupTable);
        tableView->setObjectName("tableView");

        gridLayout_3->addWidget(tableView, 0, 0, 1, 2);

        statusLabel = new QLabel(groupTable);
        statusLabel->setObjectName("statusLabel");

        gridLayout_3->addWidget(statusLabel, 1, 0, 1, 1);


        gridLayout->addWidget(groupTable, 13, 0, 1, 4);

        groupBox = new QGroupBox(WordBookTab);
        groupBox->setObjectName("groupBox");
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName("gridLayout_2");
        editDef = new QLineEdit(groupBox);
        editDef->setObjectName("editDef");
        editDef->setReadOnly(true);

        gridLayout_2->addWidget(editDef, 1, 1, 1, 1);

        editPhonetic = new QLineEdit(groupBox);
        editPhonetic->setObjectName("editPhonetic");
        editPhonetic->setReadOnly(true);

        gridLayout_2->addWidget(editPhonetic, 0, 1, 1, 1);

        labelDef = new QLabel(groupBox);
        labelDef->setObjectName("labelDef");

        gridLayout_2->addWidget(labelDef, 1, 0, 1, 1);

        labelPhonetic = new QLabel(groupBox);
        labelPhonetic->setObjectName("labelPhonetic");

        gridLayout_2->addWidget(labelPhonetic, 0, 0, 1, 1);

        editExample = new QLineEdit(groupBox);
        editExample->setObjectName("editExample");
        editExample->setReadOnly(true);

        gridLayout_2->addWidget(editExample, 2, 1, 1, 1);

        labelExample = new QLabel(groupBox);
        labelExample->setObjectName("labelExample");

        gridLayout_2->addWidget(labelExample, 2, 0, 1, 1);


        gridLayout->addWidget(groupBox, 2, 1, 3, 1);

        btnSearch = new QPushButton(WordBookTab);
        btnSearch->setObjectName("btnSearch");

        gridLayout->addWidget(btnSearch, 3, 3, 1, 1);


        retranslateUi(WordBookTab);

        QMetaObject::connectSlotsByName(WordBookTab);
    } // setupUi

    void retranslateUi(QWidget *WordBookTab)
    {
        labelGrade->setText(QCoreApplication::translate("WordBookTab", "\345\275\223\345\211\215\345\271\264\347\272\247\357\274\232", nullptr));
        btnAddWord->setText(QCoreApplication::translate("WordBookTab", "\346\267\273\345\212\240", nullptr));
        btnplayer->setText(QCoreApplication::translate("WordBookTab", "\346\222\255\346\224\276", nullptr));
        labelWord->setText(QCoreApplication::translate("WordBookTab", "\345\215\225\350\257\215\357\274\232", nullptr));
        groupTable->setTitle(QCoreApplication::translate("WordBookTab", "\345\275\223\345\211\215\345\271\264\347\272\247\345\215\225\350\257\215\350\241\250", nullptr));
        statusLabel->setText(QString());
        groupBox->setTitle(QCoreApplication::translate("WordBookTab", "\345\215\225\350\257\215\350\257\246\346\203\205", nullptr));
        labelDef->setText(QCoreApplication::translate("WordBookTab", "\351\207\212\344\271\211", nullptr));
        labelPhonetic->setText(QCoreApplication::translate("WordBookTab", "\351\237\263\346\240\207", nullptr));
        labelExample->setText(QCoreApplication::translate("WordBookTab", "\344\276\213\345\217\245", nullptr));
        btnSearch->setText(QCoreApplication::translate("WordBookTab", "\346\237\245\346\211\276", nullptr));
        (void)WordBookTab;
    } // retranslateUi

};

namespace Ui {
    class WordBookTab: public Ui_WordBookTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDBOOKTAB_H
