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
  RTI::CodeAutoGraderInfo getAutoGraderCodeData(const std::string &rawTxt, const std::string & offSolution, const std::string & userSolution, const std::string & maxPoints){
    RTI::CodeAutoGraderInfo cInfo;
    cInfo.officialSource=lastCode(offSolution);
    cInfo.userSource=lastCode(userSolution);
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(rawTxt,pos,"_language_","_/language_");
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
    pos=0;allD=SF::extract(rawTxt,pos,"_embedIntoCode_","_/embedIntoCode_");
    if(allD.second==1){
      cInfo.codeToEmbed=allD.first;
    }

    cInfo.inputTestCases.resize(0);
    pos=0;allD=SF::extract(rawTxt,pos,"_testCases_","_/testCases_");
    if(allD.second==1){
      cInfo.inputTestCases=SF::stringToVector(allD.first,"_n*_","_/n*_");
    }

    cInfo.includes.resize(0);
    pos=0;allD=SF::extract(rawTxt,pos,"_includes_","_/includes_");
    if(allD.second==1){
      cInfo.includes=SF::stringToVector(allD.first,"_n*_","_/n*_");
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
      pos=0;allD=SF::extract(rawTxt,pos,"_pointsTestCases_","_/pointsTestCases_");
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


    std::string testEmbed=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_","");
    if(cInfo.codeToEmbed!=testEmbed){
      cInfo.officialSource=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_",cInfo.officialSource);
      cInfo.userSource=SF::findAndReplace(cInfo.codeToEmbed,"_*embedHere*_",cInfo.userSource);
    }

    return cInfo;

  }
  int sufficientlyEqual(const std::string & _a, const std::string &_b){
    if(TWDVF::eraseLeadingAndTrailingEmptyCharacters(_a)==TWDVF::eraseLeadingAndTrailingEmptyCharacters(_b)){
      return 1;
    }
    return 0;
  }
  std::string calculateScore(const std::vector<std::string> &officialOutput, const std::vector<std::string> &userOutput, const std::vector<double> &pts){
    std::string res="badScore";
    long sz=officialOutput.size();
    if(sz!=userOutput.size()){
      return res;
    }
    if(sz!=pts.size()){
      return res;
    }
    double total=0.0;
    long indicatorOfficialSolutionBad=0;
    long i=0;
    while((i<sz) && (indicatorOfficialSolutionBad==0)){
      if(officialOutput[i] != SF::findAndReplace(officialOutput[i],"Error:OutputTooBig","")){
        indicatorOfficialSolutionBad=1;
      }
      if(officialOutput[i] != SF::findAndReplace(officialOutput[i],"Error:DidNotCompile","")){
        indicatorOfficialSolutionBad=1;
      }
      if(officialOutput[i] ==""){
        indicatorOfficialSolutionBad=1;
      }
      if( sufficientlyEqual(officialOutput[i],userOutput[i])==1 ){
        total+=pts[i];
      }
      ++i;
    }
    if(indicatorOfficialSolutionBad==1){
      total=-100.0;
    }
    return BF::eraseTrailingZeros(std::to_string(total));
  }
  int codeAutoGradeAndUpdateMap(const PSDI::SessionData & _psd, std::map<std::string,std::string>& oldScores, const std::map<std::string,RTI::CodeAutoGraderInfo> & delayedAG){
    std::map<std::string,RTI::CodeAutoGraderInfo>::const_iterator it,itE;
    long numCodes=delayedAG.size();
    if(numCodes<1){
      return 0;
    }

    std::vector<std::string> sources;
    std::vector<std::string> languages;
    std::vector<std::vector<std::string> >includes;
    std::vector<std::vector<std::string> >inputTestCases;
    std::vector<std::vector<double> > scores;
    std::vector<std::string> labels;
    long twoNC=2*numCodes;
    sources.resize(twoNC);
    languages.resize(twoNC);
    includes.resize(twoNC);
    inputTestCases.resize(twoNC);
    labels.resize(twoNC);
    scores.resize(twoNC);
    long i=0;
    it=delayedAG.begin();
    itE=delayedAG.end();
    while(it!=itE){
      sources[i]=(it->second).userSource;
      languages[i]=(it->second).language;
      includes[i]=(it->second).includes;
      inputTestCases[i]=(it->second).inputTestCases;
      scores[i]=(it->second).pointsTestCases;
      labels[i]=it->first;
      ++i;
      sources[i]=(it->second).officialSource;
      languages[i]=(it->second).language;
      includes[i]=(it->second).includes;
      inputTestCases[i]=(it->second).inputTestCases;
      scores[i]=(it->second).pointsTestCases;
      labels[i]=it->first;
      ++i;
      ++it;
    }
    std::pair<std::vector<std::vector<std::string> >,int> aGOutput=DCEI::executePrograms(_psd,sources,languages,includes,inputTestCases);
    if(aGOutput.second==0){
      return 0;
    }
    if(aGOutput.first.size()!=twoNC){
      return 0;
    }
    i=0;
    std::string score1Problem;
    while(i<numCodes){
      score1Problem=calculateScore(aGOutput.first[2*i+1],aGOutput.first[2*i],scores[2*i]);
      if(score1Problem!="badScore"){
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
  std::string executionResult(const PSDI::SessionData & _psd, const std::string & rawAgr, const std::string & offSolution){
    RTI::CodeAutoGraderInfo cInfo=getAutoGraderCodeData(rawAgr,offSolution,"","100");
    std::vector<std::string> sources,languages;
    std::vector<std::vector<std::string> > includes, inputTestCases;
    sources.resize(1);languages.resize(1);
    includes.resize(1);inputTestCases.resize(1);
    sources[0]=cInfo.officialSource;
    languages[0]=cInfo.language;
    includes[0]=cInfo.includes;
    inputTestCases[0]=cInfo.inputTestCases;
    std::pair<std::vector<std::vector<std::string> >,int> aGOutput=DCEI::executePrograms(_psd, sources,languages,includes,inputTestCases);
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