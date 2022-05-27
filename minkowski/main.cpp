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

double cross_prod(Point V, Point W) {
  return V.x*W.y - W.x*V.y;
}

void scan_vector(std::vector<Point> &A) {
  for (int i = 0; i < A.size(); i++) {
    std::cin >> A[i].x >> A[i].y;
  }
}

bool eq(double a, double b) {
  return fabs(a - b) <= 1e-10;
}

void preprocess(std::vector<Point> &P) {
  int index = 0;
  for (int i = 1; i < P.size(); i++) {
    if (P[i].y < P[index].y || (P[i].y == P[index].y && P[i].x < P[index].x)) {
      index = i;
    }
  }
  std::rotate(P.begin(), P.begin() + index, P.end());
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
    if(cross > 0 || eq(cross, 0))
        ++i;
    if(cross < 0 || eq(cross, 0))
        ++j;
  }

  return S;
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
  
  std::vector<Point> S = solve(V, W);
  
  for(size_t i = 0; i < S.size(); i++) {
    std::cout << S[i].x << " " << S[i].y << "\n";
  }

  return 0;
}
