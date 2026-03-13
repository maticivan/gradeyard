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

#ifndef _INCL_PlainTextKeeper_CPP
#define _INCL_PlainTextKeeper_CPP
namespace PTKF{
  std::string GL_sepBStB="_pTB*";
  std::string GL_sepBStE="_/pTB*";
  std::string GL_rnd_code1="*"+RNDF::genRandCode(5)+"*@";
  std::string GL_rnd_code2="~"+RNDF::genRandCode(5)+"@!";
  std::string GL_rnd_code3=RNDF::genRandCode(5);
  std::string GL_rnd_code4=RNDF::genRandCode(3);
  std::string GL_rnd_codeHR="."+RNDF::genRandCode(5)+"+~";
  std::string GL_openHidden="_|"+GL_rnd_code1;
  std::string GL_closeHidden="_/"+GL_rnd_code1;
  std::string GL_instructionOpen="~|"+GL_rnd_code2;
  std::string GL_instructionClose="~/"+GL_rnd_code2;
  std::string GL_mainHidingWord="_|*"+GL_rnd_code2+"_)";
  std::string GL_hideRevealOpenH="!~"+GL_rnd_codeHR;
  std::string GL_hideRevealCloseH="!/~"+GL_rnd_codeHR;
  std::string GL_htmlInstructionCode="#h"+GL_rnd_code3;
  std::string GL_instrSepO=".~"+GL_rnd_code3;
  std::string GL_instrSepC="~."+GL_rnd_code3;
  std::string GL_instructionMath="#m"+GL_rnd_code4+"@";
  std::string GL_instructionCode="#c"+GL_rnd_code4+"@";
  std::string GL_instructionPre="#p"+GL_rnd_code4+"@";
  std::string GL_instructionAHref="#a"+GL_rnd_code4+"@";
  std::string GL_thisWebsiteURL="";//will be updated in MWII/WSI
  std::string cleanRandCodes(const std::string& in){
      std::map<std::string,std::string> replMap;
      replMap[GL_openHidden]="";
      replMap[GL_closeHidden]="";
      replMap[GL_instructionOpen]="";
      replMap[GL_instructionClose]="";
      return MFRF::findAndReplace(in,replMap);
  }
  std::string padIneqSigns(const std::string &in){
    std::string output;
    long sz=in.length();
    for(long i=0;i<sz;++i){
      output+=in[i];
      if( (i<sz-1) && (in[i+1]!=' ') && ((in[i]=='<')||(in[i]=='>')) ){
        output+=' ';
      }
    }
    return output;
  }
std::string treatmentPre(const std::string &_in){
    std::pair<std::stack<std::string>,std::string> cleaningTagsStr;
    cleaningTagsStr=SF::stringToStackAndRemoveItems(_in,
                                                    GL_instructionOpen,
                                                    GL_instructionClose,
                                                    1,
                                                    "!*!");
    
    
    std::map<std::string,std::string> replMap;
    replMap["<"]="&lt;";
    replMap[">"]="&gt;";
    replMap["_hideReveal_"]=GL_hideRevealOpenH;
    replMap["_/hideReveal_"]=GL_hideRevealCloseH;
    replMap[GL_openHidden]="";
    replMap[GL_closeHidden]="";
    return MFRF::findAndReplace(cleaningTagsStr.second,replMap);
}
std::string treatmentAHref(const std::string &in){
    std::map<std::string,std::string> replMap;
    replMap["<"]="";
    replMap[">"]="";
    replMap["\""]="";
    if(MFRF::findAndReplace(in,replMap)!=in){return "/index.cgi";}
    if(in.starts_with("https://") ||
       in.starts_with("http://") ||
       in.starts_with("/")){return in;}
    return GL_thisWebsiteURL+in;
}
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
      nt+="&#39;hDiv3"+counterSt+"&#39;, &#39;"+revT+"&#39;, &#39;"+hideT+"&#39; ) ;\" aria-label=\"Show hidden content ";
    nt+=counterSt+"\">";
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
std::string instructionData(const std::string& _o,
                            const std::string& _c,
                            const std::string& _i){
    std::string res= GL_instructionOpen+GL_instrSepO+_o+GL_instrSepC;
    res+=GL_instrSepO+_c+GL_instrSepC;
    res+=GL_instrSepO+_i+GL_instrSepC+GL_instructionClose;
    return res;
}
std::string applyInstruction(const std::string& in,
                             const std::string& instruction){
    if(instruction==GL_instructionMath){
        return padIneqSigns(in);
    }
    if(instruction==GL_instructionPre){
        return treatmentPre(in);
    }
    if(instruction==GL_instructionCode){
        return treatmentPre(in);
    }
    if(instruction==GL_instructionAHref){
        return treatmentAHref(in);
    }
    return in;
}
std::string followTheInstruction(const std::string& in){
    std::string res;
    std::pair<std::string,int> allD;
    long pos=0;
    allD=SF::extract(in,pos,GL_instructionOpen,GL_instructionClose);
    if(allD.second==0){
        return in;
    }
    std::string remaining=in.substr(pos,in.size()-pos);
    std::vector<std::string> iCodes=SF::stringToVector(allD.first,
                                                       GL_instrSepO,GL_instrSepC);
    if(iCodes.size()!=3){
        return remaining;
    }
    pos=0;
    allD=SF::extract(remaining,pos,iCodes[0],iCodes[1]);
    if(allD.second==0){
        return remaining;
    }
    if(iCodes[0]==GL_htmlInstructionCode){return iCodes[1];}
    return iCodes[0]+applyInstruction(allD.first,iCodes[2])+iCodes[1];
}
std::stack<std::string> prepareRecoveryTags(std::stack<std::string>& in){
    std::stack<std::string> out;
    while(!in.empty()){
        out.push(followTheInstruction(in.top()));
        in.pop();
    }
    return out;
}
struct RecoveryStructure{
public:
    std::string noTagsText;
    std::stack<std::string> savedTags;
};
RecoveryStructure hideTags(const std::string& mainText,
                           const std::vector<std::string>& openTags,
                           const std::vector<std::string>& closeTags,
                           const std::vector<std::string>& openTagReplacements,
                           const std::vector<std::string>& closeTagReplacements,
                           const std::vector<std::string>& instructions,
                           const std::vector<std::string>& htmlFormattingTags,
                           const std::string& azm,
                           const long& tolerance){
    RecoveryStructure res;
    long sz=openTags.size();
    if(sz<1){return res;}
    if(sz!=closeTags.size()){return res;}
    if(sz!=instructions.size()){return res;}
    std::map<std::string,std::string> initial;
    std::string tmp1,tmp2;
    for(long i=0;i<sz;++i){
        tmp1=openTags[i];
        tmp2=GL_openHidden+
            instructionData(openTagReplacements[i],
                            closeTagReplacements[i],
                            instructions[i])+
            openTagReplacements[i];
        initial[tmp1] = tmp2;
        tmp1=closeTags[i];
        tmp2=closeTagReplacements[i]+GL_closeHidden;
        initial[tmp1] = tmp2;
    }
    if(tolerance==1){
        for(long i=0;i<htmlFormattingTags.size();++i){
            tmp1=htmlFormattingTags[i];
            tmp2=GL_openHidden+instructionData(GL_htmlInstructionCode,
                                               tmp1,
                                               "n")+tmp1+GL_closeHidden;
            initial[tmp1] = tmp2;
        }
    }
    res.noTagsText = MFRF::findAndReplace(mainText, initial);
    std::pair<std::stack<std::string>,std::string> hidingStructure;
    hidingStructure=SF::stringToStackAndRemoveItems(res.noTagsText,
                                                    GL_openHidden,
                                                    GL_closeHidden,
                                                    1,
                                                    "!*!",
                                                    GL_mainHidingWord,
                                                    "",
                                                    -1);
    res.noTagsText=std::move(hidingStructure.second);
    res.savedTags=prepareRecoveryTags(hidingStructure.first);
    return res;
}
std::string recoverText(RecoveryStructure& iRS){
    std::string res;
    std::pair<std::string, int> allD;
    long pos=0;long oldPos;
    while(pos<iRS.noTagsText.size()){
        oldPos=pos;
        allD=SF::getEverythingBefore(iRS.noTagsText,pos,GL_mainHidingWord);
        if(allD.second==1){
            res+=allD.first;
            if(!iRS.savedTags.empty()){
                res+=iRS.savedTags.top();
                iRS.savedTags.pop();
            }
        }
        else{
            pos=oldPos;
            while(pos<iRS.noTagsText.size()){
                res+=iRS.noTagsText[pos];
                ++pos;
            }
        }
    }
    return res;
}

