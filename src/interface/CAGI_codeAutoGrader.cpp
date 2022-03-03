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

#ifndef _INCL_CAGI_CodeAutoGrader_CPP
#define _INCL_CAGI_CodeAutoGrader_CPP
namespace CAGI{
  struct GROneTestCase{
  public:
    std::string result, input, output, correct;
  };
  struct GradingResult{
  public:
    double numericScore;
    std::string score;
    std::string comment,shorterComment;
    std::vector<GROneTestCase> testCasesRes;
  };
  struct DockerCodeCounter{
  public:
    long remainingCodes;
  } GL_Code_Counter;//will be initialized in mainFunction.cpp
  std::string lastCode(const std::string & _input){
    std::string input=SF::findAndReplace(_input,"_code_","<pre>");
    input=SF::findAndReplace(input,"_/code_","</pre>");
    std::vector<std::string> allCodes= SF::stringToVector(input,"<pre>","</pre>");
    long sz=allCodes.size();
    if(sz<1){
      return _input;
    }
    return allCodes[sz-1];
  }
  RTI::CodeAutoGraderInfo getAutoGraderCodeData(const std::string &agParameters, const std::string & offSolution, const std::string & userSolution, const std::string & maxPoints){
    RTI::CodeAutoGraderInfo cInfo;
    cInfo.officialSource=lastCode(offSolution);
    cInfo.userSource=lastCode(userSolution);
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(agParameters,pos,"_language_","_/language_");
    cInfo.language="none";
    if(allD.second==1){
      cInfo.language=allD.first;
      if((cInfo.language=="c++")||(cInfo.language=="C++")||(cInfo.language=="CPP")){
        cInfo.language="cpp";
      }
      if((cInfo.language!="cpp")&&(cInfo.language!="py")){
        cInfo.language="cpp";
      }
    }
    cInfo.codeToEmbed="";
    pos=0;allD=SF::extract(agParameters,pos,"_embedIntoCode_","_/embedIntoCode_");
    if(allD.second==1){
      cInfo.codeToEmbed=allD.first;
    }
    cInfo.messageDidNotCompile="Code did not compile";
    pos=0;allD=SF::extract(agParameters,pos,"_compilerErrorMessage_","_/compilerErrorMessage_");
    if(allD.second==1){
      cInfo.messageDidNotCompile=allD.first;
    }
    cInfo.compilerFlags="-std=c++11";
    pos=0;allD=SF::extract(agParameters,pos,"_compilerFlags_","_/compilerFlags_");
    if(allD.second==1){
      cInfo.compilerFlags=allD.first;
    }
    cInfo.inputTestCases.resize(0);
    pos=0;allD=SF::extract(agParameters,pos,"_testCases_","_/testCases_");
    if(allD.second==1){
      cInfo.inputTestCases=SF::stringToVector(allD.first,"_n*_","_/n*_");
    }
    cInfo.publicTestCases.resize(0);
    pos=0;allD=SF::extract(agParameters,pos,"_publicTestCases_","_/publicTestCases_");
    if(allD.second==1){
      cInfo.publicTestCases=SF::stringToVector(allD.first,"_n*_","_/n*_");
    }
    cInfo.includes.resize(0);
    pos=0;allD=SF::extract(agParameters,pos,"_includes_","_/includes_");
    if(allD.second==1){
      cInfo.includes=SF::stringToVector(allD.first,"_n*_","_/n*_");
    }
    cInfo.forbiddenStrs.resize(0);
    pos=0;allD=SF::extract(agParameters,pos,"_forbidden_","_/forbidden_");
    if(allD.second==1){
      cInfo.forbiddenStrs=SF::stringToVector(allD.first,"_n*_","_/n*_");
    }
    cInfo.pointsTestCases.resize(0);
    long numTestCases=cInfo.inputTestCases.size();
    if(numTestCases>0){
      cInfo.pointsTestCases.resize(numTestCases);
      double maxScore=BF::stringToDouble(maxPoints);
      double scoreByNow=0.0;
      for(long i=0;i<numTestCases;++i){
        cInfo.pointsTestCases[i]=maxScore / static_cast<double>(numTestCases);
        scoreByNow+=cInfo.pointsTestCases[i];
      }
      pos=0;allD=SF::extract(agParameters,pos,"_revealTestCasesAfterGrading_","_/revealTestCasesAfterGrading_");
      if(allD.second==0){
        cInfo.revealTestCasesAfterGrading.resize(numTestCases);
        for(long i=0;i<numTestCases;++i){
          cInfo.revealTestCasesAfterGrading[i]="no";
        }
      }
      else{
        cInfo.revealTestCasesAfterGrading=SF::stringToVector(allD.first,"_n*_","_/n*_");
      }
      pos=0;allD=SF::extract(agParameters,pos,"_pointsTestCases_","_/pointsTestCases_");
      if(allD.second==1){
        std::vector<std::string> ptsTestCases = SF::stringToVector(allD.first,"_n*_","_/n*_");
        if(ptsTestCases.size()==numTestCases){
          scoreByNow=0.0;
          for(long i=0;i<numTestCases;++i){
            cInfo.pointsTestCases[i]=BF::stringToDouble(ptsTestCases[i]);
            scoreByNow+=cInfo.pointsTestCases[i];
          }
        }
      }
      if((maxScore>BF::GLOBAL_EPSILON) && ((scoreByNow-maxScore)*(scoreByNow-maxScore)>BF::GLOBAL_EPSILON)){
        double rescale=maxScore/scoreByNow;
        for(long i=0;i<numTestCases;++i){
          cInfo.pointsTestCases[i] *= rescale;
        }
      }
    }
    cInfo.publicPointsTestCases.resize(0);
    long pNumTC=cInfo.publicTestCases.size();
    if(pNumTC>0){
      cInfo.publicPointsTestCases.resize(pNumTC);
      double maxScore=100.0;
      double scoreByNow=0.0;
      for(long i=0;i<pNumTC;++i){
        cInfo.publicPointsTestCases[i]=maxScore / static_cast<double>(pNumTC);
        scoreByNow+=cInfo.publicPointsTestCases[i];
      }
      cInfo.publicRevealTestCasesAfterGrading.resize(pNumTC);
      for(long i=0;i<pNumTC;++i){
        cInfo.publicRevealTestCasesAfterGrading[i]="yes";
      }
    }
    std::string testEmbed=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_","");
    if(cInfo.codeToEmbed!=testEmbed){
      cInfo.officialSource=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_",cInfo.officialSource);
      cInfo.userSource=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_",cInfo.userSource);
    }
    return cInfo;
  }
  int sufficientlyEqual(const std::string & _a, const std::string &_b){
    std::string a=TWDVF::eraseLeadingAndTrailingEmptyCharacters(_a);
    std::string b=TWDVF::eraseLeadingAndTrailingEmptyCharacters(_b);
    a=SF::findAndReplace(a,"\t"," ");
    b=SF::findAndReplace(b,"\t"," ");
    a=SF::findAndReplace(a,"   "," ");
    b=SF::findAndReplace(b,"   "," ");
    a=SF::findAndReplace(a,"  "," ");
    b=SF::findAndReplace(b,"  "," ");
    if(a==b){
      return 1;
    }
    return 0;
  }
  GradingResult calculateScore(const std::vector<std::string> &officialOutput, const std::vector<std::string> &userOutput, const std::vector<double> &pts, const std::vector<std::string> & inputTestCases, const std::vector<std::string> &revealAfter, const std::string & mDNCompile){
    GradingResult res;
    res.comment="";res.shorterComment="";
    res.numericScore=-100.0;
    res.score="badScore";
    long sz=officialOutput.size();
    if(sz!=userOutput.size()){
      return res;
    }
    if(sz!=pts.size()){
      return res;
    }
    res.testCasesRes.resize(sz);
    GROneTestCase tcRes;
    double total=0.0;
    long indicatorOfficialSolutionBad=0;
    long indicatorDidNotCompile=0;
    long i=0;
    while((i<sz) && (indicatorOfficialSolutionBad==0)){
      if(officialOutput[i] != SF::findAndReplace(officialOutput[i],GF::GL_errorOutputTooBig,"")){
        indicatorOfficialSolutionBad=1;
      }
      if(officialOutput[i] != SF::findAndReplace(officialOutput[i],GF::GL_errorDidNotCompile,"")){
        indicatorOfficialSolutionBad=1;
      }
      if(officialOutput[i] ==""){
        indicatorOfficialSolutionBad=1;
      }
      if(officialOutput[i]=="fileNotFound"){
        indicatorOfficialSolutionBad=1;
      }
      res.comment+="\nTest case "+std::to_string(i+1)+": ";
      tcRes.result="failure";
      if( sufficientlyEqual(officialOutput[i],userOutput[i])==1 ){
        total+=pts[i];
        tcRes.result="success";
      }
      res.comment+=tcRes.result;
      tcRes.input="";tcRes.output="";tcRes.correct="";
      if(revealAfter[i]=="yes"){
        res.comment+="\n  Input: "+inputTestCases[i];
        res.comment+="\n Output: "+userOutput[i];
        res.comment+="\nCorrect: "+officialOutput[i]+"\n";
        tcRes.input=inputTestCases[i];tcRes.output=userOutput[i];tcRes.correct=officialOutput[i];
      }
      if(userOutput[i] != SF::findAndReplace(userOutput[i],GF::GL_errorDidNotCompile,"")){
        indicatorDidNotCompile=1;
      }
      res.testCasesRes[i]=tcRes;
      ++i;
    }
    if(indicatorOfficialSolutionBad==1){
      total=-100.0;
      res.comment="\nProblem with autograder. Please wait a few minutes and try again.";
      res.testCasesRes.resize(0);
    }
    if(indicatorDidNotCompile==1){
      res.comment="\n"+mDNCompile;
      res.testCasesRes.resize(0);
    }
    res.score=BF::eraseTrailingZeros(std::to_string(total));
    res.numericScore=total;
    res.shorterComment=res.comment;
    res.comment="_code_AUTOGRADER COMMENT BEGIN"+res.comment+"\nAUTOGRADER COMMENT END_/code_";
    if(MWII::GL_WI.getDefaultWebText("ShowAutoGraderComment")=="no"){
      res.comment="";
    }
    return res;
  }
  int codeAutoGradeAndUpdateMap(const PSDI::SessionData & _psd, std::map<std::string,GradingResult>& oldScores, const std::map<std::string,RTI::CodeAutoGraderInfo> & delayedAG, const std::string & testCasesType){
    std::map<std::string,RTI::CodeAutoGraderInfo>::const_iterator it,itE;
    long numCodes=delayedAG.size();
    if(numCodes<1){
      return 0;
    }
    std::vector<std::string> sources;
    std::vector<std::string> languages;
    std::vector<std::string> mDidNotCompile;
    std::vector<std::string> cFlags;
    std::vector<std::vector<std::string> >includes;
    std::vector<std::vector<std::string> >forbiddenStrs;
    std::vector<std::vector<std::string> >inputTestCases;
    std::vector<std::vector<std::string> >revealTCAfterGrading;
    std::vector<std::vector<double> > scores;
    std::vector<std::string> labels;
    long twoNC=2*numCodes;
    sources.resize(twoNC);
    languages.resize(twoNC);
    mDidNotCompile.resize(twoNC);
    cFlags.resize(twoNC);
    includes.resize(twoNC);
    forbiddenStrs.resize(twoNC);
    inputTestCases.resize(twoNC);
    revealTCAfterGrading.resize(twoNC);
    labels.resize(twoNC);
    scores.resize(twoNC);
    long i=0;
    it=delayedAG.begin();
    itE=delayedAG.end();
    while(it!=itE){
      sources[i]=(it->second).userSource;
      languages[i]=(it->second).language;
      includes[i]=(it->second).includes;
      forbiddenStrs[i]=(it->second).forbiddenStrs;
      if(testCasesType=="publicTestCases"){
        inputTestCases[i]=(it->second).publicTestCases;
        revealTCAfterGrading[i]=(it->second).publicRevealTestCasesAfterGrading;
        scores[i]=(it->second).publicPointsTestCases;
      }
      else{
        inputTestCases[i]=(it->second).inputTestCases;
        revealTCAfterGrading[i]=(it->second).revealTestCasesAfterGrading;
        scores[i]=(it->second).pointsTestCases;
      }
      mDidNotCompile[i]=(it->second).messageDidNotCompile;
      cFlags[i]=(it->second).compilerFlags;
      labels[i]=it->first;
      ++i;
      sources[i]=(it->second).officialSource;
      languages[i]=(it->second).language;
      includes[i]=(it->second).includes;
      forbiddenStrs[i]=(it->second).forbiddenStrs;
      if(testCasesType=="publicTestCases"){
        inputTestCases[i]=(it->second).publicTestCases;
        revealTCAfterGrading[i]=(it->second).publicRevealTestCasesAfterGrading;
        scores[i]=(it->second).publicPointsTestCases;
      }
      else{
        inputTestCases[i]=(it->second).inputTestCases;
        revealTCAfterGrading[i]=(it->second).revealTestCasesAfterGrading;
        scores[i]=(it->second).pointsTestCases;
      }
      mDidNotCompile[i]=(it->second).messageDidNotCompile;
      cFlags[i]=(it->second).compilerFlags;
      labels[i]=it->first;
      ++i;
      ++it;
    }
    std::pair<std::vector<std::vector<std::string> >,int> aGOutput=DCEI::executePrograms(_psd,sources,languages,cFlags,includes,forbiddenStrs,inputTestCases);
    if(aGOutput.second==0){
      return 0;
    }
    if(aGOutput.first.size()!=twoNC){
      return 0;
    }
    i=0;
    GradingResult score1Problem;
    while(i<numCodes){
      score1Problem=calculateScore(aGOutput.first[2*i+1],aGOutput.first[2*i],scores[2*i],inputTestCases[2*i],revealTCAfterGrading[2*i],mDidNotCompile[2*i]);
      if(score1Problem.score!="badScore"){
        oldScores[labels[2*i]]=score1Problem;
      }
      ++i;
    }
    return 1;
  }
  std::string wrongLengthsOfVectors(){
    std::string fR="";
    fR+="<p>Error:</p>";
    fR+="<p>One of the following error has occured:</p>";
    fR+="<ul>";
    fR+="<li>Error in the source code.</li>";
    fR+="<li>Programming language is not correctly specified.</li>";
    fR+="<li>Libraries that need to be included are not correctly specified.</li>";
    fR+="<li>Test cases are not correctly specified.</li>";
    fR+="</ul>";
    return fR;
  }
  std::string presentationOfResultsOfTests(const std::vector<std::string> &testCases,const std::vector<std::string> &results){
    std::string fR="<p>Error: Test cases were incorrectly specified.</p>";
    long sz=results.size();
    if(testCases.size()!=sz){
      return fR;
    }
    if(sz<1){
      return fR;
    }
    fR="";
    fR+="<ul>";
    for(long i=0;i<sz;++i){
      fR+="<li>";
      fR+="<p><b>Test case "+std::to_string(i+1)+"</b></p>";
      fR+="<p>Input:</p><pre>"+testCases[i]+"</pre>";
      fR+="<p>Output:</p><pre>"+results[i]+"</pre>";
      fR+="</li>";
    }
    fR+="</ul>";
    return fR;
  }
  std::string executionResult(const PSDI::SessionData & _psd, const std::string & agrParameters, const std::string & offSolution){
    RTI::CodeAutoGraderInfo cInfo=getAutoGraderCodeData(agrParameters,offSolution,"","100");
    std::vector<std::string> sources,languages,cFlags;
    std::vector<std::vector<std::string> > includes, forbiddenStrs, inputTestCases;
    sources.resize(1);languages.resize(1);cFlags.resize(1);
    includes.resize(1);forbiddenStrs.resize(1);inputTestCases.resize(1);
    sources[0]=cInfo.officialSource;
    languages[0]=cInfo.language;
    cFlags[0]=cInfo.compilerFlags;
    includes[0]=cInfo.includes;
    forbiddenStrs[0]=cInfo.forbiddenStrs;
    inputTestCases[0]=cInfo.inputTestCases;
    std::pair<std::vector<std::vector<std::string> >,int> aGOutput=DCEI::executePrograms(_psd, sources,languages,cFlags,includes,forbiddenStrs,inputTestCases);
    std::string fR="<h4>Code execution on test cases</h4>";
    if((aGOutput.second==0)||(aGOutput.first.size()!=1)){
      fR+=wrongLengthsOfVectors();
      return fR;
    }
    fR+=presentationOfResultsOfTests(inputTestCases[0],aGOutput.first[0]);
    return fR;
  }
}
#endif
