#ifndef LINE_POINTS_H
#define LINE_POINTS_H

#include <QPoint>

int intcmp(int x1, int x2)
{
  return
    (x1 > x2) ? 1 :
    (x1 < x2) ? -1 : 0;
}

//Итерация по точкам линии с вызовом функции (функтора) для каждой точки
//Прототип функции должен допускать вызов f(p), где p имеет тип QPoint

template<typename F>
void foreach_line_point(const QPoint& p1, const QPoint& p2, F&& f)
{
  auto dx = std::abs(p2.x() - p1.x()) + 1;
  auto dy = std::abs(p2.y() - p1.y()) + 1;

  auto xmax = dx >= dy;

  auto dmax = std::max(dx, dy);
  auto dmin = std::min(dx, dy);

  dx = intcmp(p2.x(), p1.x()); //единичное смещение по x (-1, 1, 0)
  dy = intcmp(p2.y(), p1.y()); //единичное смещение по y

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
    f(cur1);
    if (cur2_x1 == cur1_x1)
      break;
    cur1_x1 += d1;
    f(cur2);
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

#endif // LINE_POINTS_H
