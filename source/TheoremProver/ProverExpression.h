#if !defined(prover_expression_h)
#define prover_expression_h

#include <sstream>
#include <string>

enum GCLCexperssion_type {
  ep_point,
  ep_number,
  ep_constant,
  ep_equality,
  ep_inequality,
  ep_sum,
  ep_mult,
  ep_ratio,
  ep_segment,
  ep_segment_ratio,
  ep_s3,
  ep_s4,
  ep_p3,
  ep_p4,
  ep_identical,
  ep_collinear,
  ep_perpendicular,
  ep_parallel,
  ep_midpoint,
  ep_same_length,
  ep_harmonic,
  ep_unknown,
  // new
  ep_diffx, // used in rationalization
  ep_diffy, // used in rationalization
  ep_algsumzero3,
  ep_angle,
  ep_tangens_num, // used in rationalization of ep_angle
  ep_tangens_den  // used in rationalization of ep_angle
};

enum GCLCalgebraic_rule {
  er_RatioCancellation,
  er_MultiplicationOfConstants,
  er_MultiplicationWithOne,
  er_ZeroFractionUp,
  er_MultiplicationWithZero,
  er_AdditionWithZero,
  er_CommutativityWithNumber,
  er_AssociativityAndCommutativity,
  er_RightAssoc,
  er_MultiplicationOfFractions,
  er_DistrMultOverAdd,
  er_MultipleFraction,
  er_SumOfFractions,
  er_SimilarSummands,
  er_SimilarSummandsOnTwoSides,
  er_FractionEqualZero,
  er_FractionWithNumberDenominator,
  er_EliminateFraction,
  er_OneSide,
  er_FractionsOnTwoSides,
  er_RuleMax,
};

class Rules {
public:
  std::string m_asRuleName[er_RuleMax];
  GCLCalgebraic_rule m_aiRule[er_RuleMax];
  unsigned m_nNumberOfRules;

  Rules() {
    int index = 0;
    m_aiRule[index++] = er_MultiplicationWithZero;
    m_aiRule[index++] = er_MultiplicationWithOne;
    m_aiRule[index++] = er_ZeroFractionUp;
    m_aiRule[index++] = er_FractionEqualZero;
    m_aiRule[index++] = er_AdditionWithZero;
    m_aiRule[index++] = er_MultiplicationOfConstants;
    m_aiRule[index++] = er_RatioCancellation;
    m_aiRule[index++] = er_SimilarSummands;
    m_aiRule[index++] = er_SimilarSummandsOnTwoSides;
    m_aiRule[index++] = er_FractionWithNumberDenominator;
    m_aiRule[index++] = er_MultipleFraction;
    m_aiRule[index++] = er_MultiplicationOfFractions;
    m_aiRule[index++] = er_CommutativityWithNumber;
    m_aiRule[index++] = er_AssociativityAndCommutativity;
    m_aiRule[index++] = er_RightAssoc;
    m_aiRule[index++] = er_DistrMultOverAdd;
    m_aiRule[index++] = er_SumOfFractions;
    m_aiRule[index++] = er_FractionsOnTwoSides;
    m_aiRule[index++] = er_EliminateFraction;
    m_aiRule[index++] = er_OneSide;
    m_nNumberOfRules = index;

    m_asRuleName[er_RatioCancellation] = "ratio cancellation";
    m_asRuleName[er_MultiplicationOfConstants] = "multiplication of constants";
    m_asRuleName[er_MultiplicationWithOne] = "multiplication by 1";
    m_asRuleName[er_AdditionWithZero] = "addition with 0";
    m_asRuleName[er_CommutativityWithNumber] = "commutativity with constant";
    m_asRuleName[er_AssociativityAndCommutativity] =
        "associativity and commutativity";
    m_asRuleName[er_RightAssoc] = "right association";
    m_asRuleName[er_MultiplicationOfFractions] = "multiplication of fractions";
    m_asRuleName[er_DistrMultOverAdd] =
        "distribution of multiplication over addition";
    m_asRuleName[er_MultiplicationWithZero] = "multiplication by 0";
    m_asRuleName[er_ZeroFractionUp] = "0 numerator in fraction";
    m_asRuleName[er_MultipleFraction] = "multiple fraction simplification";
    m_asRuleName[er_SumOfFractions] = "sum of fractions";
    m_asRuleName[er_SimilarSummands] = "similar summands";
    m_asRuleName[er_SimilarSummandsOnTwoSides] =
        "similar summands on two sides";
    m_asRuleName[er_FractionEqualZero] = "fraction equal 0";
    m_asRuleName[er_FractionWithNumberDenominator] =
        "fraction with number denominator";
    m_asRuleName[er_FractionsOnTwoSides] = "fractions on two sides";
    m_asRuleName[er_EliminateFraction] = "elimination of fraction";
    m_asRuleName[er_OneSide] = "one side rule";
  }
};

const unsigned ExpressionArgCount = 4;
class CGCLCProverExpression {
public:
  CGCLCProverExpression();
  CGCLCProverExpression(const CGCLCProverExpression &r);
  CGCLCProverExpression(GCLCexperssion_type t, const std::string &arg);
  CGCLCProverExpression(GCLCexperssion_type t, const std::string &a0,
                        const std::string &a1);
  CGCLCProverExpression(GCLCexperssion_type t, const std::string &a0,
                        const std::string &a1, const std::string &a2);
  CGCLCProverExpression(GCLCexperssion_type t, const std::string &a0,
                        const std::string &a1, const std::string &a2, const std::string &a3);
  CGCLCProverExpression(const double n);

