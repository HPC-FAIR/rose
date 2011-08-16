#ifndef memory_object_impl_INCLUDED
#define memory_object_impl_INCLUDED

#include "memory_object.h"

namespace AbstractMemoryObject 
{

  // A simple implementation of the abstract memory object interface
  // Four categories: scalar, labeled aggregate, array, and pointer
  class Scalar_Impl : public Scalar 
  {
    public:
      virtual size_t objCount();    
  };

  class LabeledAggregate_Impl : public LabeledAggregate
  {
    public:
      virtual size_t objCount();    

  };

  class Array_Impl : public Array
  {
    public:
      virtual size_t objCount();    
  };

  class Pointer_Impl: public Pointer 
  {
    public:
      virtual size_t objCount();    
  };

  // The connection to the ROSE AST, all concrete type, size , etc. information come from this side
  //
  // Three kinds of memory objects in ROSE AST: each of them can be one of the four categories above.
  // 1) SgExpression temporary variables: each SgExpression which is not named memory objects 
  // 2) named memory objects : one object for each named variable 
  // 3) aliased memory objects: one object for each type

  class ExprObj // one object for each SgExpression which does not have a corresponding symbol
  { 
    public:
      SgExpression* anchor_exp; 
      SgType* type; 
      ObjSet* parent; // Can we decide that the parent mem object is always ExprObj?? 
                       // Does a parent ever exist for ExprObj???     
      ExprObj (SgExpression* a, SgType* t, ObjSet* p): anchor_exp(a), type(t),parent(p){};
      SgType* getType() {return type;}
  };

  class NamedObj  // one object for each named variable with a symbol
  { 
    public:
      SgSymbol* anchor_symbol; 
      SgType* type; 
      ObjSet* parent;  //Only exists for compound variables like a.b, where a is b's parent
      //Is this always true that the parent of a named object must be an expr object?
      NamedObj (SgSymbol* a, SgType* t, ObjSet* p):anchor_symbol(a), type(t),parent(p){};
      SgType* getType() {return type;}
  };

  class AliasedObj 
  {  // One object for each type
    public: 
    SgType* type; 
    AliasedObj (SgType* t): type(t) {};
    SgType* getType() {return type;}
  };

  //Derived classes for each kind of each category
  // expression object ------------------------------
  class ScalarExprObj: public Scalar_Impl, public ExprObj
  {
  };

  class LabeledAggregateExprObj: public LabeledAggregate_Impl, public ExprObj
  {
  };

  class ArrayExprObj: public Array_Impl, public ExprObj
  {};

  class PointerExprbj: public Pointer_Impl, public ExprObj
  {};

  // named object --------------------------------
   class ScalarNamedObj: public Scalar_Impl, public NamedObj 
  {
  };

  class LabeledAggregateNamedObj: public LabeledAggregate_Impl, public NamedObj
  {
  };

  class ArrayNamedObj: public Array_Impl, public NamedObj
  {};

  class PointerNamedObj: public Pointer_Impl, public NamedObj
  {};


 
  // aliased object -----------------------------
   class ScalarAliasedObj: public Scalar_Impl, public AliasedObj
  {
    public:
      ScalarAliasedObj (SgType* t): AliasedObj(t){}
      std::set<SgType*> getType();
  };

  class LabeledAggregateAliasedObj : public  LabeledAggregate_Impl, public AliasedObj
  {
    public:
    LabeledAggregateAliasedObj (SgType* t): AliasedObj(t){}
    std::set<SgType*> getType();
  };

  class ArrayAliasedObj: public Array_Impl, public AliasedObj
  {
    public:
      ArrayAliasedObj (SgType* t): AliasedObj(t){}
      std::set<SgType*> getType();
  };

  class PointerAliasedObj: public Pointer_Impl, public AliasedObj
  {
    public:
      PointerAliasedObj (SgType* t): AliasedObj(t){}
      std::set<SgType*> getType();
  };

  // builder for different objects -----------------------
  ObjSet* createAliasedObjSet(SgType*t);  // One object per type, Type based alias analysis
  ObjSet* createNamedObjSet(SgSymbol* anchor_symbol, SgType* t, ObjSet* parent); // any 
  ObjSet* createExpressionObjSet(SgExpression* anchor_exp, SgType*t, ObjSet* parent); 

} // end namespace

#endif
