#include "mainwidget.h"
#include "QVBoxLayout"
#include "QMouseEvent"
#include "QPainter"

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent)
{
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::RightButton)
  {
    QWidget::mousePressEvent(event);
    return;
  }

  auto& p = (p1.x() < 0) ? p1 : p2;
  p = event->pos();
  p.rx() /= cell_size;
  p.ry() /= cell_size;
  update();
}

void MainWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);

  //Рисуем сетку
  for (int x = cell_size; x < width(); x += cell_size)
    painter.drawLine(x, 0, x, height());
  for (int y = cell_size; y < height(); y += cell_size)
    painter.drawLine(0, y, width(), y);

  if (p1.x() < 0)
    return;

  if (p2.x() >= 0 && p2 != p1)
  {
    drawLine(painter);
    p1 = p2 = {-1, -1};
  }
  else
    drawPoint(p1, painter);
}

int cmp(int x1, int x2)
{
  return
    (x1 > x2) ? 1 :
    (x1 < x2) ? -1 : 0;
}

void MainWidget::drawLine(QPainter& painter)
{
  auto dx = std::abs(p2.x() - p1.x()) + 1;
  auto dy = std::abs(p2.y() - p1.y()) + 1;

  auto xmax = dx >= dy;

  auto dmax = std::max(dx, dy);
  auto dmin = std::min(dx, dy);

  dx = cmp(p2.x(), p1.x()); //единичное смещение по x (-1, 1, 0)
  dy = cmp(p2.y(), p1.y()); //единичное смещение по y

  auto d1 = xmax ? dx : dy; //единичное смещение по ведущей (длинной) координате
  auto d2 = xmax ? dy : dx; //единичное смещение по ведомой (короткой) координате

  //Линия рисуется как набор малых отрезков, у которых начальная и конечная точка отличаются только ведущей координатой
  //Каждый такой отрезок виртуально представляем как прямоугольник размером dmax на dmin
  //Точку виртуально представляем как квадрат со стороной dmin
  //Общее число точек линии - dmax
  //В цикле по точкам движемся с двух сторон, встречаемся в центре
  //Ведущая координата смещается на каждом шаге цикла,
  //а ведомая смещается, когда центр следующей виртуальной точки по ведущей координате окажется на следующем виртуальном отрезке,
  //то есть выйдет за длину текущего виртуального отрезка (dmax).
  //Центр виртуальной точки стартует с dmin/2, и на каждом шаге смещается на размер виртуальной точки (dmin)

  auto cur1 = p1;
  auto& cur1_x1 = xmax ? cur1.rx() : cur1.ry(); //ведущая координата
  auto& cur1_x2 = xmax ? cur1.ry() : cur1.rx(); //ведомая координата

  auto cur2 = p2;
  auto& cur2_x1 = xmax ? cur2.rx() : cur2.ry();
  auto& cur2_x2 = xmax ? cur2.ry() : cur2.rx();

  auto vp_center = dmin >> 1; //деление на 2

  for (; ; )
  {
    drawPoint(cur1, painter);
    if (cur2_x1 == cur1_x1)
      break;
    cur1_x1 += d1;
    drawPoint(cur2, painter);
    if (cur1_x1 == cur2_x1)
      break;
    cur2_x1 -= d1;
    vp_center += dmin;
    if (vp_center >= dmax)
    {
      cur1_x2 += d2;
      cur2_x2 -= d2;
      vp_center -= dmax;
    }
  }
}

void MainWidget::drawPoint(const QPoint& p, QPainter& painter)
{
  auto center = cell_size >> 1; //деление на 2
  auto radius = cell_size >> 2; //деление на 4

  painter.drawEllipse(QPoint{p.x() * cell_size + center, p.y() * cell_size + center}, radius, radius);
}
