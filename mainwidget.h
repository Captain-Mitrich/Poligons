#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  MainWidget(QWidget *parent = nullptr);

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

  //Прямая линия от p1 до p2 включительно
  void drawLine(QPainter& painter);

  void drawPoint(const QPoint& p, QPainter& painter);

private:
  QPoint p1{-1, -1}, p2{-1, -1};

  static const int cell_size = 24;
};

#endif // MAINWIDGET_H
