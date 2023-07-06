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


#ifndef _INCL_AGRDI_AutoGraderResultDisplay_CPP
#define _INCL_AGRDI_AutoGraderResultDisplay_CPP
namespace AGRDI{

  struct Parameters{
    long colSize;
    std::string compilerErrors;
  } GL_AGParameters;
  std::string grResultSummaryTable(const CAGI::GradingResult& rGr){
    long numR=rGr.testCasesRes.size();
    long maxNumResInOneRow=GL_AGParameters.colSize;
    std::string pillSucc,pillFail,pillNA,currentPill;
    pillSucc="<span class=\"badge badge-pill bg-success\">$\\pmb{\\checkmark}$</span>";
    pillFail="<span class=\"badge badge-pill bg-danger\">$\\pmb{\\times}$</span>";
    pillNA="<span class=\"badge badge-pill bg-secondary\">-</span>";
    std::stack<std::vector<std::string> > stS;
    std::vector<std::string> mainL,statusLine,statusLineDefault;
    mainL.resize(maxNumResInOneRow);
    statusLine.resize(maxNumResInOneRow);
    statusLineDefault.resize(maxNumResInOneRow);
    for(long i=0;i<maxNumResInOneRow;++i){
      mainL[i]=std::to_string(i+1);
      statusLineDefault[i]=pillNA;
    }
    long i=0;
    while(i<numR){
      long j=0;
      statusLine=statusLineDefault;
      while((j<maxNumResInOneRow)&&(i<numR)){
        currentPill=pillFail;
        if(rGr.testCasesRes[i].result=="success"){
          currentPill=pillSucc;
        }
        statusLine[j]=currentPill;
        ++j;++i;
      }
      stS.push(statusLine);
    }
    stS.push(mainL);
    return HSF::tableFromStack(stS,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string explainErrorMessage(const std::string& rawError, const std::string& language){
    if(language=="cpp"){
      return EMAI::explainCPPErrorMessage(rawError);
    }
    return "";
  }
  std::string prepareErrorForDisplay(const std::string& rawError, const std::string& language){
    std::string res;
    res+=MWII::GL_WI.getDefaultWebText("Autograder Itemize Begin");
    res+=MWII::GL_WI.getDefaultWebText("Autograder Raw Message Begin");
    res+=rawError;
    res+=MWII::GL_WI.getDefaultWebText("Autograder Raw Message End");
    std::string explanation=explainErrorMessage(rawError,language);
    if(explanation!=""){
      res+=MWII::GL_WI.getDefaultWebText("Autograder Explanation Begin");
      res+=explanation;
      res+=MWII::GL_WI.getDefaultWebText("Autograder Explanation End");
    }
    res+=MWII::GL_WI.getDefaultWebText("Autograder Itemize End");
    return res;
  }
  CAGI::GROneTestCase improveTestCaseDisplay(const CAGI::GROneTestCase& in){
    CAGI::GROneTestCase out=in;
    std::string displayInput,displayOutputOfficial,displayOutputUser;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(in.correct,pos,"|inD|","|/inD|");
    if(allD.second==0){return in;}
    out.input=allD.first;
    pos=0; allD=SF::extract(in.correct,pos,"|outD|","|/outD|");
    if(allD.second==0){return in;}
    out.correct=allD.first;
    pos=0; allD=SF::extract(in.output,pos,"|outD|","|/outD|");
    if(allD.second==1){
      out.output=allD.first;
    }
    return out;
  }
  std::string prepareOneTestCase(const CAGI::GROneTestCase& tc, const long& num){
    std::string resultPrepared=MWII::GL_WI.getDefaultWebText("Autgrader One Test Case");
    resultPrepared=SF::findAndReplace(resultPrepared,"_*IDNUMBER*_",std::to_string(num));
    std::string color="text-danger";
    if(tc.result=="success"){
      color="text-success";
    }
    resultPrepared=SF::findAndReplace(resultPrepared,"_*TEXTCOLOR*_",color);
    resultPrepared=SF::findAndReplace(resultPrepared,"_*TESTRESULT*_",tc.result);
    resultPrepared=SF::findAndReplace(resultPrepared,"_*TESTCASEINPUT*_",tc.input);
    resultPrepared=SF::findAndReplace(resultPrepared,"_*TESTCASEOUTPUT*_",tc.output);
    resultPrepared=SF::findAndReplace(resultPrepared,"_*TESTCASECORRECT*_",tc.correct);
    return resultPrepared;
  }
  std::string prepareGrResult(const CAGI::GradingResult & rGr){
    if(rGr.testCasesRes.size()==0){
      std::string errorRes=MWII::GL_WI.getDefaultWebText("Autograding Failed Message Begin");
      if(rGr.errorMessage!=""){
        errorRes+=prepareErrorForDisplay(rGr.errorMessage,rGr.language);
      }
      errorRes+=MWII::GL_WI.getDefaultWebText("Autograding Failed Message End");
      return errorRes;
    }
    long numR=rGr.testCasesRes.size();
    std::string tRes;
    std::string niceRes;
    niceRes+=grResultSummaryTable(rGr);
    niceRes+=MWII::GL_WI.getDefaultWebText("Autograder Itemize Begin");
    for(long i=0;i<numR;++i){
      niceRes+=prepareOneTestCase(improveTestCaseDisplay(rGr.testCasesRes[i]),i+1);
    }
    niceRes+=MWII::GL_WI.getDefaultWebText("Autograder Itemize End");
    return niceRes;
  }
  std::string analyzeError(const std::string &errorMessage, const std::string & language,const std::string & officialShortError){
    std::string res;
    if(GL_AGParameters.compilerErrors!="yes"){
      return officialShortError;
    }
    res=errorMessage;
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(errorMessage,pos,"_eTxt*|_","_/eTxt*|_");
    if(allD.second==1){
      res=allD.first;
    }
    res=EMAI::adjustIfForbiddenWordWasUsed(res);
    res=EMAI::removeFileNames(res,language);
    return res;
  }

}
#endif
