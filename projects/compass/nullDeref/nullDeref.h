// Null Deref
// Author: Thomas Panas
// Date: 24-August-2007

#include "compass.h"

#ifndef COMPASS_NULL_DEREF_H
#define COMPASS_NULL_DEREF_H

#ifdef  USE_ROSE_BOOST_WAVE_SUPPORT
#include "BoostGraphInterface.hxx"
#endif

namespace CompassAnalyses
{ 
  namespace NullDeref
    { 
      /*! \brief Null Deref: Add your description here 
       */

      extern const std::string checkerName;
      extern const std::string shortDescription;
      extern const std::string longDescription;

      // Specification of Checker Output Implementation
      class CheckerOutput: public Compass::OutputViolationBase
	{ 
	public:
	  CheckerOutput(std::string problem, SgNode* node);
	};

      // Specification of Checker Traversal Implementation

#ifdef  USE_ROSE_BOOST_WAVE_SUPPORT
      class Traversal
	: public AstSimpleProcessing, public Compass::TraversalBase,  BOOSTGraphInterface::tps_graph_interface
	{
	  // Checker specific parameters should be allocated here.

	  std::string getFileName(SgFile* src);
	  bool isNULL(SgExpression* rightSide) const;
	  bool isLegitimateNullPointerCheck(SgExpression* expr, SgInitializedName* pointerVar, bool invertCheck) const;
	  void checkNullDeref(std::string analysisname, SgFunctionDeclaration* funcDecl, SgExpression* theExp, std::string name, int now, int max);

	  bool switchForAssignment(std::vector<BOOSTGraphInterface::tps_node> &vec, 
					  BOOSTGraphInterface::tps_node n, BOOSTGraphInterface::tps_node oldN, SgExpression* rhs) const;
	  std::vector <BOOSTGraphInterface::tps_node> tps_out_edges(BOOSTGraphInterface::tps_node node) const;

	  static std::string printTrace(const std::vector<BOOSTGraphInterface::tps_node>& trace);
	  static std::string print_tps_error(std::string error, BOOSTGraphInterface::tps_node n, BOOSTGraphInterface::tps_node oldN);

	  static std::string trimIfNeeded(const std::string& s);
	  static SgStatement* getStatement(SgNode* n);

	  template<typename T>    
	    static std::string ToString(T t){
	    std::ostringstream myStream; //creates an ostringstream object               
	    myStream << t << std::flush;       
	    return myStream.str(); //returns the string form of the stringstream object
	  }                                 

	  int counter;
	  int max;
	  static bool addressOp;
	  static bool debug;
#else
      class Traversal
	: public AstSimpleProcessing, public Compass::TraversalBase
	{

#endif

	public:
	  Traversal(Compass::Parameters inputParameters, Compass::OutputObject* output);

#ifdef  USE_ROSE_BOOST_WAVE_SUPPORT
	  // The implementation of the run function has to match the traversal being called.
	  void run(SgNode* n){ 
	    counter=0;
	    debug=false;
	    std::vector<SgNode*> exprList = NodeQuery:: querySubTree (n, V_SgFunctionDeclaration);
	    max = exprList.size();
	    this->traverse(n, preorder); 
	  };
#else
	  void run(SgNode* n){
       if (Compass::verboseSetting >= 0)   
            std::cout << " TURN ON BOOST FOR THIS ANALYSIS. " << std::endl;
	    this->traverse(n, preorder); };

#endif

	  void visit(SgNode* n);
	};
    }
}

// COMPASS_NULL_DEREF_H
#endif 


