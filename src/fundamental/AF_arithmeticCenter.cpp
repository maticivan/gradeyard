//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_ARITHMETICCENTER_CPP
#define _INCL_ARITHMETICCENTER_CPP
namespace AF{
  struct ArithmeticOperations{
  public:
    long maxLength;
    std::set<std::string> oSet;
    ArithmeticOperations();
  } GL_ArO;
  ArithmeticOperations::ArithmeticOperations(){
    maxLength=0;
    oSet.insert("+");
    oSet.insert("*");
    oSet.insert("/");
    oSet.insert("^");
    oSet.insert("&");//min
    oSet.insert("|");//max
    oSet.insert("@");// indicator function - f@s = {s, if f>0; 0 otherwise}
    oSet.insert("~");// round - f~d = f rounded to d decimal digits
    std::set<std::string>::iterator it;
    for(it=oSet.begin();it!=oSet.end();++it){
      if((*it).length()>maxLength){
        maxLength=(*it).length();
      }
    }
  }
  std::string treatAdditiveSymbols(const std::string &s,const std::string &r=""){
    // replace -- with +
    // replace +- with -
    // replace -+ with -
    // replace ++ with +
    std::string newSt=s;
    if(r.length()==0){
      std::string originalSt=s;
      long shouldWorkAgain=1;
      while(shouldWorkAgain==1){
        newSt=treatAdditiveSymbols(newSt,"--");
        newSt=treatAdditiveSymbols(newSt,"-+");
        newSt=treatAdditiveSymbols(newSt,"+-");
        newSt=treatAdditiveSymbols(newSt,"++");
        if(newSt.length()==originalSt.length()){
          shouldWorkAgain=0;
        }
        originalSt=newSt;
      }
    }
    if(r.length()>0){
      std::string newSign="+";
      if((r=="+-")||(r=="-+")){
        newSign="-";
      }
      newSt=SF::findAndReplace(newSt, r, newSign);
    }
    return newSt;
  }
  std::string treatUnaryMinus(const std::string& s){
    return SF::findAndReplace(s,"-","+(-1)*");
  }
  std::string deleteUnnecessaryPlus(const std::string & s){
    std::string t=s;
    if(s[0]=='+'){
      long i=1;
      t="";
      long len=s.length();
      while(i<len){
        t+=s[i];
        ++i;
      }
    }
    return SF::findAndReplace(t,"(+","(");
  }
  long matching(const std::string &s1, const std::string &s2,const long &i1=0){
    // returns 1 if the string s2 occurs in s1 starting at position i1
    long m=1;
    long pos1=i1;
    long pos2=0;
    long len1=s1.length();
    long len2=s2.length();
    if(len2>len1-pos1){return 0;}
    while((m==1)&&(pos2<len2)){
      if(s1[pos1]!=s2[pos2]){
        m=0;
      }
      ++pos1;++pos2;
    }
    return m;
  }
  std::string reverseOrder(const std::string &s){
    std::stack<char> forRev;
    long len=s.length();
    for(long i=0;i<len;++i){
      forRev.push(s[i]);
    }
    std::string rev="";
    while(!forRev.empty()){
      if(forRev.top()=='('){
        rev+=")";
      }
      if(forRev.top()==')'){
        rev+="(";
      }
      if((forRev.top()!='(')&&(forRev.top()!=')')){
        rev+=forRev.top();
      }
      forRev.pop();
    }
    return rev;
  }
  std::string addBrackets( const std::string &s){
    long len=s.length();
    long debt=0;
    long i=0;
    long jobDone=0;
    std::string res="";long oL,j;std::string tempSt;
    while((i<len)&&(jobDone==0)){
      if(debt==0){
        j=0;
        while((j<GL_ArO.maxLength)&&(jobDone==0)){
          oL=j+1;
          tempSt="";
          if(i+oL<len){
            for(long k=0;k<oL;++k){
              tempSt+=s[i+k];
            }
          }
          if(GL_ArO.oSet.find(tempSt)!=GL_ArO.oSet.end()){
            jobDone=1;
          }
          if(s[i]==')'){
            jobDone=1;
          }
          if(jobDone==1){
            res+=")";
            while(i<len){
              res+=s[i];
              ++i;
            }
          }
          ++j;
        }
      }
      if(jobDone==0){
        if(s[i]=='('){
          ++debt;
        }
        if(s[i]==')'){
          --debt;
        }
        res+=s[i];
        ++i;
      }
    }
    if(jobDone==0){
      res+=")";
    }
    return res;
  }
  std::string prioritizeOperation(const std::string &s, const std::string &operation, long finished=0){
    std::string nSt="";
    long i=0;
    long len=s.length();
    long found=-1;
    while((found<finished)&&(i<len)){
      if(matching(s,operation,i)==1){
        ++found;
        if(found==finished){
          nSt=reverseOrder(nSt);
          nSt=addBrackets(nSt);
          nSt=reverseOrder(nSt);
        }
        nSt+=operation;
        i+=operation.length();
        if(found==finished){
          std::string nextPart="";
          while(i<len){
            nextPart+=s[i];
            ++i;
          }
          nextPart=addBrackets(nextPart);
          nSt+=nextPart;
        }
      }
      else{
        nSt+=s[i];
        ++i;
      }
    }
    if(found==finished){
      nSt=prioritizeOperation(nSt,operation,finished+1);
    }
    return nSt;
  }
  double round(const double & f, const double & d){
    if(f< -BF::GLOBAL_EPSILON){
      return -round(-f,d);
    }
    double result;
    if(d<-BF::GLOBAL_EPSILON){
      return f;//cannot round to negative number of digits
    }
    long dL= static_cast<long>(d+BF::GLOBAL_EPSILON);
    double multiplier=0.1;
    double shiftedF=f;
    ++dL;
    for(long i=0;i<dL;++i){
      multiplier *= 10.0;
      shiftedF *= 10.0;
    }
    long fForNextDigitInt = static_cast<long>(shiftedF+BF::GLOBAL_EPSILON);
    long nextDigit=fForNextDigitInt % 10;
    if(nextDigit>4){
      fForNextDigitInt += 10;
    }
    fForNextDigitInt/=10;
    return (static_cast<double>(fForNextDigitInt)/multiplier);
  }
  double evaluateNum(const double &f, const double &s, const std::string & o){
    if(o=="+"){
      return f+s;
    }
    if(o=="-"){
      return f-s;
    }
    if(o=="*"){
      return f*s;
    }
    if(o=="^"){
      if((f<BF::GLOBAL_EPSILON)&&(f>-BF::GLOBAL_EPSILON)){
        return 0.0;
      }
      double sgn=1.0;
      double absf=f;
      if(f<0.0){
        long secondInt=static_cast<long>(s+BF::GLOBAL_EPSILON);
        if(secondInt%2==1){
          sgn =-1.0;
        }
        absf *= -1.0;
      }
      return sgn * std::exp(s*std::log(absf));
    }
    if(o=="/"){
      if(s!=0.0){
        return f/s;
      }
      else{
        return 0.0;
      }
    }
    if(o=="|"){
      if(f>s){
        return f;
      }
      else{
        return s;
      }
    }
    if(o=="&"){
      if(f>s){
        return s;
      }
      else{
        return f;
      }
    }
    if(o=="@"){
      if(f>0){
        return s;
      }
      return 0.0;
    }
    if(o=="~"){
      return round(f,s);
    }
    return 0.0;
  }
  std::pair<double,int> numericalEvaluationOfPreparedFormula(const std::string &s){
    // returns pair (result, evaluationPossible)
    // the second argument is 0 if the expression was not possible to evaluate

    long len=s.length();
    std::string symbolNum="d";
    std::stack<std::pair<double,std::string> > evaluator;
    std::pair<double,std::string> tempP,rP,lP,mP;
    long i=0,oldI=-1;
    double value=0.0;
    double tempNum;
    double zero=static_cast<double>(static_cast<long>('0'));
    long identifiedOperation,j,oL;
    std::string tempOperation;
    while (i<len){
      if(i==oldI){
        return std::pair<double,long>(-17.0,0);
      }
      oldI=i;
      if(s[i]==')'){
        if(evaluator.empty()){
          return std::pair<double,long>(-1.0,0);
        }
        rP=evaluator.top();
        evaluator.pop();
        if(evaluator.empty()){
          return std::pair<double,long>(-2.0,0);
        }
        mP=evaluator.top();
        evaluator.pop();
        if(mP.second=="("){
          evaluator.push(rP);
        }
        else{
          if(evaluator.empty()){
            return std::pair<double,long>(-3.0,0);
          }
          lP=evaluator.top();
          evaluator.pop();
          if(evaluator.empty()){
            return std::pair<double,long>(-4.0,0);
          }
          evaluator.pop();
          tempNum=evaluateNum(lP.first,rP.first,mP.second);
          tempP.first=tempNum;
          tempP.second=symbolNum;
          evaluator.push(tempP);
        }
        ++i;
      }
      else{
        identifiedOperation=0;
        j=0;
        if(s[i]=='('){
          tempP.first=0.0;
          tempP.second="(";
          evaluator.push(tempP);
          ++i;
          identifiedOperation=1;
        }
        if(identifiedOperation==0){
          if((i+1<len)&&(s[i]=='-')&&(s[i+1]=='1')){
            tempP.first=-1.0;
            tempP.second=symbolNum;
            evaluator.push(tempP);
            i+=2;
            identifiedOperation=1;
          }
        }
        while((identifiedOperation==0)&&(j<GL_ArO.maxLength)){
          oL=j+1;
          if(i+oL<len){
            tempOperation="";
            for(long k=0;k<oL;++k){
              tempOperation+=s[i+k];
            }
            if(GL_ArO.oSet.find(tempOperation)!=GL_ArO.oSet.end()){
              identifiedOperation=1;
              tempP.second=tempOperation;
              tempP.first=0.0;
              evaluator.push(tempP);
              i+=oL;
            }
          }
          ++j;
        }
        if(identifiedOperation==0){
          tempNum=0.0;
          while((i<len)&&(s[i]>='0')&&(s[i]<='9')){
            tempNum*=10.0;
            tempNum+= static_cast<double>(static_cast<long>(s[i]))-zero;
            ++i;
          }
          if((i<len)&&(s[i]==BF::CONST_DECIMAL_SEPARATOR)){
            // We reached the decimal separator
            double decimalMultiplier=0.1;
            ++i;
            while((i<len)&&(s[i]>='0')&&(s[i]<='9')) {
              tempNum+= decimalMultiplier*(static_cast<double>( (s[i]))-zero);
              decimalMultiplier*=0.1;
              ++i;
            }
          }
          tempP.first=tempNum;
          tempP.second=symbolNum;
          evaluator.push(tempP);
        }
      }
    }
    if(evaluator.empty()){
      return std::pair<double,long>(-5.0,0);
    }
    tempP=evaluator.top();
    evaluator.pop();
    return std::pair<double,long>(tempP.first,1);
  }
  std::string prepareFormula(const std::string &_raw){
    std::string rawFormula=_raw;
    rawFormula=BF::cleanSpaces(rawFormula,1);
    rawFormula=treatAdditiveSymbols(rawFormula);
    rawFormula=treatUnaryMinus(rawFormula);
    rawFormula=deleteUnnecessaryPlus(rawFormula);
    rawFormula=prioritizeOperation(rawFormula,"^");
    rawFormula=prioritizeOperation(rawFormula,"*");
    rawFormula=prioritizeOperation(rawFormula,"/");
    rawFormula=prioritizeOperation(rawFormula,"+");
    rawFormula=SF::findAndReplace(rawFormula,"-1",GF::GL_HIDING_STRING_FORMULACENTER);
    rawFormula=prioritizeOperation(rawFormula,"-");
    rawFormula=SF::findAndReplace(rawFormula,GF::GL_HIDING_STRING_FORMULACENTER,"-1");
    rawFormula=prioritizeOperation(rawFormula,"&");
    rawFormula=prioritizeOperation(rawFormula,"|");
    rawFormula=prioritizeOperation(rawFormula,"@");
    rawFormula=prioritizeOperation(rawFormula,"~");
    std::string thousands_sep="";
    thousands_sep+=BF::CONST_THOUSANDS_SEPARATOR;
    rawFormula=SF::findAndReplace(rawFormula, thousands_sep,"");
    return rawFormula;
  }
  std::pair<double,int> evFDouble(const std::string & _raw){
    return numericalEvaluationOfPreparedFormula(prepareFormula(_raw));
  }
}
#endif
