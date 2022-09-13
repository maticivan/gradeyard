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


#ifndef _INCL_COMMANDCENTER_CPP
#define _INCL_COMMANDCENTER_CPP

namespace CCI{


  Command::Command(const std::string & _initSt){
    initialized=0;
    initFromString(_initSt);
  }
  void Command::deInit(){
    type="";
    initialized=0;
    arguments.resize(0);
  }
  void Command::initFromString(const std::string & _initSt){
    std::vector<std::string> iI= SF::stringToVector(_initSt,"_nc***_","_/nc***_");
    long sz=iI.size();
    if(sz>0){
      std::vector<std::string> temp_args;
      temp_args.clear();

      if(sz>1){
        long j=1;
        temp_args.resize(sz-1);
        while(j<sz){
          temp_args[j-1]=iI[j];
          ++j;
        }

      }
      if(syntaxCorrect(iI[0],temp_args)==1){
        initialized=1;
        type=iI[0];
        if((type=="createUser")||(type=="modifyUser")){
          //the createUser and modifyUser are special cases of createUserExtraAdvanced and modifyUserExtraAdvanced
          // they miss the last three arguments which are added to be "no" so the extra advanced versions can be
          // called
          temp_args.resize(sz+2);
          std::string defaultValue="no";
          if(type=="modifyUser"){
            defaultValue="!*!";
          }
          temp_args[sz-1]=defaultValue;
          temp_args[sz]=defaultValue;
          temp_args[sz+1]=defaultValue;


        }
        if((type=="createUserAdvanced")||(type=="modifyUserAdvanced")){
          //the createUserAdvanced and modifyUserAdvanced are special cases of createUserExtraAdvanced and modifyUserExtraAdvanced
          // they miss the last three argument which are added to be "no" so the extra advanced versions can be
          temp_args.resize(sz);
          std::string defaultValue="no";
          if(type=="modifyUserAdvanced"){
            defaultValue="!*!";
          }
          temp_args[sz-1]=defaultValue;
        }
        arguments=std::move(temp_args);
      }
    }

  }
  int Command::syntaxCorrect(const std::string & _tp, const std::vector<std::string> & _ta) const{
    // not yet implemented
    // This function should recognize the command and depending on its type check whether the arguments
    // follow the syntax rules
    return 1;
  }

  int Command::allowedToExecute(SII::SessionInformation & _s) const{
    std::vector<std::string> openTags,closeTags;
    long sz=17;
    openTags.resize(sz);closeTags.resize(sz);
    openTags[0]="_insert_";
    closeTags[0]="_/insert_";
    openTags[1]="_n*_";
    closeTags[1]="_/n*_";
    openTags[2]="_code_";
    closeTags[2]="_/code_";
    openTags[3]="_cde_";
    closeTags[3]="_/cde_";
    openTags[4]="<div";
    closeTags[4]="</div>";
    openTags[5]="<p>";
    closeTags[5]="</p>";
    openTags[6]="<b>";
    closeTags[6]="</b>";
    openTags[7]="<i>";
    closeTags[7]="</i>";
    openTags[8]="_codeBox_";
    closeTags[8]="_/codeBox_";

    openTags[9]="<h1";
    closeTags[9]="</h1>";
    openTags[10]="<h2";
    closeTags[10]="</h2>";
    openTags[11]="<h3";
    closeTags[11]="</h3>";
    openTags[12]="<h4";
    closeTags[12]="</h4>";
    openTags[13]="<h5";
    closeTags[13]="</h5>";
    openTags[14]="<center>";
    closeTags[14]="</center>";
    openTags[15]="<code>";
    closeTags[15]="</code>";
    openTags[16]="<pre>";
    closeTags[16]="</pre>";

    long asz=arguments.size();
    long i=0;
    while(i<asz){
      if(HSF::matchingTags(arguments[i],openTags,closeTags)==0){
        return 0;
      }
      ++i;
    }
    return 1;
  }
  std::string Command::commandTextForDebugging() const{
    std::string fR;
    fR="The type of the command is: "+type+";<BR>\n";
    long asz=arguments.size();
    for(long i=0;i<asz;++i){
      fR+="Argument "+std::to_string(i)+" is "+arguments[i]+";<BR>\n";
    }
    return fR;
  }

