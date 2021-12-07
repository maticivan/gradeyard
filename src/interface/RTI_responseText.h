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


#ifndef _INCL_WI_Response_H
#define _INCL_WI_Response_H
namespace RTI{
  class PossibleStatusOfTheForm{
  private:
    std::set<std::string> psForm;
    std::set<std::string>::const_iterator it,itE;
  public:
    PossibleStatusOfTheForm();
    std::string checkForLegalStatus(const std::string &);
  } GL_FORM_POSSIBLE_STATUS;
  PossibleStatusOfTheForm::PossibleStatusOfTheForm(){
    psForm.insert("nyo");//not yet open - do not show the form nor the answers
    //show form and accept answers
    psForm.insert("awd");//accept answers before deadline
    psForm.insert("a");//accept answers
    //show only questions
    psForm.insert("q");
    //show submissions and solutions
    psForm.insert("s");//show submissions of the user and official solutions
    psForm.insert("p");//show submissions of the user and points of the grader (but not comments)
    psForm.insert("c");//show submissions of the user and comments of the grader (incl. points)
    psForm.insert("sc");//both s and c
    psForm.insert("cs");//both s and c
    psForm.insert("sp");//both s and p
    psForm.insert("ps");//both s and p
    itE=psForm.end();
  }
  std::string PossibleStatusOfTheForm::checkForLegalStatus(const std::string &ch){
    it=psForm.find(ch);
    if(it==itE){
      return "no";
    }
    return "yes";
  }
  struct CodeAutoGraderInfo{
  public:
    std::string language, officialSource,userSource,codeToEmbed;
    std::vector<std::string> inputTestCases, includes;
    std::vector<double> pointsTestCases; 
  };
  struct SingleQuestionInfo{
  public:
    long num;
    std::string QNum, formulation, officialSolution, officialAnswer, maxPoints,
                displType, allChoicesSt, fileAllowed, userSolution, userAnswer,
                userPointsEarned, graderComment;
    CodeAutoGraderInfo autoGraderCodeData;
    long acceptResp, displaySol, displayPoints, displayComments, displayQs;
  };
  struct ProblemCommentsAndScores{
  public:
    std::map<long,std::string> completionProgress;
    std::map<long,std::string> scoringProgress;
  };
  struct ResponderInfo{
  public:
    std::vector<std::string> fileInfoV;
    std::string statusOfTheForm;
    std::string formulationDocument;
    std::string idInfoData;
    std::string solverUserName;
    std::string exitStatus;// the function infoFromResponseText will set this to either "badForm" or "ok"
    std::string prefixForExamPDF;
    std::string addNameIndicator;
    long acceptResp;
    long acceptGrade;
    long displaySol;
    long displayPoints;
    long displayComments;
    long displayQs;
    std::string documentType;
    std::string gradingStatus;
    std::map<std::string,std::string> gradersComments;
    std::set<std::string> questionLabels;
    int indicatorOfPOST;
    std::string debuggingPrint() const;
  };
  struct LocationOfDocuments{
  public:
    std::string userName;
    std::string testLocation;
    std::string gradeLocation;
    std::string studentEmail;
    std::string studentName;
    std::string graderUName;
    void getFromString(const std::string &);
    std::string putIntoString() const;
  };
  class Response:public APTI::AbstractText{
  protected:
    int ind_initSuccess;
    RMD::Response meRM;
    std::string myDocType;//can be responseToTest or gradeOfResponse
    std::string st_responseToTest="responseToTest";
    std::string st_gradeOfResponse="gradeOfResponse";
    std::string st_gradingColor="purple";
    RMD::Response responseDocumentForGrader;
    // when the object is a grading document
    //     then this attribute contains the responseDocument that has to be graded.
    // (when the object is a response document this attribute is irrelevant and not set to anything)
    int indRespDocForGraderCorrectlySet=0;
    RMD::Response gradingDocumentForReponse;
    // when the object is a response
    //     then this attribute contains the gradingDocument.
    // (when the object is a grading document this attribute is irrelevant and not set to anything)
    int indGradingDocForRespCorrectlySet=0;
    std::string rawTextOfResponseVisibleToGrader;
    // when the object is a grading document, this attribute is the rawText of the response document
    std::string e_formNameRT=MWII::GL_WI.get_e_formNameRT();
    std::string e_actNameRT=MWII::GL_WI.get_e_respSubmitReqRT()+"="+MWII::GL_WI.get_e_respSubmitSuccRT();//="respSubmit=yes";
    std::string s_gradeLinkInResp_QRTB="_grd*|_";
    std::string s_gradeLinkInResp_QRTE="_/grd*|_";
    std::string s_respLinkInGrade_QRTB="_rsp*|_";
    std::string s_respLinkInGrade_QRTE="_/rsp*|_";
    std::string s_nextQ_QRTB="_in*|_";
    std::string s_nextQ_QRTE="_/in*|_";
    std::string s_tFormulation_QRTB="_tx*|_";
    std::string s_tFormulation_QRTE="_/tx*|_";
    std::string s_solution_QRTB="_sl*|_";
    std::string s_solution_QRTE="_/sl*|_";
    std::string s_answer_QRTB="_aw*|_";
    std::string s_answer_QRTE="_/aw*|_";
    std::string s_points_QRTB="_pt*|_";
    std::string s_points_QRTE="_/pt*|_";
    std::string s_requestType_QRTB="_rq*|_";
    std::string s_requestType_QRTE="_/rq*|_";
    std::string s_radioButtons_QRTB="_rbAll*|_";
    std::string s_radioButtons_QRTE="_/rbAll*|_";
    std::string s_fileAllowed_QRTB="_fa*|_";
    std::string s_fileAllowed_QRTE="_/fa*|_";
    std::string s_autograderInfo_QRTB="_agr*|_";
    std::string s_autograderInfo_QRTE="_/agr*|_";
    std::string s_label_QRTB="_lb*|_";
    std::string s_label_QRTE="_/lb*|_";
    std::string s_buttonLabel_QRTB="_blb*|_";
    std::string s_buttonLabel_QRTE="_/blb*|_";
    std::string s_importantInfo_QRTB="_iinf*|_";
    std::string s_importantInfo_QRTE="_/iinf*|_";
    std::string s_notStartedYet_QRTB="_notOpen*|_";
    std::string s_notStartedYet_QRTE="_/notOpen*|_";
    std::string s_notReadyForGrading_QRTB="_noGrading*|_";
    std::string s_notReadyForGrading_QRTE="_/noGrading*|_";
    std::string s_score_GRTB="_score_";
    std::string s_score_GRTE="_/score_";
    std::string s_comment_GRTB="_comment_";
    std::string s_comment_GRTE="_/comment_";
    std::string s_statusOfTheForm_ARTB="_sf*|_";
    std::string s_statusOfTheForm_ARTE="_/sf*|_";
    std::string s_statusOfTheGrading_ARTB="_sgr*|_";
    std::string s_statusOfTheGrading_ARTE="_/sgr*|_";
    std::string s_formulRespRec_ARTB="_f*|_";
    std::string s_formulRespRec_ARTE="_/f*|_";
    std::string s_masterStatus_ARTB="_ms*|_";
    std::string s_masterStatus_ARTE="_/ms*|_";
    std::string s_deadlineRespRec_ARTB="_dl*|_";
    std::string s_deadlineRespRec_ARTE="_/dl*|_";
    std::string s_idInfo_ARTB="_iI*|_";
    std::string s_idInfo_ARTE="_/iI*|_";
    std::string s_solverUName_ARTB="_userName*|_";
    std::string s_solverUName_ARTE="_/userName*|_";
    std::string s_comment_ARTB="_gc*|_";
    std::string s_comment_ARTE="_/gc*|_";
    std::string s_numFilesAllowed_ARTB="_nf*|_";
    std::string s_numFilesAllowed_ARTE="_/nf*|_";
    std::string s_verSepBB="_v";
    std::string s_verSepEB="_/v";
    std::string s_verSepE="*|_";
    std::string s_indFileSep_ARTBB="_f";
    std::string s_indFileSep_ARTEB="_/f";
    std::string s_infFileSep_ARTBEE="*|_";
    std::string s_accessLogs_ARTB="_alo*|_";
    std::string s_accessLogs_ARTE="_/alo*|_";
    std::string s_accessLogsInnerSep_ARTB="_alIn*|_";
    std::string s_accessLogsInnerSep_ARTE="_/alIn*|_";
    std::string s_accessLogsNumDevs_ARTB="_alND*|_";
    std::string s_accessLogsNumDevs_ARTE="_/alND*|_";
    std::string labelOnTheSubmitButton;
    std::string instructionsOnTopOfTheForm;
    std::string messageUnavailableYet;
    std::string messageNoGradingYet;
    std::vector<std::string> mainFileInfoVector;
    long numFilesAllowed;
    std::map<std::string,std::vector<std::string> > userRTAnswMap;
    //key: question number; value = < solution,answer, pointsEarned, graders Comment>
    std::map<std::string,std::vector<std::string> > formRTQsMap;
    //key: question number; value = <question, formulation, solution, answer, points, displayType, fileAllowed, autoGraderExecutionData>
    long numberOfDifferentDevicesThatAccessedTheForm;
    std::set<std::string> allDevicesThatAccessedTheForm;
    std::string modifyMe() const;
    std::string closestLegalRequest(const PSDI::SessionData &, const std::string &) const;
    std::vector<long> getStatusVector(const PSDI::SessionData & , const std::string & , const std::string & ) const;
    int analyzeAccessAndReportSuspiciousBehavior(const PSDI::SessionData &);
    std::string createStatusAndProgressLineTwoRowTable(const ProblemCommentsAndScores &, const double &, const ResponderInfo &) const;
    std::string createStatusAndProgressLine(const ProblemCommentsAndScores &, const double &, const ResponderInfo &, const long &) const;
    std::string userAnswerDisplay(const SingleQuestionInfo &, long &) const;
    std::string singleProblemDisplay(const SingleQuestionInfo&, ProblemCommentsAndScores &, long &, double &) const;
    std::string singleProblemDisplayForGrader(const SingleQuestionInfo &, ProblemCommentsAndScores &, long &, double &) const;
  public:
    Response(const std::string & = "mainTextFirstPage", const std::string & = "no1117", const std::string & = "everyone");
    int initialize(const std::string & = "mainTextFirstPage", const std::string & = "no211", const std::string & = "everyone");
    std::string automaticGradingOfAllTests(const PSDI::SessionData  & ,const std::string & );
    std::string automaticGrading(const PSDI::SessionData &, const std::string &);
    std::string displayRespRec(const PSDI::SessionData &);
    std::string prepareDefaultRequest(const PSDI::SessionData &, const std::string &);
    std::pair<std::string,std::vector<std::string> > getFormQVector(const std::string &) const;
    std::pair<std::string,std::vector<std::string> > getUsrAVect(const std::string &) const;
    ResponderInfo infoFromResponseText(const PSDI::SessionData &, const std::string &, const int & = 0);
    std::vector<LocationOfDocuments> getLocations(const PSDI::SessionData &) const;
    std::string indFileSepB(const long &) const;
    std::string indFileSepE(const long &) const;
    std::string fileLabelForHTML(const long &) const;
    std::string fileFieldForProblemInputForm(const std::string &, const long &) const;
    std::string fileForProblemInfo(const long &) const;
    std::string assignGraders(const PSDI::SessionData &, const std::string &);
    std::pair<std::string,std::string> getRespRecStatus(const PSDI::SessionData &) const;
    std::string changeResponseReceiverStatus(const PSDI::SessionData & , const std::string & , const std::string & );
    int isInitialized() const;
  };
}
#endif
