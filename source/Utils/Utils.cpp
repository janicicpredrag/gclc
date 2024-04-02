#include "Utils.h"
#include "Common.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string i2s(int n, unsigned width) {
  std::string s = std::to_string(n);
  if (width > s.size())
    s = std::string(width - s.size(), ' ') + s;
  return s;
}

// ----------------------------------------------------------------------------////

std::string d2s(double n, int precision) {
  std::ostringstream stream;
  if (precision < 0) {
    if (-EPS < n - floor(n) && n - floor(n) < EPS)
      stream << floor(n);
    else
      stream << n;
  } else
    stream << std::fixed << std::setprecision(precision) << n;
  return stream.str();
}

// ----------------------------------------------------------------------------////

bool is_blank(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

// ----------------------------------------------------------------------------////

// remove leading and trailing spaces from a string
const std::string trim(const std::string &sInput, const std::string &t) {
  std::string s = sInput;
  std::string::size_type i = s.find_last_not_of(t);
  if (i == std::string::npos)
    return "";
  return s.erase(i + 1).erase(0, sInput.find_first_not_of(t));
}

// ----------------------------------------------------------------------------////

// remove backspaces from a string
const std::string removeBackspaces(const std::string &sInput) {
  std::string s = sInput;
  std::string::size_type i;
  while ((i = s.find('\b')) != std::string::npos) {
    if (i == 0)
      s.erase(0, 1);
    else
      s.erase(i - 1, 2);
  }
  return s;
}

// ----------------------------------------------------------------------------////

void trimrightzeros(std::string &name) {
  while (name[name.size()] == '0')
    name.resize(name.size() - 1);
}

// ----------------------------------------------------------------------------////

bool convert_int(const std::string &word, int &number) {
  int i = 0;
  char c;
  number = (int)0.00;
  do {
    c = word[i++];
    if (((c < '0') || (c > '9')) && (c != '\0'))
      return false;
    if (c != '\0')
      number = (int)(10 * number + (double)(c - '0'));
  } while (c != '\0');
  return true;
}

// ----------------------------------------------------------------------------////

bool convert(const std::string &word, double &number) {
  int i = 0, sign = 1;
  double dp = 1.00;
  double exp = 0;
  bool decimal = false;
  bool exponent = false;
  char c;
  number = 0.00;

  if (word == "")
    return false;

  if (word[0] == '-') {
    sign = -1;
    i++;
  }

  do {
    c = word[i++];
    if (((c < '0') || (c > '9')) && (c != '.') && (c != '\0') && (c != 'E') &&
        (c != 'e'))
      return false;
    if (c != '\0') {
      if ((c == 'E') || (c == 'e')) {
        if ((sign == 1 && i == 1) || (sign == -1 && i == 2))
          return false;

        if (exponent == false) {
          decimal = true;
          if (!convert(word.substr(i), exp))
            return false;
          const double mult = pow(10.0, (int)exp);
          number *= mult;
          return true;
        } else
          return false;
      } else {
        if (c != '.') {
          if (decimal == false)
            number = 10 * number + (double)(c - '0');
          else {
            dp = dp * 10.00;
            number = number + ((double)(c - '0')) / dp;
          }
        } else {
          if (decimal == false)
            decimal = true;
          else
            return false;
        }
      }
    }
  } while (c != '\0');

  number *= sign;
  return true;
}

// ----------------------------------------------------------------------------////

int log_div(int d, int c) {
  int p = 0;
  while (!(d % 10)) {
    d /= 10;
    p++;
  }
  if (d == 1)
    return (p % c);
  else
    return 1;
}

// ----------------------------------------------------------------------------////

bool same_point(double x1, double y1, double x2, double y2) {
  if (fabs(x1 - x2) > EPS)
    return false;
  if (fabs(y1 - y2) > EPS)
    return false;
  return true;
}

// ----------------------------------------------------------------------------////

bool between(double x, double y, double z) {
  return (((x <= y + EPS) && (y <= z + EPS)) ||
          ((z <= y + EPS) && (y <= x + EPS)));
}

// ----------------------------------------------------------------------------////

bool bet(double x1, double y1, double x2, double y2, double x3, double y3) {
  if (fabs((x1 - x2) * (y2 - y3) - (y1 - y2) * (x2 - x3)) > EPS)
    return false;
  if ((x1 - x2) * (x2 - x3) < -EPS)
    return false;
  if ((y1 - y2) * (y2 - y3) < -EPS)
    return false;
  if (fabs(x1 - x2) < EPS && fabs(y1 - y2) < EPS &&
      !(fabs(x1 - x3) < EPS && fabs(y1 - y3) < EPS))
    return false; // 01.07.2006
  if (fabs(x3 - x2) < EPS && fabs(y3 - y2) < EPS &&
      !(fabs(x1 - x3) < EPS && fabs(y1 - y3) < EPS))
    return false; // 01.07.2006
  return true;
}

// ----------------------------------------------------------------------------////

void rotate(double x1, double y1, double x2, double y2, double angle, double &x,
            double &y) {
  double sinn = sin(PI * angle / 180);
  double cosn = cos(PI * angle / 180);
  x = (x2 - x1) * cosn - (y2 - y1) * sinn + x1;
  y = (y2 - y1) * cosn + (x2 - x1) * sinn + y1;
}

// ----------------------------------------------------------------------------////

double compute_angle_in_degrees(double x1, double y1, double xc, double yc,
                                double x2, double y2) {
  double a, b, c, ee, angle;

  c = sqrt((x1 - xc) * (x1 - xc) + (y1 - yc) * (y1 - yc));
  a = sqrt((x2 - xc) * (x2 - xc) + (y2 - yc) * (y2 - yc));
  b = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

  ee = (a * a + c * c - b * b) / (2 * a * c);
  if (ee <= -1)
    angle = -PI;
  else if (ee >= 1.0)
    angle = 0;
  else
    angle = acos(ee);

  if ((xc - x1) * (y2 - y1) - (x2 - x1) * (yc - y1) > 0)
    angle = -1 * angle;
  angle = (180.0 * angle / PI);

  return angle;
}

// ----------------------------------------------------------------------------////

bool clip_line(double x1, double y1, double x2, double y2, double x_lb,
               double y_lb, double x_rt, double y_rt, double &x11, double &y11,
               double &x12, double &y12) {
  double a, b, c, d;

  if ((x_lb == 0) && (x_rt == 0) && (y_lb == 0) && (y_rt == 0)) {
    x11 = x1;
    y11 = y1;
    x12 = x2;
    y12 = y2;
    return true;
  }

  a = y2 - y1;
  b = x1 - x2;
  c = (y1 - y2) * x1 + y1 * (x2 - x1);

  if (same_point(x1, y1, x2, y2))
    return false;

  if (a != 0) {
    d = (-b * y_lb - c) / a;
    if ((d >= x_lb) && (d <= x_rt)) {
      x11 = d;
      y11 = y_lb;
      d = (-b * y_rt - c) / a;
      if ((d >= x_lb) && (d <= x_rt)) {
        x12 = d;
        y12 = y_rt;
      } else {
        d = (-a * x_lb - c) / b;
        if ((d >= y_lb) && (d <= y_rt) && !same_point(x11, y11, x_lb, d)) {
          x12 = x_lb;
          y12 = d;
        } else {
          d = (-a * x_rt - c) / b;
          if ((d >= y_lb) && (d <= y_rt)) {
            x12 = x_rt;
            y12 = d;
          } else
            return false;
        }
      }
    } else {
      d = (-b * y_rt - c) / a;
      if ((d >= x_lb) && (d <= x_rt)) {
        x11 = d;
        y11 = y_rt;
        d = (-a * x_lb - c) / b;
        if (((d >= y_lb) && (d <= y_rt)) && !same_point(x11, y11, x_lb, d)) {
          x12 = x_lb;
          y12 = d;
        } else {
          d = (-a * x_rt - c) / b;
          if ((d >= y_lb) && (d <= y_rt) && !same_point(x11, y11, x_rt, d)) {
            x12 = x_rt;
            y12 = d;
          } else
            return false;
        }
      } else {
        d = (-a * x_lb - c) / b;
        if ((d >= y_lb) && (d <= y_rt)) {
          x11 = x_lb;
          y11 = d;
          d = (-a * x_rt - c) / b;
          x12 = x_rt;
          y12 = d;
        } else
          return false;
      }
    }
  } else {
    d = (-a * x_lb - c) / b;
    if ((d >= y_lb) && (d <= y_rt)) {
      x11 = x_lb;
      y11 = d;
      x12 = x_rt;
      y12 = d;
    } else
      return false;
  }
  return true;
}

// ----------------------------------------------------------------------------////

bool clip_segment(double x1, double y1, double x2, double y2, double x_lb,
                  double y_lb, double x_rt, double y_rt, double &x1c,
                  double &y1c, double &x2c, double &y2c) {
  if (clip_line(x1, y1, x2, y2, x_lb, y_lb, x_rt, y_rt, x1c, y1c, x2c, y2c) ==
      false)
    return false;

  if (fabs(x1 - x1c) < EPS && fabs(y1 - y1c) < EPS) {
    if (bet(x1, y1, x2, y2, x2c, y2c) ||
        (fabs(x2 - x2c) < EPS && fabs(y2 - y2c) < EPS)) {
      x1c = x1;
      y1c = y1;
      x2c = x2;
      y2c = y2;
      return true;
    } else {
      if (bet(x1, y1, x2c, y2c, x2, y2)) {
        x1c = x1;
        y1c = y1;
        x2c = x2c;
        y2c = y2c;
        return true;
      } else
        return false;
    }
  }

  if (fabs(x1 - x2c) < EPS && fabs(y1 - y2c) < EPS) {
    if (bet(x1, y1, x2, y2, x1c, y1c) ||
        (fabs(x2 - x1c) < EPS && fabs(y2 - y1c) < EPS)) {
      x1c = x1;
      y1c = y1;
      x2c = x2;
      y2c = y2;
      return true;
    } else {
      if (bet(x1, y1, x1c, y1c, x2, y2)) {
        x2c = x1c;
        y2c = y1c;
        x1c = x1;
        y1c = y1;
        return true;
      } else
        return false;
    }
  }

  if (fabs(x2 - x1c) < EPS && fabs(y2 - y1c) < EPS) {
    if (bet(x2, y2, x1, y1, x2c, y2c) ||
        (fabs(x1 - x2c) < EPS && fabs(y1 - y2c) < EPS)) {
      x1c = x1;
      y1c = y1;
      x2c = x2;
      y2c = y2;
      return true;
    } else {
      if (bet(x2, y2, x2c, y2c, x1, y1)) {
        x1c = x2;
        y1c = y2;
        x2c = x2c;
        y2c = y2c;
        return true;
      } else
        return false;
    }
  }

  if (fabs(x2 - x2c) < EPS && fabs(y2 - y2c) < EPS) {
    if (bet(x2, y2, x1, y1, x1c, y1c) ||
        (fabs(x1 - x1c) < EPS && fabs(y1 - y1c) < EPS)) {
      x1c = x1;
      y1c = y1;
      x2c = x2;
      y2c = y2;
      return true;
    } else {
      if (bet(x2, y2, x1c, y1c, x1, y1)) {
        x2c = x2;
        y2c = y2;
        return true;
      } else
        return false;
    }
  }

  if (!bet(x1, y1, x1c, y1c, x2, y2)) // ~B(p1,p1c,p2)
  {
    if (!bet(x1, y1, x2c, y2c, x2, y2)) // ~B(p1,p2c,p2)
    {
      if (!bet(x1c, y1c, x1, y1, x2c, y2c)) // ~B(p1c,p1,p2,p2c)
        return false;
      else // B(p1c,p2c,p1,p2)
      {
        x1c = x1;
        y1c = y1;
        x2c = x2;
        y2c = y2;
        return true;
      }
    } else // B(p1,p2c,p2)
    {
      if (!bet(x1c, y1c, x1, y1, x2, y2)) // ~B(p1c,p1,p2c,p2)
      {
        x1c = x2;
        y1c = y2;
        return true;
      } else {
        x1c = x1;
        y1c = y1;
        return true;
      }
    }
  } else {
    if (!bet(x1, y1, x2c, y2c, x2, y2)) // ~B(p1,p2c,p2)
    {
      if (!bet(x2c, y2c, x1, y1, x1c, y1c)) // B(p2c,p1,p1c,p2)
      {
        x2c = x2;
        y2c = y2;
        return true;
      } else {
        x2c = x1;
        y2c = y1;
        return true;
      }
    } else // B(p1,p2c,p2)
      return true;
  }
  return false;
}

// ----------------------------------------------------------------------------////

double transform_ellipse_angle(double a, double b, double phi) {
  while (phi > 360)
    phi -= 360;
  while (phi < 0)
    phi += 360;

  if (fabs(phi) < EPS)
    return 0;
  else if (fabs(phi - 90) < EPS)
    return phi;
  else if (fabs(phi - 180) < EPS)
    return phi;
  else if (fabs(phi - 270) < EPS)
    return 270;
  else {
    double phi1 = 180 * atan(tan(PI * phi / 180) * a / b) / PI;
    while (phi1 > 360)
      phi1 -= 360;
    while (phi1 < 0)
      phi1 += 360;
    if (fabs(phi - phi1) > 90)
      phi = phi1 - 180;
    else
      phi = phi1;
  }
  while (phi > 360)
    phi -= 360;
  while (phi < 0)
    phi += 360;

  return phi;
}

// ----------------------------------------------------------------------------////

bool MakeArcPath(double x1, double y1, double a, double b, double phi1,
                 double phi2, std::string &sPath) {
  double xs, ys, xe, ye, xi, yi;
  double psi, phi;
  double phi1t, phi2t;

  while (phi1 < 0.0)
    phi1 += 360.0;
  while (phi1 > 360.0)
    phi1 -= 360.0;
  while (phi2 < 0.0)
    phi2 += 360.0;
  while (phi2 > 360.0)
    phi2 -= 360.0;

  if (a != b) {
    phi1t = transform_ellipse_angle(a, b, phi1);
    phi2t = transform_ellipse_angle(a, b, phi2);
  } else {
    phi1t = phi1;
    phi2t = phi2;
  }

  while (phi1t < 0.0)
    phi1t += 360.0;
  while (phi1t > 360.0)
    phi1t -= 360.0;
  while (phi2t < 0.0)
    phi2t += 360.0;
  while (phi2t > 360.0)
    phi2t -= 360.0;

  double sinn, cosn;

  sPath = "";
  if (phi1t - (int)phi1t > EPS &&
      (phi2t <= phi1t ||
       (int)phi2t >=
           (int)(phi1t + 1.0))) // 14.10.2015. fix - additional condition
  {
    psi = PI * phi1t / 180;
    sinn = sin(psi);
    cosn = cos(psi);

    // a small line segment before the starting point and the point
    // corresponding an integer angle
    xi = cosn * a + x1;
    yi = sinn * b + y1;
    sPath += "(" + d2s(xi / 10, 6) + "," + d2s(yi / 10, 6) + ") -- ";

    phi1t = (int)(phi1t + 1.0);
  }

  psi = PI * phi1t / 180;
  sinn = sin(psi);
  cosn = cos(psi);

  xs = cosn * a + x1;
  ys = sinn * b + y1;

  if ((int)phi2t >= (int)phi1t)
    sPath += "(" + d2s(xs / 10, 6) + "," + d2s(ys / 10, 6) + ") arc (" +
             i2s((int)phi1t) + ":" + i2s((int)phi2t) + ":" + d2s(a / 10, 6) +
             " and " + d2s(b / 10, 6) + ")";
  else {
    if (phi2 < phi1) {
      sPath += "(" + d2s(xs / 10, 6) + "," + d2s(ys / 10, 6) + ") arc (" +
               i2s((int)phi1t) + ":" + i2s(360) + ":" + d2s(a / 10, 6) +
               " and " + d2s(b / 10, 6) + ") --";
      sPath += "(" + d2s((x1 + a) / 10, 6) + "," + d2s(y1 / 10, 6) + ") arc (" +
               i2s(0) + ":" + i2s((int)phi2t) + ":" + d2s(a / 10, 6) + " and " +
               d2s(b / 10, 6) + ")";
    } else { // 14.10.2015. fix - additional case
      sPath += "(" + d2s(xs / 10, 6) + "," + d2s(ys / 10, 6) + ") arc (" +
               i2s((int)phi1t) + ":" + i2s((int)phi1t) + ":" + d2s(a / 10, 6) +
               " and " + d2s(b / 10, 6) + ")";
    }
  }

  if (phi2t - (int)phi2t > EPS) {
    // a small line segment between the ending point and the point corresponding
    // an integer angle
    psi = PI * ((int)phi2t) / 180;
    sinn = sin(psi);
    cosn = cos(psi);

    phi = PI * phi2t / 180;
    sinn = sin(phi);
    cosn = cos(phi);
    xe = cosn * a + x1;
    ye = sinn * b + y1;
    sPath += " -- (" + d2s(xe / 10, 6) + "," + d2s(ye / 10, 6) + ")";
  }

  return true;
}

// ----------------------------------------------------------------------------////

int solve(double a, double b, double c, double &x1, double &x2, int &r) {
  double d;

  if (a == 0) {
    if (b == 0) {
      r = 0;
      return 0;
    } else {
      x1 = -c / b;
      x2 = -c / b;
      r = 1;
      return 1;
    }
  } else {
    d = b * b - 4 * a * c;
    if (d < -EPS) {
      r = 0;
      return 0;
    } else if (fabs(d) < EPS) {
      x1 = -b / (2 * a);
      x2 = -b / (2 * a);
      r = 1;
      return 1;
    } else {
      x1 = (-b + sqrt(d)) / (2 * a);
      x2 = (-b - sqrt(d)) / (2 * a);
      r = 2;
      return 2;
    }
  }
}

// ----------------------------------------------------------------------------////

double HeronArea(double x1, double y1, double x2, double y2, double x3,
                 double y3) {
  double a, b, c, p, s;
  double sign = 1;

  if ((x3 - x2) * (y1 - y2) < (x1 - x2) * (y3 - y2))
    sign = -1;
  else
    sign = 1;

  a = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  b = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
  c = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
  p = (a + b + c) / 2;

  s = p * (p - a) * (p - b) * (p - c);
  if (s < 0) // possible because of small errors
    s = 0;

  return sign * sqrt(s);
}

// ----------------------------------------------------------------------------////

std::string make_indent(int indent) {
  int i;
  std::string s;
  for (i = 0; i < indent; i++)
    s += "\t";
  return s;
}

// ----------------------------------------------------------------------------////

void Print(std::ostream &h, std::string s) { h << s; }

// ----------------------------------------------------------------------------////

void Print(std::ofstream &h, std::string s) { h << s; }

// ----------------------------------------------------------------------------////

void Print(std::ostream &h, char c) { h << c; }

// ----------------------------------------------------------------------------////

#if defined(_PLATFORM_WIN_)

int getrusage(int who, struct rusage *rusage) {
  FILETIME starttime;
  FILETIME exittime;
  FILETIME kerneltime;
  FILETIME usertime;
  ULARGE_INTEGER li;

  if (who != RUSAGE_SELF)
    return -1;

  if (rusage == (struct rusage *)NULL)
    return -1;

  memset(rusage, 0, sizeof(struct rusage));
  if (GetProcessTimes(GetCurrentProcess(), &starttime, &exittime, &kerneltime,
                      &usertime) == 0)
    return -1;

  /* Convert FILETIMEs (0.1 us) to struct timeval */
  memcpy(&li, &kerneltime, sizeof(FILETIME));
  li.QuadPart /= 10L; /* Convert to microseconds */
  rusage->ru_stime.tv_sec = li.QuadPart / 1000000L;
  rusage->ru_stime.tv_usec = li.QuadPart % 1000000L;

  memcpy(&li, &usertime, sizeof(FILETIME));
  li.QuadPart /= 10L; /* Convert to microseconds */
  rusage->ru_utime.tv_sec = li.QuadPart / 1000000L;
  rusage->ru_utime.tv_usec = li.QuadPart % 1000000L;
  return 0;
}
#endif