  std::string Command::executeDeleteFile(SII::SessionInformation & _s){
    long correctSizeForThisCommand=1;
    // arguments:
    // 0) externalFileCode
    long sz=arguments.size();

    if((type!="deleteFile")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete method with wrong number of arguments";
    }

    return _s.deleteFile(arguments[0]);
  }
  std::string Command::executeCreateText(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textName
    // 1) rawTextForDatabase
    long sz=arguments.size();
    if((type!="createText")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called create text method with wrong number of arguments";
    }
    return _s.createText(arguments[0],arguments[1]);
  }



  std::string Command::executeModifyText(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textName
    // 1) rawTextForDatabase
    long sz=arguments.size();
    if((type!="modifyText")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called modify text method with wrong number of arguments";
    }
    return _s.modifyText(arguments[0],arguments[1]);
  }
  std::string Command::executeDeleteText(SII::SessionInformation & _s){
    long correctSizeForThisCommand=1;
    //arguments:
    // 0) textName
    long sz=arguments.size();
    if((type!="deleteText")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete text method with wrong number of arguments";
    }
    return _s.deleteText(arguments[0]);
  }

  std::string Command::executeCreateForum(SII::SessionInformation & _s){
    long correctSize=3;
    //arguments:
    // 0) userName
    // 1) forumOptions
    // 2) collectorId
    long sz=arguments.size();
    if((type!="createForum")||(sz!=correctSize)){
      return "!failed!:badCommand: Called create forum method with wrong number of arguments";
    }
    return _s.createForum(arguments[0],arguments[1],arguments[2]);
  }

  std::string Command::executeCreateStandardCourse(SII::SessionInformation & _s){
    long correctSize=2;
    //arguments:
    // 0) userName
    // 1) forumOptions
    // 2) collectorId
    long sz=arguments.size();
    if((type!="createStandardCourse")||(sz!=correctSize)){
      return "!failed!:badCommand: Called createStandardCourse method with wrong number of arguments";
    }
    return _s.createStandardCourse(arguments[0],arguments[1]);
  }
  std::string Command::executeCreateMessage(SII::SessionInformation & _s){
    long correctSize=3;
    //arguments:
    // 0) userName
    // 1) messageText
    // 2) idOfMessageThatCollectsTheMessages
    long sz=arguments.size();
    if((type!="createMessage")||(sz!=correctSize)){
      return "!failed!:badCommand: Called create message method with wrong number of arguments";
    }
    return _s.createMessage(arguments[0],arguments[1],arguments[2]);
  }

