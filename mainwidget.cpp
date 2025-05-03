#include "mainwidget.h"
#include "line_points.h"
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

void MainWidget::drawLine(QPainter& painter)
{
  foreach_line_point(p1, p2, [this, &painter](const QPoint& p)
  {
    this->drawPoint(p, painter);
  });
}

void MainWidget::drawPoint(const QPoint& p, QPainter& painter)
{
  auto center = cell_size >> 1; //деление на 2
  auto radius = cell_size >> 2; //деление на 4

  painter.drawEllipse(QPoint{p.x() * cell_size + center, p.y() * cell_size + center}, radius, radius);
}
