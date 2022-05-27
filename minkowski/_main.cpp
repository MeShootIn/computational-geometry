#include <iostream>
#include <vector>

struct Point {
  double x;
  double y;
};

void scan_vector(std::vector<Point> &A, int n) {
  for (int i = 0; i < n; i++) {
    std::cin >> A[i].x >> A[i].y;
  }
}

int index_left_down(std::vector<Point> &A, int n) {
  int ans = 0;

  for (int i = 1; i < n; i++) {
    if (A[i].y < A[ans].y && A[i].x < A[ans].x) {
      ans = i;
    }
  }

  return ans;
}

std::vector<Point> solve(std::vector<Point> &A, std::vector<Point> &B, int n,
                         int m) {
  std::vector<bool> usedA(n, false), usedB(m, false);
  int iA = index_left_down(A, n);
  int iB = index_left_down(B, m);
  std::vector<Point> S((n + 1) + (m + 1));
  int iS = 1;

  auto pushS = [&S, &iS](Point a, Point b) -> void {
    S[iS].x = a.x - b.x + S[iS - 1].x;
    S[iS].y = a.y - b.y + S[iS - 1].y;
    iS++;
  };

  A[n] = A[0];
  B[m] = B[0];
  S[0].x = A[iA].x + B[iB].x;
  S[0].y = A[iA].y + B[iB].y;

  while (true) {
    if (iA == n) {
      iA = 0;
    }

    if (iB == m) {
      iB = 0;
    }

    if (usedA[iA] && usedB[iB]) {
      break;
    }

    if ((A[iA + 1].x - A[iA].x) * (B[iB + 1].y - B[iB].y) -
                (A[iA + 1].y - A[iA].y) * (B[iB + 1].x - B[iB].x) <
            0 &&
        (usedA[iA] || !usedB[iB])) {
      pushS(B[iB + 1], B[iB]);
      usedB[iB++] = true;
    } else {
      pushS(A[iA + 1], A[iA]);
      usedA[iA++] = true;
    }
  }

  std::vector<Point> ans(n + m);

  for (int i = 0; i < n + m; ++i) {
    ans[i] = S[i];
  }

  return ans;
}

int main() {
  std::cout.precision(0);
  std::cout.setf(std::ios::fixed, std::ios::floatfield);

  int n, m;
  std::cin >> n >> m;

  std::vector<Point> A(n + 1), B(m + 1);
  scan_vector(A, n);
  scan_vector(B, m);

  std::vector<Point> ans = solve(A, B, n, m);

  for (int i = 0; i < (int)ans.size(); i++) {
    std::cout << ans[i].x << " " << ans[i].y << "\n";
  }

  // system("PAUSE");
  return 0;
}
