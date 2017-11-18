#include "clicklabel.h"


void ClickLabel::mousePressEvent( QMouseEvent* ev )
{
    const QPoint p = ev->pos();
    emit mousePressed(p);
}

void ClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    const QPoint p = ev->pos();
    emit mouseReleased(p);
}

ClickLabel::ClickLabel( QWidget * parent, Qt::WindowFlags f )
    : QLabel( parent, f ) {}

ClickLabel::ClickLabel( const QString& text, QWidget* parent, Qt::WindowFlags f )
    : QLabel( text, parent, f ) {}
