//    GradeYard learning management system
//
//    Copyright (C) 2026 Ivan Matic, https://gradeyard.com
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
std::string openOrCloseTag(long& toggleCounter,
                            const std::string& openTag,
                            const std::string& closeTag){
    ++toggleCounter;
    if(toggleCounter % 2 == 1){
        return openTag;
    }
    return closeTag;
}
std::string convertDollarsToLatexDelimiters(const std::string& in){
    std::string out;
    long inlineMathToggle  = 0;
    long displayMathToggle = 0;
    const std::string inlineOpen  = "\\(";
    const std::string inlineClose = "\\)";
    const std::string displayOpen  = "\\[";
    const std::string displayClose = "\\]";
    long i = 0;
    while(i < in.size()){
        if(in[i] == '$'){
            bool escapedDollar = (i > 0) && (in[i-1] == '\\');
            bool displayMath   = (i < in.size()-1) && (in[i+1] == '$');
            if(escapedDollar){
                out += '$';
            }
            else if(displayMath){
                out += openOrCloseTag(displayMathToggle, displayOpen, displayClose);
                ++i;
            }
            else{
                out += openOrCloseTag(inlineMathToggle, inlineOpen, inlineClose);
            }
        }
        else{
            out += in[i];
        }
        ++i;
    }
    return out;
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
    //std::vector<std::string> alakazams;
    std::map<std::string,std::string> tagRemovalMap;
    HTML_Tags();
  };
  HTML_Tags::HTML_Tags(){
    long i;
    formattingTags.resize(26);
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
    ++i;formattingTags[i]="</a>";
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
      std::string dnchaB="_doNotChangeAlphabet*_";
      std::string dnchaE="_/doNotChangeAlphabet*_";
      if(GF::GL_Alphabet=="english"){
        dnchaB=""; dnchaE="";
      }
      std::string t=_t;
      PTKF::PlainTextKeeper keeperOfBoxCodes("sc0bb");
    std::string unsafe="Latex Error ";
    int indicatorSafety;
    indicatorSafety=removeBoxCodeToSafety(keeperOfBoxCodes,t,"_codeBox_","_/codeBox_");
    if((1-indicatorSafety)*(rs.exitWithErrorIfUnsafe)==1){
      return unsafe+" 4";
    }
    keeperOfBoxCodes.treatBoxCode();
    if(rs.convertDollarsToLatex==1){
        t=convertDollarsToLatexDelimiters(t);
    }
    std::string tForTesting1=t;
    tForTesting1=SF::findAndReplace(tForTesting1,"_/comment_"," _/comment_");
    if((MFRF::find(tForTesting1,GF::GL_DANGERS.strings).second>-1)&&(rs.exitWithErrorIfUnsafe==1)){
      return "unsafeInput";
    }
    t=MFRF::findAndReplace(t,GF::GL_DANGERS.veryBadStringsToBeErased);
      std::vector<std::string> openTags, closeTags;
      std::vector<std::string> openTagReplacements, closeTagReplacements;
      std::vector<std::string> instructions;
      long numTags=12,numMathTags=7;
      if(rs.htmlTolerance<2){++numTags;}
      openTags.resize(numTags);
      closeTags.resize(numTags);
      instructions.resize(numTags);
      for(long i=0;i<numMathTags;++i){
          instructions[i]="math";
      }
      long tagCntr=numMathTags;
      instructions[tagCntr]="pre";++tagCntr;
      instructions[tagCntr]="code";++tagCntr;
      instructions[tagCntr]="pre";++tagCntr;
      instructions[tagCntr]="code";++tagCntr;
      instructions[tagCntr]="code";++tagCntr;
      if(rs.htmlTolerance<2){instructions[tagCntr]="aHref";++tagCntr;}
      tagCntr=0;
      openTags[tagCntr]="\\(";
      closeTags[tagCntr]="\\)";
      ++tagCntr;
      openTags[tagCntr]="\\[";
      closeTags[tagCntr]="\\]";
      ++tagCntr;
      openTags[tagCntr]="\\begin{eqnarray*}";
      closeTags[tagCntr]="\\end{eqnarray*}";
      ++tagCntr;
      openTags[tagCntr]="\\begin{eqnarray}";
      closeTags[tagCntr]="\\end{eqnarray}";
      ++tagCntr;
      openTags[tagCntr]="\\begin{equation}";
      closeTags[tagCntr]="\\end{equation}";
      ++tagCntr;
      openTags[tagCntr]="\\begin{align*}";
      closeTags[tagCntr]="\\end{align*}";
      ++tagCntr;
      openTags[tagCntr]="\\begin{align}";
      closeTags[tagCntr]="\\end{align}";
      ++tagCntr;
      openTags[tagCntr]="<pre>";
      closeTags[tagCntr]="</pre>";
      ++tagCntr;
      openTags[tagCntr]="<code>";
      closeTags[tagCntr]="</code>";
      ++tagCntr;
      openTags[tagCntr]="_code_";
      closeTags[tagCntr]="_/code_";
      ++tagCntr;
      openTags[tagCntr]="_cde_";
      closeTags[tagCntr]="_/cde_";
      ++tagCntr;
      openTags[tagCntr]="_verbatim_";
      closeTags[tagCntr]="_/verbatim_";
      ++tagCntr;
      openTagReplacements=openTags;
      closeTagReplacements=closeTags;
      tagCntr -= 3;
      //This CMS may still contain texts from the very old CMS, and
      //has to be robust enough to understand those texts.
      //In those old texts, _code_ is what we now call <pre>
      //and _cde_ is what we now call <code>
      //Now we have openTagReplacements[tagCntr]=="_code_";
      openTagReplacements[tagCntr]=dnchaB+"<pre>";
      closeTagReplacements[tagCntr]="</pre>"+dnchaE;
      ++tagCntr;
      //Now we have openTagReplacements[tagCntr]=="_cde_";
      openTagReplacements[tagCntr]=dnchaB+"<code>";
      closeTagReplacements[tagCntr]="</code>"+dnchaE;
      ++tagCntr;
      //Now we have openTagReplacements[tagCntr]=="_verbatim_";
      openTagReplacements[tagCntr]=dnchaB+"<code>";
      closeTagReplacements[tagCntr]="</code>"+dnchaE;
      ++tagCntr;
      //Now we are back with tagCntr, we had tagCntr-=3, and then we had
      // 3 instructions ++tagCntr;
      if(rs.htmlTolerance<2){
          openTags[tagCntr]="<a href=\"";
          closeTags[tagCntr]="\">";
          openTagReplacements[tagCntr]=openTags[tagCntr];
          closeTagReplacements[tagCntr]=closeTags[tagCntr];
          ++tagCntr;
      }
      PTKF::RecoveryStructure recS=PTKF::hideTags(t,
                                              openTags,
                                              closeTags,
                                              openTagReplacements,
                                              closeTagReplacements,
                                              instructions,
                                              GL_HTML_Tags.formattingTags,
                                              GF::GL_HIDING_STRING_DISPPF02,
                                              rs.htmlTolerance);
      if(rs.htmlTolerance<2){
          std::map<std::string,std::string> replMap;
          replMap["<"]="\\(<\\)";
          replMap[">"]="\\(>\\)";
          recS.noTagsText=MFRF::findAndReplace(recS.noTagsText,replMap);
      }
      t=recoverText(recS);
      t=PTKF::treatHideReveal(t);
      std::map<std::string,std::string> replMap;
      replMap[PTKF::GL_hideRevealOpenH]="_hideReveal_";
      replMap[PTKF::GL_hideRevealCloseH]="_/hideReveal_";
      replMap[PTKF::GL_openHidden]="";
      replMap[PTKF::GL_closeHidden]="";
      replMap[PTKF::GL_instructionOpen]="";
      replMap[PTKF::GL_instructionClose]="";
      replMap[PTKF::GL_instrSepO]="";
      replMap[PTKF::GL_instrSepC]="";
      replMap[PTKF::GL_htmlInstructionCode]="";
      t=MFRF::findAndReplace(t,replMap);
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
    fR=MFRF::findAndReplace(fR,HCBF::verySafePlaceInv,1);
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
