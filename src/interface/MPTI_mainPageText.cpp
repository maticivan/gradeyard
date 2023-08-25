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

#ifndef _INCL_WI_MainText_CPP
#define _INCL_WI_MainText_CPP

namespace MPTI{
  MainText::MainText(const std::string & _nameInDB, const std::string & _sysR, const std::string & _u){
    initialize(_nameInDB,_sysR,_u);
  }
  std::string MainText::modifyMe() const{
    return "modifyText";
  }
  int MainText::initialize(const std::string & _nameInDB, const std::string & _sysReq, const std:: string & _uName){
    TMD::MText m;
    int s=m.setFromTextName(_nameInDB);
    permitRead.clear();
    permitWrite.clear();
    permitRead.insert(s_root);
    permitWrite.insert(s_root);
    tCreated="unknown";
    createdBy="unknown";
    tModified="unknown";
    modifiedBy="unknown";
    documentType="unknown";
    sysDataRequested="no111";
    subTextRecursionDepth=0;
    myUserName=_uName;
    sysDataRequested=_sysReq;
    if(s==1){
      initText=m.getTextData();
      long pos=0;
      std::pair<std::string,int> allSD=SF::extract(initText,pos,s_sysDataB,s_sysDataE);
      pos=0;
      std::pair<std::string,int> allTD=SF::extract(initText,pos,s_tDataB,s_tDataE);
      if(allSD.second==1){
        sysDataRaw=allSD.first;
        std::string pText=s_notFound;
        HSF::parametersFromString(allSD.first,tCreated,createdBy,tModified,modifiedBy,pText,documentType);
        if(pText!=s_notFound){
          PBD::createPermitSet(permitRead, pText,s_individualPermissionB, s_individualPermissionE, "read");
          PBD::createPermitSet(permitWrite, pText,s_individualPermissionB, s_individualPermissionE, "write");
        }
      }
      tName=m.getTextName();
      tExternalId=m.getExternalCodeFromInternalNumber();
      if(allTD.second==1){
        rawText=allTD.first;
        translationVarToVal=getTranslationMap(rawText);
      }
      return 1;
    }
    return 0;
  }
  void MainText::selectVersionForListOfFiles(const std::string & version, const std::string & startOfList){
    std::string systemText;
    long pos;
    std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(rawText,pos,"_systemText*!_","_/systemText*!_");
    if(allD.second==1){
      systemText=allD.first;
      long good=1;
      std::string headerText,headerChoice,actionText,actionChoice,headerOld,actionOld,existingTitleOrDescription="";
      pos=0;allD=SF::extract(systemText,pos,"_sectionHD*!_","_/sectionHD*!_");
      if(allD.second==1){headerText=allD.first;}else{good=0;}
      pos=0;allD=SF::extract(systemText,pos,"_actionText*!_","_/actionText*!_");
      if(allD.second==1){actionText=allD.first;}else{good=0;}
      if(good==1){
        if(version==""){
          pos=0;allD=SF::extract(headerText,pos,"_defaultHD*!_","_/defaultHD*!_");
          if(allD.second==1){headerChoice=allD.first;}else{good=0;}
          pos=0;allD=SF::extract(actionText,pos,"_defaultAction*!_","_/defaultAction*!_");
          if(allD.second==1){actionChoice=allD.first;}else{good=0;}
        }
        else{
          FMD::FileManager sf;
          int scc=sf.setFromExternalCode(version);
          if(scc==1){
            pos=0;allD=SF::extract(sf.getTextData(),pos,"_fInfo*|_","_/fInfo*|_");
            if(allD.second==1){
              existingTitleOrDescription=allD.first;
            }
          }
          pos=0;allD=SF::extract(headerText,pos,"_specialHD*!_","_/specialHD*!_");
          if(allD.second==1){headerChoice=SF::findAndReplace(allD.first,"_*FileCodeToModify*!_",version);}else{good=0;}
          pos=0;allD=SF::extract(actionText,pos,"_specialAction*!_","_/specialAction*!_");
          if(allD.second==1){
            actionChoice=SF::findAndReplace(allD.first,"_*FileCodeToModify*!_",version);
          }
          else{
            good=0;
          }
        }
      }
      if(good==1){
        std::map<std::string,std::string> replMap;
        replMap["_sectionHD*!_"+headerText+"_/sectionHD*!_"]=headerChoice;
        replMap["_actionText*!_"+actionText+"_/actionText*!_"]=actionChoice;
        replMap["_existingTitleOrDescription!*_"]=existingTitleOrDescription;
        std::string cleanForm=MFRF::findAndReplace(systemText,replMap);
        replMap.clear();
        replMap["_systemText*!_"+systemText+"_/systemText*!_"]=cleanForm;
        replMap["_startOfList*!!_"]=startOfList;
        rawText=MFRF::findAndReplace(rawText,replMap);
      }
    }
  }
}
#endif
