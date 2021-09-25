#include <stdio.h>
#include <tuple>
#include <vector>
#include <cstring>
#include <cmath>
#include <windows.h>
#include <time.h>

#pragma GCC optimize("Ofast,unroll-loops,no-stack-protector,fast-math")

using namespace std;

typedef tuple<float, float, float> point;

const int N = 10, height = N << 2, width = N << 2;
const point light = {-1, 0, -1};
const char shade[] = ".,-~:;=!*#$@";

inline float dot(const point& v1, const point& v2) {
	return {get<0>(v1) * get<0>(v2) + get<1>(v1) * get<1>(v2) + get<2>(v1) * get<2>(v2)};
}

inline void rotate_x(point& p, const float& c, const float& s) {
	float x, y, z;
	tie(x, y, z) = p;
	get<1>(p) = y * c + z * s;
	get<2>(p) = -y * s + z * c;
}

inline void rotate_z(point& p, const float& c, const float& s) {
	float x, y, z;
	tie(x, y, z) = p;
	get<0>(p) = x * c + y * s;
	get<1>(p) = -x * s + y * c;
}

void init(vector<point>& arr, vector<point>& normal) {
	// top and bottom
	for(int x = -N; x <= N; ++x) {
		for(int y = -N; y <= N; ++y) {
			for(int z : {-N, N}) {
				arr.emplace_back(x, y, z);
				normal.emplace_back(0, 0, z > 0 ? 1 : -1);
			}
		}
	}
	// left and right
	for(int x = -N; x <= N; ++x) {
		for(int z = -N; z <= N; ++z) {
			for(int y : {-N, N}) {
				arr.emplace_back(x, y, z);
				normal.emplace_back(0, y > 0 ? 1 : -1, 0);
			}
		}
	}
	// front and back
	for(int y = -N; y <= N; ++y) {
		for(int z = -N; z <= N; ++z) {
			for(int x : {-N, N}) {
				arr.emplace_back(x, y, z);
				normal.emplace_back(x > 0 ? 1 : -1, 0, 0);
			}
		}
	}
}

void show_cube(const vector<point>& p, const vector<point>& normal) {
	static char buffer[height][width];
	static int mask[height][width];
	static float a, b, c;
	const int n = p.size();
	memset(mask, -1, sizeof(mask));
	for(int i = 0; i < n; ++i) {
		tie(a, b, c) = p[i];
		int y = b + N * 2, z = c + N * 2;
		if(y >= 0 && y < width && z >= 0 && z < height) {
			int brightness = dot(light, normal[i]) * 8;
			if(brightness >= 0) {
				if(brightness < 12)
					mask[z][y] = max(mask[z][y], brightness);
				else
					mask[z][y] = 11;
			} else
				mask[z][y] = max(mask[z][y], 0);
		}
	}
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j)
			buffer[i][j] = ~mask[i][j] ? shade[mask[i][j]] : ' ';
		buffer[i][width - 1] = '\0';
	}
	system("cls");
	for(int i = 0; i < height; ++i)
		puts(buffer[i]);
	_sleep(0);
}

void rotate_3D_cube() {
	vector<point> p, normal;
	init(p, normal);
	const int n = p.size();
	float A = 0, B = 0;
	while(true) {
		show_cube(p, normal);
		float ca = cos(A), sa = sin(A);
		float cb = cos(B), sb = sin(B);
		for(int i = 0; i < n; ++i) {
			rotate_x(p[i], ca, sa);
			rotate_x(normal[i], ca, sa);
			rotate_z(p[i], cb, sb);
			rotate_z(normal[i], cb, sb);
		}
		A = A * 0.85 + 0.01;
		B = B * 0.85 + 0.02;
	}
}

int main() {
	rotate_3D_cube();
	return 0;
}
