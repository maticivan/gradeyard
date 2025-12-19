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

#ifndef _INCL_WI_Response_CPP
#define _INCL_WI_Response_CPP
namespace RTI{
  void LocationOfDocuments::getFromString(const std::string & in){
    std::vector<std::string> vecDocs=SF::stringToVector(in,"_n*|_","_/n*|_");
    long sz=vecDocs.size();
    if(sz>4){
      testLocation=vecDocs[0];
      gradeLocation=vecDocs[1];
      studentName=vecDocs[2];
      studentEmail=vecDocs[3];
      userName=vecDocs[4];
      graderUName=vecDocs[5];
    }
  }
  std::string LocationOfDocuments::putIntoString() const{
    std::string fR="";
    std::string sepB="_n*|_",sepE="_/n*|_";
    fR+=sepB+testLocation+sepE+"\n";
    fR+=sepB+gradeLocation+sepE+"\n";
    fR+=sepB+studentName+sepE+"\n";
    fR+=sepB+studentEmail+sepE+"\n";
    fR+=sepB+userName+sepE+"\n";
    fR+=sepB+graderUName+sepE+"\n";
    fR+=sepB+""+sepE+"\n";
    return fR;
  }
  int Response::isInitialized() const{
    return ind_initSuccess;
  }
  std::string Response::indFileSepB(const long & i) const{
    return APTI::GL_syntax.s_indFileSep_ARTBB+BF::padded(i,100,"0")+APTI::GL_syntax.s_infFileSep_ARTBEE;
  }
  std::string Response::indFileSepE(const long & i) const{
    return APTI::GL_syntax.s_indFileSep_ARTEB +BF::padded(i,100,"0")+APTI::GL_syntax.s_infFileSep_ARTBEE;
  }
  std::string Response::fileLabelForHTML(const long & i) const{
    return "fi"+BF::padded(i,100,"0");
  }
  Response::Response(const std::string & _nameInDB, const std::string & _sysR, const std::string & _u){
    initialize(_nameInDB,_sysR,_u);
  }
  std::string Response::modifyMe() const{
    return "modifyResponseReceiver";
  }
  int Response::initialize(const std::string & _nameInDB, const std::string & _sysReq, const std:: string & _uName){
    ind_initSuccess=0;
    int s=meRM.setFromTextName(_nameInDB);
    permitRead.clear();
    permitWrite.clear();
    permitRead.insert(APTI::GL_syntax.s_root);
    permitWrite.insert(APTI::GL_syntax.s_root);
    tCreated="unknown";
    createdBy="unknown";
    tModified="unknown";
    modifiedBy="unknown";
    documentType="unknown";
    sysDataRequested="no111";
    myUserName=_uName;
    sysDataRequested=_sysReq;
    numberOfDifferentDevicesThatAccessedTheForm=-1;
    if(s==1){
      initText=meRM.getTextData();
      long pos=0;
      std::pair<std::string,int> allSD=SF::extract(initText,pos,APTI::GL_syntax.s_sysDataB,APTI::GL_syntax.s_sysDataE);
      pos=0;
      std::pair<std::string,int> allTD=SF::extract(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE);
      if(allSD.second==1){
        sysDataRaw=allSD.first;
        std::string pText=APTI::GL_syntax.s_notFound;
          std::string userMap_notNeededForRR;
        HSF::parametersFromString(allSD.first,tCreated,createdBy,tModified,modifiedBy,pText,documentType,userMap_notNeededForRR);
        if(pText!=APTI::GL_syntax.s_notFound){
          std::vector<std::string> rawPermisssionsRead= SF::stringToVector(pText,APTI::GL_syntax.s_individualPermissionB, APTI::GL_syntax.s_individualPermissionE,"_name_read_/name_");
          std::vector<std::string> rawPermisssionsWrite= SF::stringToVector(pText,APTI::GL_syntax.s_individualPermissionB, APTI::GL_syntax.s_individualPermissionE,"_name_write_/name_");
          std::vector<std::string> pReadV=PBD::getPermitsFromRaw_N(rawPermisssionsRead);
          std::vector<std::string> pWriteV=PBD::getPermitsFromRaw_N(rawPermisssionsWrite);
          long sz=pReadV.size();
          for(long i=0;i<sz;++i){
            permitRead.insert(pReadV[i]);
          }
          sz=pWriteV.size();
          for(long i=0;i<sz;++i){
            permitWrite.insert(pWriteV[i]);
          }
        }
      }
      tName=meRM.getTextName();
      tExternalId=meRM.getExternalCodeFromInternalNumber();
      if(allTD.second==1){
        rawText=allTD.first;
      }
      ind_initSuccess=1;
      return 1;
    }
    return 0;
  }
  std::string Response::closestLegalRequest(const PSDI::SessionData &_psd, const std::string &_req) const{
    //req can be one of
    // df - default (form will tell us what it wants to do)
    // aw - answer mode (answer the questions)
    // sl - solutions mode (display solutions)
    // st - status mode (administrator can see the progress of the response)
    // rw - raw text mode (administrator can see the raw text and debug the form)
    // pm - print mode (pdf of the form is created)
  //WARNING: the function should be improved.
  // At the moment only administrators can have any requests
  // Everyone else can only have "df" request
    if(_psd.isRoot=="yes"){
      return _req;
    }
    return "df";
  }
  std::string Response::displayRespRec(const PSDI::SessionData &_psd){
    std::string req=_psd.respRecMode;
    std::string backButton="";
    if( (req=="gr")|| (req=="ag")){
      std::map<std::string,std::string>::const_iterator it,itE;
      itE=_psd.respMap.end();
      it=_psd.respMap.find("fP");
      if(it!=itE){
        std::string uEV=HSF::urlEnvVarsFromForwardedParameters(_psd.respMap,LI::GL_LN.st_envVName_Variable, LI::GL_LN.st_envVName_Value, LI::GL_LN.st_envVName_ForwardVariable, LI::GL_LN.st_envVName_ForwardValue, 10);
        std::string url="index.cgi?"+uEV;
        std::string webText=MWII::GL_WI.getDefaultWebText("automaticGradingComplete");
        if(webText==""){
          webText="_insert__n*_buttonLink_/n*__n*_Back to Gr. Room_/n*__n*_*BLREPL*_/n*__/insert_ ";
        }
        webText=SF::findAndReplace(webText,"*BLREPL*",url);
        backButton= treatInserts(_psd,webText,"_insert_","_/insert_");
      }
    }
    if(req=="gr"){
      std::string agResult=automaticGrading(_psd,req);
      if(backButton!=""){
        agResult=backButton;
      }
      return agResult;
    }
    if(req=="ag"){
      std::string agResult=automaticGradingOfAllTests(_psd,req);
      if(backButton!=""){
        agResult=backButton;
      }
      return agResult;
    }
    if((req.length()==6)&&(req[0]=='c')&&(req[1]=='h')){
      std::string changeStatusExam="",changeStatusGrading="";
      changeStatusExam+=req[2];
      changeStatusExam+=req[3];
      changeStatusGrading+=req[4];
      changeStatusGrading+=req[5];
      return changeResponseReceiverStatus(_psd,changeStatusExam,changeStatusGrading);
    }
    labelOnTheSubmitButton="Update and Save";
    instructionsOnTopOfTheForm="";
    timerCode="";
    messageUnavailableYet="<B>"+MWII::GL_WI.getDefaultWebText("The exam has not started yet. Please visit again later.")+"</B>";
    messageNoGradingYet="<B>" + MWII::GL_WI.getDefaultWebText("The exam cannot be graded now. Either the grading is over or the exam is not ready yet.")+"</B>";
    messageStartExam= MWII::GL_WI.getDefaultWebText("Click on the button below when you are ready.");
    req=closestLegalRequest(_psd,req);
    if(req=="rw"){
      return prepare(_psd,rawText);
    }
    if((_psd.pEditReq=="y")||(_psd.pEditReq=="r")){
      return prepare(_psd,rawText);
    }
    return prepareDefaultRequest(_psd,req);
  }
  std::pair<std::string,OfficialProblemData > Response::getFormQData(const std::string & _s) const{
    std::pair<std::string,OfficialProblemData >fR;
    OfficialProblemData secondComp;
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(_s,pos,APTI::GL_syntax.s_label_QRTB,APTI::GL_syntax.s_label_QRTE);
    if(allD.second==0){
      fR.first="notFound";
      fR.second=secondComp;
      return fR;
    }
    fR.first=allD.first;
    pos=0;allD=SF::extract(_s,pos,APTI::GL_syntax.s_tFormulation_QRTB,APTI::GL_syntax.s_tFormulation_QRTE);
    if(allD.second==1){
      secondComp.formulation=allD.first;
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_solution_QRTB, APTI::GL_syntax.s_solution_QRTE);
    if(allD.second==1){
      secondComp.solution=allD.first;
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_answer_QRTB, APTI::GL_syntax.s_answer_QRTE);
    if(allD.second==1){
      secondComp.answer=SF::answerToStandardForm(allD.first);
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_requestType_QRTB, APTI::GL_syntax.s_requestType_QRTE);
    if(allD.second==1){
      secondComp.displayType=allD.first;
    }
    else{
      pos=0;allD=SF::extract(_s,pos, APTI::GL_syntax.s_requestType_QRTB, APTI::GL_syntax.s_requestType_QRTE);
      if(allD.second==1){
        secondComp.displayType=allD.first;
      }
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_radioButtons_QRTB, APTI::GL_syntax.s_radioButtons_QRTE);
    if(allD.second==1){
      secondComp.choices=allD.first;
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_autograderInfo_QRTB, APTI::GL_syntax.s_autograderInfo_QRTE);
    if(allD.second==1){
      secondComp.autoGraderInfo=allD.first;
    }
    else{
      pos=0;allD=SF::extract(_s,pos, APTI::GL_syntax.s_autograderInfo_QRTB, APTI::GL_syntax.s_autograderInfo_QRTE);
      if(allD.second==1){
        secondComp.autoGraderInfo=allD.first;
      }
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_latexPrintingInstructions_QRTB, APTI::GL_syntax.s_latexPrintingInstructions_QRTE);
    if(allD.second==1){
      secondComp.latexPrintingInstructions=allD.first;
    }
    else{
      pos=0;allD=SF::extract(_s,pos, APTI::GL_syntax.s_latexPrintingInstructions_QRTB, APTI::GL_syntax.s_latexPrintingInstructions_QRTE);
      if(allD.second==1){
        secondComp.latexPrintingInstructions=allD.first;
      }
    }
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_points_QRTB, APTI::GL_syntax.s_points_QRTE);
    if(allD.second==1){
      secondComp.points=allD.first;
    }
    else{
      pos=0;allD=SF::extract(_s,pos, APTI::GL_syntax.s_points_QRTB, APTI::GL_syntax.s_points_QRTE);
      if(allD.second==1){
        secondComp.points=allD.first;
      }
    }
    allD=SF::extract(_s,pos,APTI::GL_syntax.s_gradingRules_QRTB,APTI::GL_syntax.s_gradingRules_QRTE);
    if(allD.second==1){
      secondComp.gRules=GRI::gradingRulesMapFromRaw(allD.first);
    }
    fR.second=secondComp;
    return fR;
  }
  std::pair<std::string,std::vector<std::string> >  Response::getUsrAVect(const std::string & _s) const{
    std::pair<std::string,std::vector<std::string> >fR;
    std::vector<std::string> secondComp;
    std::pair<std::string,int> allD;
    long pos=0;
    long ssz=4;
    secondComp.resize(ssz);
    allD=SF::extract(_s,pos,APTI::GL_syntax.s_label_QRTB,APTI::GL_syntax.s_label_QRTE);
    if(allD.second==0){
      fR.first="notFound";
      fR.second=secondComp;
      return fR;
    }
    fR.first=allD.first;
    for(long i=0;i<ssz;++i){secondComp[i]="notFound";}
    pos=0;
    allD=SF::extract(_s,pos,APTI::GL_syntax.s_solution_QRTB,APTI::GL_syntax.s_solution_QRTE);
    if(allD.second==1){
      secondComp[0]=allD.first;
    }
    pos=0;
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_answer_QRTB, APTI::GL_syntax.s_answer_QRTE);
    if(allD.second==1){
      secondComp[1]=allD.first;
    }
    pos=0;
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_points_QRTB, APTI::GL_syntax.s_points_QRTE);
    if(allD.second==1){
      secondComp[2]=allD.first;
    }
    pos=0;
    allD=SF::extract(_s,pos, APTI::GL_syntax.s_comment_ARTB, APTI::GL_syntax.s_comment_ARTE);
    if(allD.second==1){
      secondComp[3]=allD.first;
    }
    fR.second=secondComp;
    return fR;
  }
  std::vector<long> Response::getStatusVector(const PSDI::SessionData &_psd, const std::string &_req, const std::string &statusOfTheForm) const{
    long acceptResponses=0;
    long displayOfficialSolutions=0;
    long display_Points=0;
    long display_Comments=0;
    long displayQuestions=0;
    if(_req=="df"){
      if((statusOfTheForm=="awd")||(statusOfTheForm=="a")||(statusOfTheForm=="accept")){
        acceptResponses=1;
        displayQuestions=1;
      }
      else{
        if((statusOfTheForm=="s")||(statusOfTheForm=="cs")||(statusOfTheForm=="sc")||(statusOfTheForm=="sp")||(statusOfTheForm=="ps")){
          displayOfficialSolutions=1;
          displayQuestions=1;
        }
        if((statusOfTheForm=="c")||(statusOfTheForm=="cs")||(statusOfTheForm=="sc")){
          display_Comments=1;display_Points=1;
          displayQuestions=1;
        }
        if((statusOfTheForm=="p")||(statusOfTheForm=="ps")||(statusOfTheForm=="sp")){
          display_Points=1;
          displayQuestions=1;
        }
      }
    }
    if(_req=="aw"){
      if((statusOfTheForm=="awd")||(statusOfTheForm=="a")||(statusOfTheForm=="accept")){
        acceptResponses=1;
        displayQuestions=1;
      }
      if(_psd.isRoot=="yes"){
        acceptResponses=1;
        displayQuestions=1;
      }
    }
    if(_req=="sl"){
      if((statusOfTheForm=="s")||(statusOfTheForm=="cs")||(statusOfTheForm=="sc")){
        displayOfficialSolutions=1;
        displayQuestions=1;
      }
      if(_psd.isRoot=="yes"){
        displayOfficialSolutions=1;
        displayQuestions=1;
      }
    }
    if(_req=="st"){
      if((statusOfTheForm=="c")||(statusOfTheForm=="cs")||(statusOfTheForm=="sc")){
        display_Points=1;display_Comments=1;
        displayQuestions=1;
      }
      if(_psd.isRoot=="yes"){
        display_Points=1;display_Comments=1;
        displayQuestions=1;
      }
    }
    if(_req=="pm"){
      displayQuestions=1;
    }
    if(statusOfTheForm=="q"){
      displayQuestions=1;
    }
    std::vector<long> fR;
    fR.resize(5);
    fR[0]=acceptResponses;fR[1]=displayOfficialSolutions;fR[2]=display_Points;
    fR[3]=display_Comments;fR[4]=displayQuestions;
    return fR;
  }
  std::string Response::fileForProblemInfo(const long & fileCounter) const{
    std::string fR="";
    if(fileCounter< numFilesAllowed){
      std::vector<std::string> fileURL_And_Code=SF::stringToVector(mainFileInfoVector[fileCounter],"_n*|_","_/n*|_");
      if(fileURL_And_Code.size()>0){
        fR+=MWII::GL_WI.getDefaultWebText("File submitted")+". <a target=\"_new\" href=\""+ fileURL_And_Code[0]+"\">";
        fR+=MWII::GL_WI.getDefaultWebText("Download")+". </a>";
      }
    }
    return fR;
  }
  std::string Response::fileFieldForProblemInputForm(const std::string & _textBefore, const long &fileCounter) const{
    std::string fR="";
    if(fileCounter>=numFilesAllowed){
      return "";
    }
    std::string fileInfo=fileForProblemInfo(fileCounter);
    if(fileInfo!=""){
      fileInfo+="<BR>"+MWII::GL_WI.getDefaultWebText("You may replace it with another file if you wish");
    }
    else{
      fileInfo=MWII::GL_WI.getDefaultWebText("No file submitted yet");
    }
    fR+="_insert__n*_fileRequestField_/n*__n*_"+e_formNameRT+"_/n*_\n_n*_";
    fR+=fileLabelForHTML(fileCounter)+"_/n*__n*_";
    fR+=_textBefore+"<BR>"+fileInfo+"_/n*__/insert_";
    return fR;
  }
  std::string prepareUserAnswerForDisplayIfCodeIsNotWrittenNicely(const SingleQuestionInfo &sqi){
    std::string answerPrepared=sqi.userAnswer;
    if((sqi.autoGraderCodeData.language=="cpp")||(sqi.autoGraderCodeData.language=="py")){
      answerPrepared=HSF::codeDisplayForNonAdvanceUsers(answerPrepared);
    }
    return answerPrepared;
  }
  std::string divClassCardTag(const std::string& att,const std::string& text){
    return "<div class=\""+att+"\">"+text+"</div>\n";
  }
  std::string cardWithHeaderAndBody(const std::string& headerText, const std::string& bodyText, const std::string & headerAtt, const std::string& bodyAtt){
    std::string res;
    if(headerText!=""){
      res+=divClassCardTag(headerAtt,headerText);
    }
    if(bodyText!=""){
      res+=divClassCardTag(bodyAtt,bodyText);
    }
    return res;
  }
  std::string Response::userAnswerDisplayInnerCard(const SingleQuestionInfo &sqi, long &fileCounter) const{
    std::string answHeader="<b>"+MWII::GL_WI.getDefaultWebText("Submission")+"</b>";
    std::string displUserAnswer=MWII::GL_WI.getDefaultWebText("(No submission)");
    if((sqi.userAnswer!="notFound")&&(sqi.userAnswer!="")){
      displUserAnswer=prepareUserAnswerForDisplayIfCodeIsNotWrittenNicely(sqi);
      if(sqi.displType==APTI::GL_syntax.s_textInputReqField){
        displUserAnswer=DISPPF::prepareForHTMLDisplayTextInputField(displUserAnswer);
      }
      else{
        if(sqi.displType==APTI::GL_syntax.s_textAreaReqField){
          displUserAnswer=DISPPF::prepareForHTMLDisplay(displUserAnswer);
        }
        else{
          displUserAnswer=DISPPF::prepareForHTMLDisplay(sqi.userAnswer);
        }
      }
    }
    if(sqi.fileAllowed=="yes"){
      displUserAnswer+="<p>";
      displUserAnswer+=fileForProblemInfo(fileCounter);
      displUserAnswer+="</p>\n";
      ++fileCounter;
    }
    return cardWithHeaderAndBody(answHeader,displUserAnswer, "card-header", "card-body");
  }
  std::string Response::userAnswerDisplay(const SingleQuestionInfo &sqi, long &fileCounter, const int& squeezeToLeftHalf) const{
    std::string fR="";
    if(squeezeToLeftHalf){fR+="<div class=\"col-md-6\" >\n";}
    fR+="<div class=\"card text-dark\">\n<div class=\"card-body\">";
      if((sqi.userAnswer!="notFound")&&(sqi.userAnswer!="")){
        fR+="<B>"+MWII::GL_WI.getDefaultWebText("Answer submitted")+": </B> ";
        std::string displUserAnswer=prepareUserAnswerForDisplayIfCodeIsNotWrittenNicely(sqi);
        if(sqi.displType==APTI::GL_syntax.s_textInputReqField){
          fR+=DISPPF::prepareForHTMLDisplayTextInputField(displUserAnswer);
        }
        else{
          if(sqi.displType==APTI::GL_syntax.s_textAreaReqField){
            fR+="<BR>"+DISPPF::prepareForHTMLDisplay(displUserAnswer);
          }
          else{
            fR+=DISPPF::prepareForHTMLDisplay(sqi.userAnswer);
          }
        }
      }
      else{
        fR+="<B>"+MWII::GL_WI.getDefaultWebText("(No submission)")+"</B>";
      }
      fR+="\n</div></div>\n";
      if(sqi.fileAllowed=="yes"){
        fR+="<div class=\"card\">\n<div class=\"card-body\">";
        fR+=fileForProblemInfo(fileCounter);
        fR+="</div>\n</div>\n";
        ++fileCounter;
      }
      if(squeezeToLeftHalf){fR+="</div>\n";}
      return fR;
  }
  std::string problemCardOpeningTags(const SingleQuestionInfo &sqi, const std::string & lastComponent, const int & indicatorRegularProblem, const std::string & pointsSt){
    std::string fR="";
    fR+=" <div class=\"card border-dark text-dark\">\n <div class=\"card-header\">\n";
    fR+="<div class=\"row\"><div class=\"col-sm-1\"><B>";
    if(indicatorRegularProblem){fR+=std::to_string(sqi.num);}
    fR+="</B></div><div class=\"col-sm-8\">";
    if((indicatorRegularProblem)&&(sqi.maxPoints!="notFound")&&(sqi.maxPoints!="")){
      std::string bgColor="bg-dark";
      std::string ptsBegin="";
      if(pointsSt!=""){
        bgColor="bg-primary";
        ptsBegin=pointsSt+"/";
      }
      fR+="<span class=\"badge badge-pill "+bgColor+"\">";
      fR+=ptsBegin+sqi.maxPoints+" "+MWII::GL_WI.getDefaultWebText("points");
      fR+="</span>\n";
    }
    fR+="</div><div class=\"col-sm-3\"> ";
    fR+=lastComponent;
    fR+=" </div>\n</div>\n</div><div class=\"card-body\"> "+sqi.formulation;
    return fR;
  }
  std::string submissionOpeningTags(const SingleQuestionInfo &sqi, const std::string & firstComponent, const std::string &lastComponent){
    std::string fR="";
    fR+="<div class=\"card border-info text-dark\">\n<div class=\"card-header\">";
    fR+="<div class=\"row\"><div class=\"col-sm-10\"> <B>"+firstComponent+" </B></div>";
    fR+="<div class=\"col-sm-2\">"+lastComponent+"</div></div></div>\n <div class=\"card-body\">";
    return fR;
  }
  std::string Response::singleProblemDisplay(const SingleQuestionInfo &sqi, ProblemCommentsAndScores & pcas, long &fileCounter, double &pointsEarned) const{
    pointsEarned=-999.99;
    if(sqi.displType=="certificate"){return "certificate";}
    std::string fR="";
    std::string answerSubmitted="no";
    std::string barBgColor="bg-warning";
    std::string barTextColor="text-dark";
    std::string answSubmittedForTopBar;
    pcas.completionProgress[sqi.num]="n";
    pcas.scoringProgress[sqi.num]="-";
    if((sqi.userAnswer!="notFound")&&(sqi.userAnswer!="")&&(sqi.userAnswer!="N")) {
      answerSubmitted="yes";
      barBgColor="bg-secondary";
      barTextColor="text-warning";
      pcas.completionProgress[sqi.num]="y";
    }
    if(sqi.displayQs==0){
      return fR;
    }
    if(sqi.acceptResp==1){
      std::string diffText1_Label="textInputField";
      std::string diffText2_Label="_/n*__n*_15_/n*_\n_/insert_\n";
      answSubmittedForTopBar="<B>"+MWII::GL_WI.getDefaultWebText("Submitted")+" </B>\n ";
      std::string linkCodeAutograderTest="";
      if(APTI::GL_studentsAllowedToExecuteCodeOnPublicTestCases=="yes"){
        if(sqi.autoGraderCodeData.publicTestCases.size()>0){
          linkCodeAutograderTest+="<a href=\"?page=cts&ev0=rrc&el0=";
          linkCodeAutograderTest+=tName+"&ev1=nrr&el1="+sqi.QNum+"\"><b><button type=\"button\" ";
          linkCodeAutograderTest+=" class=\"btn btn-outline-dark btn-sm\">Check</button></b></a>\n";
        }
      }
      if(sqi.displType==APTI::GL_syntax.s_textAreaReqField){
        diffText1_Label="textAreaField";
        diffText2_Label="_/n*__n*_10_/n*__n*_90_/n*_\n_/insert_\n";
      }
      if(sqi.displType==APTI::GL_syntax.s_radioButtonsField){
        diffText1_Label="radioButtonsField";
        answSubmittedForTopBar="<B>"+MWII::GL_WI.getDefaultWebText("Current submission")+": ";
        answSubmittedForTopBar+=sqi.userAnswer+"</B>\n";
      }
      fR+="_insert__n*_"+diffText1_Label+"_/n*__n*_"+e_formNameRT+"_/n*_\n_n*_";
      fR+=sqi.QNum+"_/n*__n*_";
      std::string answBarLastComponent="<span class=\"text-danger-aa\"><B>"+MWII::GL_WI.getDefaultWebText("No answer yet")+"</B></span>";
      if(answerSubmitted=="yes"){
        answBarLastComponent="<span class=\"text-success-aa\">"+answSubmittedForTopBar+"</span>";
      }
      fR+=problemCardOpeningTags(sqi,answBarLastComponent,1,"");
      fR+="</div>\n</div>";
      if((sqi.displType==APTI::GL_syntax.s_textInputReqField)||(sqi.displType==APTI::GL_syntax.s_textAreaReqField)){
         if((sqi.userAnswer!="notFound")&&(sqi.userAnswer!="")){
           fR+=submissionOpeningTags(sqi,MWII::GL_WI.getDefaultWebText("Current submission"),linkCodeAutograderTest);
           std::string displUserAnswer=prepareUserAnswerForDisplayIfCodeIsNotWrittenNicely(sqi);
           if(sqi.displType==APTI::GL_syntax.s_textInputReqField){
             fR+=DISPPF::prepareForHTMLDisplayTextInputField(displUserAnswer);
           }
           else{
             fR+="<BR>"+DISPPF::prepareForHTMLDisplay(displUserAnswer);
           }
           fR+="</div></div> ";
        }
        fR+="_/n*__n*_";
        if(sqi.userAnswer!="notFound"){
          fR+=sqi.userAnswer;
        }
        fR+= diffText2_Label;
      }
      if(sqi.displType==APTI::GL_syntax.s_radioButtonsField){
        fR+="_/n*__n*_";
        fR+="_/n*__n*_"+sqi.allChoicesSt;
        fR+="_/n*__n*_"+sqi.userAnswer;
        fR+="_/n*__/insert_";
      }
      if(sqi.fileAllowed=="yes"){
         fR+=fileFieldForProblemInputForm("<B>"+MWII::GL_WI.getDefaultWebText("File for problem")+" "+std::to_string(sqi.num)+ "</B> ("+MWII::GL_WI.getDefaultWebText("optional")+")",fileCounter);
         ++fileCounter;
      }
    }
    else{
      std::string topBarScore="0";
      if(sqi.scoreForDisplay!=""){
        topBarScore=sqi.scoreForDisplay;
      }
      if(sqi.displayPoints==0){
        topBarScore="";
      }
      fR+=problemCardOpeningTags(sqi,"",1,topBarScore);
      if(sqi.displType==APTI::GL_syntax.s_radioButtonsField){
        std::vector<std::string> pairsOfValDescriptions=SF::stringToVector(sqi.allChoicesSt,"_rb*_","_/rb*_");
        std::vector<std::string> smallSt;
        std::string openB,closeB;
        long sz=pairsOfValDescriptions.size();
        fR+="<ul>";
        for(long i=0;i<sz;++i){
          smallSt=SF::stringToVector(pairsOfValDescriptions[i],"_vl*_","_/vl*_");
          if(smallSt.size()>1){
            if(smallSt[0]!=""){
              std::map<std::string,std::string> replMap;
              replMap["<B>"]="";
              replMap["</B>"]="";
              smallSt[1]=MFRF::findAndReplace(smallSt[1],replMap);
              openB="";closeB="";
              if(smallSt[0]==sqi.userAnswer){
                openB="<B>";closeB="</B>";
              }
              fR+="<li>"+openB+smallSt[1]+closeB;
              fR+="</li>";
            }
          }
        }
        fR+="</ul>\n";
      }
      fR+="</div>\n ";
      fR+="<div class=\"row\">\n";
      std::string usAnswDisplText="";
      usAnswDisplText=userAnswerDisplayInnerCard(sqi,fileCounter);
      std::string commentAddition="";
      if((sqi.displayComments==1)&&(sqi.graderComment!="notFound")&&(sqi.grCommentDisplay!="")){
        commentAddition+=cardWithHeaderAndBody("<font color=\"purple\"><b>"
                                                +MWII::GL_WI.getDefaultWebText("Grading feedback")
                                                +"</b></font>",
                                               "<font color=\"purple\">"+sqi.grCommentDisplay+"</font>",
                                               "card-header",
                                               "card-body");
      }
      fR+="<div class=\"card-group\" >\n";
      fR+="<div class=\"card\">\n"+usAnswDisplText+commentAddition+"</div>\n";
      if(sqi.displaySol==1){
        std::string headerText="<b>"+MWII::GL_WI.getDefaultWebText("Solution")+"</b>";
        std::string officialSolutionText;
        if((sqi.officialAnswer!="notFound")&&(sqi.officialAnswer!="")&&(sqi.officialAnswer!=GF::GL_officialNA)){
          headerText="<b>"+MWII::GL_WI.getDefaultWebText("Correct answer")+": "+sqi.officialAnswer+"</b>";
        }
        if((sqi.officialSolution!="notFound")&&(sqi.officialSolution!="")){
          officialSolutionText=sqi.officialSolution;
        }
        fR+="<div class=\"card\">\n";
        fR+=cardWithHeaderAndBody(headerText,officialSolutionText,"card-header","card-body");
        fR+="</div>";
      }
      if((sqi.displayPoints==1)||(sqi.displayComments==1)){
        if((sqi.graderComment!="notFound")&&(sqi.graderComment!="")){
          pointsEarned=sqi.scoreForCalculations;
          if(sqi.displayPoints==1){
            pcas.scoringProgress[sqi.num]=sqi.scoreForDisplay;
          }
        }
      }
      fR+="</div></div></div>\n<p></p>\n";
    }
    return fR;
  }
  std::string displayGradingRulesToGrader(const std::map<std::string,PSDI::GradingRule>& m){
    std::string res,resM,resRaw;
    std::map<std::string,PSDI::GradingRule>::const_iterator it,itE;
    it=m.begin(); itE=m.end();
    while(it!=itE){
      resM+="<tr>";
      resM+="<td>"+it->first+"</td><td>";
      resM+=BF::doubleToString((it->second).points);
      resM+="</td><td>";
      resM+=(it->second).display;
      resM+="</td></tr>\n";
      resRaw+="\n\n"+MWII::GL_WI.getDefaultWebText("[addRule]")+it->first+";\n";
      resRaw+=BF::doubleToString((it->second).points)+";";
      if((it->second).display!=""){
        resRaw+="\n"+(it->second).display;
      }
      resRaw+="\n"+MWII::GL_WI.getDefaultWebText("[/addRule]");
      ++it;
    }
    if(resM!=""){
      std::string oldRes=res;
      res="<div class=\"card\">\n<div class=\"card-body\">";
      res+="<table class=\"table table-striped\" aria-label=\"progress\"><thead class=\"table-secondary\" aria-label=\"list of grading rules\"><tr>";
      res+="<th scope=\"col\">Name</th><th scope=\"col\">Points</th><th scope=\"col\">Display</th></tr></thead>\n<tbody>\n";
      res+=resM;
      res+="\n</tbody></table><br>";
      res+="_hideReveal__revealTitle_Show source codes for rules_/revealTitle__hideTitle_Hide source codes for rules_/hideTitle_<pre>";
      res+=resRaw;
      res+="</pre>_/hideReveal_\n";
      res+="</div></div>\n";
    }
    return res;
  }
  std::pair<std::string,int> Response::singleProblemDisplayForGrader(const SingleQuestionInfo &sqi, ProblemCommentsAndScores & pcas,long &fileCounter,double &pointsEarned) const{
    pointsEarned=-999.99;
    std::pair<std::string,int> fR;
    fR.second=1;
    std::string certComment;
    if(sqi.displType=="certificate"){
      fR.second=0;
      std::map<std::string,std::string> replMap;
      replMap["_comment_"]="";
      replMap["_/comment_"]="";
      certComment=MFRF::findAndReplace(sqi.graderComment,replMap);
    }
    std::string answerSubmitted="no";
    std::string barBgColor="bg-secondary";
    std::string barTextColor="text-warning";
    std::string pointsEarnedForDisplay="";
    if(fR.second){
      pcas.completionProgress[sqi.num]="n";
      pcas.scoringProgress[sqi.num]="-";
      if((sqi.graderComment!="notFound")&&(sqi.graderComment!="")){
        pointsEarned=sqi.scoreForCalculations;
        pcas.scoringProgress[sqi.num]=sqi.scoreForDisplay;
        pointsEarnedForDisplay="<B>"+MWII::GL_WI.getDefaultWebText("Score")+": "+pcas.scoringProgress[sqi.num]+"</B>";
      }
    }
    if(sqi.acceptResp==1){
      fR.first+="_insert__n*_textAreaField_/n*__n*_"+e_formNameRT+"_/n*_\n_n*_";
      fR.first+=sqi.QNum+"_/n*__n*_"; ;
      fR.first+=problemCardOpeningTags(sqi,pointsEarnedForDisplay,fR.second,"");
      if(sqi.displType==APTI::GL_syntax.s_radioButtonsField){
        std::vector<std::string> pairsOfValDescriptions=SF::stringToVector(sqi.allChoicesSt,"_rb*_","_/rb*_");
        std::vector<std::string> smallSt;
        std::string openB,closeB;
        long sz=pairsOfValDescriptions.size();
        for(long i=0;i<sz;++i){
          smallSt=SF::stringToVector(pairsOfValDescriptions[i],"_vl*_","_/vl*_");
          if(smallSt.size()>1){
            if(smallSt[0]!=""){
              std::map<std::string,std::string> replMap;
              replMap["<B>"]="";
              replMap["</B>"]="";
              smallSt[1]=MFRF::findAndReplace(smallSt[1],replMap);
              openB="";closeB="";
              if(smallSt[0]==sqi.userAnswer){
                openB="<B>";closeB="</B>";
              }
              fR.first+="<DIV>"+openB+smallSt[1]+ closeB+"</DIV>";
            }
          }
        }
      }
      fR.first+="</div>\n</div>";
      if(fR.second){
        if(sqi.displaySol){
          fR.first+="<div class=\"row\">\n";
        }
        fR.first+=userAnswerDisplay(sqi,fileCounter,sqi.displaySol);
      }
      if(sqi.displaySol==1){
        fR.first+="<div class=\"col-md-6\" >\n";
        fR.first+="<div class=\"card\">\n<div class=\"card-body\">";
        if((sqi.officialAnswer!="notFound")&&(sqi.officialAnswer!="")&&(fR.second)){
          fR.first+="<B>"+MWII::GL_WI.getDefaultWebText("Correct answer")+": "+sqi.officialAnswer+"</B>";
        }
        if((sqi.officialSolution!="notFound")&&(sqi.officialSolution!="")){
          if(fR.second){
            fR.first+="<BR><B>";
            fR.first+=MWII::GL_WI.getDefaultWebText("Solution")+":";
            fR.first+="</B> "+sqi.officialSolution;
          }
          else{
            fR.first+="<BR> "+CERTCLI::displayCertificateOptions(sqi.officialSolution);
          }
        }
        fR.first+="\n</div></div>\n";
        fR.first+="</div>\n";
        if(fR.second){fR.first+="</div>\n";}
      }
      if((sqi.displayPoints==1)||(sqi.displayComments==1)){
        if(sqi.gRulesMap.size()>0){
          fR.first+="<div class=\"row\">\n<div class=\"col-md-6\" >\n";
        }
        fR.first+="<div class=\"card\">\n<div class=\"card-body\">";
        fR.first+="<FONT COLOR=\""+st_gradingColor+"\">";
        if((fR.second)&&(sqi.userPointsEarned!="notFound")&&(sqi.userPointsEarned!="")){
          fR.first+="<B>"+MWII::GL_WI.getDefaultWebText("Score")+": "+sqi.userPointsEarned+" "+MWII::GL_WI.getDefaultWebText("points")+".</B>";
        }
        if((sqi.graderComment!="notFound")&&(sqi.graderComment!="")){
          if(fR.second){fR.first+=pointsEarnedForDisplay;}
          if((pointsEarned>-999.99)||(fR.second==0)){
            fR.first+="<BR>";
            if(MWII::GL_WI.getDefaultWebText("Comment")!="No"){
              fR.first+="<B>";
              if(fR.second){
                fR.first+=MWII::GL_WI.getDefaultWebText("Comment");
              }
              else{
                fR.first+=MWII::GL_WI.getDefaultWebText("Certificate");
              }
              fR.first+=":</B> ";
            }
            if(fR.second){
              fR.first+=sqi.grCommentDisplay;
            }
            else{
              fR.first+=certComment;
            }
          }
        }
        fR.first+="\n</font>\n</div></div>\n";
        if(sqi.gRulesMap.size()>0){
          fR.first+="</div>\n";
          fR.first+="<div class=\"col-md-6\" >\n";
          fR.first+=displayGradingRulesToGrader(sqi.gRulesMap);
          fR.first+="</div>\n</div>\n";
        }
      }
      fR.first+="\n<p></p>\n";
      fR.first+="_/n*__n*_";
      if(fR.second){
        fR.first+=sqi.rawComment;
      }
      else{
        fR.first+=certComment;
      }
      fR.first+= "_/n*__n*_10_/n*__n*_90_/n*_\n_/insert_\n";
      if(fR.second){
        fR.first+="_insert__n*_textInputField_/n*__n*_"+e_formNameRT+"_/n*_\n_n*_";
        fR.first+="P"+sqi.QNum+"_/n*__n*_";
        fR.first+="<B>"+MWII::GL_WI.getDefaultWebText("Points")+":</B> ";
        fR.first+="_/n*__n*_";
        if(sqi.rawScore!="notFound"){
          fR.first+=sqi.rawScore;
        }
        fR.first+= "_/n*__n*_15_/n*_\n_/insert_\n";
      }
    }
    return fR;
  }
  std::vector<LocationOfDocuments> Response::getLocations(const PSDI::SessionData & _psd) const{
    std::vector<LocationOfDocuments> fR;
    std::vector<std::string> vStudents=SF::stringToVector(rawText,"_st*|_","_/st*|_");
    long sz=vStudents.size();
    if(sz>0){
      fR.resize(sz);
      for(long i=0;i<sz;++i){
        (fR[i]).getFromString(vStudents[i]);
      }
    }
    return fR;
  }
  int includeInGradersCommentsMap(const std::string& qLabel, const int & includeCertificateIndicator){
    if(includeCertificateIndicator==1){
      return 1;
    }
    if(qLabel.size()<2){
      return 1;
    }
    if((qLabel[0]=='Q')&&(qLabel[1]=='C')){
      return 0;
    }
    return 1;
  }
  ResponderInfo Response::infoFromResponseText(const PSDI::SessionData & _psd,  const std::string & _req, const int & optimizationParameter, const int & includeCertificateInGradersCommentsMap){
      ResponderInfo rInf;
      rInf.docName=tName;
      rInf.acceptResp=0;
      rInf.acceptGrade=0;
      rInf.displaySol=0;
      rInf.displayPoints=0;
      rInf.displayComments=0;
      rInf.displayQs=0;
      rInf.exitStatus="badForm";
      rInf.prefixForExamPDF="ef_";
      rInf.addNameIndicator="no";
      rInf.acceptGrade=0;
      rInf.commAllowedTime="notFound";
      rInf.commAbsoluteEnd="notFound";
      rInf.commTimerVersion=0;
      rInf.indEndingTime="notFound";
      rInf.indTimerVersion=0;
      rInf.userCanTakeActionAndActivateExam=0;
      rInf.potentialEndingTimeIfExamStartsNow="notFound";
      rInf.actRequest=0;
      rInf.remainingSeconds=-1;
      rInf.roundingTimeSec=30;
      rInf.certificateDescriptionCode="notFound";
      rInf.certificateIdCode="notFound";
      long pos;
      std::pair<std::string,int> allD;
      pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_iEnd_QRTB,APTI::GL_syntax.s_iEnd_QRTE);
      if(allD.second==1){
        rInf.indEndingTime=allD.first;
      }
      pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_iTVersion_QRTB,APTI::GL_syntax.s_iTVersion_QRTE);
      if(allD.second==1){
        long tmpLong=BF::stringToInteger(allD.first);
        if(tmpLong>-1){
          rInf.indTimerVersion=tmpLong;
        }
      }
      pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_statusOfTheForm_ARTB,APTI::GL_syntax.s_statusOfTheForm_ARTE);
      rInf.statusOfTheForm="nyo";
      if(allD.second==1){
        rInf.statusOfTheForm=allD.first;
      }
      rInf.gradingStatus="nyo";
      pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_statusOfTheGrading_ARTB,APTI::GL_syntax.s_statusOfTheGrading_ARTE);
      if(allD.second==1){
        rInf.gradingStatus=allD.first;
      }
      rInf.documentType=st_responseToTest;
      rawTextOfResponseVisibleToGrader="";
      pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_respLinkInGrade_QRTB,APTI::GL_syntax.s_respLinkInGrade_QRTE);
      std::string responseDocument;
      if(allD.second==1){
        rInf.documentType=st_gradeOfResponse;
        indRespDocForGraderCorrectlySet=responseDocumentForGrader.setFromTextName(allD.first);
        rawTextOfResponseVisibleToGrader=responseDocumentForGrader.getTextData();
      }
      std::pair<std::string,OfficialProblemData > tmpForm;
      pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_formulRespRec_ARTB,APTI::GL_syntax.s_formulRespRec_ARTE);
      if(allD.second==0){
        std::vector<std::string> questions=SF::stringToVector(rawText,APTI::GL_syntax.s_nextQ_QRTB,APTI::GL_syntax.s_nextQ_QRTE);
        long qSz=questions.size();
        for(long i=0;i<qSz;++i){
          tmpForm=getFormQData(questions[i]);
          if(tmpForm.first!="notFound"){
            (rInf.maxPoints)[tmpForm.first]=(tmpForm.second).points;
            if(((tmpForm.second).gRules).size()>0){
              (rInf.gradingRules)[tmpForm.first]=(tmpForm.second).gRules;
            }
          }
        }
        return rInf;
      }
      rInf.formulationDocument=allD.first;
      std::string masterStatusDoc="";
      std::string masterStatus="notFound";
      std::string masterStatusRawData="";
      std::string locationOfCertificateInfo=rawText;
      if(optimizationParameter==0){
        pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_masterStatus_ARTB,APTI::GL_syntax.s_masterStatus_ARTE);
        if(allD.second==1){
          masterStatusDoc=allD.first;
          RMD::Response mSD;
          int msdi=mSD.setFromTextName(masterStatusDoc);
          if(msdi==0){
            return rInf;
          }
          masterStatusRawData=mSD.getTextData();
          pos=0;allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_statusOfTheForm_ARTB,APTI::GL_syntax.s_statusOfTheForm_ARTE);
          if(allD.second==0){
            return rInf;
          }
          masterStatus=allD.first;
          pos=0;allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_statusOfTheGrading_ARTB,APTI::GL_syntax.s_statusOfTheGrading_ARTE);
          if(allD.second==1){
            rInf.gradingStatus=allD.first;
          }
          pos=0; allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_buttonLabel_QRTB,APTI::GL_syntax.s_buttonLabel_QRTE);
          if(allD.second==1){
            labelOnTheSubmitButton=allD.first;
          }
          pos=0; allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_importantInfo_QRTB,APTI::GL_syntax.s_importantInfo_QRTE);
          if(allD.second==1){
            instructionsOnTopOfTheForm=allD.first;
          }
          pos=0; allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_notStartedYet_QRTB,APTI::GL_syntax.s_notStartedYet_QRTE);
          if(allD.second==1){
            messageUnavailableYet=allD.first;
          }
          pos=0; allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_notReadyForGrading_QRTB,APTI::GL_syntax.s_notReadyForGrading_QRTE);
          if(allD.second==1){
            messageNoGradingYet=allD.first;
          }
          pos=0; allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_needsActivation_QRTB,APTI::GL_syntax.s_needsActivation_QRTE);
          if(allD.second==1){
            messageStartExam=allD.first;
          }
          rInf.statusOfTheForm=masterStatus;
          pos=0;allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_cAllowedTime_QRTB,APTI::GL_syntax.s_cAllowedTime_QRTE);
          if(allD.second==1){
            rInf.commAllowedTime=allD.first;
          }
          pos=0;allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_cAbsEnd_QRTB,APTI::GL_syntax.s_cAbsEnd_QRTE);
          if(allD.second==1){
            rInf.commAbsoluteEnd=allD.first;
          }
          pos=0;allD=SF::extract(masterStatusRawData,pos,APTI::GL_syntax.s_cTVersion_QRTB,APTI::GL_syntax.s_cTVersion_QRTE);
          if(allD.second==1){
            long tmpLong=BF::stringToInteger(allD.first);
            if(tmpLong>-1){
              rInf.commTimerVersion=tmpLong;
            }
          }
        }
        if(GL_FORM_POSSIBLE_STATUS.checkForLegalStatus(rInf.statusOfTheForm)=="no"){
          rInf.statusOfTheForm="nyo";
        }
        if(GL_FORM_POSSIBLE_STATUS.checkForLegalStatus(rInf.gradingStatus)=="no"){
          rInf.gradingStatus="nyo";
        }
      }
      std::string formulationDocumentData;
      if(optimizationParameter==0){
        if(rInf.formulationDocument!=masterStatusDoc){
          RMD::Response rF;
          int fde=rF.setFromTextName(rInf.formulationDocument);
          if(fde==0){
            return rInf;
          }
          formulationDocumentData=rF.getTextData();
          if(((_psd.addModifyRulesCommands).size()>0)&&(PSDI::GL_alreadyModifiedGradingRules==0)){
            PSDI::GL_alreadyModifiedGradingRules=1;
            formulationDocumentData=GRI::updateGradingRules(formulationDocumentData,_psd.addModifyRulesCommands);
            rF.setTextData(formulationDocumentData);
            rF.putInDB();
          }
        }
        else{
          formulationDocumentData=masterStatusRawData;
        }
      }
      if((rInf.documentType==st_responseToTest)&&((rInf.statusOfTheForm=="sc")||(rInf.statusOfTheForm=="cs")||(rInf.statusOfTheForm=="c")||   (rInf.statusOfTheForm=="sp")||(rInf.statusOfTheForm=="ps")||(rInf.statusOfTheForm=="p"))){
        pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_gradeLinkInResp_QRTB,APTI::GL_syntax.s_gradeLinkInResp_QRTE);
        if(allD.second==0){
          return rInf;
        }
        indGradingDocForRespCorrectlySet=gradingDocumentForReponse.setFromTextName(allD.first);
        if(indGradingDocForRespCorrectlySet==1){
          locationOfCertificateInfo=gradingDocumentForReponse.getTextData();
          std::vector<std::string> vecAllGCs=SF::stringToVector(gradingDocumentForReponse.getTextData(), APTI::GL_syntax.s_nextQ_QRTB, APTI::GL_syntax.s_nextQ_QRTE);
          long sz=vecAllGCs.size();
          std::pair<std::string,std::vector<std::string> > tmpGr;
          for(long i=0;i<sz;++i){
            tmpGr=getUsrAVect(vecAllGCs[i]);
            if(tmpGr.first!="notFound"){
              (rInf.gradersComments)[tmpGr.first]=(tmpGr.second)[3];
            }
          }
        }
      }
      if( ((rInf.documentType==st_gradeOfResponse)&&(rInf.gradingStatus=="a")) || (optimizationParameter==1)){
        rInf.acceptGrade=1;
        std::vector<std::string> vecAllGCs=SF::stringToVector(rawText, APTI::GL_syntax.s_nextQ_QRTB, APTI::GL_syntax.s_nextQ_QRTE);
        long sz=vecAllGCs.size();
        std::pair<std::string,std::vector<std::string> > tmpGr;
        for(long i=0;i<sz;++i){
          tmpGr=getUsrAVect(vecAllGCs[i]);
          if(tmpGr.first!="notFound"){
            if(includeInGradersCommentsMap(tmpGr.first,includeCertificateInGradersCommentsMap)){
              (rInf.gradersComments)[tmpGr.first]=(tmpGr.second)[3];
            }
          }
        }
      }
      if((rInf.documentType==st_gradeOfResponse)||(rInf.documentType==st_responseToTest)){
        pos=0; allD=SF::extract(locationOfCertificateInfo,pos,APTI::GL_syntax.s_certificateDescCodeB,APTI::GL_syntax.s_certificateDescCodeE);
        if(allD.second==1){
            rInf.certificateDescriptionCode=allD.first;
        }
        pos=0; allD=SF::extract(locationOfCertificateInfo,pos,APTI::GL_syntax.s_certificateIdCodeB,APTI::GL_syntax.s_certificateIdCodeE);
        if(allD.second==1){
            rInf.certificateIdCode=allD.first;
        }
      }
      rInf.idInfoData="notFound";
      rInf.solverUserName="notFound";
      if(rInf.documentType==st_responseToTest){
        pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_idInfo_ARTB,APTI::GL_syntax.s_idInfo_ARTE);
      }
      else{
        pos=0; allD=SF::extract(rawTextOfResponseVisibleToGrader,pos,APTI::GL_syntax.s_idInfo_ARTB,APTI::GL_syntax.s_idInfo_ARTE);
      }
      if(allD.second==1){
        rInf.idInfoData=allD.first;
      }
      if(rInf.documentType==st_responseToTest){
        pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_solverUName_ARTB,APTI::GL_syntax.s_solverUName_ARTE);
      }
      else{
        pos=0; allD=SF::extract(rawTextOfResponseVisibleToGrader,pos,APTI::GL_syntax.s_solverUName_ARTB,APTI::GL_syntax.s_solverUName_ARTE);
      }
      if(allD.second==1){
        rInf.solverUserName=allD.first;
      }
      rInf.fileInfoV.resize(0);
      numFilesAllowed=0;
      if(rInf.documentType==st_responseToTest){
        pos=0; allD=SF::extract(rawText,pos,APTI::GL_syntax.s_numFilesAllowed_ARTB,APTI::GL_syntax.s_numFilesAllowed_ARTE);
      }
      else{
        pos=0; allD=SF::extract(rawTextOfResponseVisibleToGrader,pos,APTI::GL_syntax.s_numFilesAllowed_ARTB,APTI::GL_syntax.s_numFilesAllowed_ARTE);
      }
      if(allD.second==1){
        numFilesAllowed=BF::stringToInteger(allD.first);
        if((numFilesAllowed<1)||(numFilesAllowed>100)){
          numFilesAllowed=0;
        }
      }
      if(numFilesAllowed>0){
        rInf.fileInfoV.resize(numFilesAllowed);
        for(long i=0;i<numFilesAllowed;++i){
          (rInf.fileInfoV)[i]="notFound";
        }
        for(long i=0;i<numFilesAllowed;++i){
          if(rInf.documentType==st_responseToTest){
            pos=0; allD=SF::extract(rawText,pos,indFileSepB(i),indFileSepE(i));
          }
          else{
            pos=0; allD=SF::extract(rawTextOfResponseVisibleToGrader,pos,indFileSepB(i),indFileSepE(i));
          }
          if(allD.second==1){
            (rInf.fileInfoV)[i]=allD.first;
          }
        }
      }
      if(optimizationParameter==0){
        pos=0; allD=SF::extract(formulationDocumentData,pos,"_pdfOptions*_","_/pdfOptions*_");
        if(allD.second==1){
          std::string pdfOptionsRaw=allD.first;
          pos=0;allD=SF::extract(pdfOptionsRaw,pos,"_pref*_","_/pref*_");
          if(allD.second==1){
            rInf.prefixForExamPDF=allD.first;
          }
          pos=0;allD=SF::extract(pdfOptionsRaw,pos,"_addName*_","_/addName*_");
          if(allD.second==1){
            rInf.addNameIndicator=allD.first;
          }
          pos=0;allD=SF::extract(pdfOptionsRaw,pos,"_pageTop*|_","_/pageTop*|_");
          if(allD.second==1){
            rInf.pdfTopOfThePage=allD.first;
          }
          pos=0;allD=SF::extract(pdfOptionsRaw,pos,"_newPageSeparators*|_","_/newPageSeparators*|_");
          if(allD.second==1){
            rInf.pdfNewPageSeparators=SF::stringToSet(allD.first,"_n*_","_/n*_");
          }
        }
      }
      std::vector<std::string> userAsV;
      if(rInf.documentType==st_responseToTest){
        userAsV=SF::stringToVector(rawText,APTI::GL_syntax.s_nextQ_QRTB,APTI::GL_syntax.s_nextQ_QRTE);
      }
      else{
        userAsV=SF::stringToVector(rawTextOfResponseVisibleToGrader,APTI::GL_syntax.s_nextQ_QRTB,APTI::GL_syntax.s_nextQ_QRTE);
      }
      long uaSz=userAsV.size();
      std::pair<std::string,std::vector<std::string> > tmpUsr;
      std::string qLabel,sep_version_B,sep_version_E,versionQ,problemDoc;
      if(optimizationParameter==0){
        std::vector<std::string> formQsV=SF::stringToVector(formulationDocumentData,APTI::GL_syntax.s_nextQ_QRTB,APTI::GL_syntax.s_nextQ_QRTE);
        long qSz=formQsV.size();
        for(long i=0;i<qSz;++i){
          pos=0;allD=SF::extract(formQsV[i],pos,APTI::GL_syntax.s_label_QRTB,APTI::GL_syntax.s_label_QRTE);
            versionQ="notFound";
            if(allD.second==1){
              qLabel=allD.first;
              (rInf.questionLabels).insert(qLabel);
              sep_version_B=APTI::GL_syntax.s_verSepBB+qLabel+APTI::GL_syntax.s_verSepE;
              sep_version_E=APTI::GL_syntax.s_verSepEB+qLabel+APTI::GL_syntax.s_verSepE;
              if(rInf.documentType==st_responseToTest){
                if(_psd.indChangeRespRecToPrintVersionOfCommonInClassExam==1){
                  pos=0;allD=SF::extract(_psd.probVersionsOfChangedRespRec,pos,sep_version_B,sep_version_E);
                }
                else{
                  pos=0;allD=SF::extract(rawText,pos,sep_version_B,sep_version_E);
                }
              }
              else{
                pos=0;allD=SF::extract(rawTextOfResponseVisibleToGrader,pos,sep_version_B,sep_version_E);
              }
              if(allD.second==1){
                versionQ=allD.first;
              }
              problemDoc= formQsV[i];
              rInf.questionVersions[qLabel]="0";
              if(versionQ!="notFound"){
                rInf.questionVersions[qLabel]=versionQ;
                problemDoc=TWDVF::prepareProblemForTest(problemDoc,BF::stringToInteger(versionQ));
              }
              rInf.completeProblemDocs[qLabel]=problemDoc;
              tmpForm=getFormQData(problemDoc);
              if(tmpForm.first!="notFound"){
                formRTQsMap[tmpForm.first]=tmpForm.second;
                (rInf.maxPoints)[qLabel]=(tmpForm.second).points;
                if(((tmpForm.second).gRules).size()>0){
                  (rInf.gradingRules)[qLabel]=(tmpForm.second).gRules;
                }
              }
            }
        }
      }
      for(long i=0;i<uaSz;++i){
          tmpUsr=getUsrAVect(userAsV[i]);
          if(tmpUsr.first!="notFound"){
            userRTAnswMap[tmpUsr.first]=tmpUsr.second;
            rInf.submittedAnswers[tmpUsr.first]=tmpUsr.second[1];
          }
      }
      std::map<std::string,std::string>::const_iterator itAct;
      itAct=_psd.respMap.find("act");
      if(itAct!=_psd.respMap.end()){
        if((itAct->second.length()>0)&&((itAct->second[0]=='y')||(itAct->second[0]=='Y'))){
          rInf.actRequest=1;
        }
      }
      std::vector<long> sv= getStatusVector(_psd,_req,rInf.statusOfTheForm);
      rInf.acceptResp=sv[0];
      rInf.displaySol=sv[1];
      rInf.displayPoints=sv[2];
      rInf.displayComments=sv[3];
      rInf.displayQs=sv[4];
      rInf= TFTI::analyzeTimerAndUpdateInfo(rInf);
      if(rInf.userCanTakeActionAndActivateExam==1){
        if(rInf.actRequest==0){
          rInf.acceptResp=0;rInf.displaySol=0;rInf.displayPoints=0;rInf.displayComments=0;rInf.displayQs=0;
        }
        else{
          rInf.acceptResp=1;rInf.displayQs=1;rInf.displaySol=0;rInf.displayPoints=0;rInf.displayComments=0;
          pos=0;allD=SF::extractAndReplace(rawText,pos,APTI::GL_syntax.s_iEnd_QRTB,APTI::GL_syntax.s_iEnd_QRTE,0,APTI::GL_syntax.s_iEnd_QRTB+rInf.potentialEndingTimeIfExamStartsNow+APTI::GL_syntax.s_iEnd_QRTE);
          if(allD.second==1){
            rawText=allD.first;
          }
          else{
            rawText = APTI::GL_syntax.s_iEnd_QRTB+rInf.potentialEndingTimeIfExamStartsNow+APTI::GL_syntax.s_iEnd_QRTE+rawText;
          }
          pos=0;allD=SF::extractAndReplace(rawText,pos,APTI::GL_syntax.s_iTVersion_QRTB,APTI::GL_syntax.s_iTVersion_QRTE,0,APTI::GL_syntax.s_iTVersion_QRTB+std::to_string(rInf.commTimerVersion)+APTI::GL_syntax.s_iTVersion_QRTE);
          if(allD.second==1){
            rawText=allD.first;
          }
          else{
            rawText = APTI::GL_syntax.s_iTVersion_QRTB+std::to_string(rInf.commTimerVersion)+APTI::GL_syntax.s_iTVersion_QRTE+rawText;
          }
          pos=0;allD=SF::extractAndReplace(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE,0,APTI::GL_syntax.s_tDataB+rawText+APTI::GL_syntax.s_tDataE);
          if( allD.second==1 ){
            initText=allD.first;
            meRM.setTextData(initText);
            meRM.putInDB();
          }
        }
      }
      rInf.exitStatus="ok";
      mainFileInfoVector=rInf.fileInfoV;
      myDocType=rInf.documentType;
      rInf.indicatorOfPOST= analyzeAccessAndReportSuspiciousBehavior(_psd);
      return rInf;
  }
  std::string ResponderInfo::debuggingPrint() const{
    std::string rifd="";
    rifd+="Document type: "+ documentType+"<BR>";
    rifd+="Grading status: "+ gradingStatus+"<BR>";
    rifd+="Exit status: "+ exitStatus+"<BR>";
    rifd+="ID Info Data: "+ idInfoData+"<BR>";
    rifd+="Solver username: "+ solverUserName+"<BR>";
    rifd+="Status of the form: "+ statusOfTheForm+"<BR>";
    rifd+="Formulation document: "+ formulationDocument+"<BR>";
    rifd+="Accept resp: "+std::to_string( acceptResp)+"<BR>";
    rifd+="Display sol: "+std::to_string( displaySol)+"<BR>";
    rifd+="Display Points: "+std::to_string( displayPoints)+"<BR>";
    rifd+="Display Comments: "+std::to_string( displayComments)+"<BR>";
    rifd+="Display Qs: "+std::to_string( displayQs)+"<BR>";
    long sz=( fileInfoV).size();
    rifd+="fileInfoV size: "+std::to_string(sz)+"<BR>";
    for(long i=0;i<sz;++i){
      rifd+=( fileInfoV)[i]+"<BR>";
    }
    rifd+="Common allowed time for all users: "+commAllowedTime+"<BR>";
    rifd+="Common absolute end for all users: "+commAbsoluteEnd+"<BR>";
    rifd+="Common timer version for all users: "+std::to_string(commTimerVersion)+"<BR>";
    rifd+="Individual ending time (if set): "+indEndingTime+"<BR>";
    rifd+="Individual timer version: "+std::to_string(indTimerVersion)+"<BR>";
    rifd+="User can take action and activate: "+std::to_string(userCanTakeActionAndActivateExam)+"<BR>";
    rifd+="Ending time if exam starts now: "+potentialEndingTimeIfExamStartsNow+"<BR>";
    rifd+="Activation requested: "+std::to_string(actRequest)+"<BR>";
    rifd+="Remaining time in seconds: "+std::to_string(remainingSeconds)+"<BR>";
    sz=( gradersComments).size();
    rifd+="gradersComments size: "+std::to_string(sz)+"<BR>";
    std::map<std::string,std::string>::const_iterator it,itE=( gradersComments).end();
    it=( gradersComments).begin();
    while(it!=itE){
      rifd+="["+it->first+"]" +" = "+it->second+"<BR>";
      ++it;
    }
    return rifd;
  }
  std::string Response::createStatusAndProgressLineTwoRowTable(const ProblemCommentsAndScores &pcas, const double & totalScore, const  ResponderInfo &res) const{
    std::string fR="";
    std::string scoreTop="",scoreBottom="";
    if( (res.documentType==st_gradeOfResponse)||((res.statusOfTheForm!="n")&&(res.statusOfTheForm!="a")&&(res.statusOfTheForm!="q"))) {
      scoreTop="<th scope=\"col\">"+MWII::GL_WI.getDefaultWebText("TOTAL")+"</th>";
      scoreBottom="<td></td>";
      if(totalScore>-999.0){
        scoreBottom="<td>"+BF::doubleToString(totalScore)+"</td>";
      }
    }
    std::vector<std::string> secondRowOfTable;
    long tableLen;
    std::map<long,std::string>::const_iterator it,itE;
    if((res.documentType==st_responseToTest)&&(res.statusOfTheForm=="a")){
      tableLen=(pcas.completionProgress).size();
      it=(pcas.completionProgress).begin();
      itE=(pcas.completionProgress).end();
    }
    else{
      tableLen=(pcas.scoringProgress).size();
      it=(pcas.scoringProgress).begin();
      itE=(pcas.scoringProgress).end();
    }
    secondRowOfTable.resize(tableLen);
    long i=0;
    while(it!=itE){
      secondRowOfTable[i]=it->second;
      ++i;++it;
    }
    fR+="<div class=\"table-responsive text-center\">\n";
    fR+="<table class=\"table table-bordered\" aria-label=\"status and progress line\">\n <thead class=\"table-dark\">\n<tr>";
    for(long j=0;j<i;++j){
      fR+="<th scope=\"col\">"+std::to_string(j+1)+"</th>";
    }
    fR+=scoreTop;
    fR+="</tr></thead>\n<tbody><tr>";
    for(long j=0;j<i;++j){
      fR+="<td>";
      if((res.documentType==st_responseToTest)&&(res.statusOfTheForm=="a")) {
        if(secondRowOfTable[j]=="y"){
          fR+="\\(\\bullet\\)";
        }
        else{
          fR+="\\(\\circ\\)";
        }
      }
      else{
        fR+=secondRowOfTable[j];
      }
      fR+="</td>";
    }
    fR+=scoreBottom;
    fR+="</tr></tbody></table></div>\n";
    return fR;
  }
  std::string Response::createStatusAndProgressLine(const ProblemCommentsAndScores &pcas, const double & totalScore, const  ResponderInfo &res, const long & qOnPage) const{
    std::string fR="";
    if(res.displayQs==0){
      return "";
    }
    if(res.documentType==st_responseToTest){
      if((res.statusOfTheForm=="n")||(res.statusOfTheForm=="q")||(res.statusOfTheForm=="s")){
        return "";
      }
    }
    if((pcas.completionProgress).size()<= qOnPage){
      return createStatusAndProgressLineTwoRowTable(pcas,totalScore,res);
    }
    std::string scoreTop="",scoreBottom="";
    long displayScoreIndicator=0;
    if( (res.documentType==st_gradeOfResponse)||((res.statusOfTheForm!="n")&&(res.statusOfTheForm!="a")&&(res.statusOfTheForm!="q"))) {
      scoreTop=" <th scope=\"col\">"+MWII::GL_WI.getDefaultWebText("TOTAL")+"</th>";
      displayScoreIndicator=1;
      scoreBottom=" ";
      if(totalScore>-999.0){
        scoreBottom= BF::doubleToString(totalScore) ;
      }
    }
    std::vector<std::vector<std::string> > middleRowsOfTable;
    long displayLen;
    long dataSize;
    std::map<long,std::string>::const_iterator it,itE;
    if((res.documentType==st_responseToTest)&&(res.statusOfTheForm=="a")){
      displayLen= (pcas.completionProgress).size();
      it=(pcas.completionProgress).begin();
      itE=(pcas.completionProgress).end();
      dataSize=(pcas.completionProgress).size();
    }
    else{
      displayLen=(pcas.scoringProgress).size();
      it=(pcas.scoringProgress).begin();
      itE=(pcas.scoringProgress).end();
      dataSize=(pcas.scoringProgress).size();
    }
    long numMidRows=displayLen/qOnPage;
    if(displayLen % qOnPage>0){++numMidRows;}
    middleRowsOfTable.resize(numMidRows);
    std::vector<std::string> secondRowOfTable;
    secondRowOfTable.resize(qOnPage);
    for(long i=0;i<numMidRows;++i){
      middleRowsOfTable[i]=secondRowOfTable;
    }
    long row=0,column=0;
    while(it!=itE){
      middleRowsOfTable[row][column]=it->second;
      ++column;
      if(column==qOnPage){
        column=0;++row;
      }
      ++it;
    }
    fR+="<div class=\"table-responsive text-center\">\n";
    fR+="<table class=\"table table-bordered\" aria-label=\"status and progress line\">\n <thead class=\"table-dark\">\n<tr>";
    for(long j=0;j<qOnPage;++j){
      fR+="<th scope=\"col\"> </th>";
    }
    fR+=scoreTop;
    fR+="</tr></thead>\n<tbody>";
    row=0;
    long j=0;
    while(row<numMidRows){
      fR+="<tr>";
      column=0;
      while(column<qOnPage){
        fR+="<td>";
        if(j<dataSize){
          if((res.documentType==st_responseToTest)&&(res.statusOfTheForm=="a")) {
            fR+=std::to_string(j+1)+" ";
            if(middleRowsOfTable[row][column]=="y"){
              fR+="\\(\\bullet\\)";
            }
            else{
              fR+="\\(\\circ\\)";
            }
          }
          else{
            fR+=middleRowsOfTable[row][column];
          }
        }
        fR+="</td>";
        ++column;
        ++j;
      }
      ++row;
      if(displayScoreIndicator==1){
        fR+="<td>";
        if(row==numMidRows){
          fR+=scoreBottom;
        }
        fR+="</td>";
      }
      fR+="</tr>";
    }
    fR+=" </tbody></table></div>\n";
    return fR;
  }
  std::string Response::createLinkToCertificate(const PSDI::SessionData& _psd, const ResponderInfo& rInf) const{
    if((rInf.documentType!=st_responseToTest)&&(rInf.documentType!=st_gradeOfResponse)){
      return "";
    }
    if((rInf.documentType==st_gradeOfResponse)&&(_psd.isRoot=="no")){
      return "";
    }
    if((rInf.certificateDescriptionCode=="notFound")||(rInf.certificateIdCode=="notFound")||(rInf.certificateDescriptionCode=="")||(rInf.certificateIdCode=="")){
      return "";
    }
    std::string linkURL="index.cgi?"+MWII::GL_WI.get_e_parPage()+"=certificates&s1=u11&r1="+rInf.certificateIdCode;
    std::string certText=MWII::GL_WI.getDefaultWebText("certificateAnnouncement");
    if(certText=="certificateAnnouncement"){
      certText="<a href=\"_***CERTURL***_\">Certificate</a>";
    }
    return SF::findAndReplace(certText,"_***CERTURL***_",linkURL);
  }
  int autoGradingIsNeeded(const std::string & grComment){
    int grNeed=0;
    long pos;
    std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(grComment,pos,"_score_","_/score_");
    if((allD.second==0)||(allD.first=="") ){
       pos=0;allD=SF::extract(grComment,pos,"_comment_","_/comment_");
       if((allD.second==0)||(allD.first=="")){
         grNeed=1;
       }
       else{
         std::string autoGraderRes=allD.first;
         pos=0;allD=SF::extractAndReplace(allD.first,pos,"_code_AUTOGRADER","_/code_");
         if(allD.second==1){
           if(BF::cleanSpaces(allD.first,1)==""){
             grNeed=1;
           }
         }
       }
    }
    if(grNeed==0){
      std::string chg=SF::findAndReplace(grComment,"requestAutograde","");
      if(chg!=grComment){
        grNeed=1;
      }
    }
    return grNeed;
  }
  void copyOfficialQuestionComponents(SingleQuestionInfo& sqi, const OfficialProblemData& opd){
    sqi.formulation=opd.formulation;
    sqi.officialSolution=opd.solution;
    sqi.officialAnswer=opd.answer;
    sqi.maxPoints=opd.points;
    sqi.displType=opd.displayType;
    sqi.allChoicesSt=opd.choices;
    sqi.fileAllowed=opd.filesAllowed;
    sqi.gRulesMap=opd.gRules;
  }
  std::string Response::automaticGrading(const PSDI::SessionData  & _psd,const std::string & _req){
    if(_psd.isRoot=="no"){
      return "";
    }
    SingleQuestionInfo sqi;
    ProblemCommentsAndScores pcas;
    std::string statusAndProgressLine;
    ResponderInfo res=infoFromResponseText(_psd,_req);
    if(res.exitStatus=="badForm"){
      return automaticGradingOfAllTests(_psd,_req);
    }
    if(res.documentType!=st_gradeOfResponse){
      return automaticGradingOfAllTests(_psd,_req);
    }
    if(res.gradingStatus!="a"){
      return "";
    }
    std::string editV01;
    std::string hd="",ft="",md="";
    std::map<std::string,std::vector<std::string> >::const_iterator itu,ituE;
    std::map<std::string,OfficialProblemData >::const_iterator itf,itfE;
    itfE=formRTQsMap.end();
    ituE=userRTAnswMap.end();
    itf=formRTQsMap.begin();
    std::map<std::string,std::string>::const_iterator itGrC,itGrCE;
    itGrCE=(res.gradersComments).end();
    sqi.num=0;
    long fileCounter=0;
    double totalScore=0.0;
    double singleProblemScore;
    int everythingGraded=1;
    std::map<std::string,CAGI::GradingResult> autoGradingMap;
    long pos;std::pair<std::string,int>allD;
    std::string placeHoldersForGrader="";
    std::map<std::string,CodeAutoGraderInfo> delayedCodeAutograding;
    int grNeed;
    while(itf!=itfE){
      std::string extractedCr,extractedWr,extractedAlways,tmpAnsw;
      sqi.QNum=itf->first;
      placeHoldersForGrader+="_in*|_ _gc*|__/gc*|_ _lb*|_"+sqi.QNum+"_/lb*|_ _/in*|_";
      copyOfficialQuestionComponents(sqi,itf->second);
      sqi.graderComment="notFound";
      itGrC=(res.gradersComments).find(sqi.QNum);
      if(itGrC!=itGrCE){
        sqi.graderComment=itGrC->second;
      }
      itu=userRTAnswMap.find(sqi.QNum);
      if(itu==ituE){
        sqi.userSolution="notFound";
        sqi.userAnswer="notFound";
        sqi.userPointsEarned="notFound";
      }
      else{
        sqi.userSolution=(itu->second)[0];
        tmpAnsw=(itu->second)[1];
        SF::extractComments(tmpAnsw,extractedCr,extractedWr,extractedAlways);
        sqi.userAnswer=SF::answerToStandardForm(tmpAnsw);
        sqi.userPointsEarned=(itu->second)[2];
      }
      sqi.autoGraderCodeData=CAGI::getAutoGraderCodeData((itf->second).autoGraderInfo,sqi.officialSolution,sqi.userAnswer,sqi.maxPoints);
      ++(sqi.num);
      grNeed=autoGradingIsNeeded(sqi.graderComment);
      if((sqi.autoGraderCodeData.language=="py")||(sqi.autoGraderCodeData.language=="cpp")){
        if(grNeed){
          if(CAGI::GL_Code_Counter.remainingCodes>0){
            delayedCodeAutograding[sqi.QNum]=sqi.autoGraderCodeData;
            --CAGI::GL_Code_Counter.remainingCodes;
          }
        }
      }
      else{
        if((BF::isNumeric(sqi.officialAnswer))&&(!BF::isNumeric(sqi.userAnswer)) ){
          sqi.userAnswer=BF::doubleToString(AEF::getLastNumericalValue(sqi.userAnswer));
        }
      }
      if( (sqi.userAnswer!="notFound")
         &&(sqi.userAnswer!=GF::GL_officialNA)
         &&(sqi.userAnswer==sqi.officialAnswer) ) {
        CAGI::GradingResult tmpMaxPt;
        tmpMaxPt.score=sqi.maxPoints;
        tmpMaxPt.numericScore=BF::stringToDouble(sqi.maxPoints);
        tmpMaxPt.shorterComment=SF::prepareNewContent(sqi.graderComment,
                                                      "_comment_","_/comment_",
                                                      extractedCr+extractedAlways,
                                                      "[r]","[/r]");
        autoGradingMap[sqi.QNum]=tmpMaxPt;
      }
      else{
          if(((extractedWr!="")||(extractedAlways!="")) ){
            CAGI::GradingResult tmpRes; // default value is empty score, which is 0
            tmpRes.shorterComment=SF::prepareNewContent(sqi.graderComment,
                                                        "_comment_","_/comment_",
                                                        extractedWr+extractedAlways,
                                                        "[r]","[/r]");
            autoGradingMap[sqi.QNum]=tmpRes;
          }
      }
      ++itf;
    }
    CAGI::codeAutoGradeAndUpdateMap(_psd,autoGradingMap,delayedCodeAutograding,"secretTestCases");
    std::string newInit;
    std::vector<std::string> grInfo=SF::stringToVector(rawText,"_in*|_","_/in*|_");
    long sz=grInfo.size();
    if(sz==0){
      pos=0;allD=SF::getEverythingBefore(initText,pos,"_/textData!!_");
      if(allD.second==1){
        newInit=allD.first;
      }
      grInfo=SF::stringToVector(placeHoldersForGrader,"_in*|_","_/in*|_");
      sz=grInfo.size();
    }
    else{
      newInit=initText;
      pos=0;allD=SF::getEverythingBefore(initText,pos,"_in*|_");
      if(allD.second==1){
        newInit=allD.first;
      }
    }
    std::map<std::string,CAGI::GradingResult>::const_iterator it,itE;
    itE=autoGradingMap.end();
    long autoGrMapSz=autoGradingMap.size();
    for(long i=0;i<sz;++i){
      pos=0;allD=SF::extract(grInfo[i],pos,"_lb*|_","_/lb*|_");
      if(allD.second==1){
        it=autoGradingMap.find(allD.first);
        if(it!=itE){
          newInit+="_in*|_ _gc*|__score_"+(it->second).score+"_/score__comment_"+(it->second).shorterComment+"_/comment__/gc*|_ _lb*|_"+it->first+"_/lb*|_ _/in*|_";
        }
        else{
          newInit+="_in*|_"+grInfo[i]+"_/in*|_";
        }
      }
    }
    newInit+="_/textData!!_";
    if(autoGrMapSz>0){
      initText=newInit;
      meRM.setTextData(initText);
      meRM.putInDB();
      return MWII::GL_WI.getDefaultWebText("Automatic grader located and graded")+" "+std::to_string(autoGrMapSz)+ " "+MWII::GL_WI.getDefaultWebText("ungraded problems")+".";
    }
    return MWII::GL_WI.getDefaultWebText("There were no ungraded problems left for automatic grader")+".";
  }
  std::string Response::automaticGradingOfAllTests(const PSDI::SessionData  & _psd,const std::string & _req){
    if(_psd.isRoot=="no"){
      return "";
    }
    std::vector<LocationOfDocuments> allLocations=getLocations(_psd);
    long sz=allLocations.size();
    std::string fR="";
    fR+="<ul>";
    for(long i=0;i<sz;++i){
      fR+="<li>";
      fR+=allLocations[i].gradeLocation+"<BR>";
      Response tmpRT(allLocations[i].gradeLocation,"no",_psd.my_un);
      fR+=tmpRT.automaticGrading(_psd,"gr");
      fR+="</li>";
    }
    fR+="</ul>";
    return fR;
  }
  std::pair<std::string,std::string> Response::getRespRecStatus(const PSDI::SessionData  & _psd) const{
    std::pair<std::string,std::string> fR;
    fR.first="notFound";fR.second="notFound";
    if(_psd.isRoot=="no"){
      return fR;
    }
    std::pair<std::string,int> allD;long pos;
    pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_statusOfTheForm_ARTB,APTI::GL_syntax.s_statusOfTheForm_ARTE);
    if(allD.second==0){
      return fR;
    }
    fR.first=allD.first;
    pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_statusOfTheGrading_ARTB,APTI::GL_syntax.s_statusOfTheGrading_ARTE);
    if(allD.second==0){
      return fR;
    }
    fR.second=allD.first;
    return fR;
  }
  std::string Response::changeResponseReceiverStatus(const PSDI::SessionData & _psd, const std::string & _exStN, const std::string & _grStN){
    if(_psd.isRoot=="no"){
      return "notAllowed";
    }
    std::string fR;
    std::pair<std::string,std::string> oldSt=getRespRecStatus(_psd);
    std::string exStN,grStN;
    exStN="n";
    if(_exStN=="00"){
      exStN=oldSt.first;
    }
    if((_exStN=="0a")||(_exStN=="aa")){
      exStN="a";
    }
    if((_exStN=="0q")||(_exStN=="qq")){
      exStN="q";
    }
    if((_exStN=="0s")||(_exStN=="ss")){
      exStN="s";
    }
    if((_exStN=="0p")||(_exStN=="pp")){
      exStN="p";
    }
    if((_exStN=="0c")||(_exStN=="cc")){
      exStN="c";
    }
    if((_exStN=="sp")||(_exStN=="sc")){
      exStN=_exStN;
    }
    grStN="n";
    if(_grStN=="00"){
      grStN=oldSt.second;
    }
    if((_grStN=="0a")||(_grStN=="aa")){
      grStN="a";
    }
    std::string messageGr="gradingRoomActionTaken";
    if((oldSt.first==exStN)&&(oldSt.second==grStN)){
      messageGr="gradingRoomNothingToChange";
    }
    else{
      std::pair<std::string,int> allD;
      exStN=APTI::GL_syntax.s_statusOfTheForm_ARTB+exStN+APTI::GL_syntax.s_statusOfTheForm_ARTE;
      grStN=APTI::GL_syntax.s_statusOfTheGrading_ARTB+grStN+APTI::GL_syntax.s_statusOfTheGrading_ARTE;
      long pos;
      pos=0;allD=SF::extractAndReplace(rawText, pos,APTI::GL_syntax.s_statusOfTheForm_ARTB,APTI::GL_syntax.s_statusOfTheForm_ARTE, 0,exStN );
      if(allD.second==1){
        rawText=allD.first;
      }
      pos=0;allD=SF::extractAndReplace(rawText, pos,APTI::GL_syntax.s_statusOfTheGrading_ARTB,APTI::GL_syntax.s_statusOfTheGrading_ARTE, 0,grStN );
      if(allD.second==1){
        rawText=allD.first;
      }
      pos=0;allD=SF::extractAndReplace(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE,0,APTI::GL_syntax.s_tDataB+rawText+APTI::GL_syntax.s_tDataE);
      if( allD.second==1 ){
        initText=allD.first;
        meRM.setTextData(initText);
        meRM.putInDB();
      }
    }
    std::string uEV=HSF::urlEnvVarsFromForwardedParameters(_psd.respMap,LI::GL_LN.st_envVName_Variable, LI::GL_LN.st_envVName_Value, LI::GL_LN.st_envVName_ForwardVariable, LI::GL_LN.st_envVName_ForwardValue, 10);
    std::string url="index.cgi?"+uEV;
    std::string webText=MWII::GL_WI.getDefaultWebText(messageGr);
    if(webText==""){
      webText="_insert__n*_buttonLink_/n*__n*_"+MWII::GL_WI.getDefaultWebText("Back to Gr. Room")+"_/n*__n*_*BLREPL*_/n*__/insert_ ";
    }
    webText=SF::findAndReplace(webText,"*BLREPL*",url);
    return treatInserts(_psd,webText,"_insert_","_/insert_");
  }
  std::string Response::prepareDefaultRequest(const PSDI::SessionData  & _psd,const std::string & _req){
    long questions_on_page=MWII::GL_WI.get_q_on_page();
    long displStart=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    long displEndP1=displStart+questions_on_page+1;
    LMF::LatexDataExamPaper lD_ep;
    LMF::LatexDataSingleQuestion lD_sq;
    SingleQuestionInfo sqi;
    ProblemCommentsAndScores pcas;
    std::string statusAndProgressLine,linkToCertificateLine;
    std::set<std::string>::const_iterator it,itE;
    itE=permitRead.end();
    it=permitRead.find("everyone");
    if(it==itE){
      if(_psd.isRoot=="no"){
        return "";
      }
    }
    ResponderInfo res=infoFromResponseText(_psd,_req);
    if((_psd.isGradingAdmin=="no")&&(_psd.allowRespRecDisplayToOthers=="no")){
      if((res.documentType==st_responseToTest)&&(res.solverUserName!=_psd.my_un)){
        return "";
      }
    }
    if(res.exitStatus=="badForm"){
      return "";
    }
    if(res.remainingSeconds>-1){
      timerCode=MWII::GL_WI.getDefaultWebText("TimerJS");
      if(timerCode.length() < 10){
        timerCode="";
      }
      else{
        timerCode=SF::findAndReplace(timerCode,APTI::GL_syntax.s_tmSecRepl,std::to_string(res.remainingSeconds));
      }
    }
    instructionsOnTopOfTheForm=SF::findAndReplace(instructionsOnTopOfTheForm,APTI::GL_syntax.s_timerRepl,timerCode);
    std::string editV01;
    std::string hd="",ft="",md="";
    lD_ep.studentName=res.idInfoData;
    lD_ep.examCode=tName;
    if(res.idInfoData!="notFound"){
      if(res.documentType==st_responseToTest){
        hd+="<H1>Assignment/Exam</H1>\n";
        hd+="<H2> "+res.idInfoData+"</H2>";
      }
      if(res.documentType==st_gradeOfResponse){
        hd+="<H1>Assignment/Exam</H1>\n<H2>Grading</H2>\n";
      }
    }
    if((res.documentType==st_gradeOfResponse)&&(res.gradingStatus=="a")){
      res.displayQs=1;
      res.displaySol=1;
      res.displayPoints=1;
      res.displayComments=1;
      if(_psd.my_un!="visitor"){
        IOF::deleteOldFiles(DD::GL_DBS.getChallengeAnswStorage(),_psd.my_un,10*CERD::GL_CertificatesOptions.maxTimeToKeepPDF);
        IOF::toFile(DD::GL_DBS.getChallengeAnswStorage()+"/"+"gr_"+tName+"."+_psd.my_un,SF::mapToString(res.gradersComments,"_vV*_","_/vV*_","_ky*_","_/ky*_","_vl*_","_/vl*_"));
      }
    }
    if((res.documentType==st_gradeOfResponse)&&(res.gradingStatus!="a")){
      res.displayQs=0;
      res.displaySol=0;
      res.displayPoints=0;
      res.displayComments=0;
    }
    if(res.documentType==st_responseToTest){
      res.acceptGrade=0;
    }
    if(res.documentType==st_gradeOfResponse){
      res.acceptResp=0;
    }
    if(res.displayQs==0){
      if(res.documentType==st_responseToTest){
        if(res.userCanTakeActionAndActivateExam==1){
          hd=messageStartExam;
          hd+="<p></p>\n";
          hd+="<p><center>";
          std::string url="index.cgi?r=";
          url+=meRM.getTextName();
          url+="&act=yes";
          hd+=HSF::createButtonLink(url,MWII::GL_WI.getDefaultWebText("Start"),"standardSize","dark");
          hd+="</center></p>\n";
        }
        else{
          hd=messageUnavailableYet;
        }
      }
      if(res.documentType==st_gradeOfResponse){hd=messageNoGradingYet;}
    }
    std::string parametersForBackToGradeRoom="";
    if(_psd.isGradingAdmin=="yes"){
      parametersForBackToGradeRoom=SF::forwardValVarPairs(_psd.respMap,LI::GL_LN.st_envVName_Variable,LI::GL_LN.st_envVName_Value,10);
    }
    if((res.acceptResp==1)||(res.acceptGrade==1)){
      if((instructionsOnTopOfTheForm!="")||(_psd.respRecFlag=="rejected")||(_psd.respRecFlag=="fileTooLarge")||(_psd.respRecFlag=="badExtension")){
        int printedSomething=0;
        if(_psd.respRecFlag=="rejected"){
          hd+="<P><font color=\"red\"><B> "+MWII::GL_WI.getDefaultWebText("ERROR")+" - "+MWII::GL_WI.getDefaultWebText("LAST EDIT WAS REJECTED")+"</B></font></P>";
          printedSomething=1;
        }
        if(_psd.respRecFlag=="fileTooLarge"){
          hd+="<P><font color=\"red\"><B> "+MWII::GL_WI.getDefaultWebText("ERROR")+" - " +MWII::GL_WI.getDefaultWebText("FILE TOO LARGE")+"</B></font></P>";
          printedSomething=1;
        }
        if(_psd.respRecFlag=="badExtension"){
          hd+="<P><font color=\"red\"><B> "+MWII::GL_WI.getDefaultWebText("ERROR")+" - " +MWII::GL_WI.getDefaultWebText("BAD FILE TYPE")+"</B></font></P>";
          printedSomething=1;
        }
        if(res.acceptResp==1){
          hd+= instructionsOnTopOfTheForm;
          printedSomething=1;
        }
        if(printedSomething==1){
          hd="<div class=\"messageBox\">"+hd+"</div><P></P>";
        }
      }
      hd+="_insert__n*_formInitialization_/n*__n*_"+e_formNameRT+"_/n*_\n _n*_index.cgi?";
      hd+= MWII::GL_WI.get_e_respRecReqRT()+"="+tName;
      hd+="&st="+MWII::GL_WI.getStartOfList();
      hd+="&"+e_actNameRT;
      hd+=parametersForBackToGradeRoom;
      hd+="_/n*__/insert_\n";
      ft="_insert__n*_formPlacement_/n*__n*_"+e_formNameRT+"_/n*_";
      ft+="_n*_2_/n*_ _n*_"+labelOnTheSubmitButton+ "_/n*_";
      ft+="_/insert_ \n";
    }
    std::map<std::string,std::vector<std::string> >::const_iterator itu,ituE;
    std::map<std::string,OfficialProblemData >::const_iterator itf,itfE;
    itfE=formRTQsMap.end();
    ituE=userRTAnswMap.end();
    itf=formRTQsMap.begin();
    std::map<std::string,std::string>::const_iterator itGrC,itGrCE;
    itGrCE=(res.gradersComments).end();
    sqi.num=0;
    long fileCounter=0;
    double totalScore=0.0;
    double singleProblemScore;
    int everythingGraded=1;
    std::string problemDisplText;
    while(itf!=itfE){
      sqi.QNum=itf->first;
      copyOfficialQuestionComponents(sqi,itf->second);
      sqi.graderComment="notFound";
      itGrC=(res.gradersComments).find(sqi.QNum);
      if(itGrC!=itGrCE){
        sqi.graderComment=itGrC->second;
      }
      itu=userRTAnswMap.find(sqi.QNum);
      if(itu==ituE){
        sqi.userSolution="notFound";
        sqi.userAnswer="notFound";
        sqi.userPointsEarned="notFound";
      }
      else{
        sqi.userSolution=(itu->second)[0];
        sqi.userAnswer=SF::answerToStandardForm((itu->second)[1]);
        sqi.userPointsEarned=(itu->second)[2];
      }
      sqi.autoGraderCodeData=CAGI::getAutoGraderCodeData((itf->second).autoGraderInfo,sqi.officialSolution,sqi.userAnswer,sqi.maxPoints);
      lD_sq.lPInst=LMF::getPrintingInstructions((itf->second).latexPrintingInstructions);
      lD_sq.formulation=sqi.formulation;
      lD_sq.questionType=sqi.displType;
      lD_sq.multipleChoicesSt=sqi.allChoicesSt;
      lD_sq.pointsSt=sqi.maxPoints;
      lD_sq.solution=sqi.officialSolution;
      lD_sq.answer=sqi.officialAnswer;
      // MAKE SURE THE SOLUTION AND ANSWER DO NOT ACCIDENTALLY GET DISPLAYED TO STUDENT
      lD_sq.solution="";
      lD_sq.answer="";
      lD_sq.questionNumber=sqi.num;
      lD_ep.questionsStack.push(lD_sq);
      ++(sqi.num);
      sqi.acceptResp=res.acceptResp;
      sqi.displaySol=res.displaySol;
      sqi.displayPoints=res.displayPoints;
      sqi.displayComments=res.displayComments;
      sqi.displayQs=res.displayQs;
      GRI::updateCommentAndScoreWithGradingRules(sqi);
      if(res.documentType==st_responseToTest){
        problemDisplText=singleProblemDisplay(sqi,pcas, fileCounter, singleProblemScore);
        if(problemDisplText=="certificate"){
          --(sqi.num);
          lD_ep.questionsStack.pop();
        }
        else{
          if( (sqi.num>displStart)&&(sqi.num<displEndP1)){
            md+=problemDisplText;
          }
        }
      }
      else{
        sqi.acceptResp=res.acceptGrade;
        std::pair<std::string,int> displPairForGrader=singleProblemDisplayForGrader(sqi,pcas,fileCounter, singleProblemScore);
        if(displPairForGrader.second==0){
          --(sqi.num);
        }
        md+=displPairForGrader.first;
      }
      if((totalScore>-999.0)&&(singleProblemScore>-999.0)){
        totalScore+=singleProblemScore;
      }
      else{
        everythingGraded=0;
      }
      ++itf;
    }
    if(res.displayQs==1){
      std::string burnAfterPrinting="<B>" + MWII::GL_WI.getDefaultWebText("ADDITIONAL FILES")+"</B><BR><BR>";
      while(fileCounter<numFilesAllowed){
        if(res.acceptResp==1){
          md+=fileFieldForProblemInputForm(burnAfterPrinting+MWII::GL_WI.getDefaultWebText("Additional file")+" "+std::to_string(fileCounter+1), fileCounter);
        }
        else{
          md+="<P>";
          md+=burnAfterPrinting;
          md+=fileForProblemInfo(fileCounter);
          md+="</P>";
        }
        ++fileCounter;
        burnAfterPrinting="";
      }
    }
    statusAndProgressLine=createStatusAndProgressLine(pcas,totalScore,res,questions_on_page);
    linkToCertificateLine=createLinkToCertificate(_psd,res);
    if(res.documentType==st_gradeOfResponse){
      statusAndProgressLine+=MWII::GL_WI.getDefaultWebText("<p>Help with rules</p>");
    }
    statusAndProgressLine=linkToCertificateLine+statusAndProgressLine;
    if(sqi.num>questions_on_page){
      std::string pageAtt=MWII::GL_WI.get_e_respRecReqRT()+"="+tName;
      long start=displStart;
      if((start<0)||(start>sqi.num-1)){start=0;}
      long numb=questions_on_page;
      if(numb<0){numb=0;}
      long end=start+numb;
      if((end<0)||(end>sqi.num)){
        end=sqi.num;
      }
      hd+= BI::createScroller(start,end,sqi.num,numb,pageAtt,"5","-1",1);
    }
    if(_req=="pm"){
      lD_ep.storageLocation=MWII::GL_WI.getPublicSystemFileLoc();
      lD_ep.examPreambleLatexTemplate=MWII::GL_WI.getDefaultWebText("examPreambleLatexTemplate");
      lD_ep.answerBoxLatexTemplate=MWII::GL_WI.getDefaultWebText("answerBoxLatexTemplate");
      lD_ep.prefix=res.prefixForExamPDF;
      lD_ep.addNameIndicator=res.addNameIndicator;
      lD_ep.pdfTopOfThePage=res.pdfTopOfThePage;
      lD_ep.pdfNewPageSeparators=res.pdfNewPageSeparators;
      return LMF::prepareExamForPrinting(lD_ep,_psd.indChangeRespRecToPrintVersionOfCommonInClassExam,_psd.pdfBeforeProblems,_psd.pdfAfterProblems);
    }
    if(parametersForBackToGradeRoom!=""){
      std::string backLink="";
      backLink+="<p></p><div>\n<a href=\"index.cgi?page=GrMain";
      backLink+=parametersForBackToGradeRoom+"\">\n<button type=\"button\" class=\"btn btn-outline-dark btn-sm\">Back to Grading Room</button>\n";
      backLink+="</a>\n</div>\n";
      hd=backLink+hd;
    }
    editV01=treatInserts(_psd, hd+statusAndProgressLine+md+ft,APTI::GL_syntax.s_insertB,APTI::GL_syntax.s_insertE);
    return prepare(_psd,editV01);
  }
  int Response::analyzeAccessAndReportSuspiciousBehavior(const PSDI::SessionData & _psd){
    long pos;std::pair<std::string,int> allD;
    if((numberOfDifferentDevicesThatAccessedTheForm==-1)&&(_psd.isRoot!="yes") ){
      numberOfDifferentDevicesThatAccessedTheForm=0;
      std::string currentAccessLog;
      currentAccessLog =APTI::GL_syntax.s_accessLogsInnerSep_ARTB+_psd.usrAgent+ APTI::GL_syntax.s_accessLogsInnerSep_ARTE;
      currentAccessLog+=APTI::GL_syntax.s_accessLogsInnerSep_ARTB+_psd.remAddr+APTI::GL_syntax.s_accessLogsInnerSep_ARTE;
      currentAccessLog+=APTI::GL_syntax.s_accessLogsInnerSep_ARTB+_psd.reqMethod+APTI::GL_syntax.s_accessLogsInnerSep_ARTE;
      pos=0;allD=SF::getEverythingBefore(rawText,pos,currentAccessLog);
      if(allD.second==1){
        // Nothing new - accessing the form in the same way as before
        pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_accessLogsNumDevs_ARTB,APTI::GL_syntax.s_accessLogsNumDevs_ARTE);
        if(allD.second==1){
          numberOfDifferentDevicesThatAccessedTheForm=BF::stringToInteger(allD.first);
          if(numberOfDifferentDevicesThatAccessedTheForm<0){numberOfDifferentDevicesThatAccessedTheForm=0;}
        }
      }
      else{
        // The form is accessed in a new way
        std::string oldDataText="";
        pos=0;allD=SF::extract(rawText,pos,APTI::GL_syntax.s_accessLogs_ARTB,APTI::GL_syntax.s_accessLogs_ARTE);
        if(allD.second==1){
          oldDataText=allD.first;
          allD=SF::eraseStuffBetween(rawText,APTI::GL_syntax.s_accessLogs_ARTB,APTI::GL_syntax.s_accessLogs_ARTE);
          if(allD.second==1){rawText=allD.first;}
          pos=0;allD=SF::extract(oldDataText,pos,APTI::GL_syntax.s_accessLogsNumDevs_ARTB,APTI::GL_syntax.s_accessLogsNumDevs_ARTE);
          if(allD.second==1){
            numberOfDifferentDevicesThatAccessedTheForm=BF::stringToInteger(allD.first);
            if(numberOfDifferentDevicesThatAccessedTheForm<0){numberOfDifferentDevicesThatAccessedTheForm=0;}
            allD=SF::eraseStuffBetween(oldDataText,APTI::GL_syntax.s_accessLogsNumDevs_ARTB,APTI::GL_syntax.s_accessLogsNumDevs_ARTE);
            if(allD.second==1){oldDataText=allD.first;}
          }
        }
        ++numberOfDifferentDevicesThatAccessedTheForm;
        oldDataText+=currentAccessLog+APTI::GL_syntax.s_accessLogsNumDevs_ARTB+std::to_string(numberOfDifferentDevicesThatAccessedTheForm)+APTI::GL_syntax.s_accessLogsNumDevs_ARTE;
        rawText+=APTI::GL_syntax.s_accessLogs_ARTB+oldDataText+APTI::GL_syntax.s_accessLogs_ARTE;
        pos=0;allD=SF::extractAndReplace(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE,0,APTI::GL_syntax.s_tDataB+rawText+APTI::GL_syntax.s_tDataE);
        if( allD.second==1 ){
          initText=allD.first;
          meRM.setTextData(initText);
          meRM.putInDB();
        }
      }
    }
    pos=0;allD=SF::getEverythingBefore(rawText,pos,"_alIn*|_POST_/alIn*|_");
    return allD.second;
  }
  std::string Response::assignGraders(const PSDI::SessionData &_psd, const std::string &_options){
    int workDone=0;
    std::vector<LocationOfDocuments> allStLs=getLocations(_psd);
    std::vector<LocationOfDocuments> newStLs=allStLs;
    long sz=allStLs.size();
    if(sz==0){
      return "!failed! no students";
    }
    std::string options=_options;
    if(options==""){
      options="s";
    }
    if((options[0]=='s')||(options[0]=='S')){
      for(long i=0;i<sz;++i){
        newStLs[i].graderUName=newStLs[i].userName;
      }
      workDone=1;
    }
    SSF::StatSeq<std::string> workers;
    long wsz;
    if(workDone==0){
      if((options[0]=='a')||(options[0]=='A')){
        for(long i=0;i<sz;++i){
          workers+=newStLs[i].userName;
        }
      }
      else{
        std::vector<std::string> wV=SF::stringToVector(options,"_n*_","_/n*_");
        wsz=wV.size();
        for(long i=0;i<wsz;++i){
          workers+=wV[i];
        }
      }
      wsz=workers.size();
      if(wsz==0){
        return "!failed! No available workers";
      }
      std::vector<long> derV=DERF::genRandDerangement(wsz);
      std::map<std::string,std::string> gradingMap,gradingMapInv;
      for(long i=0;i<wsz;++i){
        gradingMap[workers[i]]=workers[derV[i]];
        gradingMapInv[workers[derV[i]]]=workers[i];
      }
      std::map<std::string,long> workLoads;
      for(long i=0;i<wsz;++i){
        workLoads[workers[i]]=0;
      }
      long maxLoad=(sz/wsz);
      if(sz%wsz!=0){++maxLoad;}
      std::map<std::string,int> assignmentDone;
      long it;
      for(long i=0;i<sz;++i){
        assignmentDone[newStLs[i].userName]=0;
        it=workers.find(newStLs[i].userName);
        if(it!=-1){
          newStLs[i].graderUName= gradingMapInv[newStLs[i].userName];
          workLoads[newStLs[i].graderUName]=1;
          assignmentDone[newStLs[i].userName]=1;
        }
      }
      long remainingWorkers;long k;std::string randomGrader;long newWL;
      for(long i=0;i<sz;++i){
        if(assignmentDone[newStLs[i].userName]==0){
          remainingWorkers=workers.size();
          if(remainingWorkers>0){
            newStLs[i].graderUName="na";
            k=RNDF::randNum(remainingWorkers);
            randomGrader=workers[k];
            newStLs[i].graderUName=randomGrader;
            newWL=workLoads[randomGrader]+1;
            if(newWL>=maxLoad){
              workers-=randomGrader;
            }
            else{
              workLoads[randomGrader]=newWL;
            }
          }
        }
      }
    }
    std::string newRawT=rawText;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extractAndReplace(newRawT,pos,"_st*|_","_/st*|_");
    while(allD.second==1){
       newRawT=allD.first;
       pos=0;allD=SF::extractAndReplace(newRawT,pos,"_st*|_","_/st*|_");
    }
    for(long i=0;i<sz;++i){
      newRawT+="_st*|_";
      newRawT+=newStLs[i].putIntoString();
      newRawT+="_/st*|_\n";
    }
    rawText=newRawT;
    pos=0;allD=SF::extractAndReplace(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE,0,APTI::GL_syntax.s_tDataB+rawText+APTI::GL_syntax.s_tDataE);
    if( allD.second==1 ){
      initText=allD.first;
      meRM.setTextData(initText);
      meRM.putInDB();
    }
    return "!success!";
  }
  std::string Response::editTimer(const PSDI::SessionData &_psd, const std::string &_options){
    int workDone=0;
    std::string allTime="3600";
    std::string absEnd="Wed, 29 Apr 3333 19:00:00 GMT";
    std::string timerVersion="0";
    std::pair<std::string,int> allD;long pos;
    pos=0;allD=SF::extract(_options,pos,APTI::GL_syntax.s_cAllowedTime_QRTB,APTI::GL_syntax.s_cAllowedTime_QRTE);
    if(allD.second==1){
      allTime=allD.first;
    }
    pos=0;allD=SF::extract(_options,pos,APTI::GL_syntax.s_cAbsEnd_QRTB,APTI::GL_syntax.s_cAbsEnd_QRTE);
    if(allD.second==1){
      absEnd=allD.first;
    }
    pos=0;allD=SF::extract(_options,pos,APTI::GL_syntax.s_cTVersion_QRTB,APTI::GL_syntax.s_cTVersion_QRTE);
    if(allD.second==1){
      timerVersion=allD.first;
    }
    pos=0;allD=SF::extractAndReplace(rawText,pos,APTI::GL_syntax.s_cAllowedTime_QRTB,APTI::GL_syntax.s_cAllowedTime_QRTE,0,APTI::GL_syntax.s_cAllowedTime_QRTB+allTime+APTI::GL_syntax.s_cAllowedTime_QRTE);
    if(allD.second==1){
      rawText=allD.first;
    }
    else{
      rawText=APTI::GL_syntax.s_cAllowedTime_QRTB+allTime+APTI::GL_syntax.s_cAllowedTime_QRTE+rawText;
    }
    pos=0;allD=SF::extractAndReplace(rawText,pos,APTI::GL_syntax.s_cAbsEnd_QRTB,APTI::GL_syntax.s_cAbsEnd_QRTE,0,APTI::GL_syntax.s_cAbsEnd_QRTB+absEnd+APTI::GL_syntax.s_cAbsEnd_QRTE);
    if(allD.second==1){
      rawText=allD.first;
    }
    else{
      rawText=APTI::GL_syntax.s_cAbsEnd_QRTB+absEnd+APTI::GL_syntax.s_cAbsEnd_QRTE+rawText;
    }
    pos=0;allD=SF::extractAndReplace(rawText,pos,APTI::GL_syntax.s_cTVersion_QRTB,APTI::GL_syntax.s_cTVersion_QRTE,0,APTI::GL_syntax.s_cTVersion_QRTB+timerVersion+APTI::GL_syntax.s_cTVersion_QRTE);
    if(allD.second==1){
      rawText=allD.first;
    }
    else{
      rawText=APTI::GL_syntax.s_cTVersion_QRTB+timerVersion+APTI::GL_syntax.s_cTVersion_QRTE+rawText;
    }
    pos=0;allD=SF::extractAndReplace(initText,pos,APTI::GL_syntax.s_tDataB,APTI::GL_syntax.s_tDataE,0,APTI::GL_syntax.s_tDataB+rawText+APTI::GL_syntax.s_tDataE);
    if( allD.second==1 ){
      initText=allD.first;
      meRM.setTextData(initText);
      meRM.putInDB();
    }
    return "!success!";
  }
}
#endif
