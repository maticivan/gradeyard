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

#ifndef _INCL_DISPLAYPREPARATIONS_CPP
#define _INCL_DISPLAYPREPARATIONS_CPP
namespace DISPPF{
  void dollarsToLatexSymbols(std::string & output,const long &singleDollarsAlso  =1){
    std::string azm=GF::GL_HIDING_STRING_DISPPF01;
    std::vector<std::string> w2El;
    w2El.resize(2);
    output=SF::findAndReplace(output,"\\$",azm);
    w2El[0]="\\[";w2El[1]="\\]";
    output=SF::findAndReplace(output,"$$",w2El);
    if(singleDollarsAlso==1){
      w2El[0]="\\(";w2El[1]="\\)";
      output=SF::findAndReplace(output,"$",w2El);
    }
    output=SF::findAndReplace(output,azm,"\\$");
  }
  int removeBoxCodeToSafety(PTKF::PlainTextKeeper &kc, std::string &t,const std::string & s_B="_codeBox_",const std::string & s_E="_/codeBox_"){
    //returns 1 if everything is removed properly
    // returns 0 if there is an open tag or a tag inside a tag
    int correct=1;
    long pos;
    std::pair<std::string,int> allD;
    std::string ttd, dRec;
    pos=0;
    allD=SF::extract(t,pos,s_B,s_E);
    while(allD.second==1){
      ttd=s_B+allD.first+s_E;
      dRec=kc.depositTxt(ttd);
      t=SF::findAndReplace(t,ttd,dRec,0);
      pos=0;
      allD=SF::extract(t,pos,s_B,s_E);
    }
    return correct;
  }
  class HTML_Tags{
  private:
    std::string azm=GF::GL_HIDING_STRING_DISPPF02;
  public:
    std::vector<std::string> formattingTags;
    std::vector<std::string> toEliminateFromSimpleTexts;
    std::vector<std::string> alakazams;
    std::map<std::string,std::string> tagRemovalMap;
    HTML_Tags();
  };
  HTML_Tags::HTML_Tags(){
    long i;
    formattingTags.resize(25);
    i=-1;
    ++i;formattingTags[i]="<i>";
    ++i;formattingTags[i]="</i>";
    ++i;formattingTags[i]="<b>";
    ++i;formattingTags[i]="</b>";
    ++i;formattingTags[i]="<u>";
    ++i;formattingTags[i]="</u>";
    ++i;formattingTags[i]="<ul>";
    ++i;formattingTags[i]="</ul>";
    ++i;formattingTags[i]="<li>";
    ++i;formattingTags[i]="</li>";
    ++i;formattingTags[i]="<p>";
    ++i;formattingTags[i]="</p>";
    ++i;formattingTags[i]="<title>";
    ++i;formattingTags[i]="</title>";
    ++i;formattingTags[i]="<h1>";
    ++i;formattingTags[i]="</h1>";
    ++i;formattingTags[i]="<h2>";
    ++i;formattingTags[i]="</h2>";
    ++i;formattingTags[i]="<h3>";
    ++i;formattingTags[i]="</h3>";
    ++i;formattingTags[i]="<h4>";
    ++i;formattingTags[i]="</h4>";
    ++i;formattingTags[i]="<h5>";
    ++i;formattingTags[i]="</h5>";
    ++i;formattingTags[i]="<br>";
    long sz=formattingTags.size();
    alakazams.resize(sz);
    for(long i=0;i<sz;++i){
      alakazams[i]=azm+std::to_string(i)+"e|";
    }
    toEliminateFromSimpleTexts.resize(21);
    i=-1;
    ++i;toEliminateFromSimpleTexts[i]="<div>";
    ++i;toEliminateFromSimpleTexts[i]="</div>";
    ++i;toEliminateFromSimpleTexts[i]="<span>";
    ++i;toEliminateFromSimpleTexts[i]="</span>";
    ++i;toEliminateFromSimpleTexts[i]="<code>";
    ++i;toEliminateFromSimpleTexts[i]="</code>";
    ++i;toEliminateFromSimpleTexts[i]="<pre>";
    ++i;toEliminateFromSimpleTexts[i]="</pre>";
    ++i;toEliminateFromSimpleTexts[i]="\n";
    ++i;toEliminateFromSimpleTexts[i]="$";
    ++i;toEliminateFromSimpleTexts[i]="\\left";
    ++i;toEliminateFromSimpleTexts[i]="\\right";
    ++i;toEliminateFromSimpleTexts[i]="\\bigg";
    ++i;toEliminateFromSimpleTexts[i]="begin{eqnarray}";
    ++i;toEliminateFromSimpleTexts[i]="begin{eqnarray*}";
    ++i;toEliminateFromSimpleTexts[i]="end{eqnarray}";
    ++i;toEliminateFromSimpleTexts[i]="end{eqnarray*}";
    ++i;toEliminateFromSimpleTexts[i]="begin{align}";
    ++i;toEliminateFromSimpleTexts[i]="begin{align*}";
    ++i;toEliminateFromSimpleTexts[i]="end{align}";
    ++i;toEliminateFromSimpleTexts[i]="end{align*}";
    for(long j=0;j<formattingTags.size();++j){
      tagRemovalMap[formattingTags[j]]="";
    }
    for(long j=0;j<toEliminateFromSimpleTexts.size();++j){
      tagRemovalMap[toEliminateFromSimpleTexts[j]]="";
    }
  }
  HTML_Tags GL_HTML_Tags;
  std::string treatHideReveal(const std::string & _input){
    std::string output=_input;
    long pos,posToSave;
    std::pair<std::string,int> allD;
    std::string revT,hideT,text,nt,oldText;
    long counter=0;std::string counterSt;
    pos=0;allD=SF::extract(output,pos,"_hideReveal_","_/hideReveal_");
    while(allD.second==1){
      text=allD.first;
      oldText="_hideReveal_"+text+"_/hideReveal_";
      posToSave=pos;
      posToSave-=text.size();
      if(posToSave<0){
        posToSave=0;
      }
      pos=0;allD=SF::extract(text,pos,"_hideTitle_","_/hideTitle_");
      hideT="Hide";
      if(allD.second==1){
        hideT=allD.first;
      }
      pos=0;allD=SF::extractAndReplace(text,pos,"_hideTitle_","_/hideTitle_");
      if(allD.second==1){
        text=allD.first;
      }
      pos=0;allD=SF::extract(text,pos,"_revealTitle_","_/revealTitle_");
      revT="Show";
      if(allD.second==1){
        revT=allD.first;
      }
      pos=0;allD=SF::extractAndReplace(text,pos,"_revealTitle_","_/revealTitle_");
      if(allD.second==1){
        text=allD.first;
      }
      counterSt=std::to_string(counter);
      nt="<div id=\"someText3"+counterSt+"\">\n";
      nt+="<a id=\"headerL3"+counterSt+"\" href=\"javascript:toggle(&#39;mCont3"+counterSt+"&#39;, ";
      nt+="&#39;hDiv3"+counterSt+"&#39;, &#39;"+revT+"&#39;, &#39;"+hideT+"&#39; ) ;\" >";
      nt+="<button type=\"button\" class=\"btn btn-outline-dark btn-sm\">";
      nt+="<span id=\"hDiv3";
      nt+=counterSt+"\">"+revT+"</span></button></a>\n";
      nt+="</div><div id=\"mCont3"+counterSt+"\" class=\"card\" style=\"background-color:#e9ebf3; display: none;\">";
      nt+="<div class=\"card-body\">";
      nt+=text+"</div></div>";
      output=SF::findAndReplace(output,oldText,nt);
      pos=posToSave;
      pos=0;
      allD=SF::extract(output,pos,"_hideReveal_","_/hideReveal_");
      ++counter;
    }
    return output;
  }
  std::string formattingCommands(const std::string & _input){
    std::string output=_input;
    output=treatHideReveal(output);
    return output;
  }
  struct RequestsForSanitizer{
  public:
    long convertDollarsToLatex;
    long htmlTolerance;
    long exitWithErrorIfUnsafe;
    RequestsForSanitizer();
  };
  RequestsForSanitizer::RequestsForSanitizer(){
    convertDollarsToLatex=0;
    htmlTolerance=0;
    exitWithErrorIfUnsafe=1;
  }
  std::string sanitizeForDisplay(const std::string & _t, const RequestsForSanitizer & rs){
  //rs.htmlTolerance:
  // 0 - no html codes
  //     Outside of math and code blocks every < is replaced by \(<\)
  //     and every > is replaced by \(>\))

  // 1 - allow formatting tags.  WARNING -- not implemented fully yet.

  // 2 - allow all tags except for dangerous ones
    PTKF::PlainTextKeeper keeperOfVerbatim("vb0b");
    PTKF::PlainTextKeeper keeperOfCodesB("sc0b");
    PTKF::PlainTextKeeper keeperOfBoxCodes("sc0bb");
    PTKF::PlainTextKeeper keeperOfCodesS("sc0s");
    PTKF::PlainTextKeeper keeperOfPresB("p0b");
    PTKF::PlainTextKeeper keeperOfPresS("p0s");
    std::string t=_t;
    std::string unsafe="Latex Error ";
    int indicatorSafety;
    indicatorSafety=PTKF::removeToSafety(keeperOfVerbatim,t,"_verbatim_","_/verbatim_");
    if((1-indicatorSafety)* (rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 1";
    }
    indicatorSafety=PTKF::removeToSafety(keeperOfCodesB,t,"_code_","_/code_");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 2";
    }
    indicatorSafety=PTKF::removeToSafety(keeperOfPresB,t,"<pre>","</pre>");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 2";
    }
    indicatorSafety=PTKF::removeToSafety(keeperOfPresS,t,"<code>","</code>");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 2";
    }
    indicatorSafety=removeBoxCodeToSafety(keeperOfBoxCodes,t,"_codeBox_","_/codeBox_");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 4";
    }
    indicatorSafety=PTKF::removeToSafety(keeperOfCodesS,t,"_cde_","_/cde_");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 3";
    }
    keeperOfVerbatim.treatCDE("_verbatim_","_/verbatim_");
    keeperOfCodesB.treatCODE();
    keeperOfCodesS.treatCDE();
    keeperOfPresB.treatPre("<pre>","</pre>");
    keeperOfPresS.treatPre("<code>","</code>");
    keeperOfBoxCodes.treatBoxCode();
    if(rs.convertDollarsToLatex==1){
      std::vector<std::string>ws;
      ws.resize(2);
      ws[0]="1";ws[1]="2";
      dollarsToLatexSymbols(t);
    }
    std::string tForTesting1=t;
    tForTesting1=SF::findAndReplace(tForTesting1,"_/comment_"," _/comment_");
    if((MFRF::find(tForTesting1,GF::GL_DANGERS.strings).second>-1)&&(rs.exitWithErrorIfUnsafe==1)){
      return "unsafeInput";
    }
    t=MFRF::findAndReplace(t,GF::GL_DANGERS.veryBadStringsToBeErased);
    PTKF::PlainTextKeeper mth1("m01");
    indicatorSafety=PTKF::removeToSafety(mth1,t,"\\(","\\)");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 4 ";
    }
    PTKF::PlainTextKeeper mth2("m02");
    indicatorSafety=PTKF::removeToSafety(mth2,t,"\\[","\\]");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"5";
    }
    PTKF::PlainTextKeeper mth3("m03");
    indicatorSafety=PTKF::removeToSafety(mth3,t,"\\begin{eqnarray*}","\\end{eqnarray*}");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"6";
    }
    PTKF::PlainTextKeeper mth4("m04");
    indicatorSafety=PTKF::removeToSafety(mth4,t,"\\begin{eqnarray}","\\end{eqnarray}");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"7";
    }
    PTKF::PlainTextKeeper mth5("m05");
    indicatorSafety=PTKF::removeToSafety(mth5,t,"\\begin{equation}","\\end{equation}");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"8";
    }
    PTKF::PlainTextKeeper mth6("m06");
    indicatorSafety=PTKF::removeToSafety(mth6,t,"\\begin{align*}","\\end{align*}");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"9";
    }
    PTKF::PlainTextKeeper mth7("m07");
    indicatorSafety=PTKF::removeToSafety(mth7,t,"\\begin{align}","\\end{align}");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+"10";
    }
    mth7.treatMath();
    mth6.treatMath();
    mth5.treatMath();
    mth4.treatMath();
    mth3.treatMath();
    mth2.treatMath();
    mth1.treatMath();
    if(rs.htmlTolerance<2){
      if(rs.htmlTolerance==1){
        t=MFRF::findAndReplace(t,GL_HTML_Tags.formattingTags,GL_HTML_Tags.alakazams);
      }
      std::map<std::string,std::string> replMap;
      replMap["<"]="\\(<\\)";
      replMap[">"]="\\(>\\)";
      t=MFRF::findAndReplace(t,replMap);
      if(rs.htmlTolerance==1){
        t=MFRF::findAndReplace(t,GL_HTML_Tags.alakazams,GL_HTML_Tags.formattingTags);
      }
    }
    t=mth7.recover(t);
    t=mth6.recover(t);
    t=mth5.recover(t);
    t=mth4.recover(t);
    t=mth3.recover(t);
    t=mth2.recover(t);
    t=mth1.recover(t);
    t=formattingCommands(t);
    t=keeperOfCodesS.recover(t);
    t=keeperOfPresS.recover(t);
    t=keeperOfPresB.recover(t);
    t=keeperOfCodesB.recover(t);
    t=keeperOfVerbatim.recover(t);
    t=keeperOfBoxCodes.recover(t);
    return t;
  }
  std::string finalizeForDisplay(const std::map<std::string,std::string> &findReplacePairs, const std::string & _t){
    std::string t=MFRF::findAndReplace(_t,findReplacePairs);
    t=LNF::labelsAndNumbers(t);
    RequestsForSanitizer reqS;
    reqS.convertDollarsToLatex=1;
    reqS.htmlTolerance=2;
    reqS.exitWithErrorIfUnsafe=0;
    std::string fR= PTKF::GL_PLAINTEXT_KEEPER.recover(sanitizeForDisplay(t,reqS));
    fR=MFRF::findAndReplace(fR,HCBF::verySafePlace,1,-1);
    return fR;
  }
  long checkForHTMLAwareness(const std::string &input){
    std::set<std::string> tagsToTest;
    tagsToTest.insert("<br>");
    tagsToTest.insert("<p>");
    tagsToTest.insert("<div>");
    tagsToTest.insert("<b>");
    if(MFRF::find(input,tagsToTest).second>-1){
      return 1;
    }
    return 0;
  }
  std::string prepareForHTMLDisplay(const std::string &_st){
    std::string output=_st;
    RequestsForSanitizer reqS;
    reqS.convertDollarsToLatex=1;
    reqS.htmlTolerance=1;
    reqS.exitWithErrorIfUnsafe=1;
    output=sanitizeForDisplay(output,reqS);
    return output;
  }
  std::string prepareForHTMLDisplayTextInputField(const std::string& in){
    return prepareForHTMLDisplay(in);
  }
}
#endif
