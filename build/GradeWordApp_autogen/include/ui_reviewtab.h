/********************************************************************************
** Form generated from reading UI file 'reviewtab.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REVIEWTAB_H
#define UI_REVIEWTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReviewTab
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lblDef;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *lblWord;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnDontKnow;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnGotIt;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lblPhonetic;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QWidget *ReviewTab)
    {
        if (ReviewTab->objectName().isEmpty())
            ReviewTab->setObjectName("ReviewTab");
        ReviewTab->resize(331, 300);
        gridLayout = new QGridLayout(ReviewTab);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        lblDef = new QLabel(ReviewTab);
        lblDef->setObjectName("lblDef");

        horizontalLayout_3->addWidget(lblDef);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);


        gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lblWord = new QLabel(ReviewTab);
        lblWord->setObjectName("lblWord");

        horizontalLayout->addWidget(lblWord);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        btnDontKnow = new QPushButton(ReviewTab);
        btnDontKnow->setObjectName("btnDontKnow");

        horizontalLayout_4->addWidget(btnDontKnow);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        btnGotIt = new QPushButton(ReviewTab);
        btnGotIt->setObjectName("btnGotIt");

        horizontalLayout_4->addWidget(btnGotIt);

        horizontalSpacer_6 = new QSpacerItem(78, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        gridLayout->addLayout(horizontalLayout_4, 3, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lblPhonetic = new QLabel(ReviewTab);
        lblPhonetic->setObjectName("lblPhonetic");

        horizontalLayout_2->addWidget(lblPhonetic);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_8);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);


        retranslateUi(ReviewTab);

        QMetaObject::connectSlotsByName(ReviewTab);
    } // setupUi

    void retranslateUi(QWidget *ReviewTab)
    {
        ReviewTab->setWindowTitle(QCoreApplication::translate("ReviewTab", "Form", nullptr));
        lblDef->setText(QCoreApplication::translate("ReviewTab", "\351\207\212\344\271\211", nullptr));
        lblWord->setText(QCoreApplication::translate("ReviewTab", "\345\215\225\350\257\215", nullptr));
        btnDontKnow->setText(QCoreApplication::translate("ReviewTab", "\344\270\215\347\237\245\351\201\223", nullptr));
        btnGotIt->setText(QCoreApplication::translate("ReviewTab", "\347\237\245\351\201\223", nullptr));
        lblPhonetic->setText(QCoreApplication::translate("ReviewTab", "\351\237\263\351\242\221", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReviewTab: public Ui_ReviewTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVIEWTAB_H
