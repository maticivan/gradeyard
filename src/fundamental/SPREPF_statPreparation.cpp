//    GradeYard learning management system
//
//    Copyright (C) 2026 Ivan Matic, https://gradeyard.com
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

#ifndef _INCL_STATPREPARATION_CPP
#define _INCL_STATPREPARATION_CPP

namespace SPREPF{
  struct StructStatConstants{
  public:
    long maxNumFiles=100;
    long maxItemsInFile=250;
    long paddingConst=100000;
    long numFilesToLookForSpammerBehavior=3;
    long numLoginAttemptsToStartCollectingData=5;
    long numLoginAttemptsToGetSuspcious=7;
    long cutoff_in_seconds_ForStalling=5;
    long GL_maxSizeForReporting=500;
    std::string sepStat_NRDB="_nD*_";
    std::string sepStat_NRDE="_/nD*_";
    std::string sepStat_akzDB="aaaDZMB";
    std::string sepStat_akzDE="aaaDZME";
    std::string sepStat_NRSB="_nS*_";
    std::string sepStat_NRSE="_/nS*_";
    std::string sepStat_akzSB="aaaSZMB";
    std::string sepStat_akzSE="aaaSZME";
    std::string catDefaultBB="_cat";
    std::string catDefaultBE="_";
    std::string catDefaultEB="_/cat";
    std::string catDefaultEE="_";
    std::string catCurrentItemB="_cIID_";
    std::string catCurrentItemE="_/cIID_";
    std::string catNameB="_nm_";
    std::string catNameE="_/nm_";
    std::string catItemVB="_v_";
    std::string catItemVE="_/v_";
    std::string catItemBB="_i";
    std::string catItemBE="_";
    std::string catItemEB="_/i";
    std::string catItemEE="_";
  } STAT_CONSTS;
  struct StatData{
  public:
    std::string userName;
    std::string loggedIn;
    std::string timeString;
    std::string ipAddr;
    std::string att_page;
    std::string att_pageID;
    std::string att_rr;
    std::string pass1;
    std::string otherStatData;
    std::string putIntoString() const;
    void setFromString(const std::string &s);
  };
  std::string statPrepare(const std::string &i){
    std::map<std::string,std::string> replMap;
    replMap[STAT_CONSTS.sepStat_NRSB]=STAT_CONSTS.sepStat_akzSB;
    replMap[STAT_CONSTS.sepStat_NRSE]=STAT_CONSTS.sepStat_akzSE;
    replMap[STAT_CONSTS.sepStat_NRDB]=STAT_CONSTS.sepStat_akzDB;
    replMap[STAT_CONSTS.sepStat_NRDE]=STAT_CONSTS.sepStat_akzDE;
    std::string o=MFRF::findAndReplace(i,replMap);
    if(o.size()>STAT_CONSTS.GL_maxSizeForReporting){
        o.resize(STAT_CONSTS.GL_maxSizeForReporting);
        o+="...(too large)";
    }
    return o;
  }
  std::string StatData::putIntoString() const{
    std::string fR="";
    std::string bS=STAT_CONSTS.sepStat_NRSB;std::string eS=STAT_CONSTS.sepStat_NRSE;
    fR+=bS+statPrepare(userName)+eS;
    fR+=bS+statPrepare(timeString)+eS;
    fR+=bS+statPrepare(ipAddr)+eS;
    fR+=bS+statPrepare(att_page)+eS;
    fR+=bS+statPrepare(att_rr)+eS;
    std::string displPass=pass1;
    if(loggedIn!=""){
          displPass="[good]";
    }
    fR+=bS+statPrepare(displPass)+eS;
    fR+=bS+statPrepare(otherStatData)+eS;
    return fR;
  }
  void StatData::setFromString(const std::string &i){
    std::vector<std::string> dv=SF::stringToVector(i,STAT_CONSTS.sepStat_NRSB,STAT_CONSTS.sepStat_NRSE);
    long sz=dv.size();
    if(sz>4){
      userName=dv[0];
      timeString=dv[1];
      ipAddr=dv[2];
      att_page=dv[3];
      att_rr=dv[4];
      if(sz>5){
        pass1=dv[5];
      }
      if(sz>6){
        otherStatData=dv[6];
      }
    }
  }
  struct StatAtom{
  public:
    std::string name;
    long visits;
  };
  class StatPeriod{
  private:
    long maxItems;
    std::string periodName;
    long currentItemIDNum;
    std::string currentItemName;
    long currentItemValue;
    std::string rawString;
    std::string usefulRawStr;
    std::string catNB;
    std::string catNE;
    std::string catCIB;
    std::string catCIE;
    void setCurrentItemFromRawString();
    void updateUsefulRawStrFirstSoRawStringCanBeUpdated();
    std::string currentItemToString() const;
  public:
    StatPeriod(const std::string & = "noName", const long & = 1000);
    std::string getPeriodName() const;
    long getMaxItems() const;
    long getCurrentItemIDNum() const;
    std::string getCurrentItemName() const;
    long getCurrentItemValue() const;
    std::string getRawString() const;
    void setPeriodName(const std::string &);
    void setMaxItems(const long &);
    void setCurrentItemIDNum(const long &);
    void setCurrentItemName(const std::string &);
    void updateIfPeriodItemIsChanging(const std::string &);
    void increaseCounter(const long & =1);
    void setCurrentItemValue(const long &);
    void setRawString(const std::string &);
    void setRawStringAndUpdate(const std::string &,const std::string &);
    void updateRawString();
    std::vector<StatAtom> statSummary(const long & = 0) const;
    std::string debugPrinting() const;
  };
  StatPeriod::StatPeriod(const std::string & pN, const long & mItems){
    setPeriodName(pN);
    setMaxItems(mItems);
  }
  std::string StatPeriod::getPeriodName() const{return periodName;}
  long StatPeriod::getMaxItems() const{return maxItems;}
  long StatPeriod::getCurrentItemIDNum() const{return currentItemIDNum;}
  std::string StatPeriod::getCurrentItemName() const{return currentItemName;}
  long StatPeriod::getCurrentItemValue() const{return currentItemValue;}
  std::string StatPeriod::getRawString() const{return rawString;}
  void StatPeriod::setMaxItems(const long & m){
    maxItems=m;
  }
  void StatPeriod::setPeriodName(const std::string & pN){
    periodName=pN;
    catNB=STAT_CONSTS.catDefaultBB+pN+STAT_CONSTS.catDefaultBE;
    catNE=STAT_CONSTS.catDefaultEB+pN+STAT_CONSTS.catDefaultEE;
  }
  void StatPeriod::setCurrentItemIDNum(const long & i){
    currentItemIDNum=i;
    catCIB=STAT_CONSTS.catItemBB+BF::padded(i,100000,"0")+STAT_CONSTS.catItemBE;
    catCIE=STAT_CONSTS.catItemEB+BF::padded(i,100000,"0")+STAT_CONSTS.catItemEE;
  }
  void StatPeriod::setCurrentItemName(const std::string & _n){currentItemName=_n;}
  void StatPeriod::setCurrentItemValue(const long & _v){currentItemValue=_v;}
  void StatPeriod::setCurrentItemFromRawString(){
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(usefulRawStr,pos,STAT_CONSTS.catCurrentItemB,STAT_CONSTS.catCurrentItemE);
    if(allD.second==1){
      setCurrentItemIDNum(BF::stringToInteger(allD.first));
      std::string currentItemRawData;
      pos=0;allD=SF::extract(usefulRawStr,pos,catCIB,catCIE);
      if(allD.second==1){
        currentItemRawData=allD.first;
        pos=0;allD=SF::extract(currentItemRawData,pos,STAT_CONSTS.catNameB,STAT_CONSTS.catNameE);
        if(allD.second==1){
          setCurrentItemName(allD.first);
          pos=0;allD=SF::extract(currentItemRawData,pos,STAT_CONSTS.catItemVB,STAT_CONSTS.catItemVE);
          if(allD.second==1){
            setCurrentItemValue(BF::stringToInteger(allD.first));
          }
        }
      }
    }
    else{
      setCurrentItemIDNum(0);
      setCurrentItemName("nothingYet");
      setCurrentItemValue(0);
    }
  }
  void StatPeriod::setRawString(const std::string & _r){
    rawString = _r;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(rawString,pos,catNB,catNE);
    usefulRawStr="";
    if(allD.second==1){
      usefulRawStr=allD.first;
    }
    setCurrentItemFromRawString();
  }
  void StatPeriod::updateIfPeriodItemIsChanging(const std::string & newPI){
    if(currentItemName=="nothingYet"){
      currentItemName=newPI;
    }
    else{
      if(newPI!=currentItemName){
        setCurrentItemIDNum(currentItemIDNum+1);
        if(currentItemIDNum>=maxItems){
          setCurrentItemIDNum(0);
        }
        currentItemName=newPI;
        currentItemValue=0;
      }
    }
  }
  void StatPeriod::setRawStringAndUpdate(const std::string &rs, const std::string & newPI){
    setRawString(rs);
    updateIfPeriodItemIsChanging(newPI);
  }
  void StatPeriod::increaseCounter(const long & inc){currentItemValue+=inc;}
  std::string StatPeriod::currentItemToString() const{
    std::string currentItemRawData=catCIB+"\n";
    currentItemRawData+=STAT_CONSTS.catNameB+currentItemName+STAT_CONSTS.catNameE;
    currentItemRawData+="\n";
    currentItemRawData+=STAT_CONSTS.catItemVB+std::to_string(currentItemValue)+STAT_CONSTS.catItemVE;
    currentItemRawData+="\n";
    currentItemRawData+=catCIE;
    return currentItemRawData;
  }
  void StatPeriod::updateUsefulRawStrFirstSoRawStringCanBeUpdated(){
    std::string currentItemRawData=currentItemToString();
    long pos; std::pair<std::string, int> allD;
    std::string newCurrentItemID=STAT_CONSTS.catCurrentItemB+std::to_string(currentItemIDNum)+STAT_CONSTS.catCurrentItemE;
    pos=0;allD=SF::extractAndReplace(usefulRawStr,pos,STAT_CONSTS.catCurrentItemB,STAT_CONSTS.catCurrentItemE,0,newCurrentItemID);
    if(allD.second==1){
      usefulRawStr=allD.first;
      pos=0;allD=SF::extractAndReplace(usefulRawStr,pos,catCIB,catCIE,0,currentItemRawData);
      if(allD.second==1){
        usefulRawStr=allD.first;
      }
      else{
        usefulRawStr+="\n"+currentItemRawData;
      }
    }
    else{
      usefulRawStr= newCurrentItemID + "\n"+currentItemRawData;
    }
  }
  void StatPeriod::updateRawString(){
    updateUsefulRawStrFirstSoRawStringCanBeUpdated();
    long pos; std::pair<std::string, int> allD;
    std::string nDataStr=catNB+ usefulRawStr+ catNE;
    pos=0;allD=SF::extractAndReplace(rawString,pos,catNB,catNE,0,nDataStr);
    if(allD.second==1){
      rawString=allD.first;
    }
    else{
      rawString+="\n"+nDataStr;
    }
  }
  std::vector<StatAtom> StatPeriod::statSummary(const long &direction) const{
    std::vector<StatAtom> result;
    result.resize(maxItems);
    std::string catTempCIB,catTempCIE;
    long pos; std::pair<std::string,int> allD;
    long cI=maxItems-1-currentItemIDNum;
    long relI;std::string tempRaw;
    for(long i=0;i<maxItems;++i){
      if(cI>=maxItems){cI=0;}
      relI=cI;
      if(direction==1){
        relI=maxItems-1-cI;
      }
      result[relI].name="NA";
      result[relI].visits=0;
      catTempCIB=STAT_CONSTS.catItemBB+BF::padded(i,100000,"0")+STAT_CONSTS.catItemBE;
      catTempCIE=STAT_CONSTS.catItemEB+BF::padded(i,100000,"0")+STAT_CONSTS.catItemEE;
      pos=0;allD=SF::extract(usefulRawStr,pos,catTempCIB,catTempCIE);
      if(allD.second==1){
        tempRaw=allD.first;
        pos=0;allD=SF::extract(tempRaw,pos,STAT_CONSTS.catNameB,STAT_CONSTS.catNameE);
        if(allD.second==1){
          result[relI].name=allD.first;
          pos=0;allD=SF::extract(tempRaw,pos,STAT_CONSTS.catItemVB,STAT_CONSTS.catItemVE);
          if(allD.second==1){
            result[relI].visits=BF::stringToInteger(allD.first);
          }
        }

      }
      ++cI;
    }
    return result;
  }
  std::string StatPeriod::debugPrinting() const{
    std::string fR;
    fR+="maxItems: "+std::to_string(maxItems)+"\n";
    fR+="periodName: "+periodName+"\n";
    fR+="currentItemIDNum: "+std::to_string(currentItemIDNum)+"\n";
    fR+="currentItemName: "+ currentItemName +"\n";
    fR+="currentItemValue: "+std::to_string(currentItemValue)+"\n";
    fR+="Separators\n";
    fR+=catNB+"..."+catNE+"\n";
    fR+=catCIB+"..."+catCIE+"\n";
    return fR;
  }
}
#endif
