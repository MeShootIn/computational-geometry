/**
 * Проверка выпуклой оболочки
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define UNDEF -2
#define EPS 1e-6

using namespace std;

// Точка - псевдоним вектора
typedef class Vector Point;

// Вектор
class Vector
{
public:
    double x;
    double y;

    Vector();
    Vector(double x, double y);
    Vector(Point const &start, Point const &end);
    // Конструктор копирования
    Vector(Vector const &v);

    // Оператор присваивания
    Vector &operator=(Vector const &v);
    // Унарный минус
    Vector operator-() const;

    // Полярный угол вектора в радианах в диапозоне [-M_PI, M_PI)
    double angle() const;
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

/**
 * a == b с погрешностью EPS.
*/
bool Equals(double a, double b)
{
    return fabs(a - b) <= EPS;
}

/**
 * a <= b с погрешностью EPS.
*/
bool LessOrEquals(double a, double b)
{
    return b - a >= EPS;
}

/**
 * Функция сигнум.
 * @param x вещественное число.
 * @return -1, 0 или 1.
 */
int sgn(double x)
{
    return (Equals(x, 0.)) ? 0 : ((LessOrEquals(x, 0.)) ? -1 : 1);
}

/**
 * Знаковая площадь треугольника по точкам. Время: O(1).
*/
double SignedS(Point const &a, Point const &b, Point const &c)
{
    return (a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x)) / 2.;
}

/**
 * Проверка оболочки на выпуклость. Оболочка должна быть отсортирована по неубыванию (невозрастанию) полярного угла 
 * относительно любой своей внутренней точки. Время: O(N), где N = |hull|.
 * @param hull проверяемая оболочка.
*/
bool isConvex(vector<Point> const &hull)
{
    auto N = hull.size();

    if (N <= 2)
    {
        return false;
    }

    auto mainSgn = UNDEF;

    for (size_t i = 0; i < N; ++i)
    {
        auto S = SignedS(hull[i], hull[(i + 1) % N], hull[(i + 2) % N]);
        auto sgnS = sgn(S);

        if (sgnS == 0)
        {
            continue;
        }

        if (mainSgn == UNDEF)
        {
            mainSgn = sgnS;
        }
        else if (sgnS != mainSgn)
        {
            return false;
        }
    }

    return true;
}

/**
 * Проверка принадлежности точки многоугольнику. Многоугольник должен быть отсортирован по неубыванию (невозрастанию) 
 * полярного угла относительно внутренней точки q. Время: O(log(N)), где N = |poly| = |polyAngles|.
 * @param poly вершины многоугольника.
 * @param polyAngles полярные углы вершин относительно q.
 * @param q любая внутренняя точка многоугольника.
 * @param p проверяемая точка.
*/
bool pointInPoly(vector<Point> const &poly, vector<double> const &polyAngles, Point const &q, Point const &p)
{
    auto N = poly.size();
    Vector pVector(q, p);
    double pAngle = pVector.angle();
    size_t i2 = lower_bound(polyAngles.begin(), polyAngles.end(), pAngle) - polyAngles.begin();
    size_t i1 = i2 - 1;

    if (i2 == 0)
    {
        i1 = N - 1;
    }

    if (i2 == N)
    {
        i1 = N - 1;
        i2 = 0;
    }

    auto sgn1 = sgn(SignedS(poly[i1], q, poly[i2]));
    auto sgn2 = sgn(SignedS(poly[i1], p, poly[i2]));

    if (sgn1 * sgn2 < 0 || (sgn1 == 0 && sgn2 != 0))
    {
        return false;
    }

    return true;
}

/**
 * Проверка принадлежности точек многоугольнику. Многоугольник должен быть отсортирован по неубыванию (невозрастанию) 
 * полярного угла относительно внутренней точки q. Время: O(N*log(M) + M), где N = |points|, M = |poly|.
 * @param poly вершины многоугольника.
 * @param q любая внутренняя точка многоугольника.
 * @param points проверяемые точки.
*/
bool pointsInPoly(vector<Point> const &poly, Point const &q, vector<Point> const &points)
{
    auto M = poly.size();
    vector<double> angles(M);

    for (size_t i = 0; i < M; ++i)
    {
        angles[i] = Vector(q, poly[i]).angle();
    }

    for (auto const &p : points)
    {
        if (!pointInPoly(poly, angles, q, p))
        {
            return false;
        }
    }

    return true;
}

/**
 * Центр тяжести треугольника. Время: O(1).
*/
Point triangleCenter(Point const &a, Point const &b, Point const &c)
{
    return (a + b + c) / 3.;
}

/**
 * Центр тяжести простого многоугольника. Время: O(N), где N = |poly|.
 * @param poly точки многоугольника.
*/
Point polyCenter(vector<Point> const &poly)
{
    auto N = poly.size();
    Vector q;

    for (auto const &p : poly)
    {
        q += p;
    }

    q /= N;

    return q;
}

/**
 * Проверка выпуклой оболочки. Время: O(N*log(C)), где N = |points|, C = |hullIndexes|.
 * @param points все точки.
 * @param hullIndexes индексы точек оболочки.
*/
bool convexHullCheck(vector<Point> const &points, vector<size_t> hullIndexes)
{
    auto N = points.size();
    auto C = hullIndexes.size();
    vector<Point> hull;
    // O(C)
    hull.reserve(C);

    auto M = N - C;
    vector<Point> Points;
    // O(M)
    Points.reserve(M);

    // Сортировка индексов по неубыванию
    // O(C*log(C))
    sort(hullIndexes.begin(), hullIndexes.end());

    // Заполнение массивов
    // O(M + C)
    for (size_t i = 0, c = 0; i < N; ++i)
    {
        if (i == hullIndexes[c])
        {
            hull.push_back(points[i]);
            ++c;
        }
        else
        {
            Points.push_back(points[i]);
        }
    }

    // Поиск центра тяжести оболочки
    // O(C)
    Vector q = polyCenter(hull);

    // Сортировка точек оболочки по неубыванию полярного угла относительно центра тяжести
    // O(C*log(C))
    sort(
        hull.begin(),
        hull.end(),
        [q](Point const &a, Point const &b) -> bool
        {
            Vector v1(q, a);
            Vector v2(q, b);

            return v1.angle() < v2.angle();
        });

    // Проверка оболочки на выпуклость
    // O(C)
    if (!isConvex(hull))
    {
        return false;
    }

    // Проверка принадлежности внутренних точек оболочке
    // O(M*log(C))
    return pointsInPoly(hull, q, Points);
}

int main()
{
    size_t N;
    cin >> N;

    vector<Point> points(N);

    for (size_t i = 0; i < N; ++i)
    {
        Point p;
        cin >> p.x >> p.y;

        points[i] = p;
    }

    size_t C;
    cin >> C;

    if (C > N)
    {
        cout << "error: C must be such that it is <= N";

        return 0;
    }

    vector<size_t> hullIndexes(C);

    for (size_t i = 0; i < C; ++i)
    {
        size_t index;
        cin >> index;

        if (!(0 <= index && index < N))
        {
            cout << "error: hullIndexes[" << i << "] must be such that it is >= 0 and < N";

            return 0;
        }

        hullIndexes[i] = index;
    }

    if (convexHullCheck(points, hullIndexes))
    {
        cout << "Yes";
    }
    else
    {
        cout << "No";
    }

    return 0;
}