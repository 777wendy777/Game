#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "cannonfield.h"
#include "lcdrange.h"

class MyWidget : public QWidget
{
public:
    MyWidget(QWidget *parent = 0);
};

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *quit = new QPushButton("Выйти");
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    //угол первой пушки
    LCDRange *angle = new LCDRange;
    angle->setRange(5, 70);

    //угол второй пушки
    LCDRange *SecAngle = new LCDRange;
    SecAngle->setRange(5,70);

    //сила первой пушки
    LCDRange *force = new LCDRange;
    force->setRange(10, 50);

    //сила второй пушки
    LCDRange *SecForce = new LCDRange;
    SecForce->setRange(10, 50);

    CannonField *cannonField = new CannonField;


    //1
    connect(angle, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setAngle(int)));

    connect(cannonField, SIGNAL(angleChanged(int)),
            angle, SLOT(setValue(int)));

    //2
    connect(SecAngle, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setSecAngle(int)));

    connect(cannonField, SIGNAL(SecAngleChanged(int)),
            SecAngle, SLOT(setValue(int)));

    //1
    connect(force, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setForce(int)));

    connect(cannonField, SIGNAL(forceChanged(int)),
            force, SLOT(setValue(int)));

    //2
    connect(SecForce, SIGNAL(valueChanged(int)),
            cannonField, SLOT(setSecForce(int)));

    connect(cannonField, SIGNAL(SecForceChanged(int)),
            SecForce, SLOT(setValue(int)));

    QPushButton *shoot = new QPushButton("Стреляй");
    connect(shoot, SIGNAL(clicked()),cannonField, SLOT(shoot()));

    QPushButton *SecShoot = new QPushButton("Стреляй");
    connect(SecShoot, SIGNAL(clicked()),cannonField, SLOT(SecShoot()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(shoot);
    topLayout->addWidget(SecShoot);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(angle);
    leftLayout->addWidget(force);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(SecAngle);
    rightLayout->addWidget(SecForce);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 0);
    gridLayout->addLayout(topLayout, 0, 1);
    gridLayout->addLayout(leftLayout, 1, 0);
    gridLayout->addLayout(rightLayout,1,2);
    gridLayout->addWidget(cannonField, 1, 1, 2, 1);
    gridLayout->setColumnStretch(1, 10);
    setLayout(gridLayout);

    angle->setValue(45);
    force->setValue(10);
    SecForce->setValue(10);
    SecAngle->setValue(45);
    angle->setFocus();
    SecAngle->setFocus();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWidget widget;
    widget.setGeometry(100, 100, 500, 355);
    widget.setFixedSize(620,355);
    widget.show();
    return app.exec();
}
