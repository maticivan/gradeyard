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


#ifndef _INCL_MODIFYCERTWIHTOUTCHECKINGPERMISSIONS_CPP
#define _INCL_MODIFYCERTWIHTOUTCHECKINGPERMISSIONS_CPP


namespace MCEWCPI{
  std::string createCertNoPermissionCheck(const PSDI::SessionData & _psd,const std::string & _cName, const std::string & _certData){
    CERD::Certificate myCert;
    int certExists=myCert.setFromTextName(_cName);
    if(certExists==1){
      return "!failed!: Certificate with this name already exists";
    }
    int succ=myCert.createText(_cName);
    if(succ==0){
      return "!failed!: Something may be bad with the database. Could not create certificate.";
    }
    myCert.setTextData(TDI::prepareTextForTextTableRaw(_psd,_certData,"!noOldData!"));
    myCert.putInDB();
    return "!success!: "+myCert.getExternalCodeFromInternalId();
  }
  std::pair<std::string,std::string> mCertWCPAdv(const PSDI::SessionData & psd,const std::string & _cName, const std::string & _certData){
    CERD::Certificate myCert;
    int certExists=myCert.setFromTextName(_cName);
    std::pair<std::string,std::string> result;
    result.first="!failed!: The certificate does not exist";
    if(certExists==0){
      return result;
    }
    std::string oldData=myCert.getTextData();
    result.second=oldData;
    myCert.setTextData(TDI::prepareTextForTextTableRaw(psd,_certData,oldData));
    myCert.putInDB();
    result.first="!success!";
    return result;
  }
  std::string modifyCertWithoutCheckingPermissions(const PSDI::SessionData & psd,const std::string & _certId, const std::string & _certData){
    return mCertWCPAdv(psd,_certId,_certData).first;
  }
  std::string modifyCertWithoutCheckingPermissionsAndCreateRecoveryCommand(PSDI::SessionData & psd,const std::string & _certId, const std::string & _certData){
    std::pair<std::string,std::string> res=mCertWCPAdv(psd,_certId,_certData);
    if(res.first=="!success!"){
      psd.recoveryOperationNames.push("certificate");
      psd.recoveryOperationCommands.push(BMD::recoveryTextWithCustomData(_certId,res.second,"modifyCertificate"));
    }
    return res.first;
  }
  std::string deleteCertNoPermissionCheck(const std::string & _cName){
    CERD::Certificate sf;
    int certExists=sf.setFromTextName(_cName);
    if(certExists==0){
      return "!failed!: Certificate name does not exist";
    }
    int succ=sf.deleteRecord();
    if(succ==0){
      return "!failed!: Something may be bad with the database. The certificate name did exist when checked but could not delete.";
    }
    return "!success!";
  }

}

#endif
