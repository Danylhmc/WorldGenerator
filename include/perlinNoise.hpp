#pragma once
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <cmath>

class Perlin{
public:
  std::vector<int> p;
  Perlin(int seed){
    p.resize(512);
    std::iota(p.begin(), p.begin() + 256, 0);
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.begin() + 256, engine);
    std::copy(p.begin() , p.begin() + 256, p.begin() + 256);
  }

  double noise(double x, double y) const{
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int aa = p[p[X] + Y];
    int ba = p[p[X + 1] + Y];
    int ab = p[p[X] + Y + 1];
    int bb = p[p[X + 1] + Y + 1];

    double result = lerp(v,
      lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
      lerp(u, grad(ab, x, y -1), grad(bb, x - 1, y - 1))
    );
    
    return (result + 1.0) / 2.0;
  }

private:
  double fade(double t) const{
     return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
  }

  double lerp(double t, double a, double b) const{
    return a + t * (b - a);
  }

  double grad(int hash, double x, double y) const {
    constexpr double d = 0.70710678118;

    switch(hash & 7) {
        case 0: return x;
        case 1: return -x;
        case 2: return y;
        case 3: return -y;
        case 4: return (x + y) * d;
        case 5: return (-x + y) * d;
        case 6: return (x - y) * d;
        default: return (-x - y) * d;
    }
}};
