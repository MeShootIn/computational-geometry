#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Point {
  ll x, y;
};

ll get_area(vector<Point> P) {
  int N = P.size();
  ll s = 0;

  for(int i = 1; i < N; i++) {
    s += P[i - 1].x * P[i].y - P[i].x * P[i - 1].y;
  }

  s += P[N - 1].x * P[0].y - P[N - 1].y * P[0].x;

  double semi_area = s / 2.;

  return llround(fabs(semi_area));
}

ll gcd(ll a, ll b) {
	while (b) {
		a %= b;
		swap (a, b);
	}

	return a;
}

ll on_edge(Point p1, Point p2) {
  return gcd(abs(p1.x - p2.x), abs(p1.y - p2.y)) - 1;
}

ll solve(vector<Point> P) {
  int N = P.size();
  ll S = get_area(P);
  ll edge = 0;

  for(int i = 0; i < N; i++) {
    edge += on_edge(P[i], P[(i + 1) % N]);
  }

  edge += N;

  return S - edge / 2 + 1;
}

int main() {
  int N;
  cin >> N;

  vector<Point> P(N);

  for(int i = 0; i < N; i++) {
    cin >> P[i].x >> P[i].y;
  }

  cout << solve(P);

  return 0;
}
