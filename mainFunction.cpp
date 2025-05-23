//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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

#include "src/fundamental/SHF_standardHeaders.cpp"
#include "src/fundamental/SAIOF_setupAndIO.cpp"
std::string GL_MAIN_SETUP_FILE_NAME=SAIOF::get_GL_MAIN_SETUP_FILE_NAME();
std::string GL_VERSION="2.10.0 (2025/04/04)";
#include "src/fundamental/MTF_mainTable.cpp"
#include "src/db/DD_databases.cpp"
#include "src/graphs/MGG_mainGraphs.cpp"
#include "src/interface/MWII_mainWI.cpp"

int main() {
  GF::GL_MAIN_SECRET_NUMBER=BF::stringToIntegerRemoveStartAndEnd(GL_MAIN_SETUP_FILE_NAME);
  TMF::Timer timeToGeneratePage;
  timeToGeneratePage.start();
  cgicc::Cgicc ch;
  std::string setups=IOF::fileToString(GL_MAIN_SETUP_FILE_NAME);
  std::map<std::string,std::string> mSetup;
  SF::varValPairs(setups,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",mSetup);
  HDPF::GLOBAL_PS.getSetupFromMap(mSetup);
  DD::GL_DBS.getSetupFromMap(mSetup);
  CAGI::GL_Code_Counter.remainingCodes=DD::GL_DBS.getMaxCodesToRun();
  DD::GL_MAIN_DB.initialize();
  ENCF::GL_SECRET_LOCATION.folder=DD::GL_DBS.getChallengeAnswStorage();
  MWII::GL_WI.getSetupFromMap(mSetup);
  SII::SessionInformation mainSession;
  mainSession.initSession(ch);
  std::string queryAnswer="";
  if(mainSession.isFormResponded()==1){
    std::string commands=mainSession.getResponse("commands");
    if(commands!="notFound"){
      commands=mainSession.enhanceTheCommandDueToComfUserEdit(commands);
      std::vector<CCI::Command> comms= CCI::getAllCommands(commands);
      long csz=comms.size();
      for(long j=0;j<csz;++j){
        std::string exRes=comms[j].executeCommand(mainSession);
      }
      mainSession.changeMainText();
    }
    std::string userReg=mainSession.getResponse("userReg");
    if(userReg=="yes"){
      if(mainSession.passedAntiSpam()){
        std::string pass1=mainSession.getResponse("pass1");
        std::string pass2=mainSession.getResponse("pass2");
        long sz1=pass1.length();
        long sz2=pass2.length();
        if((sz1<5)||(pass1!=pass2)){
          mainSession.changeMainText("problemWithPasswords");
        }
        else{
          std::string userN=mainSession.getResponse("username");
          std::string firstN=mainSession.getResponse("firstName");
          std::string lastN=mainSession.getResponse("lastName");
          std::string email=mainSession.getResponse("email");
          if(userN.length()<5){
            mainSession.changeMainText("problemWithUserName");
          }
          else{
            std::string userReg;
            WUD::User w;
            int succ=w.createUser(userN);
            if(succ==0){
              mainSession.changeMainText("usernameExists");
            }
            else{
              WUD::OthUsData tmpOUD;
              tmpOUD.password=pass1;
              tmpOUD.firstName=firstN;
              tmpOUD.lastName=lastN;
              tmpOUD.email=email;
              tmpOUD.rawEncSystemData=w.createKeyPair(RNDF::genRandCode(20));
              w.addUserData(tmpOUD);
              mainSession.changeMainText("userRegistered");
            }
          }
        }
      }
      else{
        mainSession.changeMainText("wrongAntiSpamCode");
      }
    }
    std::string query=mainSession.getResponse("query");
    if(query=="yes"){
      if((mainSession.loggedIn())||(mainSession.passedAntiSpam())){
        queryAnswer=QAF::answerTheQuestion(mainSession.getResponse("question"));
      }
      else{
        queryAnswer=QAF::wrongAntiSpamCode();
      }
    }
  }
  mainSession.addDebuggingMessagesIfInDebuggingMode();
  std::cout<<mainSession.preparePage(queryAnswer)<<"\n";
  mainSession.createRecoveryCommands();
  timeToGeneratePage.end();
  return 0;
}
