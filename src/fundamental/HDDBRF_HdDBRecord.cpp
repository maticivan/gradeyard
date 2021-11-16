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


#ifndef _INCL_MYHDRecord_CPP
#define _INCL_MYHDRecord_CPP

namespace HDDBRF{
  std::string GL_DBREC_DEB;

  class Record{
  private:
      std::vector<std::string> keys;
      std::string mainData;
      std::string mDFileName;
      std::string secureForStorage(const std::string &) const;
      std::string unpackFromStorage(const std::string &) const;

  public:
      Record();
      void setKeys(const std::vector<std::string> &);
      std::vector<std::string> getKeys() const;
      void setMainDataRTD(const std::string &);
      std::string getMainDataRTD() const;
      virtual long operator<(const Record &) const;
      virtual void loadFromString(const std::string&);
      std::string putDataIntoString() const;
      std::string putKeysIntoString() const;
      std::string putIntoString() const;
      std::string getMainDataFileName() const;
      void setMainDataFileName(const std::string &);
  };
  std::string Record::secureForStorage(const std::string & source) const{
      return SF::findAndReplace(source,"*!_",GF::GL_HIDING_STRING_HDDBRF);
  }
  std::string Record::unpackFromStorage(const std::string & source) const{
      return SF::findAndReplace(source,GF::GL_HIDING_STRING_HDDBRF,"*!_");
  }

  Record::Record(){
      keys.resize(1);
      mainData="noData";
      mDFileName="notSet";
      keys[0]="noKey";
  }
  void Record::setMainDataFileName(const std::string & fN){mDFileName=fN;}
  std::string Record::getMainDataFileName() const{return mDFileName;}
  void Record::setKeys(const std::vector<std::string> & _kV){  keys=_kV;}
  std::vector<std::string> Record::getKeys() const{return keys;}
  void Record::setMainDataRTD(const std::string &_mD){mainData=_mD;}
  std::string Record::getMainDataRTD() const{return mainData;}
  long Record::operator<(const Record & _oth) const{
      if(keys.size()<(_oth.keys).size()){
          return 1;
      }
      if(keys.size()>(_oth.keys).size()){
          return 0;
      }
      long sz=keys.size();
      long i=0;
      while(i<sz){
          if(keys[i]<(_oth.keys)[i]){
              return 1;
          }
          if(keys[i]>(_oth.keys)[i]){
              return 0;
          }
          ++i;
      }
      return 0;
  }
  void Record::loadFromString(const std::string& s){

      std::string dataB="_dB*!_";
      std::string dataE="_/dB*!_";
      std::string numKeysB="_nmk*!_";
      std::string numKeysE="_/nmk*!_";
      std::string nextKeyB="_nky*!_";
      std::string nextKeyE="_/nky*!_";


      std::string d=s+dataB+"defaultData"+dataE;
      d+= numKeysB+"1"+numKeysE+nextKeyB+"defaultKey"+nextKeyE;
      std::pair<std::string,int> res1,res2,res3;
      long pos=0;
      res1=SF::extract(d,pos,dataB,dataE);

      long nk;
      res2=SF::extract(d,pos,numKeysB,numKeysE);
      nk=BF::stringToInteger(res2.first);
      if(nk>0){
          mainData= unpackFromStorage(res1.first);
          long i=0;
          keys.resize(nk);
          while(i<nk){
              res3=SF::extract(d,pos,nextKeyB,nextKeyE);
              if(res3.second==1){
                  keys[i]=unpackFromStorage(res3.first);
              }
              else{
                  keys[i]="noKey";
              }
              ++i;
          }
      }
  }


  std::string Record::putDataIntoString() const{
      std::string dataB="_dB*!_";
      std::string dataE="_/dB*!_";

      std::string resString=dataB+secureForStorage(mainData)+dataE;

      return resString;
  }
  std::string Record::putKeysIntoString() const{
      std::string numKeysB="_nmk*!_";
      std::string numKeysE="_/nmk*!_";
      std::string nextKeyB="_nky*!_";
      std::string nextKeyE="_/nky*!_";
      std::string resString="";
      long nk=keys.size();
      if(nk>0){
          resString+=numKeysB+std::to_string(nk)+numKeysE;
          for(long i=0;i<nk;++i){
              resString+=nextKeyB+secureForStorage(keys[i])+nextKeyE;
          }
      }
      else{
          resString+=numKeysB+"1"+numKeysE+nextKeyB+"noKey"+nextKeyE;
      }
      return resString;
  }
  std::string Record::putIntoString() const{
      return putDataIntoString()+putKeysIntoString();
  }
}

#endif
