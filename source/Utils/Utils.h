#ifndef UTILS_H
#define UTILS_H

#include "Common.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static const string SPACES = " \t\r\n"; // what gets removed when we trim

const string trim(const string &sInput, const string &t = SPACES);
const string removeBackspaces(const string &sInput);
void trimrightzeros(string &name);

bool convert_int(const string &word, int &number);
bool convert(const string &word, double &number);

void power(double d, int exp, double &result);
int log_div(int d, int c);

bool same_point(double x1, double y1, double x2, double y2);
bool between(double x, double y, double z);
bool bet(double x1, double y1, double x2, double y2, double x3, double y3);
void rotate(double x1, double y1, double x2, double y2, double angle, double &x,
            double &y);
double compute_angle_in_degrees(double x1, double y1, double xc, double yc,
                                double x2, double y2);

bool clip_line(double x1, double y1, double x2, double y2, double x_lb,
               double y_lb, double x_rt, double y_rt, double &x11, double &y11,
               double &x12, double &y12);
bool clip_segment(double x1, double y1, double x2, double y2, double x_lb,
                  double y_lb, double x_rt, double y_rt, double &x11,
                  double &y11, double &x12, double &y12);

double transform_ellipse_angle(double a, double b, double phi);
bool MakeArcPath(double x1, double y1, double a, double b, double phi1,
                 double phi2, string &sPath);

int solve(double a, double b, double c, double &x1, double &x2, int &r);
double HeronArea(double x1, double y1, double x2, double y2, double x3,
                 double y3);

string make_indent(int indent);

void Print(ofstream &h, string s);
void Print(ostream &h, string s);
void Print(ostream &h, char c);

string i2s(int n, unsigned width = 0);
string d2s(double n, int precision);

bool is_blank(char c);

template <typename T> string toHexString(const T &n) {
  ostringstream stream;
  stream << setfill('0') << setw(2) << hex << uppercase << n;
  return stream.str();
}

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) ||               \
    defined(__MINGW64__)
#define _PLATFORM_WIN_
#elif defined(__linux) || defined(__linux__)
#define _PLATFORM_LINUX_
#endif

#if defined(_PLATFORM_WIN_)
#include <windows.h>
struct rusage {
  struct timeval ru_utime; /* user CPU time used */
  struct timeval ru_stime; /* system CPU time used */
  long ru_maxrss;          /* maximum resident set size */
  long ru_ixrss;           /* integral shared memory size */
  long ru_idrss;           /* integral unshared data size */
  long ru_isrss;           /* integral unshared stack size */
  long ru_minflt;          /* page reclaims (soft page
faults) */
  long ru_majflt;          /* page faults (hard page faults) */
  long ru_nswap;           /* swaps */
  long ru_inblock;         /* block input operations */
  long ru_oublock;         /* block output operations */
  long ru_msgsnd;          /* IPC messages sent */
  long ru_msgrcv;          /* IPC messages received */
  long ru_nsignals;        /* signals received */
  long ru_nvcsw;           /* voluntary context switches */
  long ru_nivcsw;          /* involuntary context switches */
};
#define RUSAGE_SELF 0
int getrusage(int who, struct rusage *rusage);

#elif defined(_PLATFORM_LINUX_)
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#else
#include <ctime>
#endif

#endif // !defined(UTILS_H)
