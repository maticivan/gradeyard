//    GradeYard learning management system
//
//    Copyright (C) 2023 Ivan Matic, https://gradeyard.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU AFFERO GENERAL PUBLIC LICENSE for more details.
//
//    You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
//    along with this program.  If not, see https://www.gnu.org/licenses/.

#ifndef _INCL_FORMULACENTER_CPP
#define _INCL_FORMULACENTER_CPP
namespace FF{
  double GL_CONST_PI=3.1415926535897932384626433832795;
  double GL_CONST_E=2.7182818284590452353602874713526624977572;
  double EPSILON=0.00000000000001;
  double INTEGRAL_ERROR_BOUND_01=0.00000001;
  double INTEGRAL_SMALLEST_STEP=0.00001;
  double INTEGRAL_LARGEST_STEP=0.1;
  double INTEGRAL_MAX_STEP_ERROR=0.000000001;
  long MAXIMAL_NUMBER_OF_INTERVALS_FOR_SIMPSON=10000;
  double NUMBER_AFTER_WHICH_THE_INTEGRAL_IS_PRONOUNCED_DIVERGENT=1000000.0;
  class FunctionOfOneVariableData{
  private:
    std::string functionName;
    std::string functionArgument;
    std::string rawString;
    int initialized;
  public:
    FunctionOfOneVariableData(const std::string & = "", const long & = 0);
    int setFromString(const std::string & , const long &);
    std::string getFunctionName() const;
    std::string getFunctionArgument() const;
    std::string getRawString() const;
    std::string debugPrinting() const;
    int isInitialized() const;
  };
  class IntegralData{
  private:
    std::string lBoundSt;
    std::string uBoundSt;
    std::string integrand;
    std::string variableOfIntegration;
    std::string rawString;
    int initialized;
  public:
    IntegralData(const std::string & = "", const long & = 0);
    int setFromString(const std::string & , const long &);
    std::string getLBoundSt() const;
    std::string getUBoundSt() const;
    std::string getIntegrand() const;
    std::string getVariableOfIntegration() const;
    std::string getRawString() const;
    std::string debugPrinting() const;
    int isInitialized() const;
  };
  std::string FunctionOfOneVariableData::getFunctionName() const{return functionName;}
  std::string FunctionOfOneVariableData::getFunctionArgument() const{return functionArgument;}
  std::string FunctionOfOneVariableData::getRawString() const{return rawString;}
  int FunctionOfOneVariableData::isInitialized() const{ return initialized;}
  int FunctionOfOneVariableData::setFromString(const std::string & _raw, const long &start){
    std::string nFN,nFA,nRS;
    functionName="";functionArgument="";rawString="";initialized=0;
    if(_raw==""){return 1;}
    long posLetter=SF::getPositionOfFirstLetterOrBS(_raw,start);
    if(posLetter<0){return 0;}
    long pos;std::pair<std::string,int> allD;
    pos=posLetter; allD=SF::getEverythingBefore(_raw,pos,"(");
    if(allD.second==0){return 0;}
    nFN=allD.first;
    pos=posLetter; allD=SF::extract(_raw,pos,"(",")");
    if(allD.second==0){return 0;}
    nFA=allD.first;
    nRS=nFN+"("+nFA+")";
    functionName=nFN;functionArgument=nFA;rawString=nRS; initialized=1;
    return 1;
  }
  FunctionOfOneVariableData::FunctionOfOneVariableData(const std::string & _r, const long & _p){
    setFromString(_r,_p);
  }
  std::string FunctionOfOneVariableData::debugPrinting() const{
    std::string fR="name: " + functionName;
    fR+="\narg: "+ functionArgument;
    fR+="\nraw: "+rawString;
    return fR;
  }
  std::pair<double,int> evaluateFunction(const std::string & _fName, const double &x){
    std::pair<double,int> y,error;
    error.first=-9999999.9;
    error.second=0;
    y.second=1;
    std::string fName;
    fName=MFRF::toLowerCase(BF::cleanAllSpaces(SF::findAndReplace(_fName,"\\","")));
    if(fName=="sin"){
      y.first=std::sin(x); return y;
    }
    if(fName=="cos"){
      y.first=std::cos(x); return y;
    }
    if(fName=="tan"){
      double cs=std::cos(x);
      if (cs*cs<EPSILON){
        return error;
      }
      y.first=std::sin(x)/cs; return y;
    }
    if((fName=="cotan")||(fName=="cot")){
      double sn=std::sin(x);
      if (sn*sn<EPSILON){
        return error;
      }
      y.first=std::cos(x)/sn; return y;
    }
    if(fName=="ln"){
      if(x<EPSILON){
        return error;
      }
      y.first=std::log(x); return y;
    }
    if(fName=="log"){
      if(x<EPSILON){
        return error;
      }
      y.first=std::log(x)/std::log(10); return y;
    }
    if(fName=="sqrt"){
      if(x<0.0){
        return error;
      }
      y.first=std::sqrt(x); return y;
    }
    return error;
  }
  IntegralData::IntegralData(const std::string & _raw, const long & _st){
    setFromString(_raw,_st);
  }
  int IntegralData::setFromString(const std::string & _raw, const long & _st){
    std::string nLBS,nUBS,nI,nVarI,nRS;
    lBoundSt="";uBoundSt="";
    integrand="";variableOfIntegration="";rawString="";initialized=0;
    if(_raw==""){return 1;}
    long pos; std::pair<std::string,int> allD;
    pos=_st; allD=SF::extract(_raw,pos,"int_","d");
    if(allD.second==0){return 0;}
    nVarI="";
    long len=_raw.size();
    while( (pos<len) && (SF::isLetterOrBS(_raw[pos]))){
      nVarI+=_raw[pos];
      ++pos;
    }
    nRS="int_"+allD.first+"d"+nVarI;
    std::string interior=allD.first;
    pos=0; allD=SF::extract(interior,pos,"{","}");
    if(allD.second==0){return 0;}
    nLBS=allD.first;
    allD=SF::extract(interior,pos,"^{","}");
    if(allD.second==0){
      return 0;
    }
    nUBS=allD.first;
    long isz=interior.size();
    nI="";
    while(pos<isz){
      nI+=interior[pos];
      ++pos;
    }
    lBoundSt=nLBS;uBoundSt=nUBS;integrand=nI;variableOfIntegration=BF::cleanAllSpaces(nVarI);
    rawString=nRS;
    lBoundSt=BF::cleanAllSpaces(lBoundSt);
    uBoundSt=BF::cleanAllSpaces(uBoundSt);
    integrand=BF::cleanAllSpaces(integrand);
    std::map<std::string,std::string> replMap;
    replMap["\\,"]="";
    replMap["\\;"]="";
    replMap["\\quad"]="";
    integrand=MFRF::findAndReplace(integrand,replMap);
    initialized=1;
    return 1;
  }
  std::string IntegralData::getLBoundSt() const{return lBoundSt;}
  std::string IntegralData::getUBoundSt() const{return uBoundSt;}
  std::string IntegralData::getIntegrand() const{return integrand;}
  std::string IntegralData::getVariableOfIntegration() const{return variableOfIntegration;}
  std::string IntegralData::getRawString() const{return rawString;}
  int IntegralData::isInitialized() const{return initialized;}
  std::string IntegralData::debugPrinting() const{
    std::string result="";
    result+="<pre>\n";
    result+="lBoundSt="+lBoundSt+"\n";
    result+="uBoundSt="+uBoundSt+"\n";
    result+="integrand="+integrand+"\n";
    result+="variable="+variableOfIntegration+"\n";
    result+="raw="+rawString+"\n";
    result+="</pre>\n";
    return result;
  }
  template<typename TTT>
  std::pair<TTT,int> getFirstObjectFromString(const std::string & _raw, const long & _st, const TTT & _templateIdentifier){
    std::pair<TTT,int> fR;
    fR.second=0;
    TTT f1(_raw,_st);
    fR.first=f1;
    fR.second=f1.isInitialized();
    return fR;
  }
  template<typename TTT>
  std::stack<TTT> getStackOfObjectsFromString(const std::string & _raw, const long & _st, const TTT& _templateIdentifier){
    std::stack<TTT> allObjects;
    std::pair<TTT,int> nextObject;
    std::pair<std::string,int> allD;
    long nextstart=_st;
    nextObject=getFirstObjectFromString(_raw,nextstart,_templateIdentifier);
    while(nextObject.second==1){
      allObjects.push(nextObject.first);
      //The following command updates nextstart to the first term after the last object
      allD=SF::getEverythingBefore(_raw,nextstart,nextObject.first.getRawString());
      if(allD.second==0){
        nextObject.second=0;
      }
      else{
        nextObject=getFirstObjectFromString(_raw,nextstart,_templateIdentifier);
      }
    }
    return allObjects;
  }
  template<typename TTT>
  std::vector<TTT> getAllObjectsFromString(const std::string & _raw, const long & _st,const TTT &_templateIdentifier){
    std::stack<TTT> stf=getStackOfObjectsFromString(_raw,_st,_templateIdentifier);
    return SF::stackToVector(stf);
  }
  std::vector<FunctionOfOneVariableData> getAllFunctionsFromString(const std::string & _raw, const long & _st){
    FunctionOfOneVariableData emptyFunctionAsTemplateIdentifier;
    return getAllObjectsFromString(_raw,_st,emptyFunctionAsTemplateIdentifier);
  }
  std::vector<IntegralData> getAllIntegralsFromString(const std::string &_raw, const long & _st){
    IntegralData emptyIntegralAsTemplateIdentifier;
    return getAllObjectsFromString(_raw,_st,emptyIntegralAsTemplateIdentifier);
  }
  int isPositiveInfinity(const std::string & s){
    if(s=="+infty") return 1;
    if(s=="+\\infty") return 1;
    if(s=="infty") return 1;
    if(s=="\\infty") return 1;
    if(s=="infinity") return 1;
    if(s=="+infinity") return 1;
    return 0;
  }
  int isNegativeInfinity(const std::string & s){
    if(s=="-infty") return 1;
    if(s=="-\\infty") return 1;
    if(s=="-infinity") return 1;
    return 0;
  }
  struct FormulaEvaluationData{
  public:
    double result;
    int success;
    int indicatorSimpson;
  };
  FormulaEvaluationData evFormulaClean(const std::string & , const long & );
  FormulaEvaluationData evaluateIntegrandAtPoint(const IntegralData &iD, const double &x, const long &compositionRecursionDepthRemaining){
   std::string formulaToEvaluate=SF::findAndReplace(iD.getIntegrand(),iD.getVariableOfIntegration(),"("+BF::doubleToString(x)+")");
   return evFormulaClean(formulaToEvaluate,compositionRecursionDepthRemaining);
 }
 FormulaEvaluationData replaceInfinityInBound(const IntegralData &iD, const double &sign, const long &compositionRecursionDepthRemaining){
   FormulaEvaluationData success,failure;
   success.success=1;failure.success=0;
   success.result=100.0 * sign; failure.result=-99999999.999;
   FormulaEvaluationData eBRes=evaluateIntegrandAtPoint(iD,success.result,compositionRecursionDepthRemaining);
   if(eBRes.success==0){
     return failure;
   }
   if(std::abs(eBRes.result) > INTEGRAL_ERROR_BOUND_01){
     return failure;
   }
   return success;
 }
 FormulaEvaluationData replaceInfinityInLowerBound(const IntegralData &iD, const long &compositionRecursionDepthRemaining){
   return replaceInfinityInBound(iD,-1.0,compositionRecursionDepthRemaining);
 }
 FormulaEvaluationData replaceInfinityInUpperBound(const IntegralData &iD, const long &compositionRecursionDepthRemaining){
   return replaceInfinityInBound(iD,1.0,compositionRecursionDepthRemaining);
 }
 struct IntegralStepData{
 public:
   double deltaX;
   double a;
   double b;
   double m;
   double fA;
   double fB;
   double fM;
   double contribution;
   int evaluationSuccess;
   int arrivedAtTheEnd;
 };
 IntegralStepData getNextStep(const IntegralData &iD, const double &a, const double &uB, const double &fA, const long &compositionRecursionDepthRemaining){
   IntegralStepData res;
   res.evaluationSuccess=0;res.arrivedAtTheEnd=0;
   res.deltaX=INTEGRAL_LARGEST_STEP;
   if(a+res.deltaX>uB){
     res.deltaX=uB-a;res.arrivedAtTheEnd=1;
   }
   FormulaEvaluationData eval;
   double simpsonErrorOrder4;
   double hForDer=res.deltaX/10.0;
   double hForDer4=hForDer*hForDer;
   hForDer4=hForDer4*hForDer4;
   std::vector<double> aForDer,fForDer;
   long d=4;
   aForDer.resize(d+1);fForDer.resize(d+1);
   aForDer[0]=a;
   long i=0;
   while(i<d){
     ++i;
     aForDer[i]=aForDer[i-1]+hForDer;
   }
   i=d+1;
   while(i>1){
     --i;
     eval=evaluateIntegrandAtPoint(iD,aForDer[i],compositionRecursionDepthRemaining);
     if(eval.success==0){
       return res;
     }
     fForDer[i]=eval.result;
   }
   fForDer[0]=fA;
   simpsonErrorOrder4=std::abs(fForDer[4]-4.0*fForDer[3]+6.0*fForDer[2]-4.0*fForDer[1]+fForDer[0]);
   simpsonErrorOrder4 /= hForDer4;
  if(simpsonErrorOrder4 * (res.deltaX)*(res.deltaX)*(res.deltaX)*(res.deltaX) > INTEGRAL_MAX_STEP_ERROR){
    double goodDelta=std::sqrt(std::sqrt((INTEGRAL_MAX_STEP_ERROR/ std::abs(simpsonErrorOrder4))) );
    if(goodDelta > INTEGRAL_SMALLEST_STEP){
      res.deltaX=goodDelta; res.arrivedAtTheEnd=0;
    }
    else{
      res.arrivedAtTheEnd=0;
      return res;
    }
  }
   res.a=a;
   res.fA=fA;
   res.b=a+res.deltaX;
   res.m=a+0.5*res.deltaX;
   eval=evaluateIntegrandAtPoint(iD,res.b,compositionRecursionDepthRemaining);
   if(eval.success==0){
     return res;
   }
   res.fB=eval.result;
   eval=evaluateIntegrandAtPoint(iD,res.m,compositionRecursionDepthRemaining);
   if(eval.success==0){
     return res;
   }
   res.fM=eval.result;
   res.evaluationSuccess=1;
   res.contribution=((res.b-res.a)/6.0)*(res.fA+4.0*res.fM+res.fB);
   return res;
 }
 FormulaEvaluationData simpsonApproximation(const IntegralData &iD, const double & _lB, const double & _uB, const long &compositionRecursionDepthRemaining){
   FormulaEvaluationData success,failure;
   success.success=1;failure.success=0;
   failure.result=-9999999.99;
   if(MAXIMAL_NUMBER_OF_INTERVALS_FOR_SIMPSON<10){
     return failure;
   }
   double intValue=0.0;
   double a,  fA,uB;
   long intervalsRemaining=MAXIMAL_NUMBER_OF_INTERVALS_FOR_SIMPSON;
   a=_lB;
   FormulaEvaluationData evaluatorL,evaluatorR, evaluatorM;
   evaluatorL=evaluateIntegrandAtPoint(iD,a,compositionRecursionDepthRemaining);
   if(evaluatorL.success==0){
     a+=INTEGRAL_SMALLEST_STEP;
     evaluatorL=evaluateIntegrandAtPoint(iD,a,compositionRecursionDepthRemaining);
     if(evaluatorL.success==0){
       return failure;
     }
   }
   uB=_uB;
   evaluatorR=evaluateIntegrandAtPoint(iD,uB,compositionRecursionDepthRemaining);
   if(evaluatorR.success==0){
     uB-=INTEGRAL_SMALLEST_STEP;
     evaluatorR=evaluateIntegrandAtPoint(iD,uB,compositionRecursionDepthRemaining);
     if(evaluatorR.success==0){
       return failure;
     }
   }
   IntegralStepData nextStep;
   nextStep=getNextStep(iD,a,uB,evaluatorL.result,compositionRecursionDepthRemaining);
   while((intervalsRemaining>0)&&(nextStep.evaluationSuccess==1)&&(nextStep.arrivedAtTheEnd==0)){
      intValue+=nextStep.contribution;
      nextStep=getNextStep(iD,nextStep.b,uB,nextStep.fB,compositionRecursionDepthRemaining);
      --intervalsRemaining;
   }
   if(nextStep.arrivedAtTheEnd==0){
     return failure;
   }
   intValue+=nextStep.contribution;
   success.result=intValue;
   success.indicatorSimpson=1;
   return success;
 }
 FormulaEvaluationData evaluateIntegral(const IntegralData & iD, const long & compositionRecursionDepthRemaining){
     FormulaEvaluationData evRes;
     evRes.result=-99999.999;
     evRes.success=0;
     if(compositionRecursionDepthRemaining<0){
       return evRes;
     }
     FormulaEvaluationData lBPair,uBPair;
     if(isPositiveInfinity(iD.getLBoundSt())) {
       evRes.result=0.0;evRes.success=1;
       return evRes;
     }
     if(isNegativeInfinity(iD.getLBoundSt())){
       lBPair=replaceInfinityInLowerBound(iD,compositionRecursionDepthRemaining);
       if(lBPair.success==0){
         // The integral is divergent at -infty
         return lBPair;
       }
     }
     else{
       lBPair=evFormulaClean(iD.getLBoundSt(),compositionRecursionDepthRemaining);
       if(lBPair.success==0){
         return lBPair;
       }
     }
     if(isNegativeInfinity(iD.getUBoundSt())){
       evRes.result=0.0;evRes.success=1;
       return evRes;
     }
     if(isPositiveInfinity(iD.getUBoundSt())){
       uBPair=replaceInfinityInUpperBound(iD,compositionRecursionDepthRemaining);
       if(uBPair.success==0){
         //The integral is divergent at +infty
         return uBPair;
       }
     }
     else{
       uBPair=evFormulaClean(iD.getUBoundSt(),compositionRecursionDepthRemaining);
       if(uBPair.success==0){
         return uBPair;
       }
     }
     double lB=lBPair.result;
     double uB=uBPair.result;
     if(lB>uB){
       evRes.result=0.0;evRes.success=1;
       return evRes;
     }
     std::pair<SICF::Polynomial,int> pAttemptPair=SICF::polynomialFromString(iD.getIntegrand(),iD.getVariableOfIntegration());
     if(pAttemptPair.second==1){
       // The integrand is a polynomial.
       // We can integrate it without Simpson.
       SICF::Polynomial antiDer=pAttemptPair.first.indefiniteIntegral();
       FormulaEvaluationData res;
       res.success=1;
       res.indicatorSimpson=0;
       res.result=antiDer.evaluate(uB)-antiDer.evaluate(lB);
       return res;
     }
     return simpsonApproximation(iD,lB,uB,compositionRecursionDepthRemaining);
   }
  FormulaEvaluationData evFormulaClean(const std::string & _rawFormula, const long & compositionRecursionDepthRemaining ){
    std::string rawFormula=_rawFormula;
    FormulaEvaluationData finalResult;
    finalResult.indicatorSimpson=0;
    FormulaEvaluationData failure;
    failure.success=0;
    // Step 1: Evaluate integrals
    if(compositionRecursionDepthRemaining>0){
      std::vector<IntegralData> allIntegrals=getAllIntegralsFromString(rawFormula,0);
      long numI=allIntegrals.size();
      if(numI>0){
        FormulaEvaluationData integralValue;
        long i=0;
        std::map<std::string,std::string> replMap;
        while(i<numI){
          integralValue=evaluateIntegral(allIntegrals[i],compositionRecursionDepthRemaining);
          if(integralValue.success==0){
            return integralValue;
          }
          if(integralValue.indicatorSimpson==1){
            finalResult.indicatorSimpson=1;
          }
          replMap[allIntegrals[i].getRawString()]=BF::doubleToString( integralValue.result );
          ++i;
        }
        rawFormula=MFRF::findAndReplace(rawFormula,replMap);
      }
    }
    // Step 2: Evaluate functions
    if(compositionRecursionDepthRemaining>0){
      std::vector<FunctionOfOneVariableData> allFunctions=getAllFunctionsFromString(rawFormula,0);
      long numF=allFunctions.size();
      if(numF>0){
        std::vector<double > valuesOfArguments;
        valuesOfArguments.resize(numF);
        long i=0; FormulaEvaluationData evcurrent;
        while(i<numF){
          evcurrent=evFormulaClean(allFunctions[i].getFunctionArgument(),compositionRecursionDepthRemaining-1);
          if(evcurrent.success==0){return evcurrent;}
          valuesOfArguments[i]=evcurrent.result;
          ++i;
        }
        i=0;
        std::pair<double,int> evfRes;
        std::map<std::string,std::string> replMap;
        while(i<numF){
          evfRes=evaluateFunction(allFunctions[i].getFunctionName(),valuesOfArguments[i]);
          if(evfRes.second==0){
            return failure;
          }
          replMap[allFunctions[i].getRawString()]=BF::doubleToString(evfRes.first);
          ++i;
        }
        rawFormula=MFRF::findAndReplace(rawFormula,replMap);
      }
    }
    // Step 3: Finalize arithmetic
    std::pair<double,int> resPair= AF::evFDouble(rawFormula);
    finalResult.result=resPair.first;
    finalResult.success=resPair.second;
    return finalResult;
  }
  std::pair<double, int> evFormula(const std::string & _rawFormula, const long & compositionRecursionDepthRemaining = 5){
    //second component is 1 if the formula was evaluated
    //                 or 0 if the formula was not possible to evaluate
    std::string rawFormula=_rawFormula;
    std::map<std::string,std::string> replMap;
    replMap["\\cdot"]="*";
    replMap["\\int_{"]="int_{";
    rawFormula=MFRF::findAndReplace(rawFormula,replMap);
    FormulaEvaluationData fedRes= evFormulaClean(rawFormula,compositionRecursionDepthRemaining);
    std::pair<double, int> finalResult;
    finalResult.second=fedRes.success;
    finalResult.first=fedRes.result;
    if(fedRes.indicatorSimpson){
      finalResult.first=AF::round(finalResult.first,5);
    }
    return finalResult;
  }
}

#endif
