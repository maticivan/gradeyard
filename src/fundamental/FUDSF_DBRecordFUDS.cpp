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


#ifndef _INCL_FUDSF_DBRecordFUDS_CPP
#define _INCL_FUDSF_DBRecordFUDS_CPP

namespace FUDSF{
  std::string sepXB="_#)!@(X_";
  std::string sepXE="_/#)!@(X_";
  std::string sepSTTotalB="_#)!@(ST_";
  std::string sepSTTotalE="_/#)!@(ST_";
  std::string sepTB="_#)!@(T_";
  std::string sepTE="_/#)!@(T_";
  std::string sepSigmaB="_#)!@(S_";
  std::string sepSigmaE="_/#)!@(S_";
  std::string sepInfB="_#)!@(I_";
  std::string sepInfE="_/#)!@(I_";


  std::string plusInfinity="z";
  std::string minusInfinity="a";
  std::string nonInfinity="nnn";

  long shift=2147483648;//2^31

  class XSigma{
  public:
    std::string x;
    long sigma;
    long t;
    long infinityIndicator; // -1: -infinity; 0: finite number; +1: +infinity
    XSigma();
    std::string putSigmaTToString() const;
    void loadSigmaTFromString(const std::string &);
    void loadFromString(const std::string &);
    std::string putIntoString() const;
    std::string debugPrinting() const;
  };
  XSigma::XSigma(){
    x="nothingYet";
    t=0;
    infinityIndicator=0;
    sigma=0;
  }
  void XSigma::loadSigmaTFromString(const std::string & st){
    std::pair<std::string,int> res;
    long pos;
    pos=0;res=SF::extract(st,pos,sepInfB,sepInfE);
    if(res.second==1){
      infinityIndicator=0;
      if(res.first==plusInfinity){
        infinityIndicator=1;
      }
      if(res.first==minusInfinity){
        infinityIndicator=-1;
      }
    }
    pos=0;res=SF::extract(st,pos,sepTB,sepTE);
    if(res.second==1){
      t=BF::stringToInteger(res.first)-shift;
    }
    pos=0;res=SF::extract(st,pos,sepSigmaB,sepSigmaE);
    if(res.second==1){
      sigma=BF::stringToInteger(res.first)-shift;
    }

  }
  void XSigma::loadFromString(const std::string & st){
    std::pair<std::string,int> res;
    long pos;
    pos=0;res=SF::extract(st,pos,sepXB,sepXE);
    if(res.second==1){
      x=res.first;
      pos=0;res=SF::extract(st,pos,sepSTTotalB,sepSTTotalE);
      if(res.second==1){
        loadSigmaTFromString(res.first);
      }
    }
  }
  std::string XSigma::putSigmaTToString() const{
    std::string res="";
    res+=sepSigmaB;
    res+=std::to_string(sigma+shift);
    res+=sepSigmaE;
    res+=sepInfB;
    if(infinityIndicator==1){
      res+=plusInfinity;
    }
    if(infinityIndicator==-1){
      res+=minusInfinity;
    }
    if(infinityIndicator==0){
      res+=nonInfinity;
    }
    res+=sepInfE+sepTB;
    if(infinityIndicator==0){
      res+=std::to_string(t+shift);
    }
    else{
      res+=std::to_string(shift);
    }
    res+=sepTE;
    return res;
  }
  std::string XSigma::putIntoString() const{
    std::string res="";
    res+=sepXB+x+sepXE;
    res+=sepSTTotalB;
    res+=putSigmaTToString();
    res+=sepSTTotalE;
    return res;
  }
  std::string XSigma::debugPrinting() const{
    std::string d="";
    d+="X: " +x+"\n";
    d+="T: ";
    if(infinityIndicator==1){
      d+="+infinity";
    }
    if(infinityIndicator==-1){
      d+="-infinity";
    }
    if(infinityIndicator==0){
      d+=std::to_string(t);
    }
    d+="\nSigma: "+std::to_string(sigma)+"\n";

    return d;
  }

  class X:public XSigma{
  public:
    int operator<(const X &) const;
  };
  int X::operator<(const X& oth)const{
    if(x < oth.x){
      return 1;
    }
    return 0;
  }
  class Sigma:public XSigma{
  public:
    int operator<(const XSigma & ) const;
  };
  int Sigma::operator<(const XSigma & oth) const{
    if(sigma < oth.sigma){
      return 1;
    }
    if(oth.sigma < sigma){
      return 0;
    }
    if( infinityIndicator < oth.infinityIndicator){
      return 1;
    }
    if( oth.infinityIndicator < infinityIndicator){
      return 0;
    }
    if(infinityIndicator != 0){
      return 0;
    }
    if(t<oth.t){
      return 1;
    }
    return 0;
  }















  class XRecord:public HDDBRF::Record{
  private:
      X mainContent;

  public:
      void setMainContent(const X &);
      void loadFromMainData();
      void loadFromString(const std::string&);
      X getMainContent() const;

      long operator<(const XRecord &) const;
  };
  void XRecord::setMainContent(const X & _x){
    mainContent=_x;
    std::vector<std::string> kToSet;
    kToSet.resize(1);
    kToSet[0]=_x.x;
    HDDBRF::Record::setKeys(kToSet);
    HDDBRF::Record::setMainDataRTD(_x.putSigmaTToString());
  }
  void XRecord::loadFromMainData(){
    mainContent.loadSigmaTFromString(HDDBRF::Record::getMainDataRTD());
    std::vector<std::string> ks=HDDBRF::Record::getKeys();
    if(ks.size()>0){
      mainContent.x=ks[0];
    }
  }
  void XRecord::loadFromString(const std::string &s){
    HDDBRF::Record::loadFromString(s);
    loadFromMainData();
  }
  X XRecord::getMainContent() const{
    return mainContent;
  }
  long XRecord::operator<(const XRecord & oth)const{
    return mainContent < oth.mainContent;
  }
  class SigmaTRecord:public HDDBRF::Record{
  private:
      Sigma mainContent;

  public:
      void setMainContent(const Sigma &);
      void loadFromMainData();
      void loadFromString(const std::string&);
      Sigma getMainContent() const;

      long operator<(const SigmaTRecord &) const;
  };
  void SigmaTRecord::setMainContent(const Sigma & _st){
    mainContent=_st;
    std::vector<std::string> kToSet;
    kToSet.resize(1);
    kToSet[0]=_st.putSigmaTToString();
    HDDBRF::Record::setKeys(kToSet);
    HDDBRF::Record::setMainDataRTD(_st.x);
  }
  void SigmaTRecord::loadFromMainData(){
    std::vector<std::string> ks=HDDBRF::Record::getKeys();
    if(ks.size()>0){
      mainContent.loadSigmaTFromString(ks[0]);
    }
    mainContent.x=HDDBRF::Record::getMainDataRTD();
  }
  void SigmaTRecord::loadFromString(const std::string &s){
    HDDBRF::Record::loadFromString(s);
    loadFromMainData();
  }
  Sigma SigmaTRecord::getMainContent() const{
    return mainContent;
  }
  long SigmaTRecord::operator<(const SigmaTRecord & oth)const{
    return mainContent < oth.mainContent;
  }


}

#endif