  class PlainTextKeeper{
  private:
    std::string sepEC="!|!|_";
    std::string sepB;
    std::string sepE;
    std::map<long,std::string> plainTextBank;
    std::string generateReceipt(const long & ) const;
  public:
    PlainTextKeeper(const std::string &);
    std::string depositTxt(const std::string &);
    std::string recover(const std::string &) const;
    void treatBoxCode();
  };
  PlainTextKeeper::PlainTextKeeper(const std::string & salt){
    plainTextBank.clear();
    sepB=GL_sepBStB+salt+sepEC;
    sepE=GL_sepBStE+salt+sepEC;
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
    it=plainTextBank.begin();
    itE=plainTextBank.end();
    std::map<std::string,std::string> replMap;
    if(plainTextBank.size()>0){
      std::string deb_str;
      while(it!=itE){
        replMap[generateReceipt(it->first)]=it->second;
        ++it;
      }
      return MFRF::findAndReplace(_textWithDeposits,replMap);
    }
    return _textWithDeposits;
  }
  void PlainTextKeeper::treatBoxCode(){
    long sz=plainTextBank.size();
    std::pair<std::string,int> receiveRepl;
    // HCBF::verySafePlace vector of characters will be used to hide all the tags that would be modified with other code
    // this will be returned in DISPPF::finalizeForDisplay()
    for(long i=0;i<sz;++i){
      receiveRepl=SF::replaceAllOuterLayerSeparators(plainTextBank[i],"_codeBox_", "_/codeBox_","<p><textarea class=\"form-control\" rows=\"10\">","</textarea></p>",HCBF::verySafePlace);
      if(receiveRepl.second==1){
        plainTextBank[i]=receiveRepl.first;
      }
    }
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
