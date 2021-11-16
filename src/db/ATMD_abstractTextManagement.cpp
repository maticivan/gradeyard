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


#ifndef _INCL_ABSTRACTTEXTMANAGEMENT_CPP
#define _INCL_ABSTRACTTEXTMANAGEMENT_CPP
namespace ATMD{

  class Text:public AMD::AbstractManager{
  protected:


    std::string textName;
    std::string ow_dbs_key0="id";
    std::string ow_dbs_key1="name";
    std::string ow_dbsMFInd="mainText";
    std::string ow_cntrName="textsCntr";


    long ow_primeIndex0=9;
    long ow_primeIndex1=20;
    long ow_permutInd=8;
    long ow_shift=76442;


  public:
    Text(const std::string & ="!*!", const std::string & ="!*!");
    std::string getTextName() const;
    std::string getKey1() const;
    void setKey1(const std::string &);




    int createText(const std::string & = "!*!");
    // arguments: 1) textName
    //    If no argument is present, the text name will be assigned

    // returns :
    // 1 - success
    // 0 - textName already used

    int textNameExists(const std::string &_tn) const;


    int setFromTextName(const std::string &);


  };
  int Text::setFromTextName(const std::string & _tn){

    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]=dbs_key1;
    v[0]=_tn;
    std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
    searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
    long sz=searchRes.size();
    if(sz==0){
      return 0;
    }
    std::string _iId=searchRes[0].first[0];
    internalVectorNumber=getInternalNumberFromInternalId(_iId);
    textName=_tn;

    textData=searchRes[0].second;

    existenceEstablishedBefore=1;
    return 1;
  }


  Text::Text(const std::string & _tn, const std::string &  _intTId){
  }

  std::string Text::getTextName() const{
    return textName;
  }
  std::string Text::getKey1() const{
    return textName;
  }
  void Text::setKey1(const std::string & _k){
    textName=_k;
  }




  int Text::textNameExists(const std::string &_tn) const{
    Text tmp;
    int exists=tmp.setFromTextName(_tn);
    return exists;
  }
  int Text::createText(const std::string & _tn){

    std::string tn="!*!";
    if(_tn!="!*!"){
      //check whether _tn already exists. If it does, exit with 0
      if(textNameExists(_tn)){
        return 0;
      }
      tn=_tn;
    }


    long textIdIsFree=0;
    std::string textId;
    std::vector<std::string> k,v;
    k.resize(1);v.resize(1);
    k[0]="id";
    while(textIdIsFree==0){
      CD::Counter tId(cntrName);
      tId.increase();
      textId=tId.getCodeWord(1);
      v[0]=textId;
      std::vector< std::pair<std::vector<std::string>, std::string> > searchRes;
      searchRes=DD::GL_MAIN_DB.dbsM[dbsMFInd].search(v,k);
      long sz=searchRes.size();
      if(sz==0){
        if(tn!="!*!"){
          textIdIsFree=1;
        }
        else{
          tn=getExternalCodeFromInternalId(textId);
          if(!textNameExists(tn)){
            textIdIsFree=1;
          }
          else{
            tn="!*!";
          }
        }
      }
    }
    textName=tn;
    internalVectorNumber=getInternalNumberFromInternalId(textId);
    putInDB();
    existenceEstablishedBefore=1;
    return 1;
  }

}
#endif
