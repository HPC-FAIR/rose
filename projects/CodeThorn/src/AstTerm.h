
#ifndef ASTTERM_H
#define ASTTERM_H

/*********************************
 * Author: Markus Schordan, 2012 *
 *********************************/

#include "rose.h"
#include "RoseAst.h"
#include <typeinfo>

using namespace std;

std::string nodeTypeName(SgNode* node);
std::string astTermToMultiLineString(SgNode* node,int sep);
std::string astTermWithNullValuesToString(SgNode* node);
std::string astTermWithNullValuesToDot(SgNode* root);
std::string astTermWithoutNullValuesToDot(SgNode* root);
std::string dotFragmentToDot(string fragment);
std::string astTermWithNullValuesToDotFragment(SgNode* root) ;
std::string astTermToDot(RoseAst::iterator start, RoseAst::iterator end);
std::string functionAstTermsWithNullValuesToDot(SgNode* root);
#endif
