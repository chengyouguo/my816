/********************************************************************************
** Form generated from reading UI file 'dictationtab.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DICTATIONTAB_H
#define UI_DICTATIONTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DictationTab
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTitle;
    QHBoxLayout *horizontalLayout;
    QLineEdit *editInput;
    QPushButton *btnPlay;
    QPushButton *btnStart;
    QLabel *lblResult;
    QTableWidget *tableWidget;
    QLabel *lblSummary;

    void setupUi(QWidget *DictationTab)
    {
        if (DictationTab->objectName().isEmpty())
            DictationTab->setObjectName("DictationTab");
        DictationTab->resize(585, 703);
        verticalLayout = new QVBoxLayout(DictationTab);
        verticalLayout->setObjectName("verticalLayout");
        lblTitle = new QLabel(DictationTab);
        lblTitle->setObjectName("lblTitle");

        verticalLayout->addWidget(lblTitle);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        editInput = new QLineEdit(DictationTab);
        editInput->setObjectName("editInput");
        editInput->setReadOnly(false);

        horizontalLayout->addWidget(editInput);

        btnPlay = new QPushButton(DictationTab);
        btnPlay->setObjectName("btnPlay");

        horizontalLayout->addWidget(btnPlay);


        verticalLayout->addLayout(horizontalLayout);

        btnStart = new QPushButton(DictationTab);
        btnStart->setObjectName("btnStart");

        verticalLayout->addWidget(btnStart);

        lblResult = new QLabel(DictationTab);
        lblResult->setObjectName("lblResult");

        verticalLayout->addWidget(lblResult);

        tableWidget = new QTableWidget(DictationTab);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setEnabled(true);
        tableWidget->setShowGrid(true);

        verticalLayout->addWidget(tableWidget);

        lblSummary = new QLabel(DictationTab);
        lblSummary->setObjectName("lblSummary");

        verticalLayout->addWidget(lblSummary);


        retranslateUi(DictationTab);

        QMetaObject::connectSlotsByName(DictationTab);
    } // setupUi

    void retranslateUi(QWidget *DictationTab)
    {
        lblTitle->setText(QCoreApplication::translate("DictationTab", "\345\220\254\345\206\231\346\265\213\350\257\225\357\274\232", nullptr));
        btnPlay->setText(QCoreApplication::translate("DictationTab", "\346\222\255\346\224\276", nullptr));
        btnStart->setText(QCoreApplication::translate("DictationTab", "\345\274\200\345\247\213\346\265\213\350\257\225", nullptr));
        lblResult->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DictationTab", "\351\224\231\350\257\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DictationTab", "\351\207\212\344\271\211", nullptr));
        lblSummary->setText(QString());
        (void)DictationTab;
    } // retranslateUi

};

namespace Ui {
    class DictationTab: public Ui_DictationTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DICTATIONTAB_H
