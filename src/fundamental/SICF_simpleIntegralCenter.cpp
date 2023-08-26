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


#ifndef _INCL_SIMPLEINTEGRALCENTER_CPP
#define _INCL_SIMPLEINTEGRALCENTER_CPP
namespace SICF{
  double GL_CONST_PI=3.1415926535897932384626433832795;
  double GL_CONST_E=2.7182818284590452353602874713526624977572;
  double GL_EPSILON=0.00000000001;
  double power(const double & x, const long &k ){
    double res=1.0;
    for(long i=0;i<k;++i){
      res*=x;
    }
    return res;
  }
  class Polynomial{
  private:
    std::map<long,double> coefficients;
    int eraseTermsWhoseCoefficientsAre0();
    void plMin(const Polynomial &, const double &);
  public:
    Polynomial(const double & = 0.0);
    int setCoefficient(const long &, const double &);
    double getCoefficient(const long &) const;
    double operator[](const long & )const;
    Polynomial& operator+=(const Polynomial &);
    Polynomial& operator-=(const Polynomial &);
    Polynomial& operator*=(const Polynomial &);
    Polynomial& operator*=(const double &);
    Polynomial& operator^=(const long &);
    double evaluate(const double &) const;
    Polynomial indefiniteIntegral() const;
    std::string debugPrinting() const;
  };
  Polynomial::Polynomial(const double & a_0){
    setCoefficient(0,a_0);
  }
  int Polynomial::eraseTermsWhoseCoefficientsAre0(){
    std::map<long, double> coeffsNew;
    std::map<long,double>::const_iterator it,itE;
    itE=coefficients.end();
    it=coefficients.begin();
    while(it!=itE){
      if(std::abs(it->second)>GL_EPSILON){
        coeffsNew[it->first]=it->second;
      }
      ++it;
    }
    coefficients=coeffsNew;
    return 1;
  }
  int Polynomial::setCoefficient(const long &k, const double & c){
    if(k<0){
      return 0;
    }
    if(std::abs(c)<GL_EPSILON){
      coefficients.erase(k);
      return 0;
    }
    coefficients[k]=c;
    return 1;
  }
  double Polynomial::getCoefficient(const long &k) const{
    std::map<long,double>::const_iterator it=coefficients.find(k);
    if(it==coefficients.end()){return 0.0;}
    return it->second;
  }
  double Polynomial::operator[](const long &k) const{
    return getCoefficient(k);
  }
  void Polynomial::plMin(const Polynomial & b, const double &sgn){
    std::map<long,double>::const_iterator itB,itBE;
    std::map<long,double>::iterator it;
    itBE=b.coefficients.end(); itB=b.coefficients.begin();
    while(itB!=itBE){
      it=coefficients.find(itB->first);
      if(it!=coefficients.end()){
        (it->second) += sgn*(itB->second);
      }
      else{
        coefficients[itB->first]=sgn*(itB->second);
      }
      ++itB;
    }
  }
  Polynomial& Polynomial::operator+=(const Polynomial & b){
    plMin(b,1.0);
    return *this;
  }
  Polynomial& Polynomial::operator-=(const Polynomial & b){
    plMin(b,-1.0);
    return *this;
  }
  Polynomial& Polynomial::operator*=(const Polynomial & b){
    if(coefficients.size()==0){return *this;}
    if(b.coefficients.size()==0){
      coefficients.clear();return *this;
    }
    std::map<long,double> coeffsNew;
    std::map<long,double>::const_iterator it, itE,itB,itBE;
    std::map<long,double>::iterator itC,itCE;
    long kN;
    it=coefficients.begin();itE=coefficients.end();
    itBE=b.coefficients.end();
    while(it!=itE){
      itB=b.coefficients.begin();
      while(itB!=itBE){
        itCE=coeffsNew.end();
        kN=(it->first)+(itB->first);
        itC=coeffsNew.find(kN);
        if(itC==itCE){
          coeffsNew[kN]=(it->second)*(itB->second);
        }
        else{
          (itC->second) += (it->second)*(itB->second);
        }
        ++itB;
      }
      ++it;
    }
    coefficients=std::move(coeffsNew);
    eraseTermsWhoseCoefficientsAre0();
    return *this;
  }
  Polynomial& Polynomial::operator*=(const double & d){
    if(std::abs(d)<GL_EPSILON){
      coefficients.clear();
      return *this;
    }
    std::map<long,double>::iterator it,itE;
    itE=coefficients.end();it=coefficients.begin();
    while(it!=itE){
      (it->second) *= d;
      ++it;
    }
    return *this;
  }
  Polynomial& Polynomial::operator^=(const long & k){
    if(k==1){
      coefficients.clear();
      coefficients[0]=1.0;
      return *this;
    }
    Polynomial singleFactor = *this;
    Polynomial res;
    res.coefficients[0]=1.0;
    for(long i=0;i<k;++i){
      res *= singleFactor;
    }
    coefficients=std::move(res.coefficients);
    return *this;
  }
  double Polynomial::evaluate(const double & x) const{
    double res=0.0;
    std::map<long,double>::const_iterator it,itE;
    itE=coefficients.end();
    it=coefficients.begin();
    while(it!=itE){
      res+=it->second*power(x,it->first);
      ++it;
    }
    return res;
  }
  std::string Polynomial::debugPrinting()const{
    std::string res="";
    std::map<long,double>::const_iterator it,itB;
    it=coefficients.end();
    itB=coefficients.begin();
    long alreadyPrinted=0;
    while(it!=itB){
      --it;
      if((it->second > 0.0)&&(alreadyPrinted==1)){
        res+="+";
      }
      res+=std::to_string(it->second)+"*x^"+std::to_string(it->first);
      alreadyPrinted=1;
    }
    return res;
  }
  Polynomial Polynomial::indefiniteIntegral() const{
    Polynomial q;
    std::map<long,double>::const_iterator it,itE;
    it=coefficients.begin();itE=coefficients.end();
    while(it!=itE){
      q.coefficients[(it->first)+1]=(it->second)/static_cast<double>((it->first)+1);
      ++it;
    }
    return q;
  }
  std::pair<Polynomial,int> getSimplePolynomialFromString(const std::string & in, const std::string &varName){
    std::pair<Polynomial,int> res;
    res.second=0;
    std::pair<double,int> arithmeticAttempt;
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::getEverythingBefore(in,pos,varName);
    if(allD.second==0){
      arithmeticAttempt= AF::evFDouble(in);
      if(arithmeticAttempt.second==0){
        return res;
      }
      res.first.setCoefficient(0,arithmeticAttempt.first);
      res.second=1;
      return res;
    }
    std::string coeffSt=SF::findAndReplace(allD.first,"*","");
    if(!BF::isNumeric(coeffSt)){
      return res;
    }
    double coeff=BF::stringToDouble(coeffSt);
    if(coeffSt==""){
      coeff=1.0;
    }
    long k=1;
    long lenIn=in.length();
    if(pos<lenIn-1){
      std::string expSt="";
      if(in[pos]=='^'){
        ++pos;
        while(pos<lenIn){
          expSt+=in[pos];
          ++pos;
        }
        arithmeticAttempt=AF::evFDouble(expSt);
        if(arithmeticAttempt.second==0){
          return res;
        }
        k=static_cast<long>(arithmeticAttempt.first+0.00001);
      }
      else{
        return res;
      }
    }
    res.second=1;
    res.first.setCoefficient(k,coeff);
    return res;
  }
  std::pair<Polynomial,int> polFromGoodStr(const std::string & in,const std::string &varName){
    std::pair<Polynomial,int> res;
    res.second=1;
    //Step 1: Check trivial cases
    if(in==""){
      return res;
    }
    if(in==varName){
      res.first.setCoefficient(1,1.0);
      return res;
    }
    std::pair<double,int> arithmeticAttempt= AF::evFDouble(in);
    if(arithmeticAttempt.second==1){
      res.first.setCoefficient(0,arithmeticAttempt.first);
      return res;
    }
    //Step 2: Treat non-trivial cases
    res.second=0;
    long len =in.length();
    long pos; std::pair<std::string,int> allD;
    if(in[0]!='('){
      std::pair<Polynomial,int> simplePolAttempt= getSimplePolynomialFromString(in,varName);
      if(simplePolAttempt.second==1){
        return simplePolAttempt;
      }
      // We may have something like 2+(anotherPolynomial)
      // We will turn it into (2)+(anotherPolynomial) and start over
      pos=0;allD=SF::getEverythingBefore(in,pos,"(");
      if(allD.second==0){
        //Things look really bad.
        // 1) We couldn't evaluate the expression as arithmetic formula
        // 2) We couldn't evaluate the expression as a simple polynomial
        // 3) The expression does not use any brackets
        return res;
      }
      pos-=2;
      //There is no chance for pos to be negative
      if((in[pos]=='+') || (in[pos]=='-') || (in[pos]=='*') || (in[pos]=='/') || (in[pos]=='^')){
        std::string inNew="(";
        long i=0;
        while(i<pos){
          inNew+=in[i];
          ++i;
        }
        inNew+=")";
        while(i<len){
          inNew+=in[i];
          ++i;
        }
        return polFromGoodStr(inNew,varName);
      }
      else{
        return res;
      }
    }
    pos=0;allD=SF::extract(in,pos,"(",")");
    if(allD.second==0){
      return res;
    }
    std::string operation="";
    if(pos > len-1){
      return polFromGoodStr(allD.first,varName);
    }
    if(in[pos]=='*'){
      operation="*";
    }
    if(in[pos]=='+'){
      operation="+";
    }
    if(in[pos]=='-'){
      operation="-";
    }
    if(in[pos]=='/'){
      operation="/";
    }
    if(in[pos]=='^'){
      operation="^";
    }
    ++pos;
    if(operation==""){
      return res;
    }
    std::string stFirst=allD.first;
    std::pair<Polynomial,int> pFirst=polFromGoodStr(stFirst,varName);
    if(pFirst.second==0){
      return res;
    }
    std::string stSecond="";
    long posSave=pos;
    allD=SF::extract(in,pos,"(",")");
    if(allD.second==0){
      long posIn=posSave;
      while(posIn<len){
        stSecond+=in[posIn];
        ++posIn;
      }
    }
    else{
      stSecond=allD.first;
    }
    if((operation=="/")||(operation=="^")){
        //Polynomial can be divided only by a number or raised only to an integer.
        // We return failure as the result if the second argument is not a number.
      arithmeticAttempt=AF::evFDouble(stSecond);
      if(arithmeticAttempt.second==0){
        return res;
      }
      if(operation=="/"){
        if(std::abs(arithmeticAttempt.first)<GL_EPSILON){
          // looks like division by 0
          return res;
        }
        pFirst.first*=(1.0/arithmeticAttempt.first);
        return pFirst;
      }
      // now, the operation is "^";
      long exponent=static_cast<long>(arithmeticAttempt.first+0.001);
      if( std::abs(static_cast<double>(exponent)-arithmeticAttempt.first) > 0.0001 ){
        // the exponent is not an integer or is a negative integer
        return res;
      }
      if(exponent<0){
        // The code will almost never enter here; However this is due to a slight trick
        // concerning the integer value of a negative number and the way that doubles are represented
        // in floating point. So, if this part of the branch is missing, someone may think that the
        // code is illogical. Thus, this statement and branch is left for clarity - this will never be
        // executed.
        return res;
      }
      pFirst.first ^= exponent;
      return pFirst;
    }
    std::pair<Polynomial,int> pSecond=polFromGoodStr(stSecond,varName);
    if(pSecond.second==0){
      return res;
    }
    if(operation=="+"){
      pFirst.first += pSecond.first;
    }
    if(operation=="-"){
      pFirst.first -= pSecond.first;
    }
    if(operation=="*"){
      pFirst.first *= pSecond.first;
    }
    return pFirst;
  }
  int checkIfOnlyOneVariable(const std::string & _in, const std::string &varName){
    std::map<std::string,std::string> replMap;
    replMap[varName]="";
    replMap["\\"]="";
    std::string in=MFRF::findAndReplace(_in,replMap);
    if(SF::getPositionOfFirstLetterOrBS(in)>-1){
      return 0;
    }
    return 1;
  }
  std::pair<Polynomial,int> polynomialFromString(const std::string & in,const std::string &varName){
    if(checkIfOnlyOneVariable(in,varName)==0){
      std::pair<Polynomial,int> failure;
      failure.second=0;
      return failure;
    }
    std::string goodStr=in;
    std::string azmEnd="!*azm&"+RNDF::genRandCode(5)+"&J";
    std::map<std::string,std::string> replMap;
    replMap[varName+")"]=varName+"^1)";
    replMap[varName+"+"]=varName+"^1+";
    replMap[varName+"-"]=varName+"^1-";
    replMap[varName+"*"]=varName+"^1*";
    replMap[varName+"/"]=varName+"^1/";
    goodStr=MFRF::findAndReplace(goodStr,replMap);
    goodStr+=azmEnd;
    goodStr=SF::findAndReplace(goodStr,varName+azmEnd,varName+"^1");
    goodStr=SF::findAndReplace(goodStr,azmEnd,"");
    goodStr=AF::prepareFormula(goodStr);
    return polFromGoodStr(goodStr,varName);
  }
}
#endif
