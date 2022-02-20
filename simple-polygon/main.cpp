/**
 * Простой многоугольник на множестве вершин.
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

int const UNDEF = -2;
double const EPS = 1e-6;

/**
 * Точка - псевдоним вектора.
 */
typedef class Vector Point;

/**
 * Вектор.
 */
class Vector
{
public:
    double x;
    double y;

    Vector();
    Vector(double x, double y);
    Vector(Point const &start, Point const &end);
    /**
     * Конструктор копирования.
     */
    Vector(Vector const &v);

    /**
     * Оператор присваивания.
     */
    Vector &operator=(Vector const &v);
    /**
     * Унарный минус.
     */
    Vector operator-() const;

    /**
     * Полярный угол в диапозоне [-M_PI, M_PI).
     */
    double angle() const;

    /**
     * Длина вектора.
     */
    double length() const;
};

Vector::Vector() : x(0.), y(0.) {}

Vector::Vector(double x, double y) : x(x), y(y) {}

Vector::Vector(Point const &start, Point const &end) : Vector(end.x - start.x, end.y - start.y) {}

Vector::Vector(Vector const &v) : Vector(v.x, v.y) {}

Vector &Vector::operator=(Vector const &v)
{
    if (this != &v)
    {
        this->x = v.x;
        this->y = v.y;
    }

    return *this;
}

Vector operator+(Vector const &a, Vector const &b)
{
    return Vector(a.x + b.x, a.y + b.y);
}

Vector operator-(Vector const &a, Vector const &b)
{
    return a + (-b);
}

Vector operator*(Vector const &v, double d)
{
    return Vector(v.x * d, v.y * d);
}

Vector operator*(double d, Vector const &v)
{
    return v * d;
}

Vector operator/(Vector const &v, double d)
{
    return v * (1. / d);
}

Vector Vector::operator-() const
{
    return *this * (-1.);
}

Vector &operator+=(Vector &a, Vector const &b)
{
    a = a + b;

    return a;
}

Vector &operator-=(Vector &a, Vector const &b)
{
    a = a - b;

    return a;
}

Vector &operator*=(Vector &v, double d)
{
    v = v * d;

    return v;
}

Vector &operator/=(Vector &v, double d)
{
    v = v / d;

    return v;
}

double Vector::angle() const
{
    return atan2(y, x);
}

double Vector::length() const
{
    return sqrt(x * x + y * y);
}

/**
 * a == b с погрешностью EPS.
 */
bool equals(double a, double b)
{
    return fabs(a - b) <= EPS;
}

/**
 * a <= b с погрешностью EPS.
 */
bool lessOrEquals(double a, double b)
{
    return b - a >= EPS;
}

/**
 * Центр тяжести простого многоугольника.
 * Время: O(N), где N = |points|.
 * @param points точки многоугольника.
 * @return Если N > 0, то точка центра тяжести. Иначе Point(nan, nan).
 */
Point gravityCenter(vector<Point> const &points)
{
    auto N = points.size();
    Vector q;

    for (auto const &p : points)
    {
        q += p;
    }

    q /= N;

    return q;
}

/**
 * Простой многоугольник (самонепересекающаяся замкнутая ломанная) на множестве точек.
 * Время: O(N*log(N)), где N = |points|.
 * @param points исходный вектор точек.
 * @return вектор точек простого многоугольника.
 */
vector<Point> simplePolygon(vector<Point> const &points)
{
    // Центр тяжести множества точек
    // O(N)
    vector<Point> poly = points;
    auto q = gravityCenter(poly);

    // Сортировка точек по неубыванию полярного угла и по невозрастанию расстояния относительно центра тяжести
    // O(N*log(N))
    sort(
        poly.begin(),
        poly.end(),
        [q](Point const &a, Point const &b) -> bool
        {
            Vector v1(q, a);
            Vector v2(q, b);

            if (equals(v1.angle(), v2.angle()))
            {
                return v1.length() < v2.length();
            }

            return v1.angle() < v2.angle();
        });

    return poly;
}

int main()
{
    size_t N;
    cin >> N;

    vector<Point> points;
    points.reserve(N);

    for (size_t i = 0; i < N; ++i)
    {
        Point p;
        cin >> p.x >> p.y;

        points.push_back(p);
    }

    auto poly = simplePolygon(points);

    for (auto const &p : poly)
    {
        cout << p.x << " " << p.y << endl;
    }

    return 0;
}