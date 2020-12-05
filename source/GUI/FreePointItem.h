#ifndef FREEPOINTITEM_H
#define FREEPOINTITEM_H

#include <QTextEdit>
#include <QTextStream>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>


class FreePointItem : public QGraphicsEllipseItem
{
public:

    FreePointItem();
    void setParams(QTextEdit* editor, qreal scaleFactor, QString name,
                   QGraphicsSimpleTextItem* item, qreal offsetX, qreal offsetY, bool animationPoint);

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:

    QString m_Name;
    QTextEdit* m_Editor;
    qreal m_ScaleFactor;
    QGraphicsScene *m_Scene;
    qreal scaleFactor, m_Offsetx, m_Offsety;
    bool m_bAnimationPoint;
    QGraphicsSimpleTextItem* m_Item;
};



#endif // FREEPOINTITEM_H
