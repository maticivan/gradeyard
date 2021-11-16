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


#ifndef _INCL_PERMISSIONBASICS_CPP
#define _INCL_PERMISSIONBASICS_CPP
namespace PBD{

  std::map<std::string, std::set<std::string> > GL_PERMITS_Y;
  std::map<std::string, std::set<std::string> > GL_PERMITS_N;

  std::map<std::string,std::set<std::string> >::iterator locatePermitSet(std::map<std::string,std::set<std::string> > & mSS, const std::string & loc){
    std::map<std::string,std::set<std::string> >::iterator it,itE;
    itE=mSS.end();
    it=mSS.find(loc);
    if(it!=itE){
      return it;
    }
    std::set<std::string> emptySet;
    mSS[loc]=emptySet;
    it=mSS.find(loc);
    return it;
  }

  int permitExists_N(const std::string & requestId, const std::set<std::string> & permissionSet, const std::string & _uN){

    std::map<std::string,std::set<std::string> >::iterator itOk,itNotOk;
    itOk=locatePermitSet(GL_PERMITS_Y,requestId);
    itNotOk=locatePermitSet(GL_PERMITS_N,requestId);

    std::set<std::string>::const_iterator cit,citE;
    citE=(itOk->second).end();
    cit=(itOk->second).find(_uN);
    if(cit!=citE){
      return 1;
    }
    citE=(itNotOk->second).end();
    cit=(itNotOk->second).find(_uN);
    if(cit!=citE){
      return 0;
    }

    std::set<std::string>::const_iterator it,itE;

    itE=permissionSet.end();
    it=permissionSet.find("everyone");
    if(it==itE){
      // Text is not available to everyone.
      WUD::User u;
      if(u.setFromUsername(_uN)==0){
        (itNotOk->second).insert(_uN);
        return 0;
      }
      it=permissionSet.begin();
      std::string permissionToCheck;
      int foundCorrectPermission=0;
      while((foundCorrectPermission==0)&&(it!=itE)){
        permissionToCheck=*it;
        foundCorrectPermission=u.existsPath(permissionToCheck,"meHigh","uName",1);
        ++it;
      }
      if(foundCorrectPermission==0){
        (itNotOk->second).insert(_uN);
        return 0;
      }
    }
    (itOk->second).insert(_uN);
    return 1;


  }
  std::vector<std::string> getPermitsFromRaw_N(const std::vector<std::string> & rawPermisssions) {
    std::string s_userOrGroupB="_userOrGroup_";
    std::string s_userOrGroupE="_/userOrGroup_";
    std::string s_root="root";
    std::vector<std::string> fR;
    long sz=rawPermisssions.size();
    fR.resize(sz);
    std::pair<std::string,int> aD;
    long pos=0;
    for(long i=0;i<sz;++i){
      fR[i]=s_root;
      pos=0;
      aD=SF::extract(rawPermisssions[i],pos,s_userOrGroupB, s_userOrGroupE);
      if(aD.second==1){
        fR[i]=aD.first;
      }

    }
    return fR;
  }
  void createPermitSet(std::set<std::string> &permitSet,
                           const std::string &pText, const std::string &s_individualPermissionB,
                           const std::string &s_individualPermissionE,
                           const std::string &_sMustHave=""){

    std::string sMH="!*!";
    std::vector<std::string> rawPermisssions;
    if(_sMustHave!=""){
      sMH="_name_"+_sMustHave+"_/name_";
    }
    rawPermisssions=SF::stringToVector(pText,s_individualPermissionB, s_individualPermissionE,sMH);
    std::vector<std::string> pV=getPermitsFromRaw_N(rawPermisssions);
    long sz=pV.size();
    for(long i=0;i<sz;++i){
      permitSet.insert(pV[i]);
    }
  }

}

#endif
