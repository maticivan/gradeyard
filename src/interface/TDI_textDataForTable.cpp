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


#ifndef _INCL_TEXTDATAFORTABLE_CPP
#define _INCL_TEXTDATAFORTABLE_CPP

namespace TDI{
  int allowedToManuallyInputSystemData(const PSDI::SessionData& psd,const std::string & _sysData, const std::string & existingPermissions, const std:: string & existingDataType){
    if((psd.isRoot=="yes")||(psd.allowedToExecuteAll=="yes")){
      return 1;
    }
    return 0;
  }
  std::string prepareSystemData(const std::string & tCreated,
                                                    const std::string & createdBy,
                                                    const std::string & tModified,
                                                    const std::string & modifiedBy,
                                                    const std::string & perm,
                                                    const std::string & dType){
    std::string fR;
    fR=LI::GL_LN.s_createdB+tCreated+LI::GL_LN.s_createdE;
    fR+=LI::GL_LN.s_createdByB+createdBy+LI::GL_LN.s_createdByE;
    fR+=LI::GL_LN.s_modifiedB+tModified+LI::GL_LN.s_modifiedE;
    fR+=LI::GL_LN.s_modifiedByB+modifiedBy+LI::GL_LN.s_modifiedByE;
    fR+=LI::GL_LN.s_permissionStringB+perm+LI::GL_LN.s_permissionStringE;
    fR+=LI::GL_LN.s_docTypeStringB+dType+LI::GL_LN.s_docTypeStringE;
    return fR;
  }
  std::string prepareEmptyTextWithAdvancedPermissions(const std::string& read,const std::string &write){
    TMF::Timer tm;
    std::string tmst=tm.timeString();
    std::string perm="_permission__name_read_/name__userOrGroup_"+read+"_/userOrGroup__/permission_";
    perm+="_permission__name_write_/name__userOrGroup_"+write+"_/userOrGroup__/permission_";
    std::string sysT= prepareSystemData(tmst,"system",tmst,"system",perm,"infoText");
    return LI::GL_LN.s_sysDataB+sysT+LI::GL_LN.s_sysDataE+LI::GL_LN.s_tDataB+""+LI::GL_LN.s_tDataE;
  }
  std::string prepareTextForTextTableRaw(const PSDI::SessionData& psd, const std::string &_nText, const std::string & _oText){
    std::string fR;
    long pos=0;
    std::pair<std::string,int> allSD_n=SF::extract(_nText,pos,LI::GL_LN.s_sysDataB,LI::GL_LN.s_sysDataE);
    pos=0;
    std::pair<std::string,int> allSD_o=SF::extract(_oText,pos,LI::GL_LN.s_sysDataB,LI::GL_LN.s_sysDataE);
    pos=0;
    std::pair<std::string,int> allTD_n=SF::extract(_nText,pos,LI::GL_LN.s_tDataB,LI::GL_LN.s_tDataE);
    TMF::Timer tm;
    long indicatorNoviceUser=1;
    std::string tNow=tm.timeString();
    std::string tCreated=tNow;
    std::string createdBy=psd.my_un;
    std::string tModified=tNow;
    std::string modifiedBy=psd.my_un;
    std::string perm="_permission__name_read_/name__userOrGroup_everyone_/userOrGroup__/permission_\n";
    perm+="_permission__name_write_/name__userOrGroup_"+psd.my_un+"_/userOrGroup__/permission_\n";
    std::string dType="regularText";
    if(allSD_o.second==1){
        std::string tModifiedThatWeDontCareAbout;
        std::string modifiedByThatWeDontCareAbout;
        HSF::parametersFromString(allSD_o.first,tCreated,createdBy,tModifiedThatWeDontCareAbout,modifiedByThatWeDontCareAbout,perm,dType);
    }
    if(allSD_n.second==1){
      indicatorNoviceUser=0;
      if(allowedToManuallyInputSystemData(psd,allSD_n.first,perm,dType)==1){
        HSF::parametersFromString(allSD_n.first,tCreated,createdBy,tModified,modifiedBy,perm,dType);
      }
    }
    std::string textData="";
    if(allTD_n.second==1){
      indicatorNoviceUser=0;
      textData=allTD_n.first;
    }
    if(indicatorNoviceUser==1){
      textData=_nText;
    }
    std::string newSystemData;
    newSystemData=prepareSystemData(tCreated,createdBy,tModified,modifiedBy,perm,dType);
    fR=LI::GL_LN.s_sysDataB+newSystemData+LI::GL_LN.s_sysDataE+LI::GL_LN.s_tDataB+textData+LI::GL_LN.s_tDataE;
    return fR;
  }
  std::string couasTextString(const std::string & mCreated, const std::string & mUserN, const std::string & mFirstN,
                              const std::string & mLastN, const std::string & preparedText){
    std::string textDataM="";
    textDataM+=LI::GL_LN.st_sepCreatedB+mCreated+LI::GL_LN.st_sepCreatedE;
    textDataM+=LI::GL_LN.st_sepUNameB+mUserN+LI::GL_LN.st_sepUNameE;
    textDataM+=LI::GL_LN.st_sepFNameB+ mFirstN+LI::GL_LN.st_sepFNameE;
    textDataM+=LI::GL_LN.st_sepLNameB+mLastN+LI::GL_LN.st_sepLNameE;
    textDataM+=LI::GL_LN.st_sepCTextB;
    textDataM+=preparedText;
    textDataM+=LI::GL_LN.st_sepCTextE;
    return textDataM;
  }
}


#endif
