//    GradeYard learning management system
//
//    Copyright (C) 2021 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_COUNTERS_CPP
#define _INCL_COUNTERS_CPP



namespace CD{
  class Counter{
  private:
    std::string name;
    std::string dbsMCInd="counters";
    std::vector<long> states;
    long numStates;

    long shift=76442;
    long maxState=500000000000;
    long primeIndex0=0;
    long primeIndex1=1;
    long permutInd=0;
    BF::BijectiveFunction f0;
    BF::BijectiveFunction f1;
    std::vector<std::vector<long> > permuts01;
    std::vector<long> indCombs;
    long lenEach;
    long setPrimesCorrectly(BF::BijectiveFunction & , const long &, const long &);
    long setPermsCombsCorrectly(const long &, const long &);
  public:
    Counter(const std::string & ="certificator", const long & = 0, const long & = 1, const long & = 0, const long & = 76442);
    void setVariables(const std::string & ="certificator", const long & = 0, const long & = 1, const long & = 0, const long & = 76442);
    std::string getName() const;
    long getPrimeInd0() const;
    long getPrimeInd1() const;
    long getPermutInd() const;
    std::vector<long> getStates() const;
    std::string getCodeWord(const std::vector<long> &, const long & =0 ) const;
    std::string getCodeWord(const long & =0) const;

    std::vector<long> plainSeqFromString(const std::string &, const long & =0)const;



    long getNumStates() const;
    void putInDB();
    void increase();

  };

  std::string Counter::getCodeWord(const std::vector<long> & v, const long & indSimple) const{
    return BF::getCodeWord(v, indCombs, permuts01, f0, f1, shift,  indSimple);

  }

  std::string Counter::getCodeWord(const long &indSimple) const{
    return getCodeWord(states,indSimple);
  }

  std::vector<long> Counter::plainSeqFromString(const std::string & input, const long & indSimple) const{
    return BF::plainSeqFromString(input, indCombs, permuts01, f0, f1, shift, indSimple);

  }

  Counter::Counter(const std::string & _n, const long & _pI0, const long & _pI1, const long & _permutInd, const long & _sh){
    setVariables(_n, _pI0, _pI1, _permutInd,_sh);
  }
  long Counter::setPrimesCorrectly(BF::BijectiveFunction & f, const long & x, const long &max){
    long ind=0;
    if((x>0)&&(max>0)){
      ind=x%max;
    }
    f.setParameters(BF::GLOBAL_PRIME_SEQUENCES.primes[ind],BF::GLOBAL_PRIME_SEQUENCES.mults[ind],BF::GLOBAL_PRIME_SEQUENCES.shifts[ind],BF::GLOBAL_PRIME_SEQUENCES.inverses[ind]);
    return ind;
  }
  long Counter::setPermsCombsCorrectly(const long & x, const long &max){
    long ind=0;
    if((x>0)&&(max>0)){
      ind=x%max;
    }
    permuts01=BF::GLOBAL_PERMUTATION_SEQUENCES.pairsPermutations[ind];
    indCombs=BF::GLOBAL_PERMUTATION_SEQUENCES.indicatorCombinations[ind];
    lenEach=permuts01[0].size();
    return ind;
  }

  void Counter::setVariables(const std::string & _n, const long & _pI0, const long & _pI1, const long & _permutInd, const long & _sh){
    primeIndex0= setPrimesCorrectly(f0,_pI0, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    primeIndex1= setPrimesCorrectly(f1,_pI1, BF::GLOBAL_NUM_PRIME_SEQUENCES);
    permutInd= setPermsCombsCorrectly(_permutInd,BF::GLOBAL_NUM_PERMUTATION_SEQUENCES);
    if((_sh>50000)&&(_sh<500000)){
      shift=_sh;
    }
    name=_n;
    numStates=1;
    std::vector<std::string> k,v,vp;
    k.resize(1);vp.resize(1);
    k[0]="counterName";

    vp[0]=name;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMCInd].search(vp,k);


    long sz=searchRes.size();
    std::string textStates="_numStates*_1_/numStates*_ _n*0_0_/n*0_";
    if(sz==0){
      DD::GL_MAIN_DB.dbsM[dbsMCInd].insert(vp,textStates);
    }
    else{
      textStates=searchRes[0].second;
    }
    long pos=0;
    std::string sB,sE;
    numStates=BF::stringToInteger((SF::extract(textStates,pos,"_numStates*_","_/numStates*_")).first);
    states.resize(numStates);
    for(long i=0;i<numStates;++i){
      sB="n*"+std::to_string(i)+"_";
      sE=sB;
      sB="_"+sB;
      sE="_/"+sE;
      pos=0;
      states[i]=BF::stringToInteger((SF::extract(textStates,pos,sB,sE)).first);
    }


  }
  long Counter::getNumStates() const{
    return numStates;
  }
  long Counter::getPrimeInd0() const{
    return primeIndex0;
  }
  long Counter::getPrimeInd1() const{
    return primeIndex1;
  }
  long Counter::getPermutInd() const{
    return permutInd;
  }
  std::string Counter::getName() const{
    return name;
  }
  std::vector<long> Counter::getStates() const{
    return states;
  }
  void Counter::putInDB(){
    std::string textToPut="_numStates*_"+std::to_string(numStates)+"_/numStates*_";
    std::string sB,sE;
    for(long i=0;i<numStates;++i){
      sB="n*"+std::to_string(i)+"_";
      sE=sB;
      sB="_"+sB;
      sE="_/"+sE;
      textToPut+=sB+std::to_string(states[i])+sE;
    }
    std::vector<std::string> vp;
    vp.resize(1);vp[0]=name;
    DD::GL_MAIN_DB.dbsM[dbsMCInd].insert(vp,textToPut);
  }

  void Counter::increase() {
    long i=0;
    long finished=0;
    while((i<numStates)&&(finished==0)){
      ++states[i];
      if(states[i]<maxState){
        finished=1;
      }
      else{
        states[i]=0;
      }
      ++i;
    }
    if((i==numStates)&&(finished==0)){
      std::vector<long> newStates;
      newStates.resize(numStates+1);
      for(long i=0;i<numStates;++i){
        newStates[i]=0;
      }
      newStates[numStates]=1;
      states=newStates;
      ++numStates;
    }
     putInDB();
  }
}



#endif
