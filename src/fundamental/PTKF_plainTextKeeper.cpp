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

#ifndef _INCL_PlainTextKeeper_CPP
#define _INCL_PlainTextKeeper_CPP
namespace PTKF{
  class PlainTextKeeper{
  private:
    std::string sepBStB="_pTB*";
    std::string sepBStE="_/pTB*";
    std::string sepEC="!|!|_";
    std::string sepB;
    std::string sepE;
    std::map<long,std::string> plainTextBank;
    std::string generateReceipt(const long & ) const;
  public:
    PlainTextKeeper(const std::string &);
    std::string depositTxt(const std::string &);
    std::string recover(const std::string &) const;
    void treatCODE(const std::string & ="_code_", const std::string & = "_/code_");
    void treatCDE(const std::string & ="_cde_", const std::string & = "_/cde_");
    void treatPre(const std::string & = "<pre>", const std::string & = "</pre>", const std::string & = "", const std::string & = "");
    void treatBoxCode();
    void treatMath();
  };
  void treatHTMLTagsInNonMathText(std::string &t){
    t=SF::findAndReplace(t,"<","\\(<\\)");
    t=SF::findAndReplace(t,">","\\(>\\)");
  }
  void makeItMoreProfessional(std::string & t){
    t=SF::findAndReplace(t,"\n","<BR>");
    std::vector<std::string> seqLongSpaces,seqQuads;
    std::string currentLongSpace="  ",currentQuad="\\quad";
    long numS=25;
    seqLongSpaces.resize(numS);seqQuads.resize(numS);
    for(long i=0;i<numS;++i){
      seqLongSpaces[i]=currentLongSpace;
      seqQuads[i]=currentQuad;
      currentLongSpace+=" ";
      if(i%2==1){
        currentQuad+="\\quad";
      }
    }
    long i=numS;
    while(i>0){
      --i;
      t=SF::findAndReplace(t,seqLongSpaces[i],"\\("+seqQuads[i]+"\\)");
    }
  }
  PlainTextKeeper::PlainTextKeeper(const std::string & salt){
    plainTextBank.clear();
    sepB=sepBStB+salt+sepEC;
    sepE=sepBStE+salt+sepEC;
  }
  std::string PlainTextKeeper::generateReceipt(const long & i) const{
    return sepB+std::to_string(i)+sepE;
  }
  std::string PlainTextKeeper::depositTxt(const std::string & _t){
    long i=plainTextBank.size();
    plainTextBank[i]=_t;
    return generateReceipt(i);
  }
  std::string PlainTextKeeper::recover(const std::string & _textWithDeposits) const{
    std::map<long,std::string>::const_iterator it,itE;
    std::string returnText=_textWithDeposits;
    it=plainTextBank.begin();
    itE=plainTextBank.end();
    std::vector<std::string> sF,rW;
    if(plainTextBank.size()>0){
      sF.resize(plainTextBank.size());
      rW.resize(plainTextBank.size());
      long i=0;
      while(it!=itE){
        sF[i]=generateReceipt(it->first);
        rW[i]=it->second;
        ++it;
        ++i;
      }
      returnText=SF::findAndReplace(returnText,sF,rW);
    }
    return returnText;
  }
  std::string padIneqSigns(const std::string &in){
    std::string output=in;
    output=SF::findAndReplace(output,"< ","!*goodSpAftOIn*!");
    output=SF::findAndReplace(output," >","!*goodSpBefCIn*!");
    output=SF::findAndReplace(output,"<","< ");
    output=SF::findAndReplace(output,">"," >");
    output=SF::findAndReplace(output,"!*goodSpBefCIn*!"," >");
    output=SF::findAndReplace(output,"!*goodSpAftOIn*!","< ");
    return output;
  }
  void PlainTextKeeper::treatMath(){
    long sz=plainTextBank.size();
    std::string st;
    for(long i=0;i<sz;++i){
      plainTextBank[i]=padIneqSigns(plainTextBank[i]);
    }
  }
  void PlainTextKeeper::treatCDE(const std::string & cdeOpen, const std::string & cdeClose){
    treatPre(cdeOpen,cdeClose,"_doNotChangeAlphabet*_<code>","</code>_/doNotChangeAlphabet*_"); 
  }
  void PlainTextKeeper::treatBoxCode(){
    long sz=plainTextBank.size();
    std::string st;
    std::pair<std::string,int> receiveRepl;
    // HCBF::verySafePlace vector of caracters will be used to hide all the tags that would be modified with other pieces of code
    // this will be returned in DISPPF::finalizeForDisplay()
    for(long i=0;i<sz;++i){
      receiveRepl=SF::replaceAllOuterLayerSeparators(plainTextBank[i],"_codeBox_", "_/codeBox_","<p><textarea class=\"form-control\" rows=\"10\">","</textarea></p>",HCBF::verySafePlace);
      if(receiveRepl.second==1){
        plainTextBank[i]=receiveRepl.first;
      }
    }
  }
  void PlainTextKeeper::treatPre(const std::string &preOpen, const std::string &preClose, const std::string & pOOverwrite, const std::string & pCOverwrite){
    long sz=plainTextBank.size();
    std::string st;
    std::string alakazamOpen=GF::GL_HIDING_STRING_PTKF01+GL_MAIN_SETUP_FILE_NAME+GF::GL_HIDING_STRING_PTKF02;
    std::string alakazamClose=GF::GL_HIDING_STRING_PTKF03+GL_MAIN_SETUP_FILE_NAME+GF::GL_HIDING_STRING_PTKF04;
    std::string oOverwrite=preOpen;
    std::string cOverwrite=preClose;
    if(pOOverwrite!=""){oOverwrite=pOOverwrite;}
    if(pCOverwrite!=""){cOverwrite=pCOverwrite;}
    for(long i=0;i<sz;++i){
      st=plainTextBank[i];
      st=SF::findAndReplace(st,preOpen,alakazamOpen);
      st=SF::findAndReplace(st,preClose,alakazamClose);
      st=SF::findAndReplace(st,"<","&lt;");
      st=SF::findAndReplace(st,">","&gt;");
      st=SF::findAndReplace(st,alakazamClose,cOverwrite);
      st=SF::findAndReplace(st,alakazamOpen,oOverwrite);
      plainTextBank[i]=st;
    }
  }
  void PlainTextKeeper::treatCODE(const std::string & codeOpen, const std::string & codeClose){
    treatPre(codeOpen,codeClose,"_doNotChangeAlphabet*_<pre>","</pre>_/doNotChangeAlphabet*_");
  }
    int removeToSafety(PTKF::PlainTextKeeper &kc, std::string &t,const std::string & s_B,const std::string & s_E,const long & keepCodes=1){
      //returns 1 if everything is removed properly
      // returns 0 if there is an open tag or a tag inside a tag
      int correct=1;
      long sz=t.size();
      long pos;
      std::pair<std::string,int> allD;
      std::string ttdS,ttdR, dRec;
      std::string updatedT="";
      long posOld;
      pos=0;
      posOld=pos;
      allD=SF::extract(t,pos,s_B,s_E);
      while(allD.second==1){
        if(correct==1){
          if(allD.first!=SF::findAndReplace(allD.first,s_B,"")) {
            correct=0;
          }
        }
        ttdS="";ttdR="";
        if(keepCodes==1){
          ttdS+=s_B;
        }
        ttdS+=allD.first;
        if(keepCodes==1){
          ttdS+=s_E;
        }
        ttdR=s_B+allD.first+s_E;
        dRec=kc.depositTxt(ttdS);
        while(posOld<pos-ttdR.length()){
          updatedT+=t[posOld];
          ++posOld;
        }
        updatedT+=dRec;
        posOld=pos;
        allD=SF::extract(t,pos,s_B,s_E);
      }
      if(allD.second==0){
        while(posOld<sz){
          updatedT+=t[posOld];
          ++posOld;
        }
      }
      t=updatedT;
      if(correct==1){
        if(t!=SF::findAndReplace(t,s_E,"")){
          correct=0;
        }
      }
      if(correct==1){
        if(t!=SF::findAndReplace(t,s_B,"")){
          correct=0;
        }
      }
      return correct;
    }
  PlainTextKeeper GL_PLAINTEXT_KEEPER("gl0bal");
}
#endif
