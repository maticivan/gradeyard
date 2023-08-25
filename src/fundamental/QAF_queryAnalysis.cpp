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


#ifndef _INCL_QUERYANALYSIS_CPP
#define _INCL_QUERYANALYSIS_CPP

namespace QAF{
  double GL_MIN_INFTY=-9999999.99;
  long GL_MIN_INFTY_INT=-9999999;
  double GL_EPSILON=0.0000001;
  long GL_bigInt=100000;
  long GL_RNGbigInt=1000000000;
  double GL_TwoPi=6.283185307179586476925;
  double GL_OneOverSqRootTwo=0.707106781186547524400844;
  struct DistributionParameters{
  public:
    std::string name;
    std::map<std::string,double> parameters;
    long sampleSize;
  };
  struct PermutationParameters{
  public:
    long smallest;
    long largest;
    long numSamples;
    std::string writingDirection;
  };
  struct NormalDistributionParameters{
  public:
    double x;
    double m;
    double sigma;
  };
  std::string toString(const DistributionParameters& dp){
    std::string res="";
    res+="name="+dp.name+"\n";
    std::map<std::string,double>::const_iterator it,itE=dp.parameters.end();
    it=dp.parameters.begin();
    while(it!=itE){
      res+=it->first+"="+std::to_string(it->second)+"\n";
      ++it;
    }
    return res;
  }
  long uniformInt(const long &l, const long &u){
    if(l>=u){
      return -l-u;
    }
    return l+RNDF::randNum(u-l);
  }
  double standardUniform(){
    long randInt=RNDF::randNum(GL_RNGbigInt);
    return static_cast<double>(randInt) / static_cast<double>(GL_RNGbigInt);
  }

