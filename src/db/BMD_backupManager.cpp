//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_WI_BACKUPMANAGER_CPP
#define _INCL_WI_BACKUPMANAGER_CPP

namespace BMD{
  
  std::string recoveryTextWithCustomData(const std::string & textName, const std::string &customData, const std::string &recCommand){
    std::string recText="";
    recText+="_command_\n";
    recText+="_nc***_"+recCommand+"_/nc***_\n";
    recText+="Document name:_nc***_";
    recText+=textName;
    recText+="_/nc***_\n";
    recText+="START EDITING AFTER THIS LINE _nc***_";
    recText+=customData;
    recText+="_/nc***_\n DO NOT EDIT THIS LINE  _/command_";
    return recText;
  }
  std::string recoveryMainText(const std::string & textName, const std::string &recoveryCommand, const std::string &rType){
    std::string recText="";
    TMD::MText sf;
    int sc=sf.setFromTextName(textName);
    if(sc==1){
      if(rType=="latex"){
        recText=AICD::prepareLatexText(sf.getTextData());
      }
      else{
        recText=recoveryTextWithCustomData(textName,sf.getTextData(),recoveryCommand);
      }
    }
    return recText;
  }
  std::string deleteCommand(const std::string & deleteCommandName, const std::string & documentName){
    std::string comm="_command_\n _nc***_"+deleteCommandName+"_/nc***_\n";
    comm+="_nc***_"+documentName+"_/nc***_\n";
    comm+="_/command_\n";
    return comm;
  }
  std::string deleteCouasOrMessage(const std::string & deleteCommandName, const std::string & documentName, const std::string &boss){
    std::string comm="_command_\n _nc***_"+deleteCommandName+"_/nc***_\n";
    comm+="_nc***_"+documentName+"_/nc***_\n";
    comm+="_nc***_"+boss+"_/nc***_\n";
    comm+="_/command_\n";
    return comm;
  }
  std::string recoveryResponse(const std::string & textName){
    std::string recText="";
    RMD::Response sf;
    int sc=sf.setFromTextName(textName);
    if(sc==1){
      recText+="DO NOT EDIT THIS LINE _command_\n";
      recText+="DO NOT EDIT THIS LINE_nc***_createResponseReceiver_/nc***_\n";
      recText+="DO NOT EDIT THIS LINE Text name:_nc***_";
      recText+=textName;
      recText+="_/nc***_\n";
      recText+="START EDITING AFTER THIS LINE _nc***_";
      recText+=sf.getTextData();
      recText+="_/nc***_\n DO NOT EDIT THIS LINE  _/command_";
    }
    return recText;
  }
  std::string recovery(const std::string & dbName, const std::string & textName,const std::string& rType){
    if(dbName=="Texts"){
      return recoveryMainText(textName,"createText",rType);
    }
    if(dbName=="Responses"){
      return recoveryResponse(textName);
    }
    return "";
  }
  std::string convertNumberToDBRowNameTexts(const long &j){
    std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["mainText"])[j];
    TMD::MText sf;
    sf.setFromInternalId(sR.first[0]);
    return sf.getTextName();
  }
  std::string convertNumberToDBRowNameRespRec(const long &j){
    std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["response"])[j];
    RMD::Response sf;
    sf.setFromInternalId(sR.first[0]);
    return sf.getTextName();
  }
  std::string convertNumberToDBRowName(const std::string &dbNameProvidedByUser, const long & j){
    if(dbNameProvidedByUser=="Responses"){
      return  convertNumberToDBRowNameRespRec(j);
    }
    return convertNumberToDBRowNameTexts(j);
  }
}
#endif
