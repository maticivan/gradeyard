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


#ifndef _INCL_AICD_ABSTRACT_INSERT_CENTER_CPP
#define _INCL_AICD_ABSTRACT_INSERT_CENTER_CPP


namespace AICD{
  std::string prepareLatexText(const std::string& , const long& =10);
  std::string removeTag(const std::string& _in, const std::string &tOpen, const std::string &tClose){
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extractAndReplace(_in,pos,tOpen,tClose,0,"");
    if(allD.second==1){
      return allD.first;
    }
    return _in;
  }
  std::string rawTextData(const std::string& textName){
    TMD::MText sf;
    int sc=sf.setFromTextName(textName);
    if(sc==1){
      return sf.getTextData();
    }
    return "";
  }
  std::string createSubText(const std::string& textName, const long& remRecDepth){
    if(remRecDepth<1){return "";}
    return prepareLatexText(rawTextData(textName),remRecDepth-1);
  }
  std::string extractFSOfProblem(const std::string &probName, const std::string& versionLabel,const std::string & oTag, const std::string &cTag){
    std::string correctVersion=TWDVF::singleVersion(rawTextData(probName),BF::stringToInteger(versionLabel));
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(correctVersion,pos,oTag,cTag);
    if(allD.second==1){
      return allD.first;
    }
    return "";
  }
  std::string evaluateLatexInsert(const std::string& _insText, const long& remRecDepth){
    long pos; std::pair<std::string,int> allD;
    std::vector<std::string> allArgs=SF::stringToVector(_insText,"_n*_","_/n*_");
    long sz=allArgs.size();
    if(sz>0){
      if((allArgs[0]=="text")&&(sz==2)){
        return createSubText(allArgs[1],remRecDepth);
      }
      if((allArgs[0]=="codeTestInNotes")){
        if(sz>2){
          return extractFSOfProblem(allArgs[1],allArgs[2],"_tx*|_","_/tx*|_");
        }
        else{
          return "";
        }
      }
      if((allArgs[0]=="buttonLink")){
        if((sz>2)&&(SF::findAndReplace(allArgs[2],"?page=ctsInL","")!=allArgs[2])){
          pos=0;allD=SF::extract(allArgs[2],pos,"&el0=","&");
          if(allD.second==1){
            std::string pName=allD.first;
            pos=0;allD=SF::extract(allArgs[2],pos,"&el0=","&");
            return "\n\\begin{solution}\n"+extractFSOfProblem(pName,allD.first,"_sl*|_","_/sl*|_")+"\n\\end{solution}\n";
          }
          return "";
        }
      }
      /*
      if((allArgs[0]==s_codeTest) && (sz==4)){
        return createCodeTest(_psd,allArgs[1],allArgs[2], allArgs[3]);
      }
      if((allArgs[0]==s_codeTestInNotes) && (sz==4)){
        return createCodeTestInNotes(_psd,allArgs[1],allArgs[2],allArgs[3]);
      }
      if((allArgs[0]==s_message)&&(sz==2)){
        return createMessageDisplay(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_invitationToSolve)&&(sz==4)){
        return createSolvingInvitation(_psd,allArgs[1],allArgs[2],allArgs[3]);
      }
      if((allArgs[0]==s_couas)&&(sz==2)){
        return createCouasDisplay(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_respRecStatus)&&(sz==2)){
        return createRespRecStatusDisplay(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_internalLink)&&(sz==3)){
        return createInternalLink(_psd,allArgs[1],allArgs[2]);
      }
      if((allArgs[0]==s_buttonLink)&&(sz==3)){
        return HSF::createButtonLink(allArgs[2],allArgs[1]);
      }
      if((allArgs[0]==s_itemTable)&&(sz==2)){
        return createItemTable(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_textAreaReqField)&&(sz==5)){
        return createTextAreaField(_psd,allArgs[1],allArgs[2],allArgs[3],allArgs[4]);
      }
      if((allArgs[0]==s_textAreaReqField)&&(sz==7)){
        return createTextAreaField(_psd,allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5],allArgs[6]);
      }
      if((allArgs[0]==s_textInputReqField)&&(sz==6)){
        if( (allArgs[1]=="reqBC") && (allArgs[2]=="question") ){
          std::string possibleReplacement=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"question","notFound");
          if(possibleReplacement!="notFound"){
            allArgs[4]=possibleReplacement;
          }
        }
        return createTextInputField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5]);
      }
      if((allArgs[0]==s_radioButtonsField)&&(sz==7)){
        return createRadioButtonsField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5],allArgs[6]);
      }
      if((allArgs[0]==s_antiSpamChallengeField)&&((sz==5)||(sz==6))){
        if(_psd.my_un!="visitor"){return "";}
        std::string _ln="8";
        if(sz==6){
          _ln=allArgs[5];
        }
        return createAntiSpamField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],_ln);
      }
      if((allArgs[0]==s_fileReqField)&&(sz==4)){
        return createFileRequestField(allArgs[1],allArgs[2],allArgs[3]);
      }
      if((allArgs[0]==s_formInitialization)&&(sz==3)){
        return initializeForm(allArgs[1],allArgs[2]);
      }
      if((allArgs[0]==s_formPlacement)&&(sz==2)){
        return placeFormInText(allArgs[1]);
      }
      if((allArgs[0]==s_formPlacement)&&(sz==4)){
        return placeFormInText(allArgs[1],allArgs[2],allArgs[3]);
      }
      if((allArgs[0]==s_userPermits)&&(sz==2)){
        return createUserPermitInfo(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_formInsertRedirect)&&(sz==2)){
        return setRedirectInfo(allArgs[1]);
      }
      if((allArgs[0]==s_statAnalysis)&&(sz==2)){
        return createStatAnalysisPage(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_logInLink)&&(sz==1)){
        return createLogInLink();
      }
      if((allArgs[0]==s_cloneInvitation)&&(sz==1)){
        return createCloneInvitation(_psd);
      }
      if((allArgs[0]==s_cRestore) && (sz==2)){
        return createRestoreCommand(_psd,allArgs[1]);
      }
      if((allArgs[0]==s_listFromDB)&&(sz==4)){
        return createListFromDB(_psd,allArgs[1],allArgs[2],allArgs[3]);
      }
      if((allArgs[0]==s_answerToQuery)&&(sz==1)){
        return createAnswerToTheQuery(_psd);
      }
      */
    }
    return "";
  }
  class LatexSource{
  public:
    std::string evaluateInsert(const std::string &, const std::string &, const std::string &, const std::string &, const long& =10) const;
  };
  std::string LatexSource::evaluateInsert(const std::string & a, const std::string& b, const std::string &c, const std::string & d, const long & remRecDepth) const{
    return evaluateLatexInsert(b,remRecDepth);
  }
  template<typename CALLING_OBJECT, typename TTT>
  std::string treatGeneralInsert(CALLING_OBJECT& cObj, const TTT &_psd, const std::string & _raw,const std::string & _iB, const std::string & _iE){
     std::string fR="",replacement;
     std::string tempSt;
     long pos=0,lastPos=0,posIn;
     std::pair<std::string,int> allD,allDIn;
     allD=SF::extract(_raw,pos,_iB,_iE);
     while(allD.second==1){
       tempSt="";
       for(long i=lastPos;i<pos;++i){
         tempSt+=_raw[i];
       }
       posIn=0;
       replacement=cObj.evaluateInsert(_psd, allD.first,_iB,_iE);
       allDIn=SF::extractAndReplace(tempSt,posIn,_iB,_iE,0,replacement);
       if(allDIn.second==1){
         fR+=allDIn.first;
       }
       lastPos=pos;
       allD=SF::extract(_raw,pos,_iB,_iE);
     }
     pos=_raw.length();
     for(long i=lastPos;i<pos;++i){
       fR+=_raw[i];
     }
     return fR;
  }
  std::string htmlHTagsToLatex(const std::string & in){
    std::string out=in;
    out=SF::findAndReplace(out,"<h1>","\\chapter{");
    out=SF::findAndReplace(out,"</h1>","}");
    out=SF::findAndReplace(out,"<h2>","\\section*{");
    out=SF::findAndReplace(out,"</h2>","}");
    out=SF::findAndReplace(out,"<h3>","\\subsection*{");
    out=SF::findAndReplace(out,"</h3>","}");
    out=SF::findAndReplace(out,"<h4>","\\subsubsection*{");
    out=SF::findAndReplace(out,"</h4>","}");
    out=SF::findAndReplace(out,"<h5>","\\subsubsection*{");
    out=SF::findAndReplace(out,"</h5>","}");
    return out;
  }
  std::string additionalCustomizationForProblemsAndSolutions(const std::string& in){
    std::string out=in;
    out=SF::findAndReplace(out,"\\begin{problem}","\n\\noindent{\\bf Problem} ");
    out=SF::findAndReplace(out,"\\end{problem}","\n");
    out=SF::findAndReplace(out,"\\begin{solution}","\n\\noindent{\\bf Solution.} ");
    out=SF::findAndReplace(out,"\\end{solution}","\n");
    out=SF::findAndReplace(out,"\\begin{box}","\\begin{tcolorbox}\n");
    out=SF::findAndReplace(out,"\\end{box}","\n\\end{tcolorbox}");
    return out;
  }
  std::string prepareLatexText(const std::string& _in, const long& insertRemainingDepth ){
    std::string res=_in;
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(res,pos,"_textData!!_","_/textData!!_");
    if(allD.second==1){
      res=allD.first;
    }
    res=removeTag(res,"_title*_","_/title*_");
    res=removeTag(res,"_metaDesc*_","_/metaDesc*_");
    pos=0; allD=SF::extract(res,pos,"<div class=\"col-md-9\">", "</div>");
    if(allD.second==1){
      res=allD.first;
    }
    LatexSource ls;
    std::string placeHolderWithIrrelevantValue;
    res= treatGeneralInsert(ls,placeHolderWithIrrelevantValue, res,"_insert_","_/insert_");
    res=LNF::labelsAndNumbers(res);
    res=LNF::labelsAndNumbers(res);
    res=LMF::htmlToLatexFormatting(res);
    res=htmlHTagsToLatex(res);
    res=additionalCustomizationForProblemsAndSolutions(res);
    return res;
  }

}
#endif