  std::string Command::executeModifyMessage(SII::SessionInformation & _s){
    long correctSize=2;
    //arguments:
    // 0) messageId
    // 1) messageText
    long sz=arguments.size();
    if((type!="modifyMessage")||(sz!=correctSize)){
      return "!failed!:badCommand: Called modify message method with wrong number of arguments";
    }
    return _s.modifyMessage(arguments[0],arguments[1]);
  }
  std::string Command::executeDeleteMessage(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) messageId
    // 1) idOfMessageThatCollectsTheMessages
    long sz=arguments.size();
    if((type!="deleteMessage")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete message method with wrong number of arguments";
    }
    return _s.deleteMessage(arguments[0],arguments[1]);
  }

  std::string Command::executeCreateCouas(SII::SessionInformation & _s){
    long correctSize=3;
    //arguments:
    // 0) userName
    // 1) couasText
    // 2) idOfCouasThatCollectsTheCouases
    long sz=arguments.size();
    if((type!="createCourseAssignment")||(sz!=correctSize)){
      return "!failed!:badCommand: Called create course or assignment method with wrong number of arguments";
    }
    return _s.createCouas(arguments[0],arguments[1],arguments[2]);
  }

  std::string Command::executeModifyCouas(SII::SessionInformation & _s){
    long correctSize=2;
    //arguments:
    // 0) messageId
    // 1) messageText
    long sz=arguments.size();
    if((type!="modifyCourseAssignment")||(sz!=correctSize)){
      return "!failed!:badCommand: Called modify course or assignment method with wrong number of arguments";
    }
    return _s.modifyCouas(arguments[0],arguments[1]);
  }
  std::string Command::executeDeleteCouas(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) messageId
    // 1) idOfMessageThatCollectsTheMessages
    long sz=arguments.size();
    if((type!="deleteCourseAssignment")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete course or assignment method with wrong number of arguments";
    }
    return _s.deleteCouas(arguments[0],arguments[1]);
  }
  std::string Command::executeCreateGradingForCourse(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) messageId
    // 1) idOfMessageThatCollectsTheMessages
    long sz=arguments.size();
    if((type!="createGradingForCourse")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called createGradingForCourse method with wrong number of arguments";
    }
    return _s.createGradingForCourse(arguments[0],arguments[1]);
  }

  std::string Command::executeCreateRespRec(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textName
    // 1) rawTextForDatabase
    long sz=arguments.size();
    if((type!="createResponseReceiver")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called create ResponseReceiver method with wrong number of arguments";
    }
    return _s.createRespRec(arguments[0],arguments[1]);
  }
  std::string Command::executeModifyRespRec(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textName
    // 1) rawTextForDatabase
    long sz=arguments.size();
    if((type!="modifyResponseReceiver")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called modify ResponseReceiver method with wrong number of arguments";
    }
    return _s.modifyRespRec(arguments[0],arguments[1],"no");
  }

  std::string Command::executeAddStudentsToExam(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) examName
    // 1) rawTextToAddStudentsFrom
    long sz=arguments.size();
    if((type!="addStudentsToExam")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called addStudentsToExam method with wrong number of arguments";
    }
    return _s.addStudentsToExam(arguments[0],arguments[1]);
  }

  std::string Command::executeGenerateExam(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textNameForExamTemplate
    // 1) rawTextToGenerateFrom
    long sz=arguments.size();
    if((type!="generateExam")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called generateExam method with wrong number of arguments";
    }
    return _s.generateExam(arguments[0],arguments[1]);
  }

  std::string Command::executeGeneratePdfsForExam(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textNameForExamTemplate
    // 1) rawTextToGenerateFrom
    long sz=arguments.size();
    if((type!="generatePdfsForExam")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called generatePdfsForExam method with wrong number of arguments";
    }
    return _s.generatePdfsForExam(arguments[0],arguments[1]);
  }
  std::string Command::executeUpdateVersionsForExam(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textNameForExamTemplate
    // 1) rawTextToUpdateFrom
    long sz=arguments.size();
    if((type!="updateVersionsForExam")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called updateVersionsForExam method with wrong number of arguments";
    }
    return _s.updateIndividualVersionsOfExam(arguments[0],arguments[1]);
  }

  std::string Command::executeExamBackupText(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) textNameForExamTemplate
    // 1) rawTextWithOtherOptions
    long sz=arguments.size();
    if((type!="examBackupText")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called examBackupText method with wrong number of arguments";
    }
    return _s.createExamBackupText(arguments[0],arguments[1]);
  }

  std::string Command::executeDistributeExamToStudents(SII::SessionInformation & _s){
    long correctSizeForThisCommand=2;
    //arguments:
    // 0) examName
    // 1) rawTextWithNamesOfStudents
    long sz=arguments.size();
    if((type!="distributeExamToStudents")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called distributeExamToStudents method with wrong number of arguments";
    }
    return _s.distributeExamToStudents(arguments[0],arguments[1]);
  }

  std::string Command::executeDeleteRespRec(SII::SessionInformation & _s){
    long correctSizeForThisCommand=1;
    //arguments:
    // 0) textName
    long sz=arguments.size();
    if((type!="deleteResponseReceiver")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete ResponseReceiver method with wrong number of arguments";
    }
    return _s.deleteRespRec(arguments[0]);
  }
  std::string Command::executeDeleteUser(SII::SessionInformation & _s){
    long correctSizeForThisCommand=1;
    //arguments:
    // 0) textName
    long sz=arguments.size();
    if((type!="deleteUser")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete user method with wrong number of arguments";
    }
    return _s.deleteUser(arguments[0]);
  }
  std::string Command::executeCreateUser(SII::SessionInformation & _s){
    long correctSizeForThisCommand=9;
    long sz=arguments.size();
    if( ( (type!="createUser") && (type!="createUserAdvanced") && (type!="createUserExtraAdvanced") )||(sz!=correctSizeForThisCommand)) {
      return "!failed!:badCommand: Called create user method with wrong number of arguments";
    }
    return _s.createUser(arguments[0],arguments[1],arguments[2],arguments[3],arguments[4],arguments[5],arguments[6],arguments[7],arguments[8]);
  }
  std::string Command::executeModifyUser(SII::SessionInformation & _s){
    long correctSizeForThisCommand=8;
    long sz=arguments.size();
    if(  ( (type!="modifyUser") && (type!="modifyUserAdvanced") && (type!="modifyUserExtraAdvanced")  )||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called modify user method with wrong number of arguments";
    }
    return _s.modifyUser(arguments[0],arguments[1],arguments[2],arguments[3],arguments[4],arguments[5],arguments[6],arguments[7]);
  }
  std::string Command::executeAddToHierarchy(SII::SessionInformation &_s){
    long correctSizeForThisCommand=2;
    long sz=arguments.size();
    if((type!="addToHierarchy")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called add to hierarchy method with wrong number of arguments";
    }
    return _s.addToHierarchy(arguments[0],arguments[1]);
  }
  std::string Command::executeRemoveFromHierarchy(SII::SessionInformation &_s){
    long correctSizeForThisCommand=2;
    long sz=arguments.size();
    if((type!="removeFromHierarchy")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called remove from hierarchy method with wrong number of arguments";
    }
    return _s.removeFromHierarchy(arguments[0],arguments[1]);
  }
  std::string Command::executeAssignGraders(SII::SessionInformation &_s){
    long correctSizeForThisCommand=2;
    long sz=arguments.size();
    if((type!="assignGraders")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called assignGraders method with wrong number of arguments";
    }
    return _s.assignGraders(arguments[0],arguments[1]);
  }
  std::string Command::executeEditTimer(SII::SessionInformation &_s){
    long correctSizeForThisCommand=2;
    long sz=arguments.size();
    if((type!="editTimer")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called editTimer method with wrong number of arguments";
    }
    return _s.editTimer(arguments[0],arguments[1]);
  }
  std::string Command::executeEnrollStudents(SII::SessionInformation &_s){
    long correctSizeForThisCommand=4;
    long sz=arguments.size();
    if((type!="enrollStudents")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called enrollStudents method with wrong number of arguments";
    }
    return _s.enrollStudents(arguments[0],arguments[1],arguments[2],arguments[3]);
  }

  std::string Command::executeCreateBackupText(SII::SessionInformation &_s){
    long correctSizeForThisCommand=2;
    long sz=arguments.size();
    if((type!="backup")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called backup method with wrong number of arguments";
    }
    return _s.backupDBs(arguments[0],arguments[1]);
  }
  std::string Command::executeDeleteWebsiteForUser(SII::SessionInformation &_s){
    long correctSizeForThisCommand=1;
    long sz=arguments.size();
    if((type!="deleteWebsiteForUser")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called delete website for user method with wrong number of arguments";
    }
    return _s.deleteCloneForGuest(arguments[0]);
  }
  std::string Command::executeCreateWebsiteForUser(SII::SessionInformation &_s){
    long correctSizeForThisCommand=1;
    long sz=arguments.size();
    if((type!="createWebsiteForUser")||(sz!=correctSizeForThisCommand)){
      return "!failed!:badCommand: Called create website for user method with wrong number of arguments";
    }
    return _s.createCloneForGuest(arguments[0]);
  }

  std::string Command::executeCommand(SII::SessionInformation & _s){
    if(allowedToExecute(_s)!=1){
      return "!failed!: Command cannot be executed";
    }
    if(type=="deleteFile"){
      return executeDeleteFile(_s);
    }
    if(type=="createText"){
      return executeCreateText(_s);
    }
    if(type=="modifyText"){
      return executeModifyText(_s);
    }

    if(type=="deleteText"){
      return executeDeleteText(_s);
    }
    if(type=="createMessage"){
      return executeCreateMessage(_s);
    }
    if(type=="modifyMessage"){
      return executeModifyMessage(_s);
    }
    if(type=="deleteMessage"){
      return executeDeleteMessage(_s);
    }
    if(type=="createForum"){
      return executeCreateForum(_s);
    }
    if(type=="createStandardCourse"){
      return executeCreateStandardCourse(_s);
    }
    if(type=="createCourseAssignment"){
      return executeCreateCouas(_s);
    }
    if(type=="modifyCourseAssignment"){
      return executeModifyCouas(_s);
    }
    if(type=="deleteCourseAssignment"){
      return executeDeleteCouas(_s);
    }
    if(type=="createGradingForCourse"){
      return executeCreateGradingForCourse(_s);
    }

    if(type=="createResponseReceiver"){
      return executeCreateRespRec(_s);
    }
    if(type=="modifyResponseReceiver"){
      return executeModifyRespRec(_s);
    }
    if(type=="generateExam"){
      return executeGenerateExam(_s);
    }
    if(type=="generatePdfsForExam"){
      return executeGeneratePdfsForExam(_s);
    }
    if(type=="updateVersionsForExam"){
      return executeUpdateVersionsForExam(_s);
    }
    if(type=="examBackupText"){
      return executeExamBackupText(_s);
    }
    if(type=="addStudentsToExam"){
      return executeAddStudentsToExam(_s);
    }

    if(type=="deleteResponseReceiver"){
      return executeDeleteRespRec(_s);
    }
    if(type=="createUser"){
      return executeCreateUser(_s);
    }
    if(type=="createUserAdvanced"){
      return executeCreateUser(_s);
    }
    if(type=="createUserExtraAdvanced"){
      return executeCreateUser(_s);
    }
    if(type=="modifyUser"){
      return executeModifyUser(_s);
    }
    if(type=="modifyUserAdvanced"){
      return executeModifyUser(_s);
    }
    if(type=="modifyUserExtraAdvanced"){
      return executeModifyUser(_s);
    }
    if(type=="deleteUser"){
      return executeDeleteUser(_s);
    }
    if(type=="addToHierarchy"){
      return executeAddToHierarchy(_s);
    }

    if(type=="removeFromHierarchy"){
      return executeRemoveFromHierarchy(_s);
    }
    if(type=="assignGraders"){
      return executeAssignGraders(_s);
    }
    if(type=="editTimer"){
      return executeEditTimer(_s);
    }
    if(type=="enrollStudents"){
      return executeEnrollStudents(_s);
    }
    if(type=="backup"){
      return executeCreateBackupText(_s);
    }
    if(type=="deleteWebsiteForUser"){
      return executeDeleteWebsiteForUser(_s);
    }

    if(type=="createWebsiteForUser"){
      return executeCreateWebsiteForUser(_s);
    }

    return "!failed!: Command not found";
  }
  std::vector<Command> getAllCommands(const std::string & _st){
    std::vector<std::string>  allCs=SF::stringToVector(_st, "_command_","_/command_");
    Command tmp;
    std::vector<Command> allCmds;
    long sz=allCs.size();
    allCmds.resize(sz);
    for(long i=0;i<sz;++i){
      tmp.deInit();
      tmp.initFromString(allCs[i]);
      allCmds[i]=tmp;
    }
    return allCmds;
  }
}
#endif
