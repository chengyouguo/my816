/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QTabWidget *tabWidget;
    QWidget *tabGradeManage;
    QVBoxLayout *gradeManageLayout;
    QLineEdit *editNewGrade;
    QComboBox *comboGrade;
    QPushButton *btnAddGrade;
    QPushButton *btnDelGrade;
    QPushButton *btnApplyGrade;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabGradeManage = new QWidget();
        tabGradeManage->setObjectName("tabGradeManage");
        gradeManageLayout = new QVBoxLayout(tabGradeManage);
        gradeManageLayout->setObjectName("gradeManageLayout");
        editNewGrade = new QLineEdit(tabGradeManage);
        editNewGrade->setObjectName("editNewGrade");

        gradeManageLayout->addWidget(editNewGrade);

        comboGrade = new QComboBox(tabGradeManage);
        comboGrade->setObjectName("comboGrade");

        gradeManageLayout->addWidget(comboGrade);

        btnAddGrade = new QPushButton(tabGradeManage);
        btnAddGrade->setObjectName("btnAddGrade");

        gradeManageLayout->addWidget(btnAddGrade);

        btnDelGrade = new QPushButton(tabGradeManage);
        btnDelGrade->setObjectName("btnDelGrade");

        gradeManageLayout->addWidget(btnDelGrade);

        btnApplyGrade = new QPushButton(tabGradeManage);
        btnApplyGrade->setObjectName("btnApplyGrade");

        gradeManageLayout->addWidget(btnApplyGrade);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gradeManageLayout->addItem(verticalSpacer);

        tabWidget->addTab(tabGradeManage, QString());

        centralLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 900, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GradeWordApp", nullptr));
        editNewGrade->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\346\226\260\345\271\264\347\272\247\345\220\215\347\247\260", nullptr));
        btnAddGrade->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\271\264\347\272\247", nullptr));
        btnDelGrade->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\345\271\264\347\272\247", nullptr));
        btnApplyGrade->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232\346\224\271\345\217\230", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGradeManage), QCoreApplication::translate("MainWindow", "\345\271\264\347\272\247\347\256\241\347\220\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