  long bernoulli(const double &p){
    double sample=standardUniform();
    if(sample<p){
      return 1;
    }
    return 0;
  }
  long binomial(const long &n , const double &p){
    long res=0;
    for(long i=0;i<n;++i){
      res+=bernoulli(p);
    }
    return res;
  }
  long geometric(const double &p){
    long g=0;
    while((g<GL_bigInt)&&(bernoulli(p)==0)){
      ++g;
    }
    return g+1;
  }
  template<typename TTT>
  TTT generalDiscrete(const std::vector<TTT> &vals, const std::vector<double> & p){
    TTT fR;
    long sz=vals.size();
    if(p.size()!=sz){
      return fR;
    }
    if(sz<1){
      return fR;
    }
    double rand=standardUniform();
    long i=0;
    double collector=0.0;
    while( (collector<rand) &&(i<sz) ){
      collector+=p[i];
      ++i;
    }
    if( i>0 ){
      return vals[i-1];
    }
    return fR;
  }
  double uniformC(const double & l, const double & u){
    if(l>u){
      return -1.0;
    }
    return l+ (u-l)*standardUniform();
  }
  double exponential(const double &lambda){
    double t=standardUniform();
    if(lambda<GL_EPSILON){return 0.0;}
    return log(t)/(-lambda);
  }
  double standardNormal(){
    return std::sqrt(-2.0* log(GL_EPSILON + (1.0-GL_EPSILON)*standardUniform())) * std::cos(GL_TwoPi * standardUniform());
  }
  double normal(const double & mu, const double & sigma){
    return mu+sigma*standardNormal();
  }
  std::pair<double,int> getParameter(const std::map<std::string,double> & m, const std::string & st){
    double res=GL_MIN_INFTY;
    std::map<std::string,double>::const_iterator
    it=m.find(st);
    if(it==m.end()){
      return std::pair<double,int>(res,0);
    }
    return std::pair<double,int>(it->second,1);
  }
  std::pair<double,int> getParameter(const std::map<std::string,double> &m, const std::vector<std::string> &st){
    std::pair<double,int> attempt;
    attempt.second=0;
    long i=0;
    long sz=st.size();
    while((attempt.second==0)&&(i<sz)){
      attempt=getParameter(m,st[i]);
      ++i;
    }
    return attempt;
  }
  long doubleToLong(const double & d){
    return static_cast<long>(d+GL_EPSILON);
  }
  template<typename TTT>
  std::pair<long,int> getIntParameter(const std::map<std::string,double> & m, const TTT & st){
    std::pair<double,int> rs1=getParameter(m,st);
    if(rs1.second==1){
      return std::pair<long,int>(doubleToLong(rs1.first),1);
    }
    return std::pair<long,int>(GL_MIN_INFTY_INT,0);
  }
  std::string cleanString(const std::string &in,const std::string& toCleanWith=";"){
    std::map<std::string,std::string> replMap;
    replMap["?"]=toCleanWith;
    replMap[","]=toCleanWith;
    return MFRF::findAndReplace(in,replMap,1);
  }
  std::pair<NormalDistributionParameters,int> getNDParametersFromDParameters(const DistributionParameters &dp){
    std::pair<NormalDistributionParameters,int> ndp;
    ndp.second=1;
    std::pair<double,int> x,mu,sigma;
    std::vector<std::string> xNames,muNames,sigmaNames,varNames;
    long nxnames=3;xNames.resize(nxnames);
    xNames[0]="value";xNames[1]="x";xNames[2]="t";
    x=getParameter(dp.parameters,xNames);
    if(x.second==0){
      x.first=0.0;
    }
    ndp.first.x=x.first;
    long nmunames=6;muNames.resize(nmunames);
    muNames[0]="mean";muNames[1]="expectation";muNames[2]="expected value";muNames[3]="expectedvalue";
    muNames[4]="mu";muNames[5]="M";
    mu=getParameter(dp.parameters,muNames);
    if(mu.second==0){
      mu.first=0.0;
    }
    ndp.first.m=mu.first;
    long nvarnames=5;varNames.resize(nvarnames);
    varNames[0]="variance";varNames[1]="sigma square";varNames[2]="sigmasquare";varNames[3]="sigma2";
    varNames[4]="sigma^2";
    sigma=getParameter(dp.parameters,varNames);
    if(sigma.second==1){
      if(sigma.first<0.0){
        sigma.first*=-1.0;
      }
      ndp.first.sigma = std::sqrt(sigma.first);
      return ndp;
    }
    long nsnames=6;sigmaNames.resize(nsnames);
    sigmaNames[0]="sigma";sigmaNames[1]="standard deviation"; sigmaNames[2]="standarddeviation"; sigmaNames[3]="st. dev.";
    sigmaNames[4]="st dev"; sigmaNames[5]="stdev";
    sigma=getParameter(dp.parameters,sigmaNames);
    if(sigma.second==0){
      sigma.first=1.0;
    }
    ndp.first.sigma=sigma.first;
    return ndp;
  }
  DistributionParameters getDParametersFromQuestion(const std::string & _q){
    std::string question=_q;
    question=BF::removeASCII10AND13(question);
    question=BF::cleanSpaces(question);
    question=MFRF::toLowerCase(question);
    std::map<std::string,std::string> replMap;
    replMap["?"]=";";
    replMap[","]=";";
    replMap["distribution"]="dist";
    replMap["sample size"]="size";
    replMap["samplesize"]="size";
    replMap["discrete uniform"]="discunif";
    replMap["discreteuniform"]="discunif";
    replMap["uniformdiscrete"]="discunif";
    replMap["uniform discrete"]="discunif";
    question=MFRF::findAndReplace(question,replMap,1);
    question=SF::findAndReplace(question,"sample","size",1,1);
    question+="; ";
    question=";"+question;
    std::vector<std::string> nums=SF::stringToVector(question,"=",";");
    std::vector<std::string> labels=SF::stringToVector(question,";","=");
    DistributionParameters fR;
    fR.name="notFound";
    long sz=nums.size();
    if(sz!=labels.size()){
      return fR;
    }
    std::string cleanLabel;
    std::string cleanNum;
    for(long i=0;i<sz;++i){
      cleanLabel=cleanString(labels[i]);
      cleanNum=cleanString(nums[i]);
      if(cleanLabel=="dist"){
        fR.name=cleanNum;
      }
      else{
        cleanLabel=BF::cleanSpaces(cleanLabel,1);
        fR.parameters[cleanLabel]=BF::stringToDouble(cleanNum);
      }
    }
    fR.sampleSize=1;
    std::pair<long,int> numSP=getIntParameter(fR.parameters,"size");
    if(numSP.second==1){
      fR.sampleSize=numSP.first;
      if(fR.sampleSize<1){fR.sampleSize=1;}
    }
    if(fR.sampleSize>20){
      fR.sampleSize=20;
    }
    return fR;
  }
  NormalDistributionParameters getNDParametersFromQuestion(const std::string &_q){
    std::string question=_q;
    DistributionParameters dp=getDParametersFromQuestion(question);
    return getNDParametersFromDParameters(dp).first;
  }
  PermutationParameters getPParametersFromQuestion(const std::string & _q){
    std::string question=_q;
    question=BF::removeASCII10AND13(question);
    question=BF::cleanSpaces(question);
    question=cleanString(question);
    PermutationParameters fR;
    fR.numSamples=-1;
    if(MFRF::find(question,"permutation").second==-1){
      return fR;
    }
    question+="; ";
    question=";"+question;
    std::vector<std::string> nums=SF::stringToVector(question,"=",";");
    fR.writingDirection="vertical";
    if(MFRF::find(question,"vertical").second==-1){
      fR.writingDirection="horizontal";
    }
    long nsz=nums.size();
    fR.smallest=1;fR.largest=10;fR.numSamples=1;
    if(nsz<2){
      return fR;
    }
    std::vector<long> threeNums;
    threeNums.resize(3);
    long numReceived=0;
    long i=0;
    while((numReceived<3)&&(i<nsz)){
      if(BF::isNumeric(BF::cleanSpaces(nums[i]))) {
        threeNums[numReceived]=BF::stringToInteger(BF::cleanSpaces(nums[i]));
        ++numReceived;
      }
      ++i;
    }
    if(numReceived<2){
      return fR;
    }
    if(numReceived==3){
      fR.numSamples=threeNums[2];
    }
    if(threeNums[0]==threeNums[1]){
      return fR;
    }
    fR.smallest=threeNums[0]; fR.largest=threeNums[1];
    if(fR.smallest>fR.largest){
      fR.smallest=threeNums[1];fR.largest=threeNums[0];
    }
    return fR;
  }
  std::vector<double> sampleFromNormal(const DistributionParameters& dp){
    std::vector<double> res;
    NormalDistributionParameters ndp=getNDParametersFromDParameters(dp).first;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    for(long i=0;i<sz;++i){
      res[i]=normal(ndp.m,ndp.sigma);
    }
    return res;
  }
  std::vector<double> sampleFromUniform(const DistributionParameters& dp){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesA,namesB;
    long nmsz=9;
    namesA.resize(nmsz);namesA[0]="a";namesA[1]="smallest";namesA[2]="min";namesA[3]="minimal";namesA[4]="minimum";
    namesA[5]="lower";namesA[6]="low";namesA[7]="lower bound"; namesA[8]="lower bound";
    namesB.resize(nmsz);namesB[0]="b";namesB[1]="largest";namesB[2]="max";namesB[3]="maximal";namesB[4]="maximum";
    namesB[5]="upper";namesB[6]="up";namesB[7]="upper bound"; namesB[8]="upper bound";
    double a=getParameter(dp.parameters,namesA).first;
    double b=getParameter(dp.parameters,namesB).first;
    if(b-a<GL_EPSILON){
      a=0.0;b=1.0;
    }
    for(long i=0;i<sz;++i){
      res[i]=uniformC(a,b);
    }
    return res;
  }
  std::vector<double> sampleFromExponential(const DistributionParameters& dp){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesA;
    long nmsz=8;
    namesA.resize(nmsz);namesA[0]="a";namesA[1]="lambda";namesA[2]="l";namesA[3]="parameter";namesA[4]="par";
    namesA[5]="p";namesA[6]="m";namesA[7]="mu";
    double a=getParameter(dp.parameters,namesA).first;
    if(a<GL_EPSILON){
      a=1.0;
    }
    for(long i=0;i<sz;++i){
      res[i]=exponential(a);
    }
    return res;
  }
  std::vector<double> sampleFromUniformDiscrete(const DistributionParameters& dp){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesA,namesB;
    long nmsz=9;
    namesA.resize(nmsz);namesA[0]="a";namesA[1]="smallest";namesA[2]="min";namesA[3]="minimal";namesA[4]="minimum";
    namesA[5]="lower";namesA[6]="low";namesA[7]="lower bound"; namesA[8]="lower bound";
    namesB.resize(nmsz);namesB[0]="b";namesB[1]="largest";namesB[2]="max";namesB[3]="maximal";namesB[4]="maximum";
    namesB[5]="upper";namesB[6]="up";namesB[7]="upper bound"; namesB[8]="upper bound";
    long a=getIntParameter(dp.parameters,namesA).first;
    long b=getIntParameter(dp.parameters,namesB).first;
    if(b-a<2){
      a=0;b=10;
    }
    ++b;
    for(long i=0;i<sz;++i){
      res[i]=static_cast<double>(uniformInt(a,b));
    }
    return res;
  }
  std::vector<double> sampleFromBinomial(const DistributionParameters& dp,const long & overwriteN=0){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesA,namesB;
    long nmsz=5;
    namesA.resize(nmsz);namesA[0]="n";namesA[1]="number of trials";namesA[2]="ntrials";namesA[3]="trials";
    namesA[4]="k";
    nmsz=6;
    namesB.resize(nmsz);namesB[0]="p";namesB[1]="probability";namesB[2]="bias";namesB[3]="b";
    namesB[4]="parameter";namesB[5]="lambda";
    long n=getIntParameter(dp.parameters,namesA).first;
    double p=getParameter(dp.parameters,namesB).first;
    if(n<1){
      n=1;
    }
    if(overwriteN>0){
      n=overwriteN;
    }
    if(n>GL_bigInt){
      n=100;
    }
    if((p<GL_EPSILON)||(p>1- GL_EPSILON)) {
      p=0.5;
    }
    for(long i=0;i<sz;++i){
      res[i]=static_cast<double>(binomial(n,p));
    }
    return res;
  }
  std::vector<double> sampleFromGeometric(const DistributionParameters& dp){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesB;
    long nmsz=6;
    namesB.resize(nmsz);namesB[0]="p";namesB[1]="probability";namesB[2]="bias";namesB[3]="b";
    namesB[4]="parameter";namesB[5]="lambda";
    double p=getParameter(dp.parameters,namesB).first;
    if((p<GL_EPSILON)||(p>1- GL_EPSILON)) {
      p=0.5;
    }
    for(long i=0;i<sz;++i){
      res[i]=static_cast<double>(geometric(p));
    }
    return res;
  }
  std::vector<double> sampleFromPoisson(const DistributionParameters& dp){
    std::vector<double> res;
    long sz=dp.sampleSize;
    if(sz<1){sz=1;}
    res.resize(sz);
    std::vector<std::string> namesB;
    long nmsz=6;
    namesB.resize(nmsz);namesB[0]="p";namesB[1]="probability";namesB[2]="bias";namesB[3]="b";
    namesB[4]="parameter";namesB[5]="lambda";
    double lambda=getParameter(dp.parameters,namesB).first;
    if(lambda<0.01 ) {
      lambda=1.0;
    }
    std::vector<double> pVect;
    long nums=1000;
    pVect.resize(nums);
    std::vector<long> valVect;
    valVect.resize(nums);
    double total=0.0;
    double current=1.0;
    for(long i=0;i<nums;++i){
      valVect[i]=i;
      pVect[i]=current;
      total+=current;
      current *= lambda;
      current /= static_cast<double>(i+1);
    }
    double recTotal=1.0/total;
    for(long i=0;i<nums;++i){
      pVect[i] *= recTotal;
    }
    std::string ms="";
    for(long i=0;i<10;++i){
      ms+=std::to_string(valVect[i])+"\t";
    }
    ms+="\n";
    for(long i=0;i<10;++i){
      ms+=std::to_string(pVect[i])+"\t";
    }
    for(long i=0;i<sz;++i){
      res[i]=static_cast<double>(generalDiscrete(valVect,pVect));
    }
    return res;
  }
  std::vector<double> sampleFromDistribution(const DistributionParameters& dp){
    std::vector<double> res;
    if(dp.name=="normal"){
      return sampleFromNormal(dp);
    }
    if(dp.name=="uniform"){
      return sampleFromUniform(dp);
    }
    if(dp.name=="exponential"){
      return sampleFromExponential(dp);
    }
    if(dp.name=="discunif"){//this is uniform discrete
      return sampleFromUniformDiscrete(dp);
    }
    if(dp.name=="binomial"){
      return sampleFromBinomial(dp);
    }
    if(dp.name=="bernoulli"){
      return sampleFromBinomial(dp,1);
    }
    if(dp.name=="geometric"){
      return sampleFromGeometric(dp);
    }
    if(dp.name=="poisson"){
      return sampleFromPoisson(dp);
    }
    return res;
  }
  std::vector<std::vector<long> > generatePermutations(const PermutationParameters& pp){
    std::vector<std::vector<long> > res;
    if(pp.numSamples<1){
      return res;
    }
    if(pp.smallest>=pp.largest){
      return res;
    }
    res.resize(pp.numSamples);
    for(long i=0;i<pp.numSamples;++i){
      res[i]=RNDF::genRandPermutation(pp.smallest,pp.largest);
    }
    if(pp.writingDirection=="vertical"){
      return BF::transposeMatrix(res);
    }
    return res;
  }
  std::vector<double> evaluateNormalCDF(const NormalDistributionParameters &ndp){
    std::vector<double> res;
    res.resize(1);
    double result=GL_MIN_INFTY;
    res[0]=GL_MIN_INFTY;
    if(ndp.sigma<GL_EPSILON){
      return res;
    }
    double rescaledinput=(ndp.x-ndp.m)/ndp.sigma;
    double sign=1.0;
    if(rescaledinput<0.0){
      sign=-1.0;
      rescaledinput*=-1.0;
    }
    res[0]=0.5*(1.0+ sign*std::erf(rescaledinput*GL_OneOverSqRootTwo));
    return res;
  }
  std::string toString(const long &t){
    return std::to_string(t);
  }
  std::string toString(const double &t){
    return BF::doubleToString(t);
  }
  template<typename TTT>
  std::string displayOneLineOfRes(const std::vector<TTT> & v,const std::string &separator, const long &numInLine=20){
    std::string res;
    long sz=v.size();
    long i=0;
    while(i<sz){
      if(i%numInLine==numInLine-1){
        if(i<sz-1){res+="\n";}
      }
      res+=toString(v[i])+separator;
      ++i;
    }
    return res;
  }
  std::string putInBox(const std::string &in){
    std::string res="";
    res+="<div class=\"card bg-light text-dark\">\n <div class=\"card-body\">\n";
    res+=in;
    res+="\n</div>\n</div>\n";
    return res;
  }
  std::string finalDisplay(const std::string &q,const std::string &in){
    return putInBox("<pre>"+in+"</pre>");
  }
  std::string wrongAntiSpamCode(){
    std::string res="";
    res+="<div class=\"card bg-light text-danger\">\n <div class=\"card-body\">\n";
    res+="<h2>Wrong anti-spam code!</h2>";
    res+="\n</div>\n</div>\n";
    return res;
  }
  template<typename TTT>
  std::string displayRes(const std::string& q, const std::vector<TTT> & v,const std::string &separator){
    std::string allLines=displayOneLineOfRes(v,separator);
    return finalDisplay(q,allLines);
  }
  template<typename TTT>
  std::string displayRes(const std::string &q,const std::vector< std::vector<TTT> > & v, const std::string &separator){
    std::string allLines="";
    long sz=v.size();
    for(long i=0;i<sz;++i){
      allLines+=displayOneLineOfRes(v[i],separator)+"\n";
    }
    return finalDisplay(q,allLines);
  }
  std::string coinDie(const long & _n){
    long num=_n;
    if((num<1)||(num>30)){
      num=10;
    }
    std::vector<std::string> coins,dies;
    coins.resize(num);dies.resize(num);
    for(long i=0;i<num;++i){
      dies[i]=std::to_string(uniformInt(1,7));
      coins[i]="H";
      if(RNDF::randNum(2)==1){
        coins[i]="T";
      }
    }
    std::string res="";
    res+="<h2>"+std::to_string(num)+" tosses of a coin</h2>\n";
    res+="<pre>";
    for(long i=0;i<num;++i){
      res+=coins[i]+" ";
    }
    res+="</pre>";
    res+="<h2>"+std::to_string(num)+" rolls of a die</h2>\n";
    res+="<pre>";
    for(long i=0;i<num;++i){
      res+=dies[i]+" ";
    }
    res+="</pre>";
    return putInBox(res);
  }
   std::string answerTheQuestion(const std::string & _q){
     if(BF::isNumeric(_q)){
       return coinDie(BF::stringToInteger(_q));
     }
     std::string res;
     std::string separator="\t";
     std::string question=SF::findAndReplace(_q,"comma","");
     if(question!=_q){
       separator=",";
     }
     DistributionParameters dp=getDParametersFromQuestion(question);
     if(dp.name!="notFound"){
       return displayRes(question,sampleFromDistribution(dp),separator);
     }
     PermutationParameters pp=getPParametersFromQuestion(question);
     if(pp.numSamples>0){
       return displayRes(question,generatePermutations(pp),separator);
     }
     std::map<std::string,std::string> replMap;
     replMap["CDF"]="";
     replMap["cumulative"]="";
     replMap["distribution"]="";
     replMap["function"]="";
     replMap["of"]="";
     replMap["the"]="";
     replMap["normal"]="";
     replMap["random"]="";
     replMap["variable"]="";
     std::string questionNRepl=MFRF::findAndReplace(question,replMap);
     if(question!=questionNRepl){
       NormalDistributionParameters ndp=getNDParametersFromQuestion(questionNRepl);
       return displayRes(question,evaluateNormalCDF(ndp),separator);
     }
     replMap.clear();
     replMap["="]="";
     replMap["?"]="";
     replMap["e"]=BF::doubleToString(FF::GL_CONST_E);
     replMap["pi"]=BF::doubleToString(FF::GL_CONST_PI);
     std::string questionCleaned=MFRF::findAndReplace(question,replMap);
     std::pair<double,int> evFormRes=FF::evFormula(questionCleaned);
     if(evFormRes.second==0){
       return finalDisplay(question,"The request was not understood.\nPlease try again.");
     }
     return finalDisplay(question, BF::doubleToString(evFormRes.first,10) );
   }
}
#endif
