#include "FreePointItem.h"
#include "qregularexpression.h"
#include <string>

// --------------------------------------------------------------------------------------------


FreePointItem::FreePointItem()
{
}


// --------------------------------------------------------------------------------------------


void FreePointItem::setParams(QTextEdit* editor, qreal scaleFactor, QString name,
               QGraphicsSimpleTextItem* item, qreal offsetX, qreal offsetY, bool animationPoint) {
    m_Editor = editor;
    m_ScaleFactor = scaleFactor;
    m_Name = name;
    m_Item = item;
    m_Offsetx = offsetX;
    m_Offsety = offsetY;
    m_bAnimationPoint = animationPoint;
}


// --------------------------------------------------------------------------------------------


void FreePointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (!this->isSelected())
        return;
    QString coordinate = "(\\s+)(-?)(\\d+)((\\.\\d+)?)";
    QPointF P = mouseEvent->scenePos();
    QString after;
    QString input = m_Editor->toPlainText(); // fixme to avoid copying string
    QTextCursor c = m_Editor->textCursor();
    int pos = 0;

    if(m_bAnimationPoint)  {
        QString point_decl;
        double x0, y0, x1, y1;
        QRegularExpression rx("\\b(point)(\\s+)" + m_Name + coordinate + coordinate + coordinate + coordinate);

  //      while ((pos = rx.indexIn(input,pos)) != -1) {
        QRegularExpressionMatchIterator i = rx.globalMatch(input);
        while (i.hasNext()) {
           QRegularExpressionMatch match = i.next();
           // point_decl = rx.cap(0);
           point_decl = match.captured();
           QTextStream myteststream(&point_decl);
           QString point, name;
           myteststream >> point >> name >> x0 >> y0 >> x1 >> y1;

           c.setPosition(pos);
           pos += match.capturedLength();
           c.setPosition(pos, QTextCursor::KeepAnchor);
           m_Editor->setTextCursor(c);
           m_Editor->textCursor().removeSelectedText();
           after = QString( "point %1 %2 %3 %4 %5" )
                                .arg( name )
                                .arg( x0 )
                                .arg( y0 )
                                .arg( (int)(P.x()/m_ScaleFactor) )
                                .arg( (int)(P.y()/m_ScaleFactor) );
        }
    }
    else  {
        QRegularExpression rx("\\b(point)(\\s+)" + m_Name + coordinate + coordinate);
        QRegularExpressionMatchIterator i = rx.globalMatch(input);
        while (i.hasNext()) {
           QRegularExpressionMatch match = i.next();
           pos = match.capturedStart();
           c.setPosition(pos);
           pos += match.capturedLength();
           c.setPosition(pos, QTextCursor::KeepAnchor);
           m_Editor->setTextCursor(c);
           m_Editor->textCursor().removeSelectedText();
           after = QString( "point %1 %2 %3" )
                                .arg( m_Name )
                                .arg( (int)(P.x()/m_ScaleFactor) )
                                .arg( (int)(P.y()/m_ScaleFactor) );
        }
    }
    m_Item->setPos(this->scenePos().x()-m_Offsetx,
                 this->scenePos().y()-m_Offsety);
    m_Editor->insertPlainText(after);
    m_Editor->update();

    mouseEvent->setAccepted(false);
    QGraphicsEllipseItem::mouseMoveEvent(mouseEvent);
}

// --------------------------------------------------------------------------------------------