  virtual ~CGCLCProverExpression();
  void CleanUp();

  CGCLCProverExpression &operator=(const CGCLCProverExpression &r);
  CGCLCProverExpression &operator=(const double n);
  CGCLCProverExpression(GCLCexperssion_type t,
                        const CGCLCProverExpression &arg0,
                        const CGCLCProverExpression &arg1);

  static CGCLCProverExpression s3(const std::string &a0, const std::string &a1,
                                  const std::string &a2);
  static CGCLCProverExpression p3(const std::string &a0, const std::string &a1,
                                  const std::string &a2);
  static CGCLCProverExpression s4(const std::string &a0, const std::string &a1,
                                  const std::string &a2, const std::string &a3);
  static CGCLCProverExpression p4(const std::string &a0, const std::string &a1,
                                  const std::string &a2, const std::string &a3);
  static CGCLCProverExpression sratio(const std::string &a0, const std::string &a1,
                                      const std::string &a2, const std::string &a3);
  static CGCLCProverExpression segment(const std::string &a0, const std::string &a1);

  CGCLCProverExpression operator*(const CGCLCProverExpression &a);
  CGCLCProverExpression operator+(const CGCLCProverExpression &a);
  CGCLCProverExpression operator/(const CGCLCProverExpression &a);

  void Set(GCLCexperssion_type t, const CGCLCProverExpression &arg0,
           const CGCLCProverExpression &arg1);
  void SetType(GCLCexperssion_type t);
  GCLCexperssion_type GetType() const { return type; }

  double GetNumber() const { return nNumber; }

  void SetName(std::string s) { sName = s; }
  const std::string GetName() const { return sName; }

  void SetArg(unsigned i, CGCLCProverExpression *a);
  void SetArg(unsigned i, const CGCLCProverExpression &a);
  void SetArgName(unsigned i, const std::string &s);
  CGCLCProverExpression &GetArg(int i) const { return *arg[i]; }
  CGCLCProverExpression *GetArgP(int i) const { return arg[i]; }
  std::string GetArgName(unsigned i) const;

  bool operator==(const CGCLCProverExpression &r) const;
  bool Equal(const CGCLCProverExpression &r) const;

  bool ToGeometricQuantities();

  bool ApplyAllAlgebraicRules(std::string &sRuleApplied, int iExceptLast = 0);
  bool ApplyAllSimpleAlgebraicRules(std::string &sRuleApplied, int iExceptLast = 0);
  bool ApplyAllComplexAlgebraicRules(std::string &sRuleApplied, int iExceptLast = 0);
  bool ApplyOneAlgebraicRule(GCLCalgebraic_rule rule);

  void PrettyPrint() const;
  void PrintLaTeX(std::ofstream &h) const;
  std::string sPrintLaTeX(bool hasFractions = true) const;
  void PrintXML(std::ofstream &h, int indent) const;
  std::string sPrintXML(int indent) const;

  int Size();

  bool ExistsAtomicExpression(const CGCLCProverExpression &subexp) const;
  bool Replace(const CGCLCProverExpression &LHS,
               const CGCLCProverExpression &RHS);

  int CountTopLevelOperands(GCLCexperssion_type t);
  void FillTopLevelOperands(GCLCexperssion_type t, CGCLCProverExpression **a,
                            int &iIndex);
  bool CancelationMult(const CGCLCProverExpression &Factor);
  bool AllSummandsHaveFactor(const CGCLCProverExpression &Factor);

  // Used (only) in proving subgoals
  void Push(CGCLCProverExpression &left, CGCLCProverExpression &right,
            const CGCLCProverExpression &a);
  void Pop(CGCLCProverExpression &left, CGCLCProverExpression &right);

  bool GetParentIndex(CGCLCProverExpression &outer,
                      CGCLCProverExpression *&parent, int &index) const;

private:
  GCLCexperssion_type type;
  CGCLCProverExpression *arg[ExpressionArgCount];

  std::string sName;
  double nNumber;

  bool MultiplicationOfConstants();
  bool MultiplicationWithZero();
  bool FractionWithNumberDenominator();
  bool ZeroFractionUp();
  bool MultiplicationWithOne();
  bool AdditionWithZero();
  bool CommutativityWithNumber();
  bool AssociativityAndCommutativity();
  bool DistrMultOverAdd();
  bool MultipleFraction();
  bool SumOfFractions();
  bool SimilarSummands();
  bool SimilarSummandsOnTwoSides();
  bool FractionEqualZero();
  bool FractionsOnTwoSides();
  bool MultiplicationOfFractions();
  bool RightAssoc();
  bool EliminateFraction();
  bool OneSide();

  bool RatioCancellation();
  bool CancelPairFirst(CGCLCProverExpression &exp2);
  bool CancelPairSecond(CGCLCProverExpression &exp2);
  bool CancelPair(CGCLCProverExpression &exp2);

  static Rules mRules;

  unsigned id;
};

unsigned arity(enum GCLCexperssion_type type);

#endif // !defined(prover_expression_h)
