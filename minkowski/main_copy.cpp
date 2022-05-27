#include <iostream>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <math.h>

struct Point {
  double x;
  double y;

  Point();
  Point(double x, double y);
  Point(Point const &A, Point const &B);
};

Point::Point() : x(0), y(0) {}

Point::Point(double x, double y) : x(x), y(y) {}

Point::Point(Point const &A, Point const &B) : Point(B.x - A.x, B.y - A.y) {}

Point sum(Point V, Point W) {
  return Point(V.x + W.x, V.y + W.y);
}

Point sub(Point V, Point W) {
  return Point(V.x - W.x, V.y - W.y);
}

Point div(Point P, double N) {
  return Point(P.x / N, P.y / N);
}

double cross_prod(Point V, Point W) {
  return V.x*W.y - W.x*V.y;
}

double angle(Point &A) {
  return atan2(A.y, A.x);
}

double angle(Point &A, Point &B) {
  Point C = Point(A, B);

  return angle(C);
}

double len(Point &A) {
  return sqrt(A.x*A.x + A.y*A.y);
}

void scan_vector(std::vector<Point> &A) {
  for (int i = 0; i < A.size(); i++) {
    std::cin >> A[i].x >> A[i].y;
  }
}

Point get_center(std::vector<Point> &V) {
  int N = V.size();
  Point q;

  for (int i = 0; i < N; i++) {
    q = sum(q, V[i]);
  }

  q = div(q, N);

  return q;
}

bool eq(double a, double b) {
  return fabs(a - b) <= 1e-10;
}

int cmp(Point &A, Point &B) {
  return (A.x < B.x && A.y < B.y) ? -1 : 1;
}

int find_left_down_index(std::vector<Point> &P) {
  int index = 0;
  for (int i = 1; i < P.size(); i++) {
    if (P[i].y < P[index].y || (P[i].y == P[index].y && P[i].x < P[index].x)) {
    //if (cmp(P[i], P[index]) < 0) {
      index = i;
    }
  }
  return index;
}

void preprocess(std::vector<Point> &A) {
  Point q = get_center(A);

  sort(
    A.begin(),
    A.end(),
    [q](Point const &a, Point const &b) -> bool {
      Point p1(q, a);
      Point p2(q, b);
      double l1 = len(p1);
      double l2 = len(p2);
      double ang1 = angle(p1);
      double ang2 = angle(p2);

      if (eq(ang1, ang2)) {
        return l1 < l2 || eq(l1, l2);
      }

      return ang1 < ang2 || eq(ang1, ang2);
    }
  );

  int index = find_left_down_index(A);
  std::rotate(A.begin(), A.begin() + index, A.end());
}

std::vector<Point> solve(std::vector<Point> &V, std::vector<Point> &W) {
  preprocess(V);
  preprocess(W);

  V.push_back(V[0]);
  V.push_back(V[1]);
  W.push_back(W[0]);
  W.push_back(W[1]);

  std::vector<Point> S;
  int i, j;
  i = j = 0;

  while(i < V.size() - 2 || j < W.size() - 2) {
    S.push_back(sum(V[i], W[j]));
    
    double cross = cross_prod(sub(V[i + 1], V[i]), sub(W[j + 1], W[j]));
    if(cross >= 0)
        ++i;
    if(cross <= 0)
        ++j;

    /*
    if (angle(V[i], V[i+1]) < angle(W[j], W[j+1])) {
      i++;
    }
    else if (angle(V[i], V[i+1]) > angle(W[j], W[j+1])) {
      j++;
    }
    else {
      i++;
      j++;
    }
    */
  }

  return S;
}
// https://cp-algorithms.com/geometry/minkowski.html#implementation
struct pt{
    long long x, y;
    pt operator + (const pt & p) const {
        return pt{x + p.x, y + p.y};
    }
    pt operator - (const pt & p) const {
        return pt{x - p.x, y - p.y};
    }
    long long cross(const pt & p) const {
        return x * p.y - y * p.x;
    }
};

void reorder_polygon(std::vector<pt> & P){
    size_t pos = 0;
    for(size_t i = 1; i < P.size(); i++){
        if(P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x))
            pos = i;
    }
    std::rotate(P.begin(), P.begin() + pos, P.end());
}
// TODO СОРТИРОВКА
std::vector<pt> minkowski(std::vector<pt> P, std::vector<pt> Q){
    // the first vertex must be the lowest
    reorder_polygon(P);
    reorder_polygon(Q);
    // we must ensure cyclic indexing
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    // main part
    std::vector<pt> result;
    size_t i = 0, j = 0;
    while(i < P.size() - 2 || j < Q.size() - 2){
        result.push_back(P[i] + Q[j]);
        auto cross = (P[i + 1] - P[i]).cross(Q[j + 1] - Q[j]);
        if(cross >= 0)
            ++i;
        if(cross <= 0)
            ++j;
    }
    return result;
}

void plot(std::vector<Point> P, int f) {
  // plot(polyshape([0 0 1 1],[1 0 0 1]))
  std::cout << "f" << f << " = figure;\n";
  std::cout << "plot(polyshape([";
  for(int i = 0; i < P.size(); i++) {
    std::cout << P[i].x;

    if(i != P.size() - 1) {
      std::cout << " ";
    }
  }
  std::cout << "],[";
  for(int i = 0; i < P.size(); i++) {
    std::cout << P[i].y;

    if(i != P.size() - 1) {
      std::cout << " ";
    }
  }
  std::cout << "]));\n";
}

int main() {
  std::cout.precision(0);
  std::cout.setf(std::ios::fixed, std::ios::floatfield);

  int N, M;
  std::cin >> N >> M;

  std::vector<Point> V(N);
  std::vector<Point> W(M);
  scan_vector(V);
  scan_vector(W);
  plot(V, 1);
  plot(W, 2);
  
  std::vector<Point> S = solve(V, W);
  //std::vector <pt> V(N);
  //std::vector <pt> W(M);
  //for(size_t i = 0; i < N; i++) {
  //  std::cin >> V[i].x >> V[i].y;
  //}
  //for(size_t i = 0; i < M; i++) {
  //  std::cin >> W[i].x >> W[i].y;
  //}

  //std::vector<pt> S = minkowski(V, W);

  plot(S, 3);
  /*std::cout << "|S| = " << S.size() << "\n";
  for(size_t i = 0; i < S.size(); i++) {
    std::cout << S[i].x << " " << S[i].y << "\n";
  }*/

  return 0;
}
