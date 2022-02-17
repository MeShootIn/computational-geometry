/**
 * Проверка выпуклой оболочки.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int const UNDEF = -2;
double const EPS = 1e-10;

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
 * Функция сигнум.
 * @param x вещественное число.
 * @return -1, 0 или 1.
 */
int sgn(double x)
{
    return (equals(x, 0.)) ? 0 : ((lessOrEquals(x, 0.)) ? -1 : 1);
}

/**
 * Знаковая площадь треугольника по точкам.
 */
double signedS(Point const &a, Point const &b, Point const &c)
{
    return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.;
}

/**
 * Проверка многоугольника на выпуклость. Многоугольник должен быть отсортирован по неубыванию (невозрастанию) полярного
 * угла относительно любой своей внутренней точки. Время: O(N), где N = |poly|.
 * @param poly точки многоугольника.
 */
bool isConvex(vector<Point> const &poly)
{
    auto N = poly.size();
    auto mainSgn = UNDEF;

    for (size_t i = 0; i < N; ++i)
    {
        auto S = signedS(poly[i], poly[(i + 1) % N], poly[(i + 2) % N]);
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

    auto sgn1 = sgn(signedS(poly[i1], q, poly[i2]));
    auto sgn2 = sgn(signedS(poly[i1], p, poly[i2]));

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
 * Центр тяжести простого многоугольника. Время: O(N), где N = |poly|.
 * @param poly точки многоугольника.
 * @return Если N > 0, то точка центра тяжести. Иначе Point(nan, nan).
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
 * Проверка выпуклой оболочки. Время: O(N*log(C)), где N = |points|, C = |hullIndexes|, причём N >= C.
 * @param points все точки.
 * @param hullIndexes индексы точек оболочки.
 */
bool convexHullCheck(vector<Point> const &points, vector<size_t> hullIndexes)
{
    auto N = points.size();
    auto C = hullIndexes.size();
    auto M = N - C;

    if (C == 0)
    {
        if (M == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Сортировка индексов по неубыванию
    sort(hullIndexes.begin(), hullIndexes.end()); // O(C*log(C))

    vector<Point> hull;
    hull.reserve(C); // O(C)
    vector<Point> Points;
    Points.reserve(M); // O(M)

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
    Vector q = polyCenter(hull); // O(C)

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
    return pointsInPoly(hull, q, Points); // O(M*log(C))
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
        cout << "error: C must be <= N";

        return 0;
    }

    vector<size_t> hullIndexes(C);

    for (size_t i = 0; i < C; ++i)
    {
        size_t index;
        cin >> index;

        if (!(0 <= index && index < N))
        {
            cout << "error: hullIndexes[" << i << "] must be >= 0 and < N";

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