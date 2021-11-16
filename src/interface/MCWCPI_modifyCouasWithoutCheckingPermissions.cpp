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


#ifndef _INCL_MODIFYCOUASWIHTOUTCHECKINGPERMISSIONS_CPP
#define _INCL_MODIFYCOUASWIHTOUTCHECKINGPERMISSIONS_CPP


namespace MCWCPI{

  std::pair<std::string,std::string> mCouasWCPAdv(const PSDI::SessionData & psd,const std::string & _couasId, const std::string & _couasData){
    CAD::CouAs myCouas;
    int couasExists=myCouas.setFromExternalCode(_couasId);
    std::pair<std::string,std::string> result;
    result.first="!failed!: The course or assignment does not exist";
    if(couasExists==0){
      return result;
    }
    std::pair<std::string,int> allD;
    long pos;
    int indicatorAdvancedUser=0;
    pos=0;allD=SF::extract(_couasData,pos,LI::GL_LN.s_tDataB,LI::GL_LN.s_tDataE);
    if(allD.second==1){
      pos=0;allD=SF::extract(_couasData,pos,LI::GL_LN.s_sysDataB,LI::GL_LN.s_sysDataE);
      if(allD.second==1){
        indicatorAdvancedUser=1;
      }
    }

    std::string oldData=myCouas.getTextData();
    result.second=oldData;
    std::string mDataProcessed=_couasData,uNM=psd.my_un,uFN=psd.myFirstName,uLN=psd.myLastName,nSt="";
    if(indicatorAdvancedUser==0){
      mDataProcessed="";
      std::string oldDT="";

      std::string createdTM;

      pos=0;allD=SF::extract(_couasData,pos,"_user_","_/user_");
      if(allD.second==1){
        mDataProcessed=_couasData;
      }
      else{
        TMF::Timer tm;
        createdTM=tm.timeString();

        pos=0;allD=SF::extract(oldData,pos,LI::GL_LN.s_tDataB,LI::GL_LN.s_tDataE);
        if(allD.second==1){
          oldDT=allD.first;
          pos=0;allD=SF::extract(oldDT,pos,"_created_","_/created_");
          if(allD.second==1){
            createdTM=allD.first;
          }
          pos=0;allD=SF::extract(oldDT,pos,"_user_","_/user_");
          if(allD.second==1){
            uNM=allD.first;
          }
          pos=0;allD=SF::extract(oldDT,pos,"_firstName_","_/firstName_");
          if(allD.second==1){
            uFN=allD.first;
          }
          pos=0;allD=SF::extract(oldDT,pos,"_lastName_","_/lastName_");
          if(allD.second==1){
            uLN=allD.first;
          }


        }

        mDataProcessed+="_created_";
        mDataProcessed+= createdTM;
        mDataProcessed+="_/created_";

        mDataProcessed+="_user_"+uNM+"_/user_";
        mDataProcessed+="_firstName_"+uFN+"_/firstName_";
        mDataProcessed+="_lastName_"+uLN+"_/lastName_";
        mDataProcessed+="_couasText!*_";
        mDataProcessed+=_couasData;
        mDataProcessed+="_/couasText!*_";
      }

    }
    else{
      mDataProcessed=_couasData;
    }
    myCouas.setTextData(TDI::prepareTextForTextTableRaw(psd,mDataProcessed,oldData));
    myCouas.putInDB();
    result.first="!success!";

    return result;
  }

  std::string modifyCouasWithoutCheckingPermissions(const PSDI::SessionData & psd,const std::string & _couasId, const std::string & _couasData){
    return mCouasWCPAdv(psd,_couasId,_couasData).first;
  }

  std::string modifyCouasWithoutCheckingPermissionsAndCreateRecoveryCommand(PSDI::SessionData & psd,const std::string & _couasId, const std::string & _couasData){
    std::pair<std::string,std::string> res=mCouasWCPAdv(psd,_couasId,_couasData);
    if(res.first=="!success!"){
      psd.recoveryOperationNames.push("enrollment");
      psd.recoveryOperationCommands.push(BMD::recoveryTextWithCustomData(_couasId,res.second,"modifyCourseAssignment"));
    }

    return res.first;
  }
}

#endif
