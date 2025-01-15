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


#ifndef _INCL_AICD_ABSTRACT_INSERT_CENTER_CPP
#define _INCL_AICD_ABSTRACT_INSERT_CENTER_CPP

namespace AICD{
  struct LatexReplacements{
    std::string websiteURL;
    std::vector<std::string> htmlTs;
    std::vector<std::string> latexTs;
  } GL_ReplStrings;
  std::string prepareLatexTextRec(const std::string& , const long& =10);
  std::string removeTag(const std::string& _in, const std::string &tOpen, const std::string &tClose){
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extractAndReplace(_in,pos,tOpen,tClose,0,"");
    if(allD.second==1){
      return allD.first;
    }
    return _in;
  }
  std::string createSubText(const std::string& textName, const long& remRecDepth){
    if(remRecDepth<1){return "";}
    return prepareLatexTextRec(TMD::rawTextData(textName),remRecDepth-1);
  }
  std::string extractFSOfProblem(const std::string &probName, const std::string& versionLabel,const std::string & oTag, const std::string &cTag){
    std::string correctVersion=TWDVF::singleVersion(TMD::rawTextData(probName),BF::stringToInteger(versionLabel));
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(correctVersion,pos,oTag,cTag);
    if(allD.second==1){
      return allD.first;
    }
    return "";
  }
  std::string createLatexButtonLink(const std::string &uRL, const std::string &label){
      return "\n\n\\verb@"+uRL+"@";
  }
  std::string createLatexInternalLink(const std::string & lN, const std::string & pageName){
    std::string link="\n\n\\verb@";
    link+=GL_ReplStrings.websiteURL+"/index.cgi?page="+pageName;
    link+="@";
    return link;
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
          return createLatexButtonLink(allArgs[2],allArgs[1]);
        }
        return "";
      }
      if((allArgs[0]=="internalLink")&&(sz==3)){
        return createLatexInternalLink(allArgs[1],allArgs[2]);
      }
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
    std::map<std::string,std::string> replMap;
    replMap["<h1>"]="\\chapter{";
    replMap["</h1>"]="}";
    replMap["<h2>"]="\\section*{";
    replMap["</h2>"]="}";
    replMap["<h3>"]="\\subsection*{";
    replMap["</h3>"]="}";
    replMap["<h4>"]="\\subsubsection*{";
    replMap["</h4>"]="}";
    replMap["<h5>"]="\\subsubsection*{";
    replMap["</h5>"]="}";
    return MFRF::findAndReplace(in,replMap);
  }
  std::string additionalCustomizationForProblemsAndSolutions(const std::string& in){
    long sz=GL_ReplStrings.htmlTs.size();
    if(sz!=GL_ReplStrings.latexTs.size()){
      return in;
    }
    std::map<std::string,std::string> replMap;
    for(long i=0;i<sz;++i){
      if(GL_ReplStrings.htmlTs[i]!=GL_ReplStrings.latexTs[i]){
        replMap[GL_ReplStrings.htmlTs[i]]=GL_ReplStrings.latexTs[i];
      }
    }
    return MFRF::findAndReplace(in,replMap);
  }
  std::string h1Tag(const std::string& _in){
    std::vector<std::string> v=SF::stringToVector(_in,"<h1>","</h1>");
    if(v.size()<1){return "";}
    return "<h1>"+v[0]+"</h1>";
  }
  std::string treatHRTag(const std::string& _in){
    std::vector<std::string> v=SF::stringToVectorSimpleSeparator(_in,"<hr>");
    if(v.size()!=3){return _in;}
    return v[0]+h1Tag(v[1])+v[2];
  }
  std::string prepareLatexTextRec(const std::string& _in, const long& insertRemainingDepth ){
    std::string res=_in;
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(res,pos,"_textData!!_","_/textData!!_");
    if(allD.second==1){
      res=allD.first;
    }
    res=removeTag(res,"_title*_","_/title*_");
    res=removeTag(res,"_metaDesc*_","_/metaDesc*_");
    res=treatHRTag(res);
    pos=0; allD=SF::extract(res,pos,"<div class=\"col-md-9\">", "</div>");
    if(allD.second==1){
      res=allD.first;
    }
    LatexSource ls;
    std::string placeHolderWithIrrelevantValue;
    res= treatGeneralInsert(ls,placeHolderWithIrrelevantValue, res,"_insert_","_/insert_");
    return res;
  }
  std::string hyperLinks(const std::string& in){
    std::string out=in;
    std::string urlOnly;std::string linkPart;
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(out,pos,"<a","</a>");
    while(allD.second==1){
      linkPart=allD.first;
      urlOnly="broken url";
      pos=0; allD=SF::extract(linkPart,pos,"href=\"","\"");
      if(allD.second==1){
        urlOnly=allD.first;
      }
      pos=0; allD=SF::extractAndReplace(out,pos,"<a","</a>",0,"\n\n\\verb@"+urlOnly+"@");
      if(allD.second==1){
        out=allD.first;
      }
      pos=0; allD=SF::extract(out,pos,"<a","</a>");
    }
    return out;
  }
  std::string prepareLatexText(const std::string& _in, const long& insertRemainingDepth = 10 ){
    std::string res=prepareLatexTextRec(_in,insertRemainingDepth);
    res=LNF::labelsAndNumbers(res);
    res=LMF::htmlToLatexFormatting(res);
    res=htmlHTagsToLatex(res);
    res=additionalCustomizationForProblemsAndSolutions(res);
    res=hyperLinks(res);
    return res;
  }
}
#endif
