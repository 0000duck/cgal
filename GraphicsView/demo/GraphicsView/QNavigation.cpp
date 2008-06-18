#include "QNavigation.h"
#include <cmath>
#include <iostream>
#include <boost/format.hpp>

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QFlags>
#include <QScrollBar>

namespace CGAL {

  QNavigation::QNavigation(QGraphicsView* v_)
    : v(v_),
      rectItem(new QGraphicsRectItem),
      dragging(false)
  {
    QColor rect_color(250, 221, 0);
    rect_color.setAlpha(50);
    rectItem->setBrush(rect_color);
    rect_color.setAlpha(255);
    rectItem->setPen(QPen(rect_color, 0, Qt::DashLine));
    rectItem->hide();
    rectItem->setZValue(10000);
  }

  QNavigation::~QNavigation()
  {
    delete rectItem;
  }

  bool 
  QNavigation::eventFilter(QObject *obj, QEvent *event)
  {
    switch(event->type()) 
    {
    case QEvent::KeyPress: {
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
      int offset = 10;
      if( (keyEvent->modifiers() & Qt::ShiftModifier)
          || (keyEvent->modifiers() & Qt::ControlModifier) ) {
        offset = 20;
      }
      switch (keyEvent->key()) {
      case Qt::Key_Up:
        translateView(0, -offset);
        break;
      case Qt::Key_Down:
        translateView(0, offset);
        break;
      case Qt::Key_Left:
        translateView(-offset, 0);
        break;
      case Qt::Key_Right:
        translateView(offset, 0);
        break;
      case Qt::Key_PageUp:
        v->rotate(-6);
        break;
      case Qt::Key_PageDown:
        v->rotate(6);
        break;
      case Qt::Key_Plus:
        scaleView(1.2);
        break;
      case Qt::Key_Minus:
        scaleView(1 / 1.2);
        break;
      default:
        return false;
      }
      //         display_parameters();
      return true;
      break;
    } // end case KeyPress
    case QEvent::KeyRelease: {
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
      if(keyEvent->key() == Qt::Key_Control && rectItem->isVisible() ) {
        dragging = false;
        v->setCursor(cursor_backup);
        v->scene()->removeItem(rectItem);
        rectItem->hide();
        return true;
      }
      return false;
      break;
    } // end case KeyRelease
    case QEvent::Wheel: {
      QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
      if(wheelEvent->orientation() != Qt::Vertical) {
        return false;
      }
      double zoom_ratio = 240.0;
      if( (wheelEvent->modifiers() & Qt::ShiftModifier)
          || (wheelEvent->modifiers() & Qt::ControlModifier) ) {
        zoom_ratio = 120.0;
      }
      scaleView(pow((double)2, -wheelEvent->delta() / zoom_ratio));

      //         display_parameters();
      return true;
      break;
    } // end case Wheel
    case QEvent::MouseButtonPress: {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if( mouseEvent->modifiers() == Qt::ControlModifier ) {
        if(mouseEvent->button() == Qt::LeftButton) {
          rect_first_point = v->mapToScene(mouseEvent->pos());
          rectItem->setRect(QRectF(rect_first_point, rect_first_point));
          rectItem->show();
          v->scene()->addItem(rectItem);
          return true;
        }
        else if( mouseEvent->button() == Qt::RightButton) {
          dragging = true;
          dragging_start = v->mapToScene(mouseEvent->pos());
          cursor_backup = v->cursor();
          v->setCursor(Qt::ClosedHandCursor);
          return true;
        }
      }
      return false;
      break;
    } // end case MouseRelease
    case QEvent::MouseMove: {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      QPointF pos = v->mapToScene(mouseEvent->pos());
      QString xy = QString(" ") + QString::number(pos.x(),'g', 6) + " , " + QString::number(pos.y(),'g', 6) + " ";
      emit mouseCoordinates(xy);
      if(rectItem->isVisible()) {
        rectItem->setRect(QRectF(rect_first_point,
                                 v->mapToScene(mouseEvent->pos())));
      }
      if( dragging )
      {
//         std::cerr << boost::format("mouseMove: globalpos=(%1%, %2%)\n"
//                                    "           pos=(%3%, %4%)\n"
//                                    "           sender=%5% (class %6%), parent class %7%\n")
//           % mouseEvent->globalPos().x()
//           % mouseEvent->globalPos().y()
//           % mouseEvent->pos().x()
//           % mouseEvent->pos().y()
//           % (&*obj)
//           % obj->metaObject()->className()
//           % obj->parent()->metaObject()->className();

//         drag_to(mouseEvent->pos());
      }
      break;
    } // end MouseMove
    case QEvent::MouseButtonRelease: {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if(rectItem->isVisible() && mouseEvent->button() == Qt::LeftButton){
        v->setSceneRect(v->sceneRect() | rectItem->rect());
        v->fitInView(rectItem->rect(), Qt::KeepAspectRatio);
        v->scene()->removeItem(rectItem);
        rectItem->hide();
        return true;
      }
      else if(  mouseEvent->button() == Qt::RightButton ) {
        if(dragging) {
          dragging = false;
          drag_to(mouseEvent->pos());
          v->setCursor(cursor_backup);
          return true;
        }
      }
      return false;
      break;
    } // end MouseRelease
    } // end switch
    return false;
  }


