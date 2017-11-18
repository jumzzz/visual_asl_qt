#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

class ClickLabel : public QLabel
{
    Q_OBJECT

signals:
    void mousePressed(const QPoint &loc);
    void mouseReleased(const QPoint &loc);
public:
    ClickLabel( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ClickLabel( const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0 );

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);


};

#endif // CLICKLABEL_H
