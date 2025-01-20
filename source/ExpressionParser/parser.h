#ifndef PARSER_H
#define PARSER_H

#ifdef WIN32

// disable warnings about long names
//  #pragma warning( disable : 4786)

// under Visual C++ 6 these do not seem to exist
/* template <typename T>
 inline T min (const T a, const T b)
   {
   return (a < b) ? a : b;
   } // end of min

 template <typename T>
 inline T max (const T a, const T b)
   {
   return (a > b) ? a : b;
   } // end of max

 template <typename T>
 inline T abs (T i)
   {
   return (i < 0) ? -i : i;
   } // end of abs
*/

/*  inline double min (double a, double b)
    {
    return (a < b) ? a : b;
    } // end of min

  inline double max (const double a, const double b)
    {
    return (a > b) ? a : b;
    } // end of max*/

// inline double abs(double i) { return (i < 0) ? -i : i; } // end of abs

#endif

#include "../GCLCEngine/GCLC.h"

#include <cmath>
#include <map>
#include <sstream>
#include <stdexcept>

class CGCLC;

// make a string on-the-fly
#define MAKE_STRING(msg)                                                       \
  (((std::ostringstream &)(std::ostringstream() << std::boolalpha << msg)).str())

class Parser {

public:
  enum TokenType {
    NONE,
    NAME,
    NUMBER,
    END,
    PLUS = '+',
    MINUS = '-',
    MULTIPLY = '*',
    DIVIDE = '/',
    //    ASSIGN='=',
    LHPAREN = '(',
    RHPAREN = ')',
    COMMA = ',',
    NOT = '!',

    // comparisons
    LT = '<',
    GT = '>',
    LE,  // <=
    GE,  // >=
    EQ,  // ==
    NE,  // !=
    AND, // &&
    OR,  // ||

    // special assignments

    ASSIGN_ADD, //  +=
    ASSIGN_SUB, //  +-
    ASSIGN_MUL, //  +*
    ASSIGN_DIV  //  +/

  };

private:
  std::string program_;

  const char *pWord_;
  const char *pWordStart_;
  // last token parsed
  TokenType type_;
  std::string word_;
  double value_;

private:
  TokenType GetToken(const bool ignoreSign = false);
  double CommaList(const bool get);
  double Expression(const bool get);
  double Comparison(const bool get);
  double AddSubtract(const bool get);
  double Term(const bool get);    // multiply and divide
  double Primary(const bool get); // primary (base) tokens

  inline void CheckToken(const TokenType wanted) {
    if (type_ != wanted)
      throw std::runtime_error(
          MAKE_STRING("'" << static_cast<char>(wanted) << "' expected."));
  }

  CGCLC *m_pGclc;

public:
  // ctor
  Parser(const std::string &program)
      : program_(program), pWord_(program_.c_str()), type_(NONE) {
    // insert pre-defined names:
    /*      symbols_ ["pi"] = 3.1415926535897932385;
          symbols_ ["e"]  = 2.7182818284590452354;      */
  }

  double Evaluate();                     // get result
  double Evaluate(const std::string program); // get result

  void SetGCLC(CGCLC *pGclc) { m_pGclc = pGclc; }

  /*// access symbols with operator []
  double & operator[] (const string & key) { return symbols_ [key]; }

  // symbol table - can be accessed directly (eg. to copy a batch in)
  map<string, double> symbols_;*/

}; // end of Parser

#endif // PARSER_H
