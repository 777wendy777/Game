#ifndef CANNONFIELD_H
#define CANNONFIELD_H

#include <QWidget>

class CannonField : public QWidget
{
    Q_OBJECT

public:
    CannonField(QWidget *parent = 0);

    int angle() const { return currentAngle; }
    int SecAngle() const { return SecCurrentAngle; }
    int force() const { return currentForce; }
    int SecForce() const {return SecCurrentForce;}

public slots:
    void setAngle(int angle);
    void setSecAngle(int SecAngle);
    void setForce(int force);
    void setSecForce(int SecForce);
    void shoot();
    void SecShoot();

private slots:
     void moveShot();
     void moveSecShot();

signals:
    void angleChanged(int newAngle);
    void SecAngleChanged(int newSecAngle);
    void forceChanged(int newForce);
    void SecForceChanged(int newSecForce);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void paintCannon(QPainter &painter);
    void paintShot(QPainter &painter);
    void paintSecShot(QPainter &painter);
    QRect cannonRect() const;
    QRect SecCannonRect() const;
    QRect shotRect() const;
    QRect SecShotRect() const;

    int currentAngle;
    int currentForce;
    int SecCurrentAngle;
    int SecCurrentForce;

    int timerCount;
    int SecTimerCount;
    QTimer *autoShootTimer;
    QTimer *SecAutoShootTimer;
    float shootAngle;
    float shootForce;
    float SecShootAngle;
    float SecShootForce;

};

#endif
