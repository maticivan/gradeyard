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


#ifndef _INCL_HSF_HTML_STRINGS_CPP
#define _INCL_HSF_HTML_STRINGS_CPP

namespace HSF{
  std::string redirectIndexHTML_F(const std::string & redirectInfo, const std::string & wsURL){
    std::string fR="";
    std::string wu=wsURL+redirectInfo;
    fR+="<HTML>\n<HEAD>\n";
    fR+="<meta HTTP-EQUIV=\"REFRESH\" content=\"0; url=";
    fR+=wu+"\">\n";
    fR+="</HEAD><BODY>\n\n<FONT SIZE=+2> Not Found </FONT> \n";
    fR+=" <BR> The requested URL not found. Please visit the home-page ";
    fR+=" <a href=\"";
    fR+=wu+"\">"+wu+"</a>";
    fR+="</BODY></HTML>";
    return fR;
  }
  void parametersFromString(const std::string & sysDataStr,
                                  std::string & tCreated,
                                  std::string & createdBy,
                                  std::string & tModified,
                                  std::string & modifiedBy,
                                  std::string & perm,
                                  std::string & dType){
    std::string s_createdB="_created!!_";
    std::string s_createdE="_/created!!_";
    std::string s_modifiedB="_modified!!_";
    std::string s_modifiedE="_/modified!!_";
    std::string s_createdByB="_createdBy!!_";
    std::string s_createdByE="_/createdBy!!_";
    std::string s_modifiedByB="_modifiedBy!!_";
    std::string s_modifiedByE="_/modifiedBy!!_";
    std::string s_permissionStringB="_permissionString!!_";
    std::string s_permissionStringE="_/permissionString!!_";
    std::string s_docTypeStringB="_documentType!!_";
    std::string s_docTypeStringE="_/documentType!!_";
    long p1=0;
    std::pair<std::string,int> allSDInTemp=SF::extract(sysDataStr,p1,s_createdB,s_createdE);
    if(allSDInTemp.second==1){
      tCreated=allSDInTemp.first;
    }
    p1=0;
    allSDInTemp=SF::extract(sysDataStr,p1,s_createdByB,s_createdByE);
    if(allSDInTemp.second==1){
      createdBy=allSDInTemp.first;
    }
    p1=0;
    allSDInTemp=SF::extract(sysDataStr,p1,s_modifiedB,s_modifiedE);
    if(allSDInTemp.second==1){
      tModified=allSDInTemp.first;
    }
    p1=0;
    allSDInTemp=SF::extract(sysDataStr,p1,s_modifiedByB,s_modifiedByE);
    if(allSDInTemp.second==1){
      modifiedBy=allSDInTemp.first;
    }
    p1=0;
    allSDInTemp=SF::extract(sysDataStr,p1,s_permissionStringB,s_permissionStringE);
    if(allSDInTemp.second==1){
      perm=allSDInTemp.first;
    }
    p1=0;
    allSDInTemp=SF::extract(sysDataStr,p1,s_docTypeStringB,s_docTypeStringE);
    if(allSDInTemp.second==1){
      dType=allSDInTemp.first;
    }
  }
  std::vector<std::string> trimVectorForAssignmentGradesSummaryCSV(const std::vector<std::string> &in){
    long sz=in.size();
    if(sz<5){return in;}
    std::vector<std::string> outV;
    sz-=4;
    outV.resize(sz);
    outV[0]=in[0];
    for(long i=1;i<sz;++i){
      outV[i]=in[i+1];
    }
    return outV;
  }
  std::string tableOrCSVFromStack(std::stack<std::vector<std::string> > &st, const std::string & separator,const std::string & tOpenTag,const std::string &thOpenTag){
    if(st.empty()){
      return "";
    }
    std::string fR="",tempLine;
    long sz=st.size();
    if(separator=="table"){
      fR+=tOpenTag+"\n";
      fR+=thOpenTag+"\n";
    }
    else{
      fR+="_codeBox_\n";
    }
    std::vector<std::string> cR;
    cR=st.top();st.pop();
    long lsz=cR.size();

    if(separator=="table"){
      fR+="<tr>\n";
      for(long i=0;i<lsz;++i){
        fR+="<th>"+cR[i]+"</th>\n";
      }
      fR+="</tr>\n";
      fR+=" </thead><tbody>";
    }
    else{
      fR+="0";
      tempLine="";
      for(long i=0;i<lsz;++i){
        tempLine+=separator+cR[i];
      }
      fR+=BF::removeASCII10AND13(tempLine)+"\n";
    }
    long num=sz-1;
    long counterTableColumn0=0;
    while(num>0){
      ++counterTableColumn0;
      cR=st.top();
      st.pop();
      --num;
      lsz=cR.size();
      if(separator=="table"){
        fR+="<tr>\n";
        for(long i=0;i<lsz;++i){
          fR+="<td>"+cR[i]+"</td>\n";
        }
        fR+="</tr>\n";
      }
      else{
        fR+=std::to_string(counterTableColumn0);
        tempLine="";
        for(long i=0;i<lsz;++i){
          tempLine+=separator+cR[i];
        }
        fR+=BF::removeASCII10AND13(tempLine)+ "\n";
      }
    }
    if(separator=="table"){
      fR+="</tbody></table>\n";
    }
    else{
      fR+="_/codeBox_\n";
    }
    return fR;
  }
  std::string  tableFromStack(std::stack<std::vector<std::string> > &st, const std::string & tOpenTag, const std::string & thOpenTag){
    return tableOrCSVFromStack(st,"table",tOpenTag,thOpenTag);
  }
  std::string linkFromPair(const std::pair<std::string,std::string> & lp){
    return "<A href=\""+lp.first+"\">"+lp.second+"</A>";
  }
  std::string createButtonLink(const std::string &uRL, const std::string &label, const std::string & bSize="btn-sm", const std::string & bOutline = "dark"){
      if(bSize=="standardSize"){
          return "<a href=\""+uRL+"\"><button type=\"button\" class=\"btn btn-outline-"+bOutline+"\">"+label+"</button></a>";
      }
      return "<a href=\""+uRL+"\"><button type=\"button\" class=\"btn btn-outline-"+bOutline+" "+bSize+"\">"+label+"</button></a>";
  }
  std::string buttonsBar(const std::stack<std::pair<std::string, std::string> > & _v, const std::string &location="mainText"){
    std::stack<std::pair<std::string, std::string> > v=_v;
    std::string fR="";
    std::pair<std::string,std::string> topSt;
        std::string ulOpening="<ul class=\"nav justify-content-end\">\n";
        std::string liOpening="<li class=\"nav-item\">\n";
        std::string bOutline="dark";
        if(location=="navBar"){
            ulOpening="<ul class=\"navbar-nav\">\n";
            bOutline="light";
        }
    if(!v.empty()){
      fR+=ulOpening;
      while(!v.empty()){
        topSt=v.top();
        v.pop();
        fR+=liOpening;
        fR+=createButtonLink(topSt.first,topSt.second,"btn-sm",bOutline);
        fR+="</li>\n";
      }
      fR+="</ul>";
    }
    return fR;
  }
  std::vector<long> countTags(const std::string & _input, const std::vector<std::string> & _tags, const int & caseSensitive=0){
    std::vector<long> fR;
    long sz=_tags.size();
    if(sz==0){
        return fR;
    }
    std::string input=_input;
    std::vector<std::string> tags=_tags;
    if(caseSensitive==0){
      input=MFRF::toLowerCase(input);
      for(long i=0;i<sz;++i){
        tags[i]=MFRF::toLowerCase(tags[i]);
      }
    }
    fR.resize(sz);
    std::vector<long> remainingLettersToMatch,nSz;
    remainingLettersToMatch.resize(sz);nSz.resize(sz);
    long rsz=0;
    for(long i=0;i<sz;++i){
      nSz[i]=tags[i].length();
      remainingLettersToMatch[i]=nSz[i];
      if(remainingLettersToMatch[i]>0){
        ++rsz;
      }
      fR[i]=0;
    }
    std::vector<long> counters;
    counters.resize(sz);
    for(long j=0;j<sz;++j){
      counters[j]=0;
    }
    long iLn=input.length();
    long countersReachedLimit=0;
    while(countersReachedLimit<rsz){
      for(long j=0;j<sz;++j){
        if(counters[j]<iLn){
          if(remainingLettersToMatch[j]>0){
            if(input[counters[j]]==(tags[j])[nSz[j]-remainingLettersToMatch[j]]){
              --remainingLettersToMatch[j];
              if(remainingLettersToMatch[j]==0){
                ++fR[j];
                remainingLettersToMatch[j]=nSz[j];
              }
            }
            else{
              counters[j] -= (nSz[j]-remainingLettersToMatch[j]);
              remainingLettersToMatch[j]=nSz[j];
            }
            ++counters[j];
            if(counters[j]==iLn){
              ++countersReachedLimit;
            }
          }
        }
      }
    }
    return fR;
  }
  std::string codeDisplayForNonAdvanceUsers(const std::string& input,
                                            const std::string& signAdvanced01="_code_",
                                            const std::string& signAdvanced02="_cde_",
                                            const std::string& addToFront="<pre>",
                                            const std::string& addToBack="</pre>"
                                            ){
    if(MFRF::find(input,signAdvanced01,signAdvanced02).second==-1){
      return addToFront+input+addToBack;
    }
    return input;
  }
  std::string urlEnvVarsFromForwardedParameters(const std::map<std::string,std::string> & respMap, const std::string & variable, const std::string & value, const std::string &forwardVariable, const std::string & forwardValue, const long & maxNumOfVarValTranslationsThatCanBeAddedThroughTheURLString=10){
    std::string fR;
    std::map<std::string,std::string>::const_iterator itEnvV,itEnvVE;
    itEnvVE=respMap.end();
    itEnvV=respMap.find("fP");
    if(itEnvV==itEnvVE){
      return "";
    }
    fR="page="+itEnvV->second;
    long i=0;std::string vr;std::string vl;
    while(i<maxNumOfVarValTranslationsThatCanBeAddedThroughTheURLString){
      itEnvV=respMap.find(forwardVariable+std::to_string(i));
      if(itEnvV!=itEnvVE){
        vr=itEnvV->second;
        itEnvV=respMap.find(forwardValue+std::to_string(i));
        if(itEnvV!=itEnvVE){
          vl=itEnvV->second;
          fR+="&"+variable+std::to_string(i)+"="+vr;
          fR+="&"+value+std::to_string(i)+"="+vl;
        }
      }
      ++i;
    }
    return fR;
  }
  int matchingTags(const std::string &input, const std::vector<std::string> & oT, const std::vector<std::string> & cT){
    long sz=oT.size();
    if(cT.size()!=sz){
      return 0;
    }
    if(sz==0){
      return 1;
    }
    std::vector<std::string> allT;
    allT.resize(2*sz);
    for(long i=0;i<sz;++i){
      allT[i]=oT[i];
      allT[i+sz]=cT[i];
    }
    std::vector<long> counts=countTags(input,allT);
    long i=0;
    while(i<sz){
      if(counts[i]!=counts[i+sz]){
        return 0;
      }
      ++i;
    }
    return 1;
  }
  double letterToNumGrade(const std::string & l,const std::string & sch="Baruch"){
    double fR=-999.99;
    std::map<std::string,std::map<std::string,double> >::const_iterator iMB,iME;
    iMB=GF::GL_GRADE_CONVERSION.find(sch);
    iME=GF::GL_GRADE_CONVERSION.end();
    if(iMB==iME){
      return fR;
    }
    std::map<std::string,double>::const_iterator it,itE;
    itE=(iMB->second).end();
    it=(iMB->second).find(l);
    if(it==itE){
      return fR;
    }
    return it->second;
  }
}
#endif
