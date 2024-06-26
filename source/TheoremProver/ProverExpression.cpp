#include "ProverExpression.h"
#include "TheoremProver.h"
#include <assert.h>
#include <cmath>
#include <cstddef>
#include <vector>

#define EPSILON 0.00001

static unsigned idCounter=0;

using namespace std;

Rules CGCLCProverExpression::mRules;

unsigned arity(enum GCLCexpression_type type) {
  switch (type) {
  case ep_number:
  case ep_point:
  case ep_constant:
  case ep_unknown:
    return 0;

  case ep_identical:
  case ep_segment:
  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
  case ep_diffx:
  case ep_diffy:
    return 2;

  case ep_s3:
  case ep_p3:
  case ep_collinear:
  case ep_midpoint:
  case ep_tangens_num:
  case ep_tangens_den:
    return 3;

  case ep_segment_ratio:
  case ep_s4:
  case ep_p4:
  case ep_parallel:
  case ep_perpendicular:
  case ep_harmonic:
  case ep_same_length:
    return 4;

  default:
    return 0;
  }
  return 0;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression() {
  sName = "";
  nNumber = 0;
  type = ep_unknown;
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::~CGCLCProverExpression() { CleanUp(); }

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(const CGCLCProverExpression &r) {
  type = r.type;
  sName = r.sName;
  nNumber = r.nNumber;
  for (unsigned i = 0; i < arity(type); i++)
    arg[i].reset(new CGCLCProverExpression(r.GetArg(i)));
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression &CGCLCProverExpression::
operator=(const CGCLCProverExpression &r) {
  if (this == &r)
    return *this;
  CleanUp();
  type = r.type;

  sName = r.sName;
  nNumber = r.nNumber;
  for (unsigned i = 0; i < arity(type); i++)
    arg[i].reset(new CGCLCProverExpression(r.GetArg(i)));
  for (unsigned i = arity(type); i < ExpressionArgCount; i++)
    arg[i] = NULL;
  return *this;
}

// --------------------------------------------------------------------------

CGCLCProverExpression &CGCLCProverExpression::operator=(const double n) {
  CleanUp();
  sName = "";
  nNumber = n;
  type = ep_number;
  for (unsigned i = 0; i < ExpressionArgCount; i++)
    arg[i] = NULL;
  return *this;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(GCLCexpression_type t,
                                             const std::string &a) {
  assert(t == ep_point || t == ep_constant);
  type = t;
  sName = a;
  nNumber = 0;
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(GCLCexpression_type t,
                                             const std::string &a0,
                                             const std::string &a1) {
  assert(t == ep_segment || t == ep_diffx || t == ep_diffy);
  type = t;
  nNumber = 0;
  arg[0].reset(new CGCLCProverExpression(ep_point, a0));
  arg[1].reset(new CGCLCProverExpression(ep_point, a1));
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(GCLCexpression_type t,
                                             const std::string &a0, const std::string &a1,
                                             const std::string &a2) {
  assert(t == ep_s3 || t == ep_p3);
  type = t;
  nNumber = 0;
  arg[0].reset(new CGCLCProverExpression(ep_point, a0));
  arg[1].reset(new CGCLCProverExpression(ep_point, a1));
  arg[2].reset(new CGCLCProverExpression(ep_point, a2));
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(GCLCexpression_type t,
                                             const std::string &a0, const std::string &a1,
                                             const std::string &a2,
                                             const std::string &a3) {
  assert(t == ep_s4 || t == ep_p4 || t == ep_segment_ratio || t == ep_parallel);
  type = t;
  nNumber = 0;
  arg[0].reset(new CGCLCProverExpression(ep_point, a0));
  arg[1].reset(new CGCLCProverExpression(ep_point, a1));
  arg[2].reset(new CGCLCProverExpression(ep_point, a2));
  arg[3].reset(new CGCLCProverExpression(ep_point, a3));
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(const double n) {
  type = ep_number;
  nNumber = n;
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression::CGCLCProverExpression(
    GCLCexpression_type t, const CGCLCProverExpression &arg0,
    const CGCLCProverExpression &arg1) {
  assert(arity(t) == 2);
  type = t;
  nNumber = 0;
  arg[0].reset(new CGCLCProverExpression(arg0));
  arg[1].reset(new CGCLCProverExpression(arg1));
  id = idCounter++;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::s3(const std::string &a0,
                                                const std::string &a1,
                                                const std::string &a2) {
  CGCLCProverExpression n(ep_s3, a0, a1, a2);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::p3(const std::string &a0,
                                                const std::string &a1,
                                                const std::string &a2) {
  CGCLCProverExpression n(ep_p3, a0, a1, a2);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::s4(const std::string &a0,
                                                const std::string &a1,
                                                const std::string &a2,
                                                const std::string &a3) {
  CGCLCProverExpression n(ep_s4, a0, a1, a2, a3);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::p4(const std::string &a0,
                                                const std::string &a1,
                                                const std::string &a2,
                                                const std::string &a3) {
  CGCLCProverExpression n(ep_p4, a0, a1, a2, a3);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::sratio(const std::string &a0,
                                                    const std::string &a1,
                                                    const std::string &a2,
                                                    const std::string &a3) {
  CGCLCProverExpression n(ep_segment_ratio, a0, a1, a2, a3);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::segment(const std::string &a0,
                                                     const std::string &a1) {
  CGCLCProverExpression n(ep_segment, a0, a1);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::
operator*(const CGCLCProverExpression &a) {
  CGCLCProverExpression n(ep_mult, *this, a);
  return n;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::
operator+(const CGCLCProverExpression &a) {
  CGCLCProverExpression e(ep_sum, *this, a);
  return e;
}

// --------------------------------------------------------------------------

CGCLCProverExpression CGCLCProverExpression::
operator/(const CGCLCProverExpression &a) {
  CGCLCProverExpression e(ep_ratio, *this, a);
  return e;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::CleanUp() {
  // this->PrettyPrint();
  // cout << "-------------" << endl;
  for (unsigned i = 0; i < ExpressionArgCount; i++)
    arg[i] = NULL;
  sName = "";
  nNumber = 0;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::Push(CGCLCProverExpression &left,
                                 CGCLCProverExpression &right,
                                 const CGCLCProverExpression &a) {
  assert(type == ep_equality);
  left = GetArg(0);
  right = GetArg(1);
  for (unsigned i = 0; i < 2; i++)
    arg[i].reset(new CGCLCProverExpression(a.GetArg(i)));
  SetType(ep_equality);
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::Pop(CGCLCProverExpression &left,
                                CGCLCProverExpression &right) {
  assert(type == ep_equality);
  arg[0].reset(new CGCLCProverExpression(left));
  arg[1].reset(new CGCLCProverExpression(right));
  SetType(ep_equality);
}


// --------------------------------------------------------------------------

void CGCLCProverExpression::Set(GCLCexpression_type t,
                                const CGCLCProverExpression &arg0,
                                const CGCLCProverExpression &arg1) {
  assert(arity(t) == 2);
  CleanUp();
  type = t;
  nNumber = 0;
  SetArg(0, CGCLCProverExpression(arg0));
  SetArg(1, CGCLCProverExpression(arg1));
  for (unsigned i = 2; i < ExpressionArgCount; i++)
    arg[i] = NULL;
  // id = idCounter++;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::SetArg(unsigned i, CGCLCProverExpression *a) {
 arg[i].reset(a);
  // id = idCounter++;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::SetArg(unsigned i, const CGCLCProverExpression &a) {
  arg[i].reset(new CGCLCProverExpression(a));
  // id = idCounter++;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::SetArgName(unsigned i, const std::string &s) {
  assert(type == ep_s3 || type == ep_p3 || type == ep_s4 || type == ep_p4 ||
         type == ep_segment_ratio || type == ep_segment ||
         type == ep_parallel || type == ep_collinear ||
         type == ep_perpendicular || type == ep_midpoint ||
         type == ep_harmonic || type == ep_same_length ||
         type == ep_tangens_num || type == ep_tangens_den || type == ep_diffx ||
         type == ep_diffy || type == ep_identical);
  assert(i < arity(type));
  arg[i]->SetName(s);
  // id = idCounter++;
}

// --------------------------------------------------------------------------

std::string CGCLCProverExpression::GetArgName(unsigned i) const {
  assert(type == ep_s3 || type == ep_p3 || type == ep_s4 || type == ep_p4 ||
         type == ep_segment_ratio || type == ep_segment ||
         type == ep_parallel || type == ep_collinear ||
         type == ep_perpendicular || type == ep_midpoint ||
         type == ep_harmonic || type == ep_same_length ||
         type == ep_tangens_num || type == ep_tangens_den || type == ep_diffx ||
         type == ep_diffy || type == ep_identical);
  assert(i < arity(type));
  return arg[i]->GetName();
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::operator==(const CGCLCProverExpression &r) const {
  return Equal(r);
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::Equal(const CGCLCProverExpression &r) const {
  // return (*this) == *r;
  if (type != r.type)
    return false;
  if (type == ep_point)
    return (sName == r.sName);
  if (type == ep_constant)
    return (sName == r.sName);
  if (type == ep_number)
    return (nNumber == r.nNumber);
  for (unsigned i = 0; i < arity(type); i++)
    if (!(arg[i]->Equal(*r.arg[i])))
      return false;
  return true;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ExistsAtomicExpression(
    const CGCLCProverExpression &subexp) const {
  switch (GetType()) {
  case ep_number:
  case ep_point:
  case ep_s3:
  case ep_segment:
  case ep_segment_ratio:
  case ep_p3:
  case ep_p4:
  case ep_s4:
    if (*this == subexp)
      return true;
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    if (GetArg(0).ExistsAtomicExpression(subexp))
      return true;
    if (GetArg(1).ExistsAtomicExpression(subexp))
      return true;
    break;

  default:
    break;
  }

  return false;
}

bool CGCLCProverExpression::GetParentIndex(CGCLCProverExpression &outer,
                                           CGCLCProverExpression *&parent,
                                           int &index) const {
  if (*this == outer)
    return false;
  for (unsigned i = 0; i < arity(outer.GetType()); i++) {
    if (*this == outer.GetArg(i)) {
      parent = &outer;
      index = i;
      return true;
    }
    if (GetParentIndex(outer.GetArg(i), parent, index))
      return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::Replace(const CGCLCProverExpression &LHS,
                                    const CGCLCProverExpression &RHS) {
  /*if (old_expression->Equal(new_expression)) {
    delete old_expression;
    old_expression = new CGCLCProverExpression(*new_expression);
    if (old_expression == NULL)
      return false;
    else
      return true;
  }*/
  for (unsigned i = 0; i < arity(type); i++)
    if (*arg[i] == LHS) {
      arg[i].reset(new CGCLCProverExpression(RHS));
    } else if (!arg[i]->Replace(LHS, RHS))
      return false;
  return true;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ApplyAllAlgebraicRules(std::string &sRuleApplied,
                                                   int iExceptLast) {
  for (unsigned i = 0; i < mRules.m_nNumberOfRules - iExceptLast; i++)
    if (ApplyOneAlgebraicRule(mRules.m_aiRule[i])) {
      sRuleApplied = mRules.m_asRuleName[mRules.m_aiRule[i]];
      return true;
    }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ApplyAllSimpleAlgebraicRules(std::string &sRuleApplied,
                                                         int iExceptLast) {
  for (unsigned i = 0; i < 14 && i < mRules.m_nNumberOfRules - iExceptLast; i++) {
    if (ApplyOneAlgebraicRule(mRules.m_aiRule[i])) {
      sRuleApplied = mRules.m_asRuleName[mRules.m_aiRule[i]];
      return true;
    }
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ApplyAllComplexAlgebraicRules(std::string &sRuleApplied,
                                                          int iExceptLast) {
  for (unsigned i = 14; i < mRules.m_nNumberOfRules - iExceptLast; i++) {
    if (ApplyOneAlgebraicRule(mRules.m_aiRule[i])) {
      sRuleApplied = mRules.m_asRuleName[mRules.m_aiRule[i]];
      return true;
    }
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ApplyOneAlgebraicRule(GCLCalgebraic_rule rule) {
  switch (rule) {

  case er_RatioCancellation:
    if (RatioCancellation())
      return true;
    break;

  case er_MultiplicationOfConstants:
    if (MultiplicationOfConstants())
      return true;
    break;

  case er_AdditionWithZero:
    if (AdditionWithZero())
      return true;
    break;

  case er_MultiplicationWithOne:
    if (MultiplicationWithOne())
      return true;
    break;

  case er_CommutativityWithNumber:
    if (CommutativityWithNumber())
      return true;
    break;

  case er_AssociativityAndCommutativity:
    if (AssociativityAndCommutativity())
      return true;
    break;

  case er_RightAssoc:
    if (RightAssoc())
      return true;
    break;

  case er_MultiplicationOfFractions:
    if (MultiplicationOfFractions())
      return true;
    break;

  case er_DistrMultOverAdd:
    if (DistrMultOverAdd())
      return true;
    break;

  case er_MultiplicationWithZero:
    if (MultiplicationWithZero())
      return true;
    break;

  case er_MultipleFraction:
    if (MultipleFraction())
      return true;
    break;

  case er_SumOfFractions:
    if (SumOfFractions())
      return true;
    break;

  case er_SimilarSummands:
    if (SimilarSummands())
      return true;
    break;

  case er_SimilarSummandsOnTwoSides:
    if (SimilarSummandsOnTwoSides())
      return true;
    break;

  case er_ZeroFractionUp:
    if (ZeroFractionUp())
      return true;
    break;

  case er_FractionEqualZero:
    if (FractionEqualZero())
      return true;
    break;

  case er_FractionWithNumberDenominator:
    if (FractionWithNumberDenominator())
      return true;
    break;

  case er_OneSide:
    if (OneSide())
      return true;
    break;

  case er_FractionsOnTwoSides:
    if (FractionsOnTwoSides())
      return true;
    break;

  case er_EliminateFraction:
    if (EliminateFraction())
      return true;
    break;

  default:
    break;
  }

  for (unsigned i = 0; i < ExpressionArgCount; i++)
    if (arg[i] != NULL)
      if (arg[i]->ApplyOneAlgebraicRule(rule))
        return true;

  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::MultiplicationOfFractions() {
  // (a/b)*(c/d) = (a*c)/(b*d)
  if (type == ep_mult && GetArg(0).type == ep_ratio &&
      GetArg(1).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    *this = ((a * c) / (b * d));
    return true;
  }

  // a*(c/d) = (a*c)/d
  if (type == ep_mult && GetArg(1).type == ep_ratio &&
      GetArg(0).type != ep_ratio) {
    CGCLCProverExpression a(GetArg(0));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    *this = ((a * c) / d);
    return true;
  }

  // (a/b)*c = (a*c)/b
  if (type == ep_mult && GetArg(0).type == ep_ratio &&
      GetArg(1).type != ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1));
    type = ep_ratio;
    *this = ((a * c) / b);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::MultiplicationWithZero() {
  // 0*x -> 0
  if (type == ep_mult && GetArg(0).type == ep_number &&
      GetArg(0).nNumber == 0.0) {
    *this = 0.0;
    return true;
  }
  // x*0 -> 0
  if (type == ep_mult && GetArg(1).type == ep_number &&
      GetArg(1).nNumber == 0.0) {
    *this = 0.0;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ZeroFractionUp() {
  // 0/x -> 0
  if (type == ep_ratio && GetArg(0).type == ep_number &&
      GetArg(0).nNumber == 0.0) {
    *this = 0.0;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::FractionEqualZero() {
  // a/b = 0 -> a = 0
  if (type == ep_equality && GetArg(0).type == ep_ratio &&
      (GetArg(1).type == ep_number) && (GetArg(1).nNumber == 0.0)) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    SetArg(0, a);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::FractionWithNumberDenominator() {
  // a/n -> 1/n * a
  if (type == ep_ratio)
    if (GetArg(0).type != ep_number && GetArg(1).type == ep_number) {
      CGCLCProverExpression a(GetArg(0));
      if (GetArg(1).nNumber == 1.0) {
        *this = a;
        return true;
      } else {
        CGCLCProverExpression m(1 / GetArg(1).nNumber);
        *this = (m * a);
        return true;
      }
    }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::MultiplicationWithOne() {
  // 1*a -> a
  if (type == ep_mult && GetArg(0).type == ep_number &&
      GetArg(0).nNumber == 1.0) {
    CGCLCProverExpression a(GetArg(1));
    *this = a;
    return true;
  }
  // a*1 -> a
  if (type == ep_mult && GetArg(1).type == ep_number &&
      GetArg(1).nNumber == 1.0) {
    CGCLCProverExpression a(GetArg(0));
    *this = a;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::AdditionWithZero() {
  // a + 0 -> a
  if (type == ep_sum && GetArg(1).type == ep_number &&
      GetArg(1).nNumber == 0.0) {
    CGCLCProverExpression a(GetArg(0));
    *this = a;
    return true;
  }
  // 0+a -> a
  if (type == ep_sum && GetArg(0).type == ep_number &&
      GetArg(0).nNumber == 0.0) {
    CGCLCProverExpression a(GetArg(1));
    *this = a;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::MultiplicationOfConstants() {
  // n1 * n1 -> m (m is the product)
  if (type == ep_mult)
    if (GetArg(0).type == ep_number && GetArg(1).type == ep_number) {
      double m = GetArg(0).nNumber * GetArg(1).nNumber;
      *this = m;
      return true;
    }

  // n1 * (n2 * a) -> m * a (m is the product)
  if (type == ep_mult)
    if (GetArg(0).type == ep_number && GetArg(1).type == ep_mult &&
        (GetArg(1).GetArg(0).type == ep_number)) {
      CGCLCProverExpression a(GetArg(1).GetArg(1));
      CGCLCProverExpression m(GetArg(0).nNumber * GetArg(1).GetArg(0).nNumber);
      *this = (m * a);
      return true;
    }

  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::CommutativityWithNumber() {
  if (type == ep_mult)
    if (GetArg(0).type != ep_number && GetArg(1).type == ep_number) {
      // a*n -> n*a
      CGCLCProverExpression a(GetArg(0));
      CGCLCProverExpression n(GetArg(1));
      *this = (n * a);
      return true;
    }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::AssociativityAndCommutativity() {
  if (type == ep_mult && GetArg(1).type == ep_mult &&
      GetArg(0).type != ep_number && GetArg(1).GetArg(0).type == ep_number) {
    // a*(n*b) -> n*(a*b)
    CGCLCProverExpression a(GetArg(0));
    CGCLCProverExpression n(GetArg(1).GetArg(0));
    CGCLCProverExpression b(GetArg(1).GetArg(1));
    *this = (n * (a * b));
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::RatioCancellation() {

  if (type == ep_segment_ratio) {
    // AB/AB -> 1
    if (GetArg(0) == GetArg(2) && GetArg(1) == GetArg(3)) {
      *this = 1.0;
      return true;
    }
    // AB/BA -> -1
    if (GetArg(0) == GetArg(3) && GetArg(1) == GetArg(2)) {
      *this = -1.0;
      return true;
    }
  }
  if (type == ep_ratio) {
    // x/x -> 1
    if (GetArg(0) == GetArg(1)) {
      *this = 1.0;
      return true;
    }
    if (GetArg(0).CancelPairFirst(GetArg(1)))
      return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::CancelPairFirst(CGCLCProverExpression &exp2) {
  if (type == ep_number)
    if (nNumber == 1)
      return false;

  switch (type) {
  case ep_mult:
    if (CancelPairSecond(exp2))
      return true;
    if (GetArg(0).CancelPairFirst(exp2))
      return true;
    if (GetArg(1).CancelPairFirst(exp2))
      return true;
    break;

  default:
    if (CancelPairSecond(exp2))
      return true;
    break;
  }

  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::CancelPairSecond(CGCLCProverExpression &exp2) {
  switch (exp2.GetType()) {
  case ep_mult:
    if (CancelPair(exp2))
      return true;
    if (CancelPairSecond(exp2.GetArg(0)))
      return true;
    if (CancelPairSecond(exp2.GetArg(1)))
      return true;
    break;

  default:
    if (CancelPair(exp2))
      return true;
    break;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::CancelPair(CGCLCProverExpression &exp2) {
  if (*this == exp2) {
    *this = 1.0;
    exp2 = 1.0;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::RightAssoc() {
  if (type == ep_mult && GetArg(0).type == ep_mult) {
    // (a*b)*c -> a*(b*c)
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1));
    *this = (a * (b * c));
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::DistrMultOverAdd() {
  // n*(a+b) -> n*a+n*b
  if (type == ep_mult && GetArg(1).type == ep_sum) {
    CGCLCProverExpression n(GetArg(0));
    CGCLCProverExpression a(GetArg(1).GetArg(0));
    CGCLCProverExpression b(GetArg(1).GetArg(1));
    *this = ((n * a) + (n * b));
    return true;
  }
  // (a+b)*n -> n*a+n*b
  if (type == ep_mult && GetArg(0).type == ep_sum) {
    CGCLCProverExpression n(GetArg(1));
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    *this = ((n * a) + (n * b));
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::MultipleFraction() {
  // (a/b)/(c/d) = (a*d)/(b*c)
  if (type == ep_ratio && GetArg(0).type == ep_ratio &&
      GetArg(1).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    *this = ((a * d) / (b * c));
    return true;
  }
  // (a/b)/c -> a/(b*c)
  if (type == ep_ratio && GetArg(0).type == ep_ratio &&
      GetArg(1).type != ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1));
    *this = (a / (b * c));
    return true;
  }
  // a/(c/d) = (a*d)/c
  if (type == ep_ratio && GetArg(0).type != ep_ratio &&
      GetArg(1).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    *this = ((a * d) / c);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::FractionsOnTwoSides() {
  // a/b = c/b -> a/1 = c/1
  if (type == ep_equality && GetArg(0).type == ep_ratio &&
      GetArg(1).type == ep_ratio && GetArg(0).GetArg(1) == GetArg(1).GetArg(1))
    if ((GetArg(0).GetArg(1).type != ep_number) ||
        (GetArg(0).GetArg(1).nNumber != 1)) {
      CGCLCProverExpression a(GetArg(0).GetArg(0));
      CGCLCProverExpression b(1.0);
      CGCLCProverExpression c(GetArg(1).GetArg(0));
      Set(ep_equality, a / b, c / b);
      return true;
    }
  // a/b = c/d -> a*d = c*b
  if (type == ep_equality && GetArg(0).type == ep_ratio &&
      GetArg(1).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    Set(ep_equality, a * d, c * b);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::EliminateFraction() {
  // a/b = c -> a = b*c
  if (type == ep_equality && GetArg(0).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0).GetArg(0));
    CGCLCProverExpression b(GetArg(0).GetArg(1));
    CGCLCProverExpression c(GetArg(1));
    Set(ep_equality, a, b * c);
    return true;
  }
  // a = c/d -> a*d = c
  if (type == ep_equality && GetArg(1).type == ep_ratio) {
    CGCLCProverExpression a(GetArg(0));
    CGCLCProverExpression c(GetArg(1).GetArg(0));
    CGCLCProverExpression d(GetArg(1).GetArg(1));
    Set(ep_equality, a * d, c);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::SumOfFractions() {
  if (type != ep_sum)
    return false;

  if (GetArg(0).type == ep_ratio) {
    if (GetArg(1).type == ep_ratio) {
      if (GetArg(0).GetArg(1) == GetArg(1).GetArg(1)) {
        // a/b + c/b = (a+c)/b
        CGCLCProverExpression a(GetArg(0).GetArg(0));
        CGCLCProverExpression b(GetArg(0).GetArg(1));
        CGCLCProverExpression c(GetArg(1).GetArg(0));
        *this = ((a + c) / b);
        return true;
      } else { // a/b + c/d = (ad+bc)/bd
        CGCLCProverExpression a(GetArg(0).GetArg(0));
        CGCLCProverExpression b(GetArg(0).GetArg(1));
        CGCLCProverExpression c(GetArg(1).GetArg(0));
        CGCLCProverExpression d(GetArg(1).GetArg(1));
        *this = (((a * d) + (b * c)) / (b * d));
        return true;
      }
    } else { // GetArg(1).type != ep_ratio
      // a/b + c = (a+cb)/b
      CGCLCProverExpression a(GetArg(0).GetArg(0));
      CGCLCProverExpression b(GetArg(0).GetArg(1));
      CGCLCProverExpression c(GetArg(1));
      *this = ((a + (c * b)) / b);
      return true;
    }
  } else { // GetArg(0).type != ep_ratio
    if (GetArg(1).type == ep_ratio) {
      // c + a/b = (cb+a)/b
      CGCLCProverExpression a(GetArg(1).GetArg(0));
      CGCLCProverExpression b(GetArg(1).GetArg(1));
      CGCLCProverExpression c(GetArg(0));
      *this = (((c * b) + a) / b);
      return true;
    }
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::CancelationMult(
    const CGCLCProverExpression &Factor) {

  int iMaxIndex = CountTopLevelOperands(ep_sum);
  std::vector<CGCLCProverExpression *> aSummands;
  aSummands.reserve(iMaxIndex);

  FillTopLevelOperands(ep_sum, aSummands);

  for (CGCLCProverExpression *j : aSummands) {
    std::vector<CGCLCProverExpression *> aMultiplicants;
    aMultiplicants.reserve(j->CountTopLevelOperands(ep_mult));

    j->FillTopLevelOperands(ep_mult, aMultiplicants);

    for (CGCLCProverExpression *k : aMultiplicants) {
      if (Factor == *k) {
        *k = 1.0;

        break;
      } else {
        if (k->GetType() == ep_number)
          if (k->GetNumber() == 0)
            break;
      }
    }
  }
  return true;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::AllSummandsHaveFactor(
    const CGCLCProverExpression &Factor) {

  int iMaxIndex = CountTopLevelOperands(ep_sum);

  std::vector<CGCLCProverExpression *> aSummands;
  aSummands.reserve(iMaxIndex);

  FillTopLevelOperands(ep_sum, aSummands);

  bool bEverywhere, bExists;

  bEverywhere = true;
  for (CGCLCProverExpression *j : aSummands) {
    if (j->GetType() != ep_number || j->GetNumber() != 0) {
      std::vector<CGCLCProverExpression *> aMultiplicants;
      aMultiplicants.reserve(j->CountTopLevelOperands(ep_mult));

      j->FillTopLevelOperands(ep_mult, aMultiplicants);

      bExists = false;

      for (CGCLCProverExpression *k : aMultiplicants) {
        if (Factor == *k) {
          bExists = true;
          break;
        }
      }
      if (!bExists) {
        bEverywhere = false;
        break;
      }
    }
  }
  return bEverywhere;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::SimilarSummandsOnTwoSides() {
  if (type != ep_equality)
    return false;
  if (GetArg(0).type == ep_number)
    return false;
  if (GetArg(1).type == ep_number)
    return false;

  int iMaxIndex1 = GetArg(0).CountTopLevelOperands(ep_sum);
  int iMaxIndex2 = GetArg(1).CountTopLevelOperands(ep_sum);

  std::vector<CGCLCProverExpression *> aSummands1;
  aSummands1.reserve(iMaxIndex1);
  GetArg(0).FillTopLevelOperands(ep_sum, aSummands1);

  std::vector<CGCLCProverExpression *> aSummands2;
  aSummands2.reserve(iMaxIndex2);
  GetArg(1).FillTopLevelOperands(ep_sum, aSummands2);

  for (CGCLCProverExpression *i : aSummands1) {

    std::vector<CGCLCProverExpression *> aMultiplicants1;
    aMultiplicants1.reserve(i->CountTopLevelOperands(ep_mult));

    i->FillTopLevelOperands(ep_mult, aMultiplicants1);

    for (CGCLCProverExpression *j : aSummands2) {
      std::vector<CGCLCProverExpression *> aMultiplicants2;
      aMultiplicants2.reserve(j->CountTopLevelOperands(ep_mult));

      j->FillTopLevelOperands(ep_mult, aMultiplicants2);

      const size_t k0 = aMultiplicants1[0]->type == ep_number ? 1 : 0;
      const size_t l0 = aMultiplicants2[0]->type == ep_number ? 1 : 0;

      if (aMultiplicants1.size() - k0 != aMultiplicants2.size() - l0) {
        continue;
      }

      bool bAllFound = true;
      for (size_t k = k0; k < aMultiplicants1.size() && bAllFound; k++) {
        bool bFoundMatching = false;
        for (size_t l = l0; l < aMultiplicants2.size() && !bFoundMatching; l++)
          if (aMultiplicants2[l] != NULL)
            if (*(aMultiplicants1[k]) == *(aMultiplicants2[l])) {
              bFoundMatching = true;
              aMultiplicants2[l] = NULL;
            }
        if (!bFoundMatching)
          bAllFound = false;
      }

      if (bAllFound) {
        if (k0 == 1) {
          if (l0 == 0)
            aMultiplicants1[0]->nNumber -= 1;
          else
            aMultiplicants1[0]->nNumber -= aMultiplicants2[0]->nNumber;
          if (fabs(aMultiplicants1[0]->nNumber) < EPSILON)
            aMultiplicants1[0]->nNumber = 0; // changed 22.02.2006.
        } else {
          double n;
          if (l0 == 0)
            n = 0;
          else
            n = 1 - aMultiplicants2[0]->nNumber;

          CGCLCProverExpression a(*i);
          CGCLCProverExpression b(n);
          *i = b * a;
        }

        *j = 0.0;

        return true;
      }
    }
  }

  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::SimilarSummands() {
  if (type != ep_sum)
    return false;

  int iMaxIndex = CountTopLevelOperands(ep_sum);
  if (iMaxIndex == 1)
    return false;

  std::vector<CGCLCProverExpression *> aSummands;
  aSummands.reserve(iMaxIndex);

  FillTopLevelOperands(ep_sum, aSummands);

  for (size_t i = 0; i < aSummands.size(); i++) {

    std::vector<CGCLCProverExpression *> aMultiplicants1;
    aMultiplicants1.reserve(aSummands[i]->CountTopLevelOperands(ep_mult));

    aSummands[i]->FillTopLevelOperands(ep_mult, aMultiplicants1);

    for (size_t j = i + 1; j < aSummands.size(); j++) {
      std::vector<CGCLCProverExpression *> aMultiplicants2;
      aMultiplicants2.reserve(aSummands[j]->CountTopLevelOperands(ep_mult));

      aSummands[j]->FillTopLevelOperands(ep_mult, aMultiplicants2);

      const size_t k0 = aMultiplicants1[0]->type == ep_number ? 1 : 0;
      const size_t l0 = aMultiplicants2[0]->type == ep_number ? 1 : 0;

      if (aMultiplicants1.size() - k0 != aMultiplicants2.size() - l0) {
        continue;
      }

      bool bAllFound = true;
      for (size_t k = k0; k < aMultiplicants1.size() && bAllFound; k++) {
        bool bFoundMatching = false;
        for (size_t l = l0; l < aMultiplicants2.size() && !bFoundMatching; l++)
          if (aMultiplicants2[l] != NULL)
            if (*(aMultiplicants1[k]) == *(aMultiplicants2[l])) {
              bFoundMatching = true;
              aMultiplicants2[l] = NULL;
            }
        if (!bFoundMatching)
          bAllFound = false;
      }

      if (bAllFound) {
        if (k0 == 1) {
          if (l0 == 0)
            aMultiplicants1[0]->nNumber += 1;
          else {
            aMultiplicants1[0]->nNumber += aMultiplicants2[0]->nNumber;
            if (fabs(aMultiplicants1[0]->nNumber) < EPSILON)
              aMultiplicants1[0]->nNumber = 0; // changed 22.02.2006.
          }
        } else {
          double n;
          if (l0 == 0)
            n = 2;
          else
            n = 1 + aMultiplicants2[0]->nNumber;

          CGCLCProverExpression a(*(aSummands[i]));
          CGCLCProverExpression b(n);
          *aSummands[i] = (b * a);
        }

        *aSummands[j] = 0.0;

        return true;
      }
    }
  }

  return false;
}

// --------------------------------------------------------------------------

int CGCLCProverExpression::CountTopLevelOperands(GCLCexpression_type t) {
  if (type != t)
    return 1;
  else
    return GetArg(0).CountTopLevelOperands(t) +
           GetArg(1).CountTopLevelOperands(t);
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::FillTopLevelOperands(GCLCexpression_type t,
                                                 std::vector<CGCLCProverExpression *> &a) {
  if (type != t)
    a.push_back(this);
  else {
    GetArg(0).FillTopLevelOperands(t, a);
    GetArg(1).FillTopLevelOperands(t, a);
  }
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::OneSide() {
  // a = b -> a + -b
  // except for b=0 and numbers a,b such that a=b
  if (type == ep_equality) {
    if (GetArg(1).type == ep_number && GetArg(1).nNumber == 0)
      return false;
    if (GetArg(0).type == ep_number && GetArg(1).type == ep_number &&
        GetArg(0).nNumber == GetArg(1).nNumber)
      return false;

    CGCLCProverExpression a(GetArg(0));
    CGCLCProverExpression b(GetArg(1));
    CGCLCProverExpression negOne(-1);
    Set(ep_equality, a + (negOne * b), 0.0);
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------

bool CGCLCProverExpression::ToGeometricQuantities() {
  CGCLCProverExpression m1, m2;

  switch (type) {
  case ep_identical:
    m1 = p3(GetArgName(0), GetArgName(1), GetArgName(0));
    m2 = 0.0;
    break;

  case ep_collinear:
    m1 = s3(GetArgName(0), GetArgName(1), GetArgName(2));
    m2 = 0.0;
    break;

  case ep_midpoint:
    m1 = sratio(GetArgName(1), GetArgName(0), GetArgName(0), GetArgName(2));
    m2 = 1.0;
    break;

  case ep_parallel:
    m1 = s3(GetArgName(0), GetArgName(2), GetArgName(3));
    m2 = s3(GetArgName(1), GetArgName(2), GetArgName(3));
    break;

  case ep_perpendicular:
    m1 = p3(GetArgName(0), GetArgName(2), GetArgName(3));
    m2 = p3(GetArgName(1), GetArgName(2), GetArgName(3));
    break;

  case ep_same_length:
    m1 = p3(GetArgName(0), GetArgName(1), GetArgName(0));
    m2 = p3(GetArgName(2), GetArgName(3), GetArgName(2));
    break;

  case ep_harmonic:
    m1 = sratio(GetArgName(0), GetArgName(2), GetArgName(2), GetArgName(1));
    m2 = sratio(GetArgName(3), GetArgName(0), GetArgName(3), GetArgName(1));
    break;

  default:
    return false;
  }

  Set(ep_equality, m1, m2);
  return true;
}

// --------------------------------------------------------------------------

bool CTheoremProver::GetPointCoordinates(const std::string &sPoint, double &x,
                                         double &y) const {
  for (std::list<CGCLCProverCommand>::const_iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if ((it->type == p_point) || (it->type == p_inter) ||
        (it->type == p_pratio) || (it->type == p_tratio) ||
        (it->type == p_foot)) {
      if (it->arg[0] == sPoint) {
        x = it->x;
        y = it->y;
        return true;
      }
    }
  }
  return false;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::SetType(GCLCexpression_type t) { type = t; }

// --------------------------------------------------------------------------

int CGCLCProverExpression::Size() {
  switch (type) {
  case ep_number:
  case ep_point:
  case ep_constant:
  case ep_segment:
  case ep_segment_ratio:
  case ep_s3:
  case ep_s4:
  case ep_p3:
  case ep_p4:
    return 1;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    return GetArg(0).Size() + GetArg(1).Size();

  default:
    return 0;
  }
  return 0;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::PrettyPrint() const {
  switch (type) {
  case ep_number:
    if ((double)((int)nNumber) == nNumber)
      Print(cout, " " + i2s((int)nNumber) + " ");
    else
      Print(cout, " " + d2s(nNumber, 2) + " ");
    break;

  case ep_constant:
    Print(cout, sName);
    break;

  case ep_point:
    Print(cout, " " + sName);
    break;

  case ep_equality:
    Print(cout, "(equal ");
    break;

  case ep_diffx:
    Print(cout, "(diffx ");
    break;

  case ep_diffy:
    Print(cout, "(diffy ");
    break;

  case ep_sum:
    Print(cout, "(sum ");
    break;

  case ep_ratio:
    Print(cout, "(ratio ");
    break;

  case ep_segment:
    Print(cout, "(segment ");
    break;

  case ep_segment_ratio:
    Print(cout, "(sratio ");
    break;

  case ep_mult:
    Print(cout, "(ep_mult ");
    break;

  case ep_s3:
    Print(cout, "(signed_area3 ");
    break;

  case ep_s4:
    Print(cout, "(signed_area4 ");
    break;

  case ep_p3:
    Print(cout, "(pythagoras_difference3 ");
    break;

  case ep_p4:
    Print(cout, "(pythagoras_difference4 ");
    break;

  default:
    break;
  }

  switch (type) {
  case ep_point:
    break;

  case ep_equality:
  case ep_sum:
  case ep_mult:
  case ep_ratio:
  case ep_segment:
  case ep_diffx:
  case ep_diffy:
    GetArg(0).PrettyPrint();
    GetArg(1).PrettyPrint();
    Print(cout, ")");
    break;

  case ep_s3:
  case ep_p3:
    GetArg(0).PrettyPrint();
    GetArg(1).PrettyPrint();
    GetArg(2).PrettyPrint();
    Print(cout, ")");
    break;

  case ep_segment_ratio:
  case ep_s4:
  case ep_p4:
    GetArg(0).PrettyPrint();
    GetArg(1).PrettyPrint();
    GetArg(2).PrettyPrint();
    GetArg(3).PrettyPrint();
    Print(cout, ")");
    break;

  case ep_unknown:
    Print(cout, "unkown");
    break;

  default:
    break;
  }
  cout << flush;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::PrintLaTeX(ofstream &h) const {
  Print(h, sPrintLaTeX(true));
}

// --------------------------------------------------------------------------

std::string CGCLCProverExpression::sPrintLaTeX(bool hasFractions) const {
  std::string s;
  switch (type) {
  case ep_point:
    s = sName;
    break;

  case ep_constant:
    s = sName;
    break;

  case ep_number:
    if ((double)((int)nNumber) == nNumber)
      s = " " + i2s((int)nNumber) + " ";
    else {
      if (nNumber == 0.5)
        s = "\\frac{1}{2} ";
      else if (nNumber == -0.5)
        s = "-\\frac{1}{2} ";
      else if (nNumber == 0.25)
        s = "\\frac{1}{4} ";
      else if (nNumber == -0.25)
        s = "-\\frac{1}{4} ";
      else if (nNumber == 0.75)
        s = "\\frac{3}{4} ";
      else if (nNumber == -0.75)
        s = "-\\frac{3}{4} ";
      else if (nNumber == 0.125)
        s = "\\frac{1}{8} ";
      else if (nNumber == -0.125)
        s = "-\\frac{1}{8} ";
      else
        s = d2s(nNumber, -1);
    }
    break;

  case ep_inequality:
    s = GetArg(0).sPrintLaTeX(hasFractions) + " \\neq " +
        GetArg(1).sPrintLaTeX(hasFractions);
    break;

  case ep_equality:
    s = GetArg(0).sPrintLaTeX(hasFractions) + " = " +
        GetArg(1).sPrintLaTeX(hasFractions);
    break;

  case ep_sum:
    s = (hasFractions ? " \\left(" : " (") +
        GetArg(0).sPrintLaTeX(hasFractions) + " + " +
        GetArg(1).sPrintLaTeX(hasFractions) + (hasFractions ? "\\right)" : ")");
    break;

  case ep_mult:
    s = (hasFractions ? " \\left(" : " (") +
        GetArg(0).sPrintLaTeX(hasFractions) + " \\cdot " +
        GetArg(1).sPrintLaTeX(hasFractions) + (hasFractions ? "\\right)" : ")");
    break;

  case ep_ratio:
    s = " \\frac{" + GetArg(0).sPrintLaTeX(hasFractions) + "}{" +
        GetArg(1).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_s3:
    s = " S_{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_p3:
    s = " P_{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_segment:
    s = " {" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_segment_ratio:
    s = " \\frac{\\overrightarrow{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "}}{\\overrightarrow{" +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + "}}";
    break;

  case ep_harmonic:
    s = " \\frac{\\overrightarrow{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + "}}{\\overrightarrow{" +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "}} \\cdot " +
        " \\frac{\\overrightarrow{" + GetArg(3).sPrintLaTeX(hasFractions) +
        GetArg(0).sPrintLaTeX(hasFractions) + "}}{\\overrightarrow{" +
        GetArg(3).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "}} = 1";
    break;

  case ep_s4:
    s = " S_{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_p4:
    s = " P_{" + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + "}";
    break;

  case ep_diffy:
  case ep_diffx:
    s = (string)(hasFractions ? "\\left(" : " (") +
        (type == ep_diffy ? "y" : "x") + (hasFractions ? "\\left(" : "(") +
        GetArg(0).sPrintLaTeX(hasFractions) +
        (hasFractions ? "\\right) - " : ") - ") +
        (type == ep_diffy ? "y" : "x") + (hasFractions ? "\\left(" : "(") +
        GetArg(1).sPrintLaTeX(hasFractions) +
        (hasFractions ? "\\right)" : ")") + (hasFractions ? "\\right)" : ")");
    break;

  case ep_algsumzero3:
    s = GetArg(0).sPrintLaTeX(hasFractions) + "+" +
        GetArg(1).sPrintLaTeX(hasFractions) + "+" +
        GetArg(2).sPrintLaTeX(hasFractions) + "=0 ";
    break;

  case ep_angle:
    s = "tan(\\angle " + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + ") ";
    break;

  case ep_tangens_num:
    s = "\\angle " + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + " ";
    break;

  case ep_tangens_den:
    s = "\\frac{1}{\\angle " + GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) +
        GetArg(2).sPrintLaTeX(hasFractions) + "} ";
    break;

  case ep_parallel:
    s = GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "\\parallel " +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + " ";
    break;

  case ep_perpendicular:
    s = GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "\\perp " +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + " ";
    break;

  case ep_collinear:
    s = "collinear( " + GetArg(0).sPrintLaTeX(hasFractions) + ", " +
        GetArg(1).sPrintLaTeX(hasFractions) + ", " +
        GetArg(2).sPrintLaTeX(hasFractions) + ") ";
    break;

  case ep_same_length:
    s = GetArg(0).sPrintLaTeX(hasFractions) +
        GetArg(1).sPrintLaTeX(hasFractions) + "\\cong " +
        GetArg(2).sPrintLaTeX(hasFractions) +
        GetArg(3).sPrintLaTeX(hasFractions) + " ";
    break;

  case ep_identical:
    s = GetArg(0).sPrintLaTeX(hasFractions) + "\\equiv " +
        GetArg(1).sPrintLaTeX(hasFractions) + " ";
    break;

  default:
    Print(cerr, "Output for command " + GetName() + " not implemented!\n");

    throw 0;
    break;
  }
  return s;
}

// --------------------------------------------------------------------------

void CGCLCProverExpression::PrintXML(ofstream &h, int indent) const {
  Print(h, sPrintXML(indent));
}

// --------------------------------------------------------------------------

std::string CGCLCProverExpression::sPrintXML(int indent) const {
  std::string s;

  switch (type) {
  case ep_point:
    s = "<point>" + sName + "</point>";
    break;

  case ep_constant:
    s = make_indent(indent) + "<constant>" + sName + "</constant>\n";
    break;

  case ep_number:
    s = make_indent(indent) + "<number>" + d2s(nNumber, -1) + "</number>\n";
    break;

  case ep_equality:
  case ep_identical: // added 25.04.2008
    s += make_indent(indent) + "<equality>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(0).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(1).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent) +
         "</equality>\n";
    break;

  case ep_sum:
    s += make_indent(indent) + "<sum>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(0).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(1).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent) +
         "</sum>\n";
    break;

  case ep_mult:
    s += make_indent(indent) + "<mult>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(0).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(1).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent) +
         "</mult>\n";
    break;

  case ep_ratio:
    s += make_indent(indent) + "<fraction>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(0).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent + 1) +
         "<expression>\n" + GetArg(1).sPrintXML(indent + 2) +
         make_indent(indent + 1) + "</expression>\n" + make_indent(indent) +
         "</fraction>\n";
    break;

  case ep_s3:
    s += make_indent(indent) + "<signed_area3>" +
         GetArg(0).sPrintXML(indent + 1) + GetArg(1).sPrintXML(indent + 1) +
         GetArg(2).sPrintXML(indent + 1) + "</signed_area3>\n";
    break;

  case ep_p3:
    s += make_indent(indent) + "<pythagoras_difference3>" +
         GetArg(0).sPrintXML(indent + 1) + GetArg(1).sPrintXML(indent + 1) +
         GetArg(2).sPrintXML(indent + 1) + "</pythagoras_difference3>\n";
    break;

  case ep_segment:
    s += make_indent(indent) + "<segment>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + "</segment>\n";
    break;

  case ep_segment_ratio:
    s += make_indent(indent) + "<segment_ratio><segment>" +
         GetArg(0).sPrintXML(indent + 1) + GetArg(1).sPrintXML(indent + 1) +
         "</segment>" + "<segment>" + GetArg(2).sPrintXML(indent + 1) +
         GetArg(3).sPrintXML(indent + 1) + "</segment></segment_ratio>\n";

    break;

  case ep_s4:
    s += make_indent(indent) + "<signed_area4>" +
         GetArg(0).sPrintXML(indent + 1) + GetArg(1).sPrintXML(indent + 1) +
         GetArg(2).sPrintXML(indent + 1) + GetArg(3).sPrintXML(indent + 1) +
         "</signed_area4>\n";
    break;

  case ep_p4:
    s += make_indent(indent) + "<pythagoras_difference4>" +
         GetArg(0).sPrintXML(indent + 1) + GetArg(1).sPrintXML(indent + 1) +
         GetArg(2).sPrintXML(indent + 1) + GetArg(3).sPrintXML(indent + 1) +
         "</pythagoras_difference4>\n";
    break;

  case ep_angle:
    s += "<angle_tangens>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         "</angle_tangens>\n";
    break;

  case ep_algsumzero3:
    s += "<algebraic_sum_is_zero>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         "</algebraic_sum_is_zero>\n";
    break;

  case ep_collinear:
    s += make_indent(indent) + "<collinear>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         "</collinear>\n";
    break;

  case ep_diffx:
    s += "<x_projection_difference>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + "</x_projection_difference>\n";
    break;

  case ep_diffy:
    s += "<y_projection_difference>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + "</y_projection_difference>\n";
    break;

  case ep_harmonic:
    s += make_indent(indent) + "<harmonic>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         GetArg(3).sPrintXML(indent + 1) + "</harmonic>\n";
    break;

  case ep_parallel:
    s += make_indent(indent) + "<parallel>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         GetArg(3).sPrintXML(indent + 1) + "</parallel>\n";
    break;

  case ep_perpendicular:
    s += make_indent(indent) + "<perpendicular>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         GetArg(3).sPrintXML(indent + 1) + "</perpendicular>\n";
    break;

  case ep_tangens_den:
    s += "<angle_tangens_denominator>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         "</angle_tangens_denominator>\n";
    break;

  case ep_tangens_num:
    s += "<angle_tangens_numerator>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + GetArg(2).sPrintXML(indent + 1) +
         "</angle_tangens_numerator>\n";
    break;

  case ep_same_length:
    s += "<equal_segments> <segment>" + GetArg(0).sPrintXML(indent + 1) +
         GetArg(1).sPrintXML(indent + 1) + "</segment>\n<segment>" +
         GetArg(2).sPrintXML(indent + 1) + GetArg(3).sPrintXML(indent + 1) +
         "</segment>\n</equal_segments>\n";
    break;

  default:
    Print(cerr, "Command " + GetName() + " is not supported for XML output!\n");
    throw - 1;
    break;
  }
  return s;
}
