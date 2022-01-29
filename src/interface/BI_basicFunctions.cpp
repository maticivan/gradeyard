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


#ifndef _INCL_WI_BASICFUNCTIONS_CPP
#define _INCL_WI_BASICFUNCTIONS_CPP

namespace BI{
  const long GL_NUM_ENV=27;
  const std::string GL_ENV[ GL_NUM_ENV ] = {
     "COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",
     "HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",
     "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",
     "HTTP_HOST", "HTTP_USER_AGENT", "PATH",
     "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",
     "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
     "SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",
     "SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",
     "SERVER_SIGNATURE","SERVER_SOFTWARE", "HTTP_COOKIE", "CONTENT_TYPE", "CONTENT_LENGTH" };
  std::string envToHTML(const std::vector<std::string> &_e){
    std::string fR;
    fR="<TABLE BORDER = \"0\">";
    for(long i=0;i<GL_NUM_ENV;++i){
      fR+="<TR><TD>"+GL_ENV[i]+"</TD><TD>";
      fR+=_e[i];
      fR+="</TD></TR>\n";
    }
    fR+="</TABLE>\n";
    return fR;
  }
  std::string privacyProtectionForIP(const std::string & i){
    return HENCF::oneWayEncHENCF(i,"m1234567890123456789012345678901234567890123456789012345678901234",1);
  }
  std::vector<std::string> getEnvVars(){
    std::vector<std::string> fR;
    long numEnvVars=GL_NUM_ENV;
    fR.resize(numEnvVars);
    for(long i=0;i<numEnvVars;++i){
      char* value =  getenv(GL_ENV[ i ].c_str() ) ;
        if ( value  != nullptr ) {
           fR[i]=    value  ;
        } else {
           fR[i]= "notFound";
        }
    }
    fR[11]=privacyProtectionForIP(fR[11]);
    return fR;
  }
  std::string textAreaField(const std::string & name="mText", const std::string & defaultT="", const long & numR=20, const long &numC=120){
    std::string fR="<textarea class=\"form-control\" name=\""+name+"\" rows=\""+std::to_string(numR);
    fR+="\"";
    fR+=">";
    fR+=PTKF::GL_PLAINTEXT_KEEPER.depositTxt(defaultT)+"</textarea>\n";
    return fR;
  }
  std::string radioButtonsField(const std::string & name, const std::vector<std::string> & allChoicesValues, const std::vector<std::string> &allChoicesDescriptions,const std::string &preSelection){
    std::string fR="";
    long sz=allChoicesValues.size();
    fR+="<div class=\"card\">\n<div class=\"card-body\">";
    std::vector<std::string> bgColors;
    bgColors.resize(2);
    bgColors[0]="bg-light";
    bgColors[1]="";
    long bgCInd=0;
    for(long i=0;i<sz;++i){
      fR+="<div class=\"form-check\"><input type=\"radio\" ";
      fR+=" class=\"form-check-input\" ";
      fR+="id=\""+name+allChoicesValues[i]+"\" ";
      fR+="name=\""+name;
      fR+="\" value=\""+allChoicesValues[i]+"\"";
      if (preSelection==allChoicesValues[i]){
        fR+=" checked=\"checked\"";
      }
      fR+=">  ";
      fR+=" <label class=\"form-check-label\" for=\"" +name+allChoicesValues[i];
      fR+="\">";
      fR+=allChoicesDescriptions[i];
      fR+="</label>";
      fR+="</div>\n";
      ++bgCInd;bgCInd%=2;
    }
    fR+="</div>\n</div>\n";
    return fR;
  }
  std::string textInputFieldMaxWidth(const std::string & name="field01", const std::string & defaultT=""){
    std::string fR="";
    fR+="<div class=\"form-group\">";
    fR+="<input class=\"form-control input-lg\" name=\""+name+"\"";
    fR+=" value=\"";
    std::string ty="password";
    if(defaultT!="password"){
      fR+=PTKF::GL_PLAINTEXT_KEEPER.depositTxt(defaultT);
      ty="text";
    }
    fR+="\" type=\""+ty+"\">";
    fR+="</div>";
    return fR;
  }
  std::string textInputField(const std::string & name="field01", const std::string & defaultT="", const long &sz = 30){
    if(sz>50){
      return textInputFieldMaxWidth(name,defaultT);
    }
    std::string fR="<INPUT NAME=\""+name;
    fR+="\" VALUE=\"";
    std::string ty="password";
    if(defaultT!="password"){
      fR+=PTKF::GL_PLAINTEXT_KEEPER.depositTxt(defaultT);
      ty="text";
    }
    fR+="\" TYPE=\""+ty+"\" SIZE=\"";
    fR+=std::to_string(sz)+"\">";
    return fR;
  }
  std::string submButton(const std::string &aN,const std::string &bL){
    std::string fR;
    fR+="<button type=\"submit\" name=\""+aN+ "\" class=\"btn btn-primary mb-2\" VALUE=\"";
    fR+=bL;
    fR+= "\">";
    fR+=bL;fR+="</button>";
    return fR;
  }
  std::string setCookie(const std::string &cookieName="mc", const std::string &cookieValue="myFirstCookie"){
    std::string fR="";
    fR+="Set-Cookie:";
    fR+=cookieName;fR+="=";fR+=cookieValue;
    fR+=";";
    return fR;
  }
  std::string oneLinkBarItem(const long & _stBox, const long & enBox, const long &_n,
                                               const long & _stCurrent, const std::string & _link_BC_start,
                                               const std::string & link_BC_middle, const std::string & link_BC_end,
                                               const std::string &scBar, const long & displayShift ) {
    std::string linkPrep;
    long stBox=_stBox;
    if(stBox<0){stBox=0;}
    std::string tText="["+std::to_string(stBox+displayShift)+","+std::to_string(enBox+displayShift)+")";
    std::string lBC_s=_link_BC_start;
    if(stBox==_stCurrent){
      lBC_s=SF::findAndReplace(lBC_s,"<li class=\"page-item\">","<li class=\"page-item active\">");
    }
    linkPrep=lBC_s+std::to_string(stBox)+scBar+link_BC_middle;
    linkPrep+=tText+link_BC_end;
    return linkPrep;
  }
  std::string createScrollerWithExtraEnd(const long & _st, const long & _en,
                             const long &_total, const long & _n, const std::string & _page= "!*!",
                             const std::string & _numOnEachSide="2",
                             const std::string & _sc="-1", const long & displayShift =0, const std::string & extEnd = "") {
    if(_n==0){return "";}
    std::string linkBar="";
    std::string dots="<li class=\"page-item\"><a class=\"page-link\" href=\"#\">  ... </a> </li>";
    std::string lBC_s="<li class=\"page-item\"><a class=\"page-link\" href=\"";
    lBC_s+= MWII::GL_WI.getWSURL()+"/index.cgi?";
    std::string page=_page;
    if(page=="!*!"){
      page="page="+MWII::GL_WI.getMainPageName();
    }
    lBC_s+=page;
    lBC_s+="&st=";
    std::string lBC_m="\">";
    std::string lBC_e="</a></li>";
    long goInEachDirection=BF::stringToInteger(_numOnEachSide);
    if(goInEachDirection<2){
      goInEachDirection=2;
    }
    long cSt=_st;long stSm,eSm;
    std::string tText;
    std::string linkPrep;
    std::string scBar="";
    std::string spaceSt="";
    if((_sc=="0")||(_sc=="1")){
      scBar="&sc="+_sc;
    }
    //Go LEFT
    long i=0;
    while((cSt>0)&&(i<goInEachDirection)){
      stSm=cSt-_n;
      linkBar=oneLinkBarItem(stSm,cSt, _n, _st, lBC_s,  lBC_m, lBC_e,scBar,displayShift )+spaceSt+linkBar;
      cSt=stSm;++i;
    }
    if(cSt>0){
      linkBar=oneLinkBarItem(0,_n, _n, _st,lBC_s,lBC_m, lBC_e, scBar,displayShift)+spaceSt+dots+spaceSt+linkBar;
    }
    //Go RIGHT
    cSt=_st;i=-1;
    while((i<goInEachDirection)&&(cSt<_total)){
      ++i;
      stSm=cSt;
      eSm=stSm+_n;
      if(eSm>_total){eSm=_total;}
      linkBar+=oneLinkBarItem(stSm, eSm, _n,_st, lBC_s, lBC_m, lBC_e, scBar,displayShift );
      cSt+=_n;
      linkBar+=spaceSt;
    }
    if(cSt<_total){
      linkBar+=dots+spaceSt+oneLinkBarItem(_total-_n,_total,_n,_st,lBC_s,lBC_m,lBC_e,scBar,displayShift);
    }
    if(extEnd!=""){
      linkBar+="<li>"+extEnd+"</li>";
    }
    return "<ul class=\"pagination\">\n"+ linkBar+"\n </ul>";
  }
  std::string createScroller(const long & _st, const long & _en,
                             const long &_total, const long & _n, const std::string & _page= "!*!",
                             const std::string & _numOnEachSide="2",
                             const std::string & _sc="-1", const long & displayShift =0) {
     return createScrollerWithExtraEnd(_st,_en,_total,_n,_page,_numOnEachSide,_sc,displayShift);
  }
  std::string updatedLinkNewEnvVars(const std::map<std::string,std::string> & _orig, const std::map<std::string,std::string> & update){
    std::map<std::string,std::string> orig=_orig;
    std::map<std::string,std::string>::const_iterator it,itE;
    it=update.begin();itE=update.end();
    while(it!=itE){
      orig[it->first]=it->second;
      ++it;
    }
    std::string fR="";
    fR+=MWII::GL_WI.getWSURL()+"/index.cgi";
    it=orig.begin();
    itE=orig.end();
    std::string nextEnvVarSep="?";
    while(it!=itE){
      if(it->second!="!remove!"){
        fR+=nextEnvVarSep;nextEnvVarSep="&";
        fR+=(it->first)+"="+(it->second);
      }
      ++it;
    }
    return fR;
  }
  std::string updatedLinkNewEnvVars(const std::map<std::string,std::string> &orig, const std::string & evK ="", const std::string & evV=""){
    std::map<std::string,std::string> update;
    if(evK!=""){update[evK]=evV;}
    return updatedLinkNewEnvVars(orig,update);
  }
  std::string createNavigationBar(const std::vector<std::string> & navLinks, const std::vector<std::string> &_navLabels){
    std::string fR="";
    long sz=navLinks.size();
    std::vector<std::string> navLabels=_navLabels;
    if(navLabels.size()!=sz){
      navLabels=navLinks;
    }
    fR+="<table class=\"table table-striped\"><tbody><tr>";
    for(long i=0;i<sz;++i){
      fR+="<td>";
      fR+="<a href=\""+navLinks[i]+"\">"+navLabels[i]+"</a>";
      fR+="</td>";
    }
    fR+="</tr></tbody></table>";
    return fR;
  }
}
#endif
