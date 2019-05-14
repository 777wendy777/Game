#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include "cannonfield.h"
#include <math.h>

CannonField::CannonField(QWidget *parent)
    : QWidget(parent)
{
    currentAngle = 45;
    currentForce = 10;
    SecCurrentAngle = 45;
    SecCurrentForce = 10;
    timerCount = 0;
    autoShootTimer = new QTimer(this);
    connect(autoShootTimer, SIGNAL(timeout()), this, SLOT(moveShot()));
    shootAngle = 0;
    shootForce = 0;
    SecTimerCount = 0;
    SecAutoShootTimer = new QTimer(this);
    connect(SecAutoShootTimer,SIGNAL(timeout()),this,SLOT(moveSecShot()));
    SecShootAngle = 0;
    SecShootForce = 0;
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
}

void CannonField::setAngle(int angle)
{
    if (angle < 5)
        angle = 5;
    if (angle > 70)
        angle = 70;
    if (currentAngle == angle)
        return;
    currentAngle = angle;
    update(cannonRect());
    emit angleChanged(currentAngle);
}

void CannonField::setSecAngle(int SecAngle)
{
    if (SecAngle < 5)
        SecAngle = 5;
    if (SecAngle > 70)
        SecAngle = 70;
    if (SecCurrentAngle == SecAngle)
        return;
    SecCurrentAngle = SecAngle;
    update(SecCannonRect());
    emit SecAngleChanged(SecCurrentAngle);
}

void CannonField::setForce(int force)
{
    if (force < 0)
        force = 0;
    if (currentForce == force)
        return;
    currentForce = force;
    emit forceChanged(currentForce);
}

void CannonField::setSecForce(int SecForce)
{
    if (SecForce < 0)
        SecForce = 0;
    if (SecCurrentForce == SecForce)
        return;
    SecCurrentForce = SecForce;
    emit SecForceChanged(SecCurrentForce);
}

void CannonField::shoot()
 {
     if (autoShootTimer->isActive())
         return;
     timerCount = 0;
     shootAngle = currentAngle;
     shootForce = currentForce;
     autoShootTimer->start(5);
 }

void CannonField::SecShoot()
 {
     if (SecAutoShootTimer->isActive())
         return;
     SecTimerCount = 0;
     SecShootAngle = SecCurrentAngle;
     SecShootForce = SecCurrentForce;
     SecAutoShootTimer->start(5);
 }

void CannonField::moveShot()
 {
     QRegion region = shotRect();
     ++timerCount;

     QRect shotR = shotRect();

     if (shotR.x() > width() || shotR.y() > height()) {
         autoShootTimer->stop();
     } else {
         region = region.united(shotR);
     }
     update(region);
 }

void CannonField::moveSecShot()
 {
     QRegion SecRegion = SecShotRect();
     ++SecTimerCount;

     QRect SecShotR = SecShotRect();

     if (SecShotR.x() > width() || SecShotR.y() > height()) {
         SecAutoShootTimer->stop();
     } else {
         SecRegion = SecRegion.united(SecShotR);
     }
     update(SecRegion);
 }

void CannonField::paintEvent(QPaintEvent *)
 {
     QPainter painter(this);

     paintCannon(painter);
     if (autoShootTimer->isActive())
         paintShot(painter);
     if (SecAutoShootTimer->isActive())
         paintSecShot(painter);
 }

void CannonField::paintShot(QPainter &painter)
  {
      painter.setPen(Qt::NoPen);
      painter.setBrush(Qt::black);
      painter.drawRect(shotRect());
  }

void CannonField::paintSecShot(QPainter &painter)
  {
      painter.setPen(Qt::NoPen);
      painter.setBrush(Qt::black);
      painter.drawRect(SecShotRect());
  }

const QRect barrelRect(30, -5, 20, 10);
const QRect SecBarrelRect(-50,-5,20,10);

void CannonField::paintCannon(QPainter &painter)
 {
     painter.setPen(Qt::NoPen);
     painter.setBrush(Qt::black);

     painter.save();
     painter.translate(0, height());
     painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
     painter.drawPie(QRect(width()-35,-35,70,70),1450, 90 * 16);
     painter.rotate(-currentAngle);
     painter.drawRect(barrelRect);
     painter.rotate(currentAngle);
     painter.translate(width(),0);
     painter.rotate(SecCurrentAngle);
     painter.drawRect(SecBarrelRect);
     painter.restore();
 }

QRect CannonField::cannonRect() const
{
    QRect result(0, 0, 50, 50);
    result.moveBottomLeft(rect().bottomLeft());
    return result;
}

QRect CannonField::SecCannonRect() const
{
    QRect SecResult(0, 0, 50, 50);
    SecResult.moveBottomRight(rect().bottomRight());
    return SecResult;
}

QRect CannonField::shotRect() const
{
    const double gravity = 4;

    double time = timerCount / 20.0;
    double velocity = shootForce;
    double radians = shootAngle * 3.14159265 / 180;

    double velx = velocity * cos(radians);
    double vely = velocity * sin(radians);
    double x0 = (barrelRect.right() + 5) * cos(radians);
    double y0 = (barrelRect.right() + 5) * sin(radians);
    double x = x0 + velx * time;
    double y = y0 + vely * time - 0.5 * gravity * time * time;

    QRect result(0, 0, 6, 6);
    result.moveCenter(QPoint(qRound(x), height() - 1 - qRound(y)));
    return result;
}

QRect CannonField::SecShotRect() const
{
    const double SecGravity = 4;
    const double pi = 3.14159265;

    double SecTime = SecTimerCount / 20.0;
    double SecVelocity = SecShootForce;
    double SecRadians = (SecShootAngle * pi) / 180;

    double SecVelx = (SecVelocity * cos(pi-SecRadians));
    double SecVely = SecVelocity * sin(pi-SecRadians);
    double SecX0 = (SecBarrelRect.left() - 5) * cos(pi-SecRadians);
    double SecY0 = (SecBarrelRect.left() + 5) * sin(pi-SecRadians);
    double SecX = SecX0 + SecVelx * SecTime;
    double SecY = SecY0 + SecVely * SecTime - 0.5 * SecGravity * SecTime * SecTime;

    QRect SecResult(0, 0, 6, 6);
    SecResult.moveCenter(QPoint(width() + qRound(SecX) , height() - 1 - qRound(SecY)));
    return SecResult;
}


