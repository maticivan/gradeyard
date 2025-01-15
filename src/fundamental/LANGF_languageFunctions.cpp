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


#ifndef _INCL_LANGF_languageFunctions_CPP
#define _INCL_LANGF_languageFunctions_CPP

namespace LANGF{
  long advanceTheCounterIfThereIsTheTag(const long & i, const std::string & input, const std::string & _o, const std::string & _c){
    long adv=i;
    long sz=input.size();
    long szO=_o.size();
    long szC=_c.size();
    if(adv+szO >= sz){
      return i;
    }
    long walker=0;
    while((walker<szO)&&(input[adv]==_o[walker])){++walker;++adv;}
    if(walker!=szO){
      return i;
    }
    std::pair<std::string, int> allD;
    long pos;
    pos=adv;allD=SF::getEverythingBefore(input,pos,_c);
    if(allD.second==0){
      return sz;
    }
    return pos;
  }
  long advanceTheCounterIfThereIsATag(const long & i, const std::string &input,const std::vector<std::string> & _o, const std::vector<std::string> & _c){
    long adv=i,advNew;
    long sz=input.size();
    long nTags=_o.size();
    if(_c.size()!=nTags){
      return i;
    }
    long tagI=0;
    while((adv<sz)&&(tagI<nTags)){
      advNew=advanceTheCounterIfThereIsTheTag(adv,input,_o[tagI],_c[tagI]);
      if(advNew!=adv){
        adv=advNew;
        tagI=0;
      }
      else{
        ++tagI;
      }
    }
    return adv;
  }
  std::string changeAlphabetCyr(const std::string &input,const std::vector<std::string> & _o, const std::vector<std::string> & _c){
    std::string output="";
    long i=0;long iAdv;
    long sz=input.length();
    long ind;
    while(i<sz){
      iAdv = advanceTheCounterIfThereIsATag(i,input,_o,_c);
      while(i<iAdv){
        output+=input[i];
        ++i;
      }
      if((i<sz-4)&&(input[i]=='_')&&(input[i+1]=='p')&&(input[i+2]=='T')&&(input[i+3]=='B')&&(input[i+4]=='*')){
        i+=5;
        output+="_pTB*";
        ind=0;
        while((ind==0)&&(i<sz)){
          if((i>4)&&(input[i-5]=='_')&&(input[i-4]=='/')&&(input[i-3]=='p')&&(input[i-2]=='T')&&(input[i-1]=='B')&&(input[i]=='*')){
            ind=1;
          }
          else{
            output+=input[i];
            ++i;
          }
        }
        if(ind==1){
          while( (ind==1) &&(i<sz) ){
            if((i>3) && (input[i-4]=='!')&&(input[i-3]=='|')&&(input[i-2]=='!')&&(input[i-1]=='|')&&(input[i]=='_')){
              ind=2;
            }
            else{
              output+=input[i];
              ++i;
            }
          }
        }
      }
      if(i<sz){
        ind = 0;
        if((ind==0)&&(i<sz-1)&&(input[i]=='d')&&(input[i+1]=='j')){ind=1;output+="&#1106;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='d')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1076;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='d')&&(input[i+1]=='z')&&(input[i+2]=='h')){ind=1;output+="&#1119;";i+=2;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='d')&&(input[i+1]==';')&&(input[i+2]=='z')){ind=1;output+="&#1076;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='z')&&(input[i+1]=='h')){ind=1;output+="&#1078;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='z')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1079;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='l')&&(input[i+1]=='j')){ind=1;output+="&#1113;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='l')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1083;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='n')&&(input[i+1]=='j')){ind=1;output+="&#1114;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='n')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1085;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='c')&&(input[i+1]=='1')){ind=1;output+="&#1115;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='c')&&(input[i+1]=='h')){ind=1;output+="&#1095;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='c')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1094;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='s')&&(input[i+1]=='h')){ind=1;output+="&#1096;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='s')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1089;";++i;}
        if((ind==0)&&(input[i]=='a')){ind=1;output+="&#1072;";}
        if((ind==0)&&(input[i]=='b')){ind=1;output+="&#1073;";}
        if((ind==0)&&(input[i]=='v')){ind=1;output+="&#1074;";}
        if((ind==0)&&(input[i]=='g')){ind=1;output+="&#1075;";}
        if((ind==0)&&(input[i]=='d')){ind=1;output+="&#1076;";}
        //dj
        if((ind==0)&&(input[i]=='e')){ind=1;output+="&#1077;";}
        //zh
        if((ind==0)&&(input[i]=='z')){ind=1;output+="&#1079;";}
        if((ind==0)&&(input[i]=='i')){ind=1;output+="&#1080;";}
        if((ind==0)&&(input[i]=='j')){ind=1;output+="&#1112;";}
        if((ind==0)&&(input[i]=='k')){ind=1;output+="&#1082;";}
        if((ind==0)&&(input[i]=='l')){ind=1;output+="&#1083;";}
        //lj
        if((ind==0)&&(input[i]=='m')){ind=1;output+="&#1084;";}
        if((ind==0)&&(input[i]=='n')){ind=1;output+="&#1085;";}
        //nj
        if((ind==0)&&(input[i]=='o')){ind=1;output+="&#1086;";}
        if((ind==0)&&(input[i]=='p')){ind=1;output+="&#1087;";}
        if((ind==0)&&(input[i]=='r')){ind=1;output+="&#1088;";}
        if((ind==0)&&(input[i]=='s')){ind=1;output+="&#1089;";}
        if((ind==0)&&(input[i]=='t')){ind=1;output+="&#1090;";}
        //c1
        if((ind==0)&&(input[i]=='u')){ind=1;output+="&#1091;";}
        if((ind==0)&&(input[i]=='f')){ind=1;output+="&#1092;";}
        if((ind==0)&&(input[i]=='h')){ind=1;output+="&#1093;";}
        if((ind==0)&&(input[i]=='c')){ind=1;output+="&#1094;";}
        //ch
        //dzh
        //sh
        if((ind==0)&&(i<sz-1)&&(input[i]=='D')&&(input[i+1]=='j')){ind=1;output+="&#1026;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='D')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1044;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='D')&&(input[i+1]=='z')&&(input[i+2]=='h')){ind=1;output+="&#1039;";i+=2;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='D')&&(input[i+1]==';')&&(input[i+2]=='z')){ind=1;output+="&#1044;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='Z')&&(input[i+1]=='h')){ind=1;output+="&#1046;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='Z')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1047;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='L')&&(input[i+1]=='j')){ind=1;output+="&#1033;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='L')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1051;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='N')&&(input[i+1]=='j')){ind=1;output+="&#1034;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='N')&&(input[i+1]==';')&&(input[i+2]=='j')){ind=1;output+="&#1053;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='C')&&(input[i+1]=='1')){ind=1;output+="&#1035;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='C')&&(input[i+1]=='h')){ind=1;output+="&#1063;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='C')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1062;";++i;}
        if((ind==0)&&(i<sz-1)&&(input[i]=='S')&&(input[i+1]=='h')){ind=1;output+="&#1064;";++i;}
        if((ind==0)&&(i<sz-2)&&(input[i]=='S')&&(input[i+1]==';')&&(input[i+2]=='h')){ind=1;output+="&#1057;";++i;}
        if((ind==0)&&(input[i]=='A')){ind=1;output+="&#1040;";}
        if((ind==0)&&(input[i]=='B')){ind=1;output+="&#1041;";}
        if((ind==0)&&(input[i]=='V')){ind=1;output+="&#1042;";}
        if((ind==0)&&(input[i]=='G')){ind=1;output+="&#1043;";}
        if((ind==0)&&(input[i]=='D')){ind=1;output+="&#1044;";}
        //Dj
        if((ind==0)&&(input[i]=='E')){ind=1;output+="&#1045;";}
        //Zh
        if((ind==0)&&(input[i]=='Z')){ind=1;output+="&#1047;";}
        if((ind==0)&&(input[i]=='I')){ind=1;output+="&#1048;";}
        if((ind==0)&&(input[i]=='J')){ind=1;output+="&#1032;";}
        if((ind==0)&&(input[i]=='K')){ind=1;output+="&#1050;";}
        if((ind==0)&&(input[i]=='L')){ind=1;output+="&#1051;";}
        //Lj
        if((ind==0)&&(input[i]=='M')){ind=1;output+="&#1052;";}
        if((ind==0)&&(input[i]=='N')){ind=1;output+="&#1053;";}
        //Nj
        if((ind==0)&&(input[i]=='O')){ind=1;output+="&#1054;";}
        if((ind==0)&&(input[i]=='P')){ind=1;output+="&#1055;";}
        if((ind==0)&&(input[i]=='R')){ind=1;output+="&#1056;";}
        if((ind==0)&&(input[i]=='S')){ind=1;output+="&#1057;";}
        if((ind==0)&&(input[i]=='T')){ind=1;output+="&#1058;";}
        //C1
        if((ind==0)&&(input[i]=='U')){ind=1;output+="&#1059;";}
        if((ind==0)&&(input[i]=='F')){ind=1;output+="&#1060;";}
        if((ind==0)&&(input[i]=='H')){ind=1;output+="&#1061;";}
        if((ind==0)&&(input[i]=='C')){ind=1;output+="&#1062;";}
        //Ch
        //Dzh
        //Sh
        if(ind==0){ind=1;output+=input[i];}
        ++i;
      }
    }
    return output;
  }
  std::string changeAlphabetInTexts(const std::string & input, const std::string & alph,const std::vector<std::string> & _o, const std::vector<std::string> & _c ){
    std::string output=input;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extractAndReplace(output,pos,"_lang*_","_/lang*_",0,"");
    if(allD.second==1){
      output=allD.first;
    }
    if(alph=="cyr"){
      return changeAlphabetCyr(output, _o,_c);
    }
    return output;
  }
  std::string changeAlphabet(const std::string & input,const std::string &defaultAlph){
    std::string result=input;
    std::string alphabet=defaultAlph;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(result,pos,"_lang*_","_/lang*_");
    if((allD.second==1)||(alphabet!="english")){
      if(allD.second==1){alphabet=allD.first;}
      long indSafety;
      TMF::Timer t1,t2,t3;
      t1.start();
      long numTags=11;
      long cntr;
      std::vector<std::string> openTags;
      std::vector<std::string> closeTags;
      openTags.resize(numTags);
      closeTags.resize(numTags);
      cntr=0;
      openTags[cntr]="{w"; closeTags[cntr]="}";
      ++cntr;
      openTags[cntr]="<code>"; closeTags[cntr]="</code>";
      ++cntr;
      openTags[cntr]="<pre>"; closeTags[cntr]="</pre>";
      ++cntr;
      openTags[cntr]="\\["; closeTags[cntr]="\\]";
      ++cntr;
      openTags[cntr]="\\("; closeTags[cntr]="\\)";
      ++cntr;
      openTags[cntr]="\\begin{eqnarray*}"; closeTags[cntr]="\\end{eqnarray*}";
      ++cntr;
      openTags[cntr]="\\begin{eqnarray}"; closeTags[cntr]="\\end{eqnarray}";
      ++cntr;
      openTags[cntr]="\\begin{equation}"; closeTags[cntr]="\\end{equation}";
      ++cntr;
      openTags[cntr]="\\begin{align*}"; closeTags[cntr]="\\end{align*}";
      ++cntr;
      openTags[cntr]="\\begin{align}"; closeTags[cntr]="\\end{align}";
      ++cntr;
      openTags[cntr]="<"; closeTags[cntr]=">";
      PTKF::PlainTextKeeper textAreaKeeper("textA");
      indSafety=PTKF::removeToSafety(textAreaKeeper,result,"<textarea","</textarea>");
      PTKF::PlainTextKeeper langSaver("lang");
      indSafety=PTKF::removeToSafety(langSaver,result,"_doNotChangeAlphabet*_","_/doNotChangeAlphabet*_",0);
      t1.end();
      t2.start();
      result=changeAlphabetInTexts(result,alphabet,openTags,closeTags);
      t2.end();
      t3.start();
      result=langSaver.recover(result);
      result=textAreaKeeper.recover(result);
      t3.end();
      GF::GL_DEB_MESSAGES.addMessage("Timer 1: "+ BF::doubleToString(t1.getTime()));
      GF::GL_DEB_MESSAGES.addMessage("Timer 2: "+ BF::doubleToString(t2.getTime()));
      GF::GL_DEB_MESSAGES.addMessage("Timer 3: "+ BF::doubleToString(t3.getTime()));
    } 
    return result;
  }
}
#endif