  void 
  QNavigation::scaleView(qreal scaleFactor)
  {
    QPointF center = v->mapToScene(v->viewport()->rect().center());
//     qreal factor = v->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    //if (factor < 0.001 || factor > 2000)
    //    return;

    v->scale(scaleFactor, scaleFactor);
    QPoint offset = v->mapFromScene(center) - v->viewport()->rect().center();
    translateView(offset.x(), offset.y());
  }

  void QNavigation::drag_to(QPoint new_pos)
  {
    QPoint dragging_start_in_view = v->mapFromScene(dragging_start);
    QPoint offset = new_pos - dragging_start_in_view;
//     std::cerr << boost::format("drag_to: origin=(%1%, %2%)\n"
//                                "         offset=(%3%, %4%)\n")
//       % dragging_start_in_view.x() % dragging_start_in_view.y()
//       % offset.x() % offset.y();
    translateView(-offset.x(), -offset.y());
    dragging_start_in_view = v->mapFromScene(dragging_start);
//     std::cerr << boost::format("         after=(%1%, %2%)\n")
//       % dragging_start_in_view.x() % dragging_start_in_view.y();
  }

  QRectF QNavigation::mapToScene(QRect rect) const
  {
    QPointF top_left = v->mapToScene(rect.topLeft());
    QPointF bottom_right = v->mapToScene(rect.bottomRight());
    return QRectF(top_left, bottom_right);
  }

  void QNavigation::translateView(int dx,  int dy)
  {
    if( dx == 0 && dy == 0 ) {
      return;
    }

    int horizontalScrollBarValue = v->horizontalScrollBar()->value();
    int verticalScrollBarValue = v->verticalScrollBar()->value();

    if( (horizontalScrollBarValue + dx <= 
         v->horizontalScrollBar()->maximum()) &&
        (horizontalScrollBarValue + dx >=
         v->horizontalScrollBar()->minimum()) &&
        (verticalScrollBarValue + dy <= 
         v->verticalScrollBar()->maximum()) &&
        (verticalScrollBarValue + dy >=
         v->verticalScrollBar()->minimum()) )
    {
      v->horizontalScrollBar()->setValue(horizontalScrollBarValue + dx);
      v->verticalScrollBar()->setValue(verticalScrollBarValue + dy);
    }
    else
    {
      QRect vp_rect = v->viewport()->rect();
      QPointF new_center = v->mapToScene(vp_rect.center() + QPoint(dx, dy));
      vp_rect |= vp_rect.translated(dx, dy);
      QRectF rect = mapToScene(vp_rect);
      v->setSceneRect(v->sceneRect() | rect);
      v->centerOn(new_center);

      // QGraphicsView::centerOn makes rounding errors.
      // The following two "if" make them unnoticable when dx==0 or dy==0.
      if(dx == 0) {
        v->horizontalScrollBar()->setValue(horizontalScrollBarValue);
      }
      if(dy == 0) {
        v->verticalScrollBar()->setValue(verticalScrollBarValue);
      }
    }
//     display_parameters();
  }

  void QNavigation::display_parameters()
  {
    std::cerr << 
      boost::format("matrix translation=(%1%, %2%)\n"
                    "       rotation=(%3% - %4% )\n"
                    "                (%5% - %6% )\n")
      % v->matrix().dx()
      % v->matrix().dy()
      % v->matrix().m11()
      % v->matrix().m12()
      % v->matrix().m21()
      % v->matrix().m22();

    QRect vp_rect = v->viewport()->rect();
    QPoint vp_top_left = vp_rect.topLeft();
    QPoint vp_bottom_right = vp_rect.bottomRight();
    QPointF top_left = v->mapToScene(vp_top_left);
    QPointF bottom_right = v->mapToScene(vp_bottom_right);

    std::cerr <<
      boost::format("view=(%1% - %2%) x (%3% - %4%)\n")
      % top_left.x() % bottom_right.x()
      % top_left.y() % bottom_right.y();
    std::cerr <<
      boost::format("viewport=(%1% - %2%) x (%3% - %4%)\n")
      % vp_top_left.x() % vp_bottom_right.x()
      % vp_top_left.y() % vp_bottom_right.y();
    std::cerr <<
      boost::format("scrollbars=(%1% <= %2% <= %3%) x (%4% <= %5% <= %6%)\n")
      % v->horizontalScrollBar()->minimum() 
      % v->horizontalScrollBar()->value()
      % v->horizontalScrollBar()->maximum()
      % v->verticalScrollBar()->minimum() 
      % v->verticalScrollBar()->value()
      % v->verticalScrollBar()->maximum();
  }

#include "QNavigation.moc"

} // namespace CGAL

