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

#ifndef _INCL_WI_AbstractText_CPP
#define _INCL_WI_AbstractText_CPP

namespace APTI{
  AbstractText::AbstractText(const std::string & _nameInDB, const std::string & _sysR, const std::string & _u){
  }
  void AbstractText::addToText(const std::string &_a){
    rawText+=_a;
  }
  std::stack<std::pair<std::string,std::string> > AbstractText::logInBarRaw(const std::string & _uN) const{
    std::stack<std::pair<std::string,std::string> > lbr;
    lbr.push( createLinkPair("page","changePassword",_uN) );
    lbr.push(createLogOutPair(MWII::GL_WI.getDefaultWebText("Log Out")));
    if(_uN=="visitor"){
      SF::clearStack(lbr);
      lbr.push(createLogInLinkPair(MWII::GL_WI.getDefaultWebText("Sign in")));
    }
    SF::flipTheStack(lbr);
    return lbr;
  }
  std::stack<std::pair<std::string,std::string> > AbstractText::secondMenuStack(const PSDI::SessionData & _psd, const std::string & _pR, const std::string & _rRR, const std::string & _uN) const{
    std::stack<std::pair<std::string,std::string> > fR;
    std::pair<std::string,std::string> tmp;
    tmp=createModifyLinkPair(_psd,_pR,_rRR, _uN,"y",MWII::GL_WI.getDefaultWebText("edit"));
    if(tmp.first!="notFound"){
      fR.push(tmp);
    }
    else{
      if(_uN!="visitor"){
        MPTI::MainText tToCheck(_psd.pageRequested,"no1117",_psd.my_un);
        if (tToCheck.allowedToModifyText(_psd,"","",_psd.my_un)==1){
          tmp=createModifyLinkPairUnsafe(_psd,_pR,_rRR, _uN,"w",MWII::GL_WI.getDefaultWebText("edit"));
          fR.push(tmp);
        }
      }
    }
    tmp=createModifyLinkPair(_psd,_pR,_rRR,_uN,"r",MWII::GL_WI.getDefaultWebText("advanced"));
    if(tmp.first!="notFound"){fR.push(tmp);}
    tmp=createCommandLinkPair(_psd,_pR,_rRR,_uN,MWII::GL_WI.getDefaultWebText("commands"));
    if(tmp.first!="notFound"){fR.push(tmp);}
      if(_psd.isRoot=="yes"){
          fR.push(createLinkPair("page","database",MWII::GL_WI.getDefaultWebText("DB")));
          fR.push(createLinkPair("page","listTexts",MWII::GL_WI.getDefaultWebText("TX")));
      }
    SF::flipTheStack(fR);
    return fR;
  }
  std::stack<std::pair<std::string,std::string> > AbstractText::thirdMenuStack(const PSDI::SessionData & _psd, const std::string & _pR, const std::string & _rRR, const std::string & _uN) const{
    std::pair<std::string,std::string> tP=createModifyLinkPair(_psd,_pR,_rRR,_uN,"n","");
    std::stack<std::pair<std::string,std::string> > mS;
    if(tP.first!="notFound"){
      mS.push(tP);
    }
    return mS;
  }
  void AbstractText::updateLogInAndMenuBars(const PSDI::SessionData & _psd, const std::string& _pR, const std::string & _rRR){
    std::map<std::string,std::string> replMap;
    replMap["_THIS*WEBSITE*URL*_"]=MWII::GL_WI.getWSURL();
    replMap["_LOGIN*OUT*REGISTER*_"]=HSF::buttonsBar(logInBarRaw(_psd.my_un));
    replMap["_LOGIN*OUT*REGISTER*NAVBAR*_"]=HSF::buttonsBar(logInBarRaw(_psd.my_un),"navBar");
    replMap["_**LINKS*LINE2*_"]=HSF::buttonsBar(secondMenuStack(_psd,  _pR, _rRR, _psd.my_un));
    replMap["_**LINKS*LINE3*_"]=HSF::buttonsBar(thirdMenuStack( _psd,  _pR, _rRR, _psd.my_un));
    replMap["_LIST*OF*MENU*ITEMS_"]="";
    rawText=MFRF::findAndReplace(rawText,replMap);
  }
  void AbstractText::updateFooterBar(const PSDI::SessionData & _psd, const std::string & _pR, const std::string & _rRR, const std::string & _uN){
    std::string tBar="";
    rawText= SF::findAndReplace(rawText,"_FOOTER*BAR*_ ",tBar);
  }
  int AbstractText::rawSysDataAllowed(const PSDI::SessionData & _psd) const{
    // WARNING this has to be modified
    return 1;
  }
  int AbstractText::sysDataAllowed(const PSDI::SessionData & _psd) const{
    // WARNING this has to be modified
    return 1;
  }
  std::string AbstractText::groupsInPermission(const std::set<std::string> & _s) const{
    std::set<std::string>::const_iterator it,itE,itB;
    std::string fR="";
    it=_s.begin();itE=_s.end();
    itB=_s.begin();
    while(it!=itE){
      if(it!=itB){
        fR+="; ";
      }
      fR+=*it;
      ++it;
    }
    return fR;
  }
  std::string AbstractText::nicelyFormattedSystemData(const PSDI::SessionData & _psd) const{
    std::string fR="<TABLE BORDER=0>\n";
    fR+="<TR><TD> Username: </TD><TD>"+myUserName+"</TD></TR>";
    fR+="<TR><TD> Read permissions: </TD><TD>" +groupsInPermission(permitRead)+"</TD></TR>";
    fR+="<TR><TD> Write permissions: </TD><TD>" +groupsInPermission(permitWrite)+"</TD></TR>";
    fR+="<TR><TD> Created: </TD><TD>" +tCreated+"</TD></TR>";
    fR+="<TR><TD> Created by: </TD><TD>" +createdBy+"</TD></TR>";
    fR+="<TR><TD> Modified: </TD><TD>" +tCreated+"</TD></TR>";
    fR+="<TR><TD> Modified by: </TD><TD>"+modifiedBy+"</TD></TR>";
    fR+="<TR><TD> Document type: </TD><TD>" +documentType+"</TD></TR>";
    fR+="</TABLE>\n";
    return fR;
  }
  std::string AbstractText::sysDataIfNeededAndAllowed(const PSDI::SessionData  & _psd) const{
    std::string fR="";
    if(sysDataRequested==GL_syntax.val_sysDataReq_RAW){
      if(rawSysDataAllowed(_psd)!=1){
        return fR;
      }
      fR = "<P>" + sysDataRaw+"</P>";
    }
    if(sysDataRequested==GL_syntax.val_sysDataReq_YES){
      if(sysDataAllowed(_psd)!=1){
        return fR;
      }
      return nicelyFormattedSystemData(_psd);
    }
    return fR;
  }
  long AbstractText::allowedToInputCommands(const PSDI::SessionData  & _psd) const{
    //WARNING - function not created yet
    return 1;
  }
  std::string AbstractText::createTextAreaField(const PSDI::SessionData & _psd, const std::string & formName, const std::string & fieldName, const std::string & _textBefore, const std::string & _textInTheField, const std::string & _nRow, const std::string & _nColl){
    std::string fR="";
    if(fieldName==GL_syntax.s_commands){
      if(allowedToInputCommands(_psd)!=1){
        return "";
      }
    }
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      FHI::FormField fFF;
      std::string textBefore="";
      std::string textInTheField="";
      if(_textInTheField!=GL_syntax.s_notFound){
        textInTheField=_textInTheField;
      }
      if(_textBefore!=GL_syntax.s_notFound){
        textBefore=_textBefore;
      }
      if((_nColl==GL_syntax.s_notFound)||(_nRow==GL_syntax.s_notFound)){
        fFF.convertToTextArea(fieldName,textBefore,textInTheField);
      }
      else{
        long nc=BF::stringToInteger(_nColl);
        long nr=BF::stringToInteger(_nRow);
        if(nr<5){nr=5;}
        if(nc<80){nc=80;}
        fFF.convertToTextArea(fieldName,textBefore,textInTheField,nr,nc);
      }
      (allForms[formName]).addChangeField(fFF);
    }
    return fR;
  }
  std::string AbstractText::createRadioButtonsField(const std::string & formName, const std::string & fieldName, const std::string & _textBefore, const std::string & _textAfter, const std::string & _allChoices, const std::string & _preSelection){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      FHI::FormField fFF;
      std::string textBefore="";
      if(_textBefore!=GL_syntax.s_notFound){
        textBefore=_textBefore;
      }
      fFF.convertToRadioButtons(fieldName,textBefore,_textAfter, _allChoices,_preSelection);
      (allForms[formName]).addChangeField(fFF);
    }
    return fR;
  }
  std::string AbstractText::createTextInputField(const std::string & formName, const std::string & fieldName, const std::string & _textBefore, const std::string & _textInTheField, const std::string & _dim){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      FHI::FormField fFF;
      std::string textBefore="";
      std::string textInTheField="";
      std::string dim="15";
      if(_textInTheField!=GL_syntax.s_notFound){
        textInTheField=_textInTheField;
      }
      if(_textBefore!=GL_syntax.s_notFound){
        textBefore=_textBefore;
      }
      if(_dim!=GL_syntax.s_notFound){
        dim=_dim;
      }
      long d=BF::stringToInteger(dim);
      if(d<3){d=3;}
      fFF.convertToTextInput(fieldName,textBefore,textInTheField,d);
      (allForms[formName]).addChangeField(fFF);
    }
    return fR;
  }
  std::string AbstractText::createAntiSpamField(const std::string & formName, const std::string & fieldName, const std::string & _textBefore, const std::string & _dim, const std::string & _len){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      FHI::FormField fFF;
      std::string textBefore="";
      std::string textInTheField="";
      std::string dim="10";
      if(_textBefore!=GL_syntax.s_notFound){
        textBefore=_textBefore;
      }
      if(_dim!=GL_syntax.s_notFound){
        dim=_dim;
      }
      long d=BF::stringToInteger(dim);
      if(d<3){d=3;}
      long len=BF::stringToInteger(_len);
      if((len<1)||(len>20)){len=8;}
      fFF.convertToAntiSpamChallenge(fieldName,textBefore,d,len);
      (allForms[formName]).addChangeField(fFF);
    }
    return fR;
  }
  std::string AbstractText::createFileRequestField(const std::string & formName, const std::string & fieldName, const std::string & _textBefore){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      FHI::FormField fFF;
      std::string textBefore="";
      if(_textBefore!=GL_syntax.s_notFound){
        textBefore=_textBefore;
      }
      fFF.convertToFileInput(fieldName,textBefore);
      (allForms[formName]).addChangeField(fFF);
    }
    return fR;
  }
  std::string AbstractText::initializeForm(const std::string & formName, const std::string & _link){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it==itE){
      std::string link="index.cgi?fSubmit=su";
      if(_link!=GL_syntax.s_notFound){
        link=_link;
      }
      FHI::InputForm inpF(link);
      allForms[formName]=inpF;
    }
    return fR;
  }
  std::string AbstractText::setRedirectInfo(const std::string & formName){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      (allForms[formName]).setRedirectInfo(MWII::GL_WI.getRedirectForward());
    }
    return fR;
  }
  std::string AbstractText::placeFormInText(const std::string & formName, const std::string & _bLoc, const std::string & _bLabel){
    std::string fR="";
    std::map<std::string,FHI::InputForm>::const_iterator it,itE;
    itE=allForms.end();
    it=allForms.find(formName);
    if(it!=itE){
      fR+=(allForms[formName]).displayForm(_bLoc,_bLabel);
    }
    return fR;
  }
  int AbstractText::allowedToDisplayText(const PSDI::SessionData & _psd,
                                             const std::string & _uN,
                                             const std::string & _position) const{
    //WARNING: needs to be improved - If text is not "regularText",
    //                                   then very limited permits are given at the moment.
    //                                   In general, only root can see these texts.
    //                                       - exception: If it is a coding problem
    //                                          then even though the documentType is "problem"
    //                                          the display will be allowed if the correct permits are given
    //                                          and the _position = "codeTestInNotes"
    if(_psd.isRoot=="yes"){
      return 1;
    }
    if(PBD::permitExists_N("read"+tName,permitRead,_uN)==0){
      return 0;
    }
    if(_position=="codeTestInNotes"){
      if(documentType=="problem"){
        return 1;
      }
    }
    if(_position=="fileFormTemplate"){
      if(documentType=="formTemplate"){
        return 1;
      }
    }
    if(documentType!=GL_syntax.v_regularText){
      if(_position=="mainTextPosition"){
        return 0;
      }
      if(documentType=="problem"){
        return 0;
      }
    }
    return 1;
  }
  int AbstractText::allowedToModifyText(const PSDI::SessionData & _psd, const std::string & _pR, const std::string & _rRR, const std::string & _uN) const{
    if(_psd.isRoot=="yes"){
      return 1;
    }
    if(PBD::permitExists_N("write"+tName,permitWrite,_uN)==0){
      return 0;
    }
    return 1;
  }
  std::pair<std::string,std::string> AbstractText::createModifyLinkPairUnsafe(const PSDI::SessionData & _psd, const std::string & _pR,const std::string & _rRR, const std::string & _uN, const std::string & _editType, const std::string & _linkText) const{
    std::pair<std::string,std::string> fR;
    std::string pageOrRRC=MWII::GL_WI.get_e_parPage();
    std::string addArg=_pR;
    if(_rRR!=""){
      pageOrRRC= MWII::GL_WI.get_e_respRecReqRT();
      addArg=_rRR;
    }
    pageOrRRC+="=";
    std::string url=MWII::GL_WI.getWSURL()+"/index.cgi?"+pageOrRRC+addArg;
    if(_editType!="n"){
      url+="&er="+_editType;
    }
    fR.first=url;
    if((_editType=="n")&&(_linkText=="")){
      fR.second=addArg;
    }
    fR.second+=_linkText;
    if( (_editType=="n") && (_linkText=="") ){
      fR.second = SF::wrapTextToPreventAlphabetChange(fR.second);
    }
    return fR;
  }
  std::pair<std::string,std::string> AbstractText::createModifyLinkPair(const PSDI::SessionData & _psd, const std::string & _pR,const std::string & _rRR, const std::string & _uN, const std::string & _editType, const std::string & _linkText) const{
    if(allowedToModifyText(_psd,_pR,_rRR,_uN)==0){
      return std::pair<std::string,std::string>("notFound","notFound");
    }
    return createModifyLinkPairUnsafe(_psd,_pR, _rRR, _uN,  _editType, _linkText);
  }
  std::string AbstractText::createModifyLink(const PSDI::SessionData & _psd, const std::string & _pR,const std::string & _rRR, const std::string & _uN, const std::string & _editType, const std::string & _linkText) const{
    std::pair<std::string,std::string> p=createModifyLinkPair(_psd,_pR, _rRR, _uN,  _editType, _linkText);
    if(p.first=="notFound"){
      return "";
    }
    return HSF::linkFromPair(p);
  }
  std::pair<std::string,std::string> AbstractText::createCommandLinkPair(const PSDI::SessionData & _psd, const std::string & _pR,  const std::string & _rRR, const std::string & _uN, const std::string & _linkText) const{
    if(allowedToModifyText(_psd,_pR,_rRR,_uN)==0){
      return std::pair<std::string,std::string> ("notFound","notFound");
    }
    return createLinkPair("page","commandForm",_linkText);
  }
  std::string AbstractText::createCommandLink(const PSDI::SessionData & _psd, const std::string & _pR,  const std::string & _rRR, const std::string & _uN, const std::string & _linkText) const{
    if(allowedToModifyText(_psd,_pR,_rRR,_uN)==0){
      return "";
    }
    return HSF::linkFromPair(createLinkPair("page","commandForm",_linkText));
  }
  std::string AbstractText::createLinkForEditUser(const std::string &uN) const{
    if(uN=="!*!"){return "";}
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page=commandToEditUser&s1=u11&r1="+uN+"&ev0=uName&el0="+uN+"\">"+uN+"</A>";
  }
  std::string AbstractText::createLinkForEditUserAdvanced(const std::string &uN,const std::string &displ) const{
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page=commandToEditUserA&s1=u11&r1="+uN+"&ev0=uName&el0="+uN+"\">"+displ+"</A>";
  }
  std::string AbstractText::createLinkForEditUserExtraAdvanced(const std::string &uN,const std::string &displ) const{
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page=commandToEditUserEA&s1=u11&r1="+uN+"&ev0=uName&el0="+uN+"\">"+displ+"</A>";
  }
  std::string AbstractText::createLinkToSwitchSortCriterion(const std::string &displ, const std::string & pg, const std::string & nsc, const std::string &st) const{
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+pg+"&st="+st+"&sc="+nsc+"\">"+displ+"</A>";
  }
  std::string AbstractText::createListOfUsers(const PSDI::SessionData & _psd,const std::string & _numInList, const std::string & _numOnEachSide) const{
    std::string sc=MWII::GL_WI.getSortCriterion();
    long numUsr=DD::GL_MAIN_DB.numUsers();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>numUsr-1)){start=0;}
    if((sc!="0")&&(sc!="1")){sc="0";}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    WUD::User w;
    int res;
    long lsz=6+DD::GL_IND_ALLOW_WEBSITE_CLONES;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    MTF::Table& uTable=DD::GL_MAIN_DB.dbsM["usernames"];
    TOSF::TSets indexUn=uTable.getIndTable("_i_1_ei_");
    long indSz=indexUn.size();
    std::string searchBeginString=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"sBg","!!notFound");
    if(searchBeginString!="!!notFound"){
      std::vector<std::string> sbstV;
      sbstV.resize(1); sbstV[0]=searchBeginString;
      long indLow=indexUn.lowerBoundRowIndex(sbstV);
      if(indLow<0){indLow=0;}
      if(indLow<indSz){
        start=indLow;sc="1";
      }
    }
    long end=start+numb;
    if((end<0)||(end>numUsr)){
      end=numUsr;
    }
    if(sc=="1"){if(indSz<end){end=indSz;}}
    std::string scSw="0";
    if(sc=="0"){scSw="1";}
    topLine[0]=createLinkToSwitchSortCriterion(MWII::GL_WI.getDefaultWebText("Username"),"listUsers",scSw,MWII::GL_WI.getStartOfList());
    topLine[1]=MWII::GL_WI.getDefaultWebText("Ext ID");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Name");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Email");
    topLine[4]=MWII::GL_WI.getDefaultWebText("Root");
    if(DD::GL_IND_ALLOW_WEBSITE_CLONES==1){
      topLine[5]=MWII::GL_WI.getDefaultWebText("Clone");
    }
    topLine[5+DD::GL_IND_ALLOW_WEBSITE_CLONES]=MWII::GL_WI.getDefaultWebText("Last Log In");
    for(long i=start;i<end;++i){
      if(sc=="1"){
        w.setFromUsername((indexUn.getKeysFromRow(i))[0]);
      }
      else{
        w.setFromIndexFromInternalIdDB(i);
      }
      if(w.isRoot()=="yes"){
        mLine[0]=w.getUsername();
      }
      else{
        mLine[0]=createLinkForEditUser(w.getUsername());
      }
      mLine[1]=w.getExternalId();
      mLine[2]=w.getFirstName()+" "+w.getLastName();
      mLine[3]=w.getEmail();
      mLine[4]=createLinkForEditUserAdvanced(w.getUsername(),w.isRoot());
      if(DD::GL_IND_ALLOW_WEBSITE_CLONES==1){
        mLine[5]=createLinkForEditUserExtraAdvanced(w.getUsername(), w.isAllowedToCloneWebsite());
      }
      mLine[5+DD::GL_IND_ALLOW_WEBSITE_CLONES]=w.getLastLogInTime();
      allLines.push(mLine);
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,numUsr,numb,"!*!",_numOnEachSide,sc)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createLinkToText(const std::string &tN,const std::string &parameter, const std::string & _label, const std::string& _addition) const{
    std::string label=_label;
    if(label==""){
      label=tN;
    }
    return HSF::linkFromPair(createLinkPair(parameter,tN,label),_addition);
  }
  std::string AbstractText::createLinkToMessage(const std::string &mId,const std::string & _messNameOrId,const std::string &_page) const{
    std::string messNameOrId=_messNameOrId;
    if(messNameOrId==""){
      messNameOrId=mId;
    }
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+mId+"\">"+messNameOrId+"</A>";
  }
  std::string AbstractText::createLinkToCouas(const std::string &cId,const std::string & _couasNameOrId,const std::string & _page) const{
    std::string cNameOrId=_couasNameOrId;
    if(cNameOrId==""){
      cNameOrId=cId;
    }
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+cId+"\">"+cNameOrId+"</A>";
  }
  std::string AbstractText::createLinkToCertificate(const std::string &cId, const std::string & _linkName, const std::string & _page, const std::string & _editExtension) const{
    std::string lName=_linkName;
    if(lName==""){
      lName=cId;
    }
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+cId+_editExtension+"\">"+lName+"</A>";
  }
  std::string AbstractText::createLinkToExecuteBackup(const std::string & backupName, const std::string & backupType) const{
    std::string textNameForLink="restoreText";
    if(backupType=="courseCreation"){textNameForLink="undoCourseCreation";}
    if(backupType=="enrollment"){textNameForLink="undoStudentEnrollment";}
    if(backupType=="assignmentCreation"){textNameForLink="undoAssignmentCreation";}
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+textNameForLink+"&ev0=docName&el0="+backupName+"\">"+backupName+"</A>";
  }
  std::string AbstractText::createLinkToExpandStat(const std::string & sName,const long &startingNum) const{
    std::string sT="";
    for(long i=startingNum;i<sName.length();++i){
      sT+=sName[i];
    }
    std::string textNameForLink="statExpand";
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+textNameForLink+"&ev0=docName&el0="+sName+"&ev1=pageName&el1="+sT+"\">"+sT+"</A>";
  }
  std::string AbstractText::createLinkToFile(const std::string &fN,const std::string &fC,const std::string & fP,const std::string &ext) const{
    return "<A target=\"new\" href=\""+fP+"/"+MWII::FILE_PREFIX+fN+"."+ext+"\">"+fC+"</A>";
  }
  std::string AbstractText::createLinkToResponseReceiver(const std::string &tN,const std::string &_dR) const{
    std::string lrr= "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?"+MWII::GL_WI.get_e_respRecReqRT()+"=";
    lrr+=tN;
    std::string nm=tN;
    if(_dR!=""){
      lrr+="&rdd="+_dR;
      nm=_dR;
    }
    lrr+="\">"+nm+"</A>";
    return lrr;
  }
  std::string AbstractText::createListOfTexts(const PSDI::SessionData & _psd,const std::string & _numInList, const std::string & _numOnEachSide) const{
    std::string sc=MWII::GL_WI.getSortCriterion();
    long nT=DD::GL_MAIN_DB.numTexts();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    if((sc!="0")&&(sc!="1")){sc="0";}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    TMD::MText sf;
    std::pair<std::string,int> allD;
    std::string dType,createdBy,createdOn,modifiedOn,othD;
    int res;
    long lsz=5;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    long pos;
    std::pair<std::vector<std::string>, std::string> sR;
    MTF::Table& textTable=DD::GL_MAIN_DB.dbsM["mainText"];
    TOSF::TSets indexTx=textTable.getIndTable("_i_1_ei_");
      std::string savingIndicator="";
    long indSz=indexTx.size();
    std::string searchBeginString=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"sBg","!!notFound");
    if(searchBeginString!="!!notFound"){
      std::vector<std::string> sbstV;
      sbstV.resize(1); sbstV[0]=searchBeginString;
      long indLow=indexTx.lowerBoundRowIndex(sbstV);
      if(indLow<0){indLow=0;}
      if(indLow<indSz){
        start=indLow;sc="1";
      }
    }
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    if(sc=="1"){if(indSz<end){end=indSz;}}
    if(sc=="0"){
      std::map<std::string,std::string>::const_iterator it=_psd.respMap.find("save");
      if(it!=_psd.respMap.end()){
        DD::GL_MAIN_DB.saveForRecovery("mainText",start,end,it->second);
          savingIndicator="savingIndicatorYes";
      }
    }
    std::string scSw="0";
    if(sc=="0"){scSw="1";}
    topLine[0]=createLinkToSwitchSortCriterion(MWII::GL_WI.getDefaultWebText("Name"),"listTexts",scSw,MWII::GL_WI.getStartOfList());
    topLine[1]=MWII::GL_WI.getDefaultWebText("Type");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Created by");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Created on");
    topLine[4]=MWII::GL_WI.getDefaultWebText("Modified on");
    for(long i=start;i<end;++i){
      if(sc=="1"){
        sf.setFromTextName(((indexTx.getKeysFromRow(i))[0]));
      }
      else{
        sR=(DD::GL_MAIN_DB.dbsM["mainText"])[i];
        sf.setFromInternalId(sR.first[0]);
      }
      mLine[0]=createLinkToText(sf.getTextName());
      pos=0;
      othD=sf.getTextData();
      allD=SF::extract(othD,pos,"_documentType!!_","_/documentType!!_");
      dType="";
      if(allD.second==1){
        dType=allD.first;
      }
      pos=0;
      allD=SF::extract(othD,pos,"_createdBy!!_","_/createdBy!!_");
      createdBy="";
      if(allD.second==1){
        createdBy=allD.first;
      }
      pos=0;
      allD=SF::extract(othD,pos,"_created!!_","_/created!!_");
      createdOn="";
      if(allD.second==1){
        createdOn=allD.first;
      }
      modifiedOn=createdOn;
      pos=0;allD=SF::extract(othD,pos,"_modified!!_","_/modified!!_");
      if(allD.second==1){
        modifiedOn=allD.first;
      }
      mLine[1]=dType;mLine[2]=createdBy;mLine[3]=createdOn;mLine[4]=modifiedOn;
      allLines.push(mLine);
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScrollerWithExtraEnd(start,end,nT,numb,"!*!",_numOnEachSide,sc,0,savingIndicator)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createListOfFiles(const std::string & _numInList, const std::string & _numOnEachSide) const{
    long nT=DD::GL_MAIN_DB.numFiles();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    FMD::FileManager sf;
    std::pair<std::string,int> allD;
    std::string code ,path,extension,userN, createdOn,othD,createdBy,respRecName,fdataType,fName;;
    int res;
    long lsz=9;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    topLine[0]=MWII::GL_WI.getDefaultWebText("ID");
    topLine[1]=MWII::GL_WI.getDefaultWebText("Edit");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Delete");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Added by");
    topLine[4]=MWII::GL_WI.getDefaultWebText("ext");
    topLine[5]=MWII::GL_WI.getDefaultWebText("Type");topLine[6]="Date";topLine[7]="Resp.";topLine[8]="Desc. / name";
    long pos;
    for(long i=start;i<end;++i){
      std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["fileMan"])[i];
      sf.setFromInternalId(sR.first[0]);
      othD=sf.getTextData();
      code=sf.getExternalCodeFromInternalId();
      pos=0;
      allD=SF::extract(othD,pos,"_filePath_","_/filePath_");
      path="";
      if(allD.second==1){path=allD.first;}
      pos=0;
      fdataType=MWII::GL_WI.getDefaultWebText("unknown");
      allD=SF::extract(othD,pos,"_fileDataType_","_/fileDataType_");
      if(allD.second==1){
        fdataType=allD.first;
      }
      fName=code;
      pos=0; allD=SF::extract(othD,pos,"_fileName_","_/fileName_");
      if(allD.second==1){
        fName=allD.first;
      }
      pos=0;
      allD=SF::extract(othD,pos,"_fileExtension_","_/fileExtension_");
      extension="";
      if(allD.second==1){extension=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_user_","_/user_");
      userN="";
      if(allD.second==1){userN=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_created_","_/created_");
      createdOn="";
      if(allD.second==1){createdOn=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_fInfo*|_","_/fInfo*|_");
      createdBy="";
      if(allD.second==1){createdBy=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_nRespRec*|_","_/nRespRec*|_");
      respRecName="";
      if(allD.second==1){respRecName=allD.first;}
      mLine[0]=createLinkToFile(fName,code,path,extension);
      mLine[1]=createLinkToText("edit",MWII::GL_WI.get_e_parPage()+"=listFiles&st="+std::to_string(start)+"&fReq="+code+"&amF",MWII::GL_WI.getDefaultWebText("edit"));
      mLine[2]="_hideReveal__revealTitle_"+MWII::GL_WI.getDefaultWebText("delete")+"_/revealTitle__hideTitle_"+MWII::GL_WI.getDefaultWebText("cancel")+"_/hideTitle_";
      mLine[2]+=createLinkToText("delete",MWII::GL_WI.get_e_parPage()+"=listFiles&st="+std::to_string(start)+"&fReq="+code+"&amF",MWII::GL_WI.getDefaultWebText("delete"));
      mLine[2]+="_/hideReveal_";
      mLine[3]=userN;
      mLine[4]=extension;
      mLine[5]=fdataType;
      mLine[6]=createdOn;
      mLine[7]=createLinkToText(respRecName, MWII::GL_WI.get_e_respRecReqRT());
      mLine[8]=createdBy;
      allLines.push(mLine);
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createListOfMessages(const std::string & _numInList, const std::string & _numOnEachSide) const{
    long nT=DD::GL_MAIN_DB.numMessages();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    MD::Message bm;
    std::pair<std::string,int> allD;
    std::string code , userN, createdOn;
    std::string othD;
    int res;
    long lsz=5;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    topLine[0]=MWII::GL_WI.getDefaultWebText("ID");
    topLine[1]=MWII::GL_WI.getDefaultWebText("Edit");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Advanced");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Added by");
    topLine[4]=MWII::GL_WI.getDefaultWebText("Date");
    long pos;
    for(long i=start;i<end;++i){
      std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["messMan"])[i];
      bm.setFromInternalId(sR.first[0]);
      othD=bm.getTextData();
      code=bm.getExternalCodeFromInternalId();
      pos=0;
      allD=SF::extract(othD,pos,"_user_","_/user_");
      userN="";
      if(allD.second==1){userN=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_created_","_/created_");
      createdOn="";
      if(allD.second==1){createdOn=allD.first;}
      MEI::MElement mEl;
      if(MEI::initMElementFromRawText(mEl,othD,code)=="success"){
        mLine[0]=createLinkToMessage(code,mEl.title(),MWII::GL_WI.getDefaultWebText("forum"));
        std::string cdB=code;
        if(mEl.numNavEls()>0){
          cdB=mEl.navEl(mEl.numNavEls()-1);
          long pos;
          std::pair<std::string, int> allD;
          pos=0;allD=SF::extract(cdB,pos,"_!code_","_/!code_");
          if(allD.second==1){
            cdB=allD.first;
          }
        }
        mLine[1]=mEl.createMTEditLink(cdB,code,"edit","",MWII::GL_WI.getDefaultWebText("forum"));
        mLine[2]=mEl.createMTEditLink("adv",code,"adv.","",MWII::GL_WI.getDefaultWebText("forum"));
        mLine[3]=userN;
        mLine[4]=createdOn;
        allLines.push(mLine);
      }
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createListOfCoursesAssignments(const PSDI::SessionData & _psd,const std::string & _numInList, const std::string & _numOnEachSide) const{
    long nT=DD::GL_MAIN_DB.numCoursesAssignments();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    CAD::CouAs myC;
    std::pair<std::string,int> allD;
    std::string code , userN, createdOn;
    std::string othD;
    int res;
    long lsz=5;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    topLine[0]=MWII::GL_WI.getDefaultWebText("Title");
    topLine[1]=MWII::GL_WI.getDefaultWebText("Code / Label");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Advanced");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Added by");
    topLine[4]=MWII::GL_WI.getDefaultWebText("Date");
    long pos;
    for(long i=start;i<end;++i){
      std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["couasMan"])[i];
      myC.setFromInternalId(sR.first[0]);
      othD=myC.getTextData();
      code=myC.getExternalCodeFromInternalId();
      pos=0;
      allD=SF::extract(othD,pos,"_user_","_/user_");
      userN="";
      if(allD.second==1){userN=allD.first;}
      pos=0;
      allD=SF::extract(othD,pos,"_created_","_/created_");
      createdOn="";
      if(allD.second==1){createdOn=allD.first;}
      CEI::CouasElement cEl(_psd);
      if(ICEI::initFromRawText(_psd,cEl,othD,code)=="success"){
        mLine[0]=createLinkToCouas(code,cEl.title(),MWII::GL_WI.getDefaultWebText("assignments"));
        mLine[1]=code;
        mLine[2]=cEl.createMTEditLink("adv",code,"adv.",MWII::GL_WI.getDefaultWebText("assignments"));
        mLine[3]=userN;
        mLine[4]=createdOn;
        allLines.push(mLine);
      }
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createListOfCertificates(const PSDI::SessionData & _psd,const std::string & _numInList, const std::string & _numOnEachSide) const{
    long nT=DD::GL_MAIN_DB.numCertificates();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    CERD::Certificate myC;
    std::pair<std::string,int> allD;
    std::string code, cHelpAdmin, createdOn, modifiedOn, intUID;
    std::string othD;
    int res;
    long lsz=5;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    topLine[0]=MWII::GL_WI.getDefaultWebText("Code / Label");
    topLine[1]=MWII::GL_WI.getDefaultWebText("Name/Description");
    topLine[2]=MWII::GL_WI.getDefaultWebText("Advanced");
    topLine[3]=MWII::GL_WI.getDefaultWebText("Created");
    topLine[4]=MWII::GL_WI.getDefaultWebText("Modified");
    long pos;
    for(long i=start;i<end;++i){
      std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["certMan"])[i];
      myC.setFromInternalId(sR.first[0]);
      othD=myC.getTextData();
      code=myC.getExternalCodeFromInternalId();
      cHelpAdmin=myC.getKey1();
      pos=0; allD=SF::extract(othD,pos,"_created!!_","_/created!!_");
      createdOn="";
      if(allD.second==1){
        createdOn=allD.first;
      }
      modifiedOn=createdOn;
      pos=0;allD=SF::extract(othD,pos,"_modified!!_","_/modified!!_");
      if(allD.second==1){
        modifiedOn=allD.first;
      }
      mLine[0]=createLinkToCertificate(code,code,MWII::GL_WI.getDefaultWebText("certificates"));
      mLine[1]=cHelpAdmin;
      mLine[2]=createLinkToCertificate(code,"edit",MWII::GL_WI.getDefaultWebText("certificates"),"&emr=r");
      mLine[3]=createdOn;
        mLine[4]=modifiedOn;
        allLines.push(mLine);

    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
    std::string AbstractText::createListOfBackups(const std::string & _numInList, const std::string & _numOnEachSide) const{
      long nT=DD::GL_MAIN_DB.numBackups();
      long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
      if((start<0)||(start>nT-1)){start=0;}
      long numb=BF::stringToInteger(_numInList);
      if(numb<0){numb=0;}
      long end=start+numb;
      if((end<0)||(end>nT)){
        end=nT;
      }
      std::pair<std::string,int> allD;
      std::string name, type, user;
      int res;
      long lsz=3;
      std::vector<std::string> topLine,mLine;
      topLine.resize(lsz);mLine.resize(lsz);
      std::stack<std::vector<std::string> > allLines;
      topLine[0]=MWII::GL_WI.getDefaultWebText("Name");
      topLine[1]=MWII::GL_WI.getDefaultWebText("Type");
      topLine[2]=MWII::GL_WI.getDefaultWebText("User");
      long pos;
      for(long i=start;i<end;++i){
        std::pair<std::vector<std::string>, std::string> sR=(DD::GL_MAIN_DB.dbsM["backupDatabase"])[i];
        name=sR.first[0];
        user="";
        pos=0;allD=SF::extract(sR.second,pos,"_user_","_/user_");
        if(allD.second==1){user=allD.first;}
        type="";
        pos=0;allD=SF::extract(sR.second,pos,"_type_","_/type_");
        if(allD.second==1){type=allD.first;}
        mLine[0]=createLinkToExecuteBackup(name,type);
        mLine[1]=type;
        mLine[2]=user;
        allLines.push(mLine);
      }
      SF::flipTheStack(allLines);
      allLines.push(topLine);
      return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
    } 
    std::string AbstractText::createListOfStatItems(const PSDI::SessionData & _psd, const std::string & _numInList, const std::string & _numOnEachSide) const{
      std::string sc=MWII::GL_WI.getSortCriterion();
      long numb=BF::stringToInteger(_numInList);
      if(numb<0){numb=0;}
      long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
      if(sc=="raw"){
        return SDIRF::getRawStats(DD::GL_DBS.getStatTable(),3*SPREPF::STAT_CONSTS.maxItemsInFile,start);
      }
      SDIRF::CompleteStats stats=SDIRF::getCompleteStats(DD::GL_DBS.getStatTable());
      long nT=(stats.sortedAccordingToName).size();
      if((start<0)||(start>nT-1)){start=0;}
      std::pair<std::string,int> allD;
      std::string name;
      long vVL,vML;
      int res;
      long lsz=3;
      std::vector<std::string> topLine,mLine;
      topLine.resize(lsz);mLine.resize(lsz);
      std::stack<std::vector<std::string> > allLines;
      std::string searchBeginString=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"sBg","!!notFound");
      if(searchBeginString!="!!notFound"){
          SDIRF::TextScorePair sBeginStruct;
          sBeginStruct.text=searchBeginString;
          sBeginStruct.score=0;
          long indLow=(stats.sortedAccordingToName).upperBound(sBeginStruct);
          --indLow;
        if(indLow<0){indLow=0;}
        if(indLow<nT){
          start=indLow;sc="0";
        }
      }
      long end=start+numb;
      if((end<0)||(end>nT)){
        end=nT;
      }
      std::string scSw="0";
      if(sc=="0"){scSw="1";}
      topLine[0]=createLinkToSwitchSortCriterion(MWII::GL_WI.getDefaultWebText("Name"),"listStat","0",MWII::GL_WI.getStartOfList());
      topLine[1]=createLinkToSwitchSortCriterion(MWII::GL_WI.getDefaultWebText("Ranking"),"listStat","1",MWII::GL_WI.getStartOfList());
        topLine[2]="Visitors";
      for(long i=start;i<end;++i){
        if(sc=="0"){
            SDIRF::TextScorePair res=(stats.sortedAccordingToName)[i];
          mLine[0]=res.text;
          mLine[1]=std::to_string(res.score);
            mLine[2]=std::to_string(res.totalVisits);
        }
        else{
            SDIRF::ScoreTextPair res=(stats.sortedAccordingToScore)[nT-i-1];
          mLine[0]=res.text;
          mLine[1]=std::to_string(res.score);
            mLine[2]=std::to_string(res.totalVisits);
        }
        mLine[0]=createLinkToExpandStat(mLine[0]);
        allLines.push(mLine);
      }
      SF::flipTheStack(allLines);
      allLines.push(topLine);
      return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide,sc)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
    }
  std::string AbstractText::createListOfResponses(const PSDI::SessionData & _psd, const std::string & _numInList, const std::string & _numOnEachSide) const{
    std::string sc=MWII::GL_WI.getSortCriterion();
    long nT=DD::GL_MAIN_DB.numResponses();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    if((sc!="0")&&(sc!="1")){sc="0";}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    RMD::Response sf;
    std::pair<std::string,int> allD;
    std::string modifiedD,createdBy,lAnswered,othD;
    int res;
    long lsz=9;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    long pos;
    std::pair<std::vector<std::string>, std::string> sR;
    MTF::Table& textTable=DD::GL_MAIN_DB.dbsM["response"];
    TOSF::TSets indexRes=textTable.getIndTable("_i_1_ei_");
    long indSz=indexRes.size();
    std::string tempTName;
    std::string searchBeginString=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"sBg","!!notFound");
    if(searchBeginString!="!!notFound"){
      std::vector<std::string> sbstV;
      sbstV.resize(1); sbstV[0]=searchBeginString;
      long indLow=indexRes.lowerBoundRowIndex(sbstV);
      if(indLow<0){indLow=0;}
      if(indLow<indSz){
        start=indLow;sc="1";
      }
    }
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    if(sc=="1"){if(indSz<end){end=indSz;}}
    std::string scSw="0";
    if(sc=="0"){scSw="1";}
    topLine[0]=createLinkToSwitchSortCriterion(MWII::GL_WI.getDefaultWebText("Code / Label"),"listResponses",scSw,MWII::GL_WI.getStartOfList());
    topLine[1]=MWII::GL_WI.getDefaultWebText("DF");
    topLine[2]=MWII::GL_WI.getDefaultWebText("AW");
    topLine[3]=MWII::GL_WI.getDefaultWebText("SL");
    topLine[4]=MWII::GL_WI.getDefaultWebText("ST");
    topLine[5]=MWII::GL_WI.getDefaultWebText("GR");
    topLine[6]=MWII::GL_WI.getDefaultWebText("Modified");
    topLine[7]=MWII::GL_WI.getDefaultWebText("Name");topLine[8]=MWII::GL_WI.getDefaultWebText("ND");
    for(long i=start;i<end;++i){
      if(sc=="1"){
        sf.setFromTextName(((indexRes.getKeysFromRow(i))[0]));
      }
      else{
        sR=(DD::GL_MAIN_DB.dbsM["response"])[i];
        sf.setFromInternalId(sR.first[0]);
      }
      tempTName=sf.getTextName();
      mLine[0]=createLinkToResponseReceiver(tempTName,"");
      mLine[1]=createLinkToResponseReceiver(tempTName,"df");
      mLine[2]=createLinkToResponseReceiver(tempTName,"aw");
      mLine[3]=createLinkToResponseReceiver(tempTName,"sl");
      mLine[4]=createLinkToResponseReceiver(tempTName,"st");
      mLine[5]=createLinkToResponseReceiver(tempTName,"gr");
      pos=0;
      othD=sf.getTextData();
      allD=SF::extract(othD,pos,"_modified!!_","_/modified!!_");
      modifiedD="";
      if(allD.second==1){
        modifiedD=allD.first;
      }
      pos=0;
      allD=SF::extract(othD,pos,"_iI*|_","_/iI*|_");
      createdBy="";
      if(allD.second==1){
        createdBy=allD.first;
        mLine[5]="";
      }
      mLine[6]=modifiedD;
      mLine[7]=createdBy;
      pos=0;allD=SF::extract(othD,pos,"_alND*|_","_/alND*|_");
      mLine[8]="";
      if(allD.second==1){
        mLine[8]=allD.first;
      }
      allLines.push(mLine);
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    return BI::createScroller(start,end,nT,numb,"!*!",_numOnEachSide,sc)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createListFromDB(const PSDI::SessionData & _psd, const std::string & _db, const std::string & _numInList, const std::string & _numOnEachSide) const{
    // WARNING: Works only for root
    if(_psd.isRoot!="yes"){
      return "";
    }
    if(_db=="users"){
      return createListOfUsers(_psd,_numInList, _numOnEachSide);
    }
    if(_db=="texts"){
      return createListOfTexts(_psd,_numInList, _numOnEachSide);
    }
    if(_db=="files"){
      return createListOfFiles(_numInList, _numOnEachSide);
    }
    if(_db=="messages"){
      return createListOfMessages(_numInList, _numOnEachSide);
    }
    if(_db=="coursesAssignments"){
      return createListOfCoursesAssignments(_psd,_numInList, _numOnEachSide);
    }
    if(_db=="certificates"){
      return createListOfCertificates(_psd,_numInList, _numOnEachSide);
    }
    if(_db=="backups"){
      return createListOfBackups(_numInList, _numOnEachSide);
    }
    if(_db=="responses"){
      return createListOfResponses(_psd,_numInList, _numOnEachSide);
    }
    if(_db=="statDB"){
      return createListOfStatItems(_psd,_numInList, _numOnEachSide);
    } 
    return "";
  }
  std::string AbstractText::displayFileForm(const PSDI::SessionData & _psd, const std::string & requestType, const std::string & templateName, const std::string & parameter01, const std::string& parameter02){
    if(requestType.length()<1){return "";}
    if(requestType=="userList"){
      return FFI::listOfFormsDisplay(_psd,requestType,templateName,parameter01,parameter02);
    }
    MPTI::MainText templateText;
    int initSucc=templateText.initialize(templateName,"no",_psd.my_un);
    if(initSucc==0){return "";}
    int displAllow=1;
    if(_psd.isRoot!="yes"){
      if((requestType[0]=='a')||(requestType[0]=='A')){
        displAllow=0;
      }
      else{
        displAllow=templateText.allowedToDisplayText(_psd,_psd.my_un,"fileFormTemplate");
      }
    }
    if(displAllow==0){
      return "";
    }
    return treatInserts(_psd,FFI::formDisplay(_psd,templateText.getRawText(), requestType,templateName, parameter01, parameter02 ),GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::pair<std::string,std::string> AbstractText::createLinkPair(const std::vector<std::string> & parameters, const std::vector<std::string> &values,const std::string &label) const{
    std::pair<std::string,std::string> fR;
    fR.first=MWII::GL_WI.getWSURL()+"/index.cgi";
    long ms=parameters.size();
    long ms2=values.size();
    if(ms2<ms){ms=ms2;}
    std::string separator="?";
    for(long i=0;i<ms;++i){
      fR.first+=separator+parameters[i]+"="+values[i];
      separator="&";
    }
    fR.second=label;
    return fR;
  }
  std::pair<std::string,std::string> AbstractText::createLinkPair(const std::string & parameter, const std::string &value,const std::string &label) const{
    return createLinkPair(SF::oneElementVector(parameter),SF::oneElementVector(value),label);
  }
  std::pair<std::string,std::string> AbstractText::createLogInLinkPair(const std::string &label) const{
    std::pair<std::string,std::string> fR;
    fR.first=MWII::GL_WI.getWSURL()+"/index.cgi?page=login";
    if((MWII::GL_WI.getFailedLogIn()=="no")&&(MWII::GL_WI.getMainPageExternalCode()!="notFound")){
      std::string randCd=RNDF::genRandCode(7);
      std::string redirectCode=SF::combineTwoWords(MWII::GL_WI.getMainPageExternalCode(),randCd);
      fR.first+="&"+GL_syntax.e_redirectToForward+"="+redirectCode;
    }
    if((MWII::GL_WI.getFailedLogIn()=="yes")&&(MWII::GL_WI.getRedirectOverwrite()!="notFound")){
      fR.first+="&"+GL_syntax.e_redirectToForward+"="+MWII::GL_WI.getRedirectOverwrite();
    }
    fR.second=label;
    return fR;
  }
  std::string AbstractText::createLogInLink() const{
    return HSF::linkFromPair(createLogInLinkPair(MWII::GL_WI.getDefaultWebText("Sign in")));
  }
  std::string AbstractText::createStatAnalysisPage(const PSDI::SessionData & _psd, const std::string & _stA) const{
    if( _psd.isRoot!="yes"){
      return "";
    }
      SPREPF::StatData forSt;
      forSt.userName=_psd.my_un;
      forSt.att_page= _stA;
      TMD::MText deb_mt;
      deb_mt.setFromTextName(_stA);
      forSt.att_pageID=deb_mt.getInternalIdFromInternalNumber();
    return SDIRF::statAnalysis(forSt,DD::GL_DBS.getStatTable(),MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string AbstractText::createCloneInvitation(const PSDI::SessionData & _psd){
    if(DD::GL_IND_ALLOW_WEBSITE_CLONES==0){
      return "";
    }
    if((_psd.myWU).isAllowedToCloneWebsite()!="yes"){
      return "";
    }
    std::string fR="_insert__n*_text_/n*__n*_invitationToCreateClone_/n*__/insert_";
    return treatInserts(_psd,fR,GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::createRestoreCommand(const PSDI::SessionData & _psd, const std::string & _backupName){
    MTF::Table& backupT=DD::GL_MAIN_DB.dbsM["backupDatabase"];
    std::vector<std::string> name;
    name.resize(1);
    name[0]=_backupName;
    std::vector< std::pair<std::vector<std::string>, std::string> > sRes=backupT.search(name);
    if(sRes.size()<1){return "";}
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(sRes[0].second,pos,"_rec!!***!_","_/rec!!***!_");
    if(allD.second==0){return "";}
    std::string fR="_insert_ _n*_formInitialization_/n*_ _n*_reqCommand_/n*_ _n*_index.cgi?comEx=yes_/n*_ _/insert_ ";
    fR+="_insert_ _n*_textAreaField_/n*_ _n*_reqCommand_/n*_ _n*_commands_/n*_ ";
    fR+="_n*_ _/n*_\n";
    fR+="_n*_"+allD.first+"_/n*_ _/insert_ _insert_ _n*_formPlacement_/n*_ _n*_reqCommand_/n*_ _/insert_ ";
    return treatInserts(_psd,fR,GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::pair<std::string,std::string> AbstractText::createLogOutPair(const std::string &label) const{
    std::string fR=MWII::GL_WI.getWSURL()+"/index.cgi?userLogIn=no";
    if(MWII::GL_WI.getMainPageExternalCode()!="notFound"){
      std::string randCd=RNDF::genRandCode(7);
      std::string redirectCode=SF::combineTwoWords(MWII::GL_WI.getMainPageExternalCode(),randCd);
      fR+="&"+GL_syntax.e_redirectToForward+"="+redirectCode;
    }

    return std::pair<std::string,std::string>(fR,label);
  }
  std::string AbstractText::createSubText(const PSDI::SessionData & _psd, const std::string & textName) const{
    if(subTextRecursionDepth>maxRecursionDepth){
      return "";
    }
    MPTI::MainText subt;
    int initSucc=subt.initialize(textName,"no",myUserName);
    if(initSucc==0){
      return "";
    }
    subt.setSubTextRecursionDepth(subTextRecursionDepth+1);
    return subt.displayText(_psd,"subText");
  }
  std::string executeAGCodeTest(const PSDI::SessionData & _psd,
                                const std::string& formulation,
                                const std::string& officialSolution,
                                const std::string& agrParameters,
                                const std::string& userAnswer,
                                const std::string& displStr,
                                const std::string& solutionAllowedToDisplay,
                                const std::string& _messageInsteadOfGrading){
    std::map<std::string,std::string> replMap;
    if((solutionAllowedToDisplay=="yes")||(solutionAllowedToDisplay=="Yes")){
      replMap["_*officialSolution*_"]=officialSolution;
    }
    else{
      replMap["_*officialSolution*_"]="";
    }
    replMap["_*text*_"]=formulation;
    replMap["_*userAnswer*_"]=HSF::codeDisplayForNonAdvanceUsers(userAnswer);
    replMap["_*rawUserAnswer*_"]=userAnswer;
    std::string grResult;
    std::string messageInsteadOfGrading=_messageInsteadOfGrading;
    if(MFRF::find(displStr,"_*gradingResult*_").second>-1){
      if(messageInsteadOfGrading==""){
        if(BF::cleanSpaces(userAnswer)==""){
          messageInsteadOfGrading="No code submitted!";
        }
      }
      if(messageInsteadOfGrading==""){
        std::map<std::string,CAGI::GradingResult> autoGradingMap;
        std::map<std::string,RTI::CodeAutoGraderInfo> autoGradingCodes;
        RTI::CodeAutoGraderInfo agData=CAGI::getAutoGraderCodeData(agrParameters,officialSolution,userAnswer,"100");
        if(agData.publicTestCases.size()<1){return "***!ERROR!***";}
        autoGradingCodes["Q00"]=agData;
        CAGI::codeAutoGradeAndUpdateMap(_psd,autoGradingMap,autoGradingCodes,"publicTestCases");
        std::map<std::string,CAGI::GradingResult>::const_iterator itGR;
        itGR=autoGradingMap.begin();
        if(itGR!=autoGradingMap.end()){
          grResult=AGRDI::prepareGrResult(itGR->second);
        }
      }
      else{
        grResult="<div class=\"card bg-light text-danger\">\n";
        grResult+="<div class=\"card-body\">\n<h2>";
        grResult+=messageInsteadOfGrading;
        grResult+="</h2>\n</div>\n</div>\n";
      }
    }
    replMap["_*gradingResult*_"]=grResult;
    return MFRF::findAndReplace(displStr,replMap);
  }
  std::string AbstractText::createCodeTest(const PSDI::SessionData & _psd, const std::string & respRecCode, const std::string & qLabel, const std::string & baseText) const{
    if(GL_studentsAllowedToExecuteCodeOnPublicTestCases!="yes"){
      return "";
    }
    RTI::Response respT(respRecCode,"no",_psd.my_un);
    if(respT.isInitialized()==0){
      return "";
    }
    RTI::ResponderInfo rinf=respT.infoFromResponseText(_psd,"df",0);
    if(rinf.displayQs==0){
      return "";
    };
    long submissionInd=0;
    if((rinf.acceptResp==1)&&(rinf.documentType=="responseToTest")){
      submissionInd=1;
    }
    std::string displStr= baseText;
    std::set<std::string>::const_iterator it,itE=rinf.questionLabels.end();
    std::map<std::string,std::string>::const_iterator itM,itME;
    itM=rinf.completeProblemDocs.find(qLabel);
    if(itM==rinf.completeProblemDocs.end()){
      return "";
    }
    std::string probData=itM->second;
    std::string formulation;
    std::pair<std::string,int> allD;
    long pos;
    pos=0; allD=SF::extract(probData,pos,"_tx*|_","_/tx*|_");
    if(allD.second==1){
      formulation=allD.first;
    }
    pos=0; allD=SF::extract(probData,pos,"_agr*|_","_/agr*|_");
    std::string agrParameters="";
    if(allD.second==1){
      agrParameters=allD.first;
    }
    std::string officialSolution="";
    pos=0; allD=SF::extract(probData,pos,"_sl*|_","_/sl*|_");
    if(allD.second==1){
      officialSolution=allD.first;
    }
    std::string userAnswer="";
    itM=rinf.submittedAnswers.find(qLabel);
    if(itM!=rinf.submittedAnswers.end()){
      userAnswer=itM->second;
    }
    displStr=executeAGCodeTest(_psd,formulation,officialSolution,agrParameters,userAnswer,displStr,"no","");
    if(displStr=="***!ERROR!***"){
      return "<div>Error: No public test cases available</div>";
    }
    return displStr;
  }
  std::string AbstractText::createCodeTestInNotes(const PSDI::SessionData & _psd,
                                                  const std::string & problemName,
                                                  const std::string & problemVersion,
                                                  const std::string & baseText){
    if(GL_studentsAllowedToExecuteCodeOnPublicTestCases!="yes"){
      return "";
    }
    MPTI::MainText problemText;
    int initSucc=problemText.initialize(problemName,"no",_psd.my_un);
    if(initSucc==0){return "";}
    int allowedToDisplay=problemText.allowedToDisplayText(_psd,_psd.my_un,"codeTestInNotes");
    if(allowedToDisplay==0){
      return "";
    }
    std::string rawT=problemText.getRawText();
    std::pair<std::string,int> allD;long pos;
    pos=0;allD=SF::extract(rawT,pos,"_apd*|_","_/apd*|_");
    if((allD.second==0)||((allD.first!="yes")&&(allD.first!="Yes"))) {
      return "<div>The problem is not allowed to be displayed to users for testing.</div>";
    }
    std::string allowedSolution="no";
    pos=0;allD=SF::extract(rawT,pos,"_sall*|_","_/sall*|_");
    if(allD.second==1){
      allowedSolution=allD.first;
    }
    std::string correctVersion=TWDVF::singleVersion(rawT,BF::stringToInteger(problemVersion));
    std::string formulation;
    pos=0; allD=SF::extract(correctVersion,pos,"_tx*|_","_/tx*|_");
    if(allD.second==1){
      formulation=allD.first;
    }
    if(HTII::GL_title.nTGenerations==0){
      HTII::GL_title.tSuggestion=HTII::extractTextForTD(formulation,HTII::GL_title.maxTitleLength);
      HTII::GL_title.dSuggestion=HTII::extractTextForTD(formulation,HTII::GL_title.maxDescLength);
      HTII::GL_title.nTGenerations=1;
      HTII::GL_title.nDGenerations=1;
    }
    pos=0; allD=SF::extract(correctVersion,pos,"_agr*|_","_/agr*|_");
    std::string agrParameters="";
    if(allD.second==1){
      agrParameters=allD.first;
    }
    std::string officialSolution="";
    pos=0; allD=SF::extract(correctVersion,pos,"_sl*|_","_/sl*|_");
    if(allD.second==1){
      officialSolution=allD.first;
    }
    std::string userAnswer="";
    std::map<std::string,std::string>::const_iterator itM;
    itM=(_psd.respMap).find("codeTestUserAnswer");
    if(itM!=(_psd.respMap).end()){
      userAnswer=itM->second;
    }
    std::string displStr=baseText;
    std::string message;
    if((_psd.my_un=="visitor")&&(_psd.passedAntiSpam==0)){
      message="Wrong anti-spam code";
    }
    displStr=executeAGCodeTest(_psd,formulation,officialSolution,agrParameters,userAnswer,displStr,allowedSolution,message);
    displStr=treatInserts(_psd,displStr,GL_syntax.s_insertB,GL_syntax.s_insertE);
    if(displStr=="***!ERROR!***"){
      displStr="<div>Error: No public test cases available</div>";
    }
    return displStr;
  }
  std::string AbstractText::createUserPermitInfo(const PSDI::SessionData & _psd, const std::string & userName){
    if( (_psd.isRoot!="yes") && (_psd.myWU.existsPath(userName,"meHigh", "uName",1)==0) ){
      return "";
    }
    WUD::User userToAnalyse,u2;
    userToAnalyse.setFromUsername(userName);
    if(userToAnalyse.encryptedMasterKeyExists()==0){
      WUD::sendMessageUsingPublicKey(userName, _psd.myWU.getMasterKeyFromRAM("sigmaM"), "encMasterKey");
    }
    std::vector<std::string> dNeighbors=userToAnalyse.hNeighborsInternalIds();
    std::string res="<p>";
    for(long i=0;i<dNeighbors.size();++i){
      u2.setFromInternalId(dNeighbors[i]);
      res += createLinkForEditUser( u2.getUsername() )+" ";
    }
    res+="</p>";
    return res;
  }
  int correctType(const char& ch, const int& digitRemoval){
    int isDigit=0;
    if((ch>='0')&&(ch<='9')){
      isDigit=1;
    }
    return (digitRemoval+isDigit+1)%2;
  }
  std::string removeStartingDigitsOrStartingNonDigits(const std::string& in){
    long len=in.length();
    if(len<1){return "";}
    int digitRemoval=0;
    if((in[0]>='0')&&(in[0]<='9')){
      digitRemoval=1;
    }
    std::string out;
    long i=0;
    while((i<len)&&(correctType(in[i],digitRemoval))) {
      ++i;
    }
    while(i<len){
      out+=in[i];
      ++i;
    }
    return out;
  }
  std::string AbstractText::checkIfTheUserIsTA(const PSDI::SessionData & _psd, const std::string& rName) const{
    if(_psd.isGradingAdmin=="yes"){
      return "yes";
    }
    if(_psd.my_un=="visitor"){
      return "no";
    }
    std::string permitToCheck;
    std::string nameReversed=SF::reverseString(rName);
    std::string result="no";
    while((result=="no")&&(nameReversed!="")){
      permitToCheck=SF::reverseString(nameReversed);
      if((_psd.my_un!=permitToCheck)&&(_psd.myWU.existsPath(permitToCheck)) ){
        // IMPORTANT The check _psd.my_un!=permitToCheck is necessary to prvent
        // possible unauthorized access to grades. A mischevious user could create
        // an account with the name "f2023mth" and then be able to access the grades
        result="yes";
      }
      nameReversed=removeStartingDigitsOrStartingNonDigits(nameReversed);
    }
    return result;
  }
  std::string AbstractText::createSolvingInvitation(const PSDI::SessionData & _psd, const std::string & respRecN,const std::string & label,   const std::string &_inv) const{
    RTI::Response respT(respRecN,"no",_psd.my_un);
    if(respT.isInitialized()==0){return ""; }
    CEI::RawGrades rgrades;
    rgrades.certificateAddition=0;
    std::map<std::string,std::pair<std::vector<std::string>,std::string> >::const_iterator itRGM,itRGME;
    CEI::CouasAttributes cIrrelevant;
    std::string csvInd="no";
    std::string isTA=_psd.isGradingAdmin;
    std::string inv=_inv;
    if(inv=="summaryTA"){
      // The call to the method checkIfTheUserIsTA is somewhat expensive. In particular,
      // the calls that return "no" are the most expensive. Thus, we make the calls only
      // if the web document has an explicit request for "summaryTA"
      // If the request is for "summary", then we won't enter this block.
      // The check was already performed. We only verified whether the
      // user was a root. Both answers "yes" and "no" were equally cheap to derive.
      isTA=checkIfTheUserIsTA(_psd,respRecN);inv="summary";
    }
    if((isTA=="yes")&&(inv=="summary")) {
      rgrades=CEI::rawGradesAndStatusFromRespReceiver(cIrrelevant, _psd,respRecN, 0, 1);
      itRGME=rgrades.grStatusMap.end();
      std::map<std::string,std::string>::const_iterator itCSVRM,itCSVRME;
      itCSVRME=(_psd.respMap).end();
      itCSVRM=(_psd.respMap).find("csvD");
      if(itCSVRM!=itCSVRME){
        csvInd= itCSVRM->second;
      }
    }
    std::vector<RTI::LocationOfDocuments> locV=respT.getLocations(_psd);
    long i=0;
    long sz=locV.size();
    long stillWorking=1;
    std::string fR="", fRCSV="";
    std::string urlTest,urlGrader,linkDispTest,linkDispGr1,linkDispGr2;
    std::stack<std::vector<std::string> > allLines, allLinesCSV;
    std::vector<std::string> topL,midL;
    long lsz=6+rgrades.certificateAddition;
    topL.resize(lsz);midL.resize(lsz);long num_gr_jobs=0; long expansionPosition;
    long expansionStatus=0;// 0 - unverified yet;
    //1 - no expansion needed;
    //2 - expansion needed. Update complete for lsz, topL, and midL
    topL[0]="Name";topL[1]=MWII::GL_WI.getDefaultWebText("T Link");
    expansionPosition=2;
    if(rgrades.certificateAddition){
      topL[2]=MWII::GL_WI.getDefaultWebText("Certificate");
    }
    topL[expansionPosition+rgrades.certificateAddition]=MWII::GL_WI.getDefaultWebText("G Link");
    topL[expansionPosition+rgrades.certificateAddition+1]=MWII::GL_WI.getDefaultWebText("T S");
    long termsAfterExpansion=2;long expansionSize=-100;
    topL[lsz-termsAfterExpansion]=MWII::GL_WI.getDefaultWebText("G S");
    topL[lsz-termsAfterExpansion+1]=MWII::GL_WI.getDefaultWebText("Gr");
    std::string forwardingVV="";
    if(isTA=="yes"){
      forwardingVV+=SF::forwardValVarPairs(_psd.respMap,LI::GL_LN.st_envVName_Variable,LI::GL_LN.st_envVName_Value,10);
    }
    while ((i<sz)&&(stillWorking=1)){
      urlTest="index.cgi?r="+locV[i].testLocation;
      urlGrader="index.cgi?r="+locV[i].gradeLocation;
      linkDispTest="<a href=\""+urlTest+forwardingVV+"\">"+label+"</a>";
      linkDispGr1="<a href=\""+urlGrader+forwardingVV+"\">"+MWII::GL_WI.getDefaultWebText("grading")+"</a>";
      linkDispGr2="<a href=\""+urlGrader+forwardingVV+"\">"+label+"</a>";
      if((isTA=="yes")&&(inv=="summary")){
        midL[0]=locV[i].studentName;midL[1]=linkDispTest;
        midL[expansionPosition]=linkDispGr1;midL[expansionPosition+1]="";midL[lsz-termsAfterExpansion]="";
        midL[lsz-termsAfterExpansion+1]=locV[i].graderUName;
        itRGM=rgrades.grStatusMap.find(locV[i].userName);
        if(itRGM!=itRGME){
          midL[lsz-termsAfterExpansion]=(itRGM->second).second;
          std::vector<std::string> scores=((itRGM->second).first);
          long scores_sz=scores.size();
          if(scores_sz>3){
            if(expansionStatus==0){
              expansionStatus=2;
              expansionSize=scores_sz-3;
              std::vector<std::string> oldMidL= midL;
              std::vector<std::string> oldTopL= topL;
              long oldLSZ=lsz;
              lsz+=expansionSize;
              topL.resize(lsz);midL.resize(lsz);
              for(long jj=0;jj<lsz;++jj){topL[jj]="";midL[jj]="";}
              for(long jj=0;jj<expansionPosition;++jj){
                topL[jj]=oldTopL[jj];
                midL[jj]=oldMidL[jj];
              }
              for(long jj=0;jj<termsAfterExpansion;++jj){
                topL[lsz-jj-1]=oldTopL[oldLSZ-jj-1];
                midL[lsz-jj-1]=oldMidL[oldLSZ-jj-1];
              }
              if(rgrades.certificateAddition){
                topL[expansionPosition]=oldTopL[expansionPosition];
              }
              for(long ii=rgrades.certificateAddition;ii<expansionSize;++ii){
                topL[expansionPosition+ii]=std::to_string(1+ii-rgrades.certificateAddition);
              }
              topL[expansionPosition+expansionSize]="$\\Sigma$";
              topL[expansionPosition+expansionSize+1]="G Link";
            }
            for(long ii=0;ii<expansionSize;++ii){
              midL[expansionPosition+ii]=scores[2+ii];
            }
            midL[expansionPosition+expansionSize]=scores[scores_sz-1];
            midL[expansionPosition+expansionSize+1]="<a href=\""+urlGrader+forwardingVV+"\">"+MWII::GL_WI.getDefaultWebText("grading")+"</a>";
          }
          else{
            midL[3]=scores[scores_sz-1];
            if(expansionStatus==0){expansionStatus=1;}
          }
        }
        allLines.push(midL);
        if(csvInd=="yes"){allLinesCSV.push(HSF::trimVectorForAssignmentGradesSummaryCSV(midL));}
      }
      else{
        if((inv=="students")&&(locV[i].userName==_psd.my_un)){
          fR+=linkDispTest;
        }
        if((inv=="graders")&&(locV[i].graderUName==_psd.my_un)){
          ++num_gr_jobs;
          fR+="<div>"+linkDispGr2+"</div>";
        }
      }
      ++i;
    }
    if(isTA=="yes") {
      SF::flipTheStack(allLines);
      if(csvInd=="yes"){
        SF::flipTheStack(allLinesCSV);
        fRCSV=HSF::tableOrCSVFromStack(allLinesCSV,",",MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
      }
      allLines.push(topL);
      fR=HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
      if(csvInd=="yes"){
        fR+=fRCSV;
      }
      if(inv!="summary"){
        fR=MWII::GL_WI.getDefaultWebText("solveInvitation");
      }
    }
    else{
      if((inv!="students")&&(inv!="graders")){
        fR="";
      }
    }
    return fR;
  }
  std::string AbstractText::createInternalLink(const PSDI::SessionData & _psd, const std::string & lN, const std::string & pageName) const{
    std::string link="<A href=\"";
    std::string url=MWII::GL_WI.getWSURL()+"/index.cgi?page="+pageName;
    link+=url+"\">";
    return link+lN+"</A>";
  }
  std::string AbstractText::createExternalURL(const PSDI::SessionData & _psd, const std::string & url, const std::string & documentWithPassword) const{
      std::string expandedURL=url;
      if(documentWithPassword!=""){
          TMD::MText pDocument;
          int scc=pDocument.setFromTextName(documentWithPassword);
          if(scc!=0){
              std::vector<std::string> tV=SF::stringToVector(pDocument.getTextData(),"_urlAdd_","_/urlAdd_");
              if(tV.size()>0){
                  expandedURL+=tV[0];
              }
          }
      }
      std::string htmlText=WSCI::readExternalWebsite(expandedURL);
      return htmlText;
  }
  std::string AbstractText::createRespRecStatusDisplay(const PSDI::SessionData &_psd, const std::string & respRecN) const{
    RTI::Response respT(respRecN,"no",_psd.my_un);
    std::stack<std::vector<std::string> > stS;
    std::vector<std::string> temp,mainL;
    std::vector<std::string> statusExam,statusGrading;
    long examL=5;
    long gradingL=2;
    long totalL=examL+gradingL;
    statusExam.resize(examL);
    statusGrading.resize(gradingL);
    temp.resize(totalL);mainL.resize(totalL);
    mainL[0]=MWII::GL_WI.getDefaultWebText("E");
    mainL[1]=MWII::GL_WI.getDefaultWebText("Q");
    mainL[2]=MWII::GL_WI.getDefaultWebText("S");
    mainL[3]=MWII::GL_WI.getDefaultWebText("P");
    mainL[4]=MWII::GL_WI.getDefaultWebText("C");
    mainL[5]=MWII::GL_WI.getDefaultWebText("G");
    mainL[6]=MWII::GL_WI.getDefaultWebText("S");
    std::string pillOn,pillOff;
    pillOn="<span class=\"badge badge-pill bg-success\">+</span>";
    pillOff="<span class=\"badge badge-pill bg-danger\">-</span>";
    if(respT.isInitialized()==0){
      temp[0]="<span class=\"badge badge-pill bg-warning\">"+MWII::GL_WI.getDefaultWebText("Exam does not exist")+"</span>";
      stS.push(temp);
      stS.push(mainL);
      return HSF::tableFromStack(stS,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
    }
    std::pair<std::string,std::string> rst=respT.getRespRecStatus(_psd);
    for(long i=0;i<totalL;++i){
      temp[i]=pillOff;
    }
    if(rst.first=="a"){
      temp[0]=pillOn;
      temp[1]=pillOn;
    }
    if(rst.first=="q"){
      temp[1]=pillOn;
    }
    if(rst.second=="a"){
      temp[5]=pillOn;
      temp[6]=pillOn;
    }
    if(rst.first=="s"){
      temp[1]=pillOn;
      temp[2]=pillOn;
    }
    if(rst.first=="sp"){
      temp[1]=pillOn;temp[2]=pillOn;temp[3]=pillOn;
    }
    if(rst.first=="sc"){
      temp[1]=pillOn;temp[2]=pillOn;temp[3]=pillOn;temp[4]=pillOn;
    }
    if(rst.first=="c"){
      temp[1]=pillOn;temp[3]=pillOn;temp[4]=pillOn;
    }
    stS.push(temp);
    stS.push(mainL);
    return HSF::tableFromStack(stS,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
std::vector< std::vector<std::string> > extractVariables(const std::string& rawT,
                                                         long numberOfTermsInLine){
    std::vector< std::vector<std::string> > res;
    std::map<std::string,std::string> replMap;
    std::string randCd=RNDF::genRandCode(3);
    std::string nSepB="*ns"+randCd+"_";
    std::string nSepE="_/"+nSepB;
    nSepB="_"+nSepB;
    std::string sTogether=nSepE+nSepB;
    replMap["<h1>"]=sTogether;
    replMap["</h1>"]=sTogether;
    replMap["<h2>"]=sTogether;
    replMap["</h2>"]=sTogether;
    replMap["<h3>"]=sTogether;
    replMap["</h3>"]=sTogether;
    replMap["<br>"]=sTogether;
    replMap["\\n"]=sTogether;
    std::string rawText=MFRF::findAndReplace(rawT,replMap);
    rawText=nSepB+rawText+nSepE;
    std::vector<std::string> lines=SF::stringToVector(rawText,nSepB,nSepE);
    std::map<long, std::vector<std::string> > selectedLines;
    long j=0;
    std::vector<std::string> vLine;
    for(long i=0;i<lines.size();++i){
        vLine=SF::stringToVectorSimpleSeparator(lines[i],",");
        if(vLine.size()==numberOfTermsInLine){
            selectedLines[j]=vLine;
            ++j;
        }
    }
    res.resize(j);
    std::map<long, std::vector<std::string> >::const_iterator it;
    it=selectedLines.begin();
    for(long i=0;i<selectedLines.size();++i){
        res[i]=it->second;
        ++it;
    }
    return res;
}
std::string AbstractText::createRepetitiveText(const PSDI::SessionData & _psd,
                                               const std::string& valuesForVariables,
                                               long numberOfTermsInLine,
                                               const std::string& textTemplate){
    if((_psd.isRoot!="yes")&&(_psd.isGradingAdmin!="yes")){
      return "";
    }
    TMD::MText m;
    if(m.setFromTextName(valuesForVariables)!=1){
        return "";
    }
    std::string res;
    initText=m.getTextData();
    std::pair<std::string,int> allD; long pos=0;
    allD=SF::extract(initText,pos,GL_syntax.s_tDataB, GL_syntax.s_tDataE);
    if(allD.second==1){
        initText=allD.first;
    }
    std::vector<std::string> keys;
    keys.resize(numberOfTermsInLine);
    for(long i=0;i<numberOfTermsInLine;++i){
        keys[i]="[term]"+std::to_string(i)+"[/term]";
    }
    std::vector< std::vector<std::string> >
    variables=extractVariables(initText,numberOfTermsInLine);
    res+="<textarea name=\"probText\" rows=\"15\"";
    res+=" cols=\"100\">";
    for(long i=0;i<variables.size();++i){
        res+=MFRF::findAndReplace(textTemplate,keys,variables[i]);
    }
    res+="</textarea>\n";
    return res;
}
std::string AbstractText::createTextWithFunctions(const PSDI::SessionData & _psd,
                                                  const std::string& raw){
    return AF::evaluateSimpleFunctions(raw);
}

  std::string AbstractText::evaluateInsert(const PSDI::SessionData & _psd, const std::string & _insText, const std::string & _iB, const std::string & _iE){
    std::string fR="";
    if(_iB==GL_syntax.s_insertB){
      std::vector<std::string> allArgs=SF::stringToVector(_insText,GL_syntax.s_insSepInB,GL_syntax.s_insSepInE);
      long sz=allArgs.size();
      if(sz>0){
        if((allArgs[0]==GL_syntax.s_subText)&&(sz==2)){
          return createSubText(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_codeTest) && (sz==4)){
          return createCodeTest(_psd,allArgs[1],allArgs[2], allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_codeTestInNotes) && (sz==4)){
          return createCodeTestInNotes(_psd,allArgs[1],allArgs[2],allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_message)&&(sz==2)){
          return createMessageDisplay(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_invitationToSolve)&&(sz==4)){
          return createSolvingInvitation(_psd,allArgs[1],allArgs[2],allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_couas)&&(sz==2)){
          return createCouasDisplay(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_cert)&&(sz==2)){
          return createCertDisplay(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_respRecStatus)&&(sz==2)){
          return createRespRecStatusDisplay(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_internalLink)&&(sz==3)){
          return createInternalLink(_psd,allArgs[1],allArgs[2]);
        }
        if((allArgs[0]==GL_syntax.s_externalURL)&&(sz==3)){
          return createExternalURL(_psd,allArgs[1],allArgs[2]);
        }
        if((allArgs[0]==GL_syntax.s_buttonLink)&&(sz==3)){
          return HSF::createButtonLink(allArgs[2],allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_repetitiveText)&&(sz==4)){
            return createRepetitiveText(_psd,
                                        allArgs[1],
                                        BF::stringToInteger(allArgs[2]),
                                        allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_textWithFunctions)&&(sz==2)){
            return createTextWithFunctions(_psd,allArgs[1]);
          }
        if((allArgs[0]==GL_syntax.s_userMapEdit)&&(sz==2)){
            return UMCI::userMapEdit(_psd,userMapRaw,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_userMapDisplay)&&(sz==2)){
            return UMCI::userMapDisplay(_psd,userMapRaw,allArgs[1]);
        }

        if((allArgs[0]==GL_syntax.s_svgAdd)&&(sz==4)){
          return SVGF::addAllSVGs(allArgs[3],BF::stringToDouble(allArgs[1]),BF::stringToDouble(allArgs[2]));
        }
        if((allArgs[0]==GL_syntax.s_itemTable)&&(sz==2)){
          return createItemTable(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_textAreaReqField)&&(sz==5)){
          return createTextAreaField(_psd,allArgs[1],allArgs[2],allArgs[3],allArgs[4]);
        }
        if((allArgs[0]==GL_syntax.s_textAreaReqField)&&(sz==7)){
          return createTextAreaField(_psd,allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5],allArgs[6]);
        }
        if((allArgs[0]==GL_syntax.s_textInputReqField)&&(sz==6)){
          if( (allArgs[1]=="reqBC") && (allArgs[2]=="question") ){
            std::string possibleReplacement=SF::getElFromMapOrNotFoundMessage(_psd.respMap,"question","notFound");
            if(possibleReplacement!="notFound"){
              allArgs[4]=possibleReplacement;
            }
          }
          return createTextInputField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5]);
        }
        if((allArgs[0]==GL_syntax.s_radioButtonsField)&&(sz==7)){
          return createRadioButtonsField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],allArgs[5],allArgs[6]);
        }
        if((allArgs[0]==GL_syntax.s_antiSpamChallengeField)&&((sz==5)||(sz==6))){
          if(_psd.my_un!="visitor"){return "";}
          std::string _ln="8";
          if(sz==6){
            _ln=allArgs[5];
          }
          return createAntiSpamField(allArgs[1],allArgs[2],allArgs[3],allArgs[4],_ln);
        }
        if((allArgs[0]==GL_syntax.s_fileReqField)&&(sz==4)){
          return createFileRequestField(allArgs[1],allArgs[2],allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_formInitialization)&&(sz==3)){
          return initializeForm(allArgs[1],allArgs[2]);
        }
        if((allArgs[0]==GL_syntax.s_formPlacement)&&(sz==2)){
          return placeFormInText(allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_formPlacement)&&(sz==4)){
          return placeFormInText(allArgs[1],allArgs[2],allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_userPermits)&&(sz==2)){
          return createUserPermitInfo(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_formInsertRedirect)&&(sz==2)){
          return setRedirectInfo(allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_statAnalysis)&&(sz==2)){
          return createStatAnalysisPage(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_logInLink)&&(sz==1)){
          return createLogInLink();
        }
        if((allArgs[0]==GL_syntax.s_cloneInvitation)&&(sz==1)){
          return createCloneInvitation(_psd);
        }
        if((allArgs[0]==GL_syntax.s_cRestore) && (sz==2)){
          return createRestoreCommand(_psd,allArgs[1]);
        }
        if((allArgs[0]==GL_syntax.s_listFromDB)&&(sz==4)){
          return createListFromDB(_psd,allArgs[1],allArgs[2],allArgs[3]);
        }
        if((allArgs[0]==GL_syntax.s_answerToQuery)&&(sz==1)){
          SF::assignToConst(_psd.queryAnswRequired,1);
          return _psd.queryAnswPlaceHolder;
        }
        if(allArgs[0]==GL_syntax.s_fileForm){
          if(sz==3){
            return displayFileForm(_psd,allArgs[1],allArgs[2]);
          }
          if(sz==4){
            return displayFileForm(_psd,allArgs[1],allArgs[2],allArgs[3]);
          }
          if(sz==5){
            return displayFileForm(_psd,allArgs[1],allArgs[2],allArgs[3],allArgs[4]);
          }
        }
      }
    }
    return fR;
  }
  std::string AbstractText::treatInserts(const PSDI::SessionData &_psd, const std::string & _raw,const std::string & _iB, const std::string & _iE){
    return AICD::treatGeneralInsert(*this,_psd,_raw,_iB,_iE);
  }
  std::string AbstractText::prepareProblem(const PSDI::SessionData & _psd, const std::string & _raw) const{
    std::string fR="";
    std::vector<long>aV=TWDVF::identifyVersions(_raw);
    long sz=aV.size();
    std::string lAdd="class=\"page-link\" ";
    std::string liB="<li class=\"page-item\">";
    std::string liBActive="<li class=\"page-item active\">";
    std::string liE="</li>";
    std::string par=MWII::GL_WI.get_e_parPage()+"="+tName+"&ver";
    long maxInLine=20;
    fR+="<div><ul class=\"pagination\">\n";
    std::string sti, stiPlane;
    for(long i=0;i<sz;++i){
      sti=std::to_string(i);
      stiPlane=sti;
      if(_psd.versionToLatex==sti){
        fR+=liBActive;
      }
      else{
        fR+=liB;
      }
      if((sz>9)&&(i<10)){
        sti="0"+sti;
      }
      if((sz>99)&&(i<100)){
        sti="0"+sti;
      }
      fR+=createLinkToText(stiPlane,par,"<code>"+sti+"</code>",lAdd)+liE;
      if( ( (i%maxInLine) == maxInLine-1) && (i<sz-1) ){
        fR+="</ul></div>\n<div><ul class=\"pagination\">\n";
      }
    }
    if(sz<maxInLine){
      if(_psd.versionToLatex=="all"){
        fR+=liBActive;
      }
      else{
        fR+=liB;
      }
      fR+=createLinkToText("all",par, MWII::GL_WI.getDefaultWebText("all versions"),lAdd)+liE;
    }
    fR+="</ul></div>\n";
    std::string probData;long pos;std::pair<std::string,int> allD;
    for(long i=0;i<sz;++i){
      if((_psd.versionToLatex=="all")||(BF::stringToInteger(_psd.versionToLatex)==i)) {
        probData=TWDVF::prepareProblemForTest(_raw,i);
        pos=0;
        allD=SF::extract(probData,pos,"_tx*|_","_/tx*|_");
        if(allD.second==1){
          fR+="\\begin{box}\n";
          fR+="<H2>"+MWII::GL_WI.getDefaultWebText("Formulation")+"</H2> <P></P>"+allD.first;
          fR+="\\end{box}\n";
        }
        pos=0;
        allD=SF::extract(probData,pos,"_rbAll*|_","_/rbAll*|_");
        if(allD.second==1){
          fR+="\\begin{box}\n<H2>"+MWII::GL_WI.getDefaultWebText("Choices")+"</H2><P></P>";
          std::vector<std::string> chSt=SF::stringToVector(allD.first,"_rb*_","_/rb*_"),chStSm;
          long sz1=chSt.size();
          std::string ul="<ul>";
          for(long j=0;j<sz1;++j){
            chStSm=SF::stringToVector(chSt[j],"_vl*_","_/vl*_");
            if(chStSm.size()>1){
              fR+=ul+"<li> "+chStSm[1]+"</li>";
              ul="";
            }
          }
          if(ul==""){
            fR+="</ul>";
          }
          fR+="\\end{box}\n";
        }
        std::string officialSolutionText,officialAnswText; int officialAnswExists;
        pos=0; allD=SF::extract(probData,pos,"_sl*|_","_/sl*|_");
        if(allD.second==1){
          officialSolutionText=allD.first;
        }
        pos=0;allD=SF::extract(probData,pos,"_aw*|_","_/aw*|_");
        officialAnswExists=allD.second;
        if(officialAnswExists){
          officialAnswText=SF::answerToStandardForm(allD.first);
        }
        if( (officialSolutionText!="")|| (officialAnswText!=GF::GL_officialNA)){
          fR+="\\begin{box}<H2>";
          if((officialAnswText!="")&&(officialAnswText!=GF::GL_officialNA)){
            fR+=MWII::GL_WI.getDefaultWebText("Answer")+": "+officialAnswText;
          }
          else{
            fR+=MWII::GL_WI.getDefaultWebText("Solution");
          }
          fR+="</H2>";
          fR+=officialSolutionText;
          fR+="\\end{box}\n";
        }
        if(officialAnswExists){
          fR+="\\begin{box}\n<H2>"+MWII::GL_WI.getDefaultWebText("Latex source")+"</H2><P></P> <textarea name=\"probText\" rows=\"15\"";
          fR+=" cols=\"100\">";
          fR+=PTKF::GL_PLAINTEXT_KEEPER.depositTxt(probData)+"</textarea>\n";
          fR+="\\end{box}\n";
        }
        pos=0;allD=SF::extract(probData,pos,"_agr*|_","_/agr*|_");
        if(allD.second==1){
          std::string rawCodeAutoGraderData=allD.first;
          std::map<std::string,std::string>::const_iterator it;
          it=_psd.respMap.find("agr");
          int executed=0;
          if(it!=_psd.respMap.end()){
            if(((it->second=="yes")||(it->second=="y"))&&(_psd.versionToLatex!="all")){
              executed=1;
              fR+=CAGI::executionResult(_psd,rawCodeAutoGraderData,officialSolutionText);
            }
          }
          if(executed==0){
            std::string linkURL="index.cgi?" +MWII::GL_WI.get_e_parPage()+"="+tName+"&ver"+"=";
            linkURL+=_psd.versionToLatex+"&agr=y";
            fR+="<p></p><p>"+HSF::createButtonLink(linkURL,"Execute code")+"</p>";
          }
        }
      }
    }
    return fR;
  }
  std::map<std::string,std::string> AbstractText::getTranslationMap(const std::string & _input) const{
      std::map<std::string,std::string> fR;
      std::string input=_input;
      long pos;std::pair<std::string,int> allD;
      pos=0;allD=SF::extract(input,pos,"_variables_","_/variables_");
      if(allD.second==0){
        return fR;
      }
      std::string nameOfTextWithTranslation=allD.first;
      pos=0;allD=SF::eraseStuffBetween(input,"_variables_","_/variables_",pos);
      if(allD.second==0){
        return fR;
      }
      TMD::MText mtm;
      mtm.setFromTextName(nameOfTextWithTranslation);
      std::string rawTextDataTransl=mtm.getTextData();
      SF::varValPairs(rawTextDataTransl,"_vVPair_","_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",fR);
      return fR;
  }
  std::string AbstractText::convertVariablesToValues(const PSDI::SessionData  & _psd, const std::string & _input) const{
    long maxNumOfVarValTranslationsThatCanBeAddedThroughTheURLString=10;
    std::string input=_input;
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::eraseStuffBetween(input,"_variables_","_/variables_",pos);
    std::string fR=input;
    if(allD.second!=0){
      fR=allD.first;
    }
    std::map<std::string,std::string> extendedTranslationMap=translationVarToVal;
    SF::extendMapWithSubMapOfAnotherMap(extendedTranslationMap,_psd.respMap,LI::GL_LN.st_envVName_Variable,LI::GL_LN.st_envVName_Value,maxNumOfVarValTranslationsThatCanBeAddedThroughTheURLString);
    fR= SF::replaceVariablesWithValues(fR,"_vr_","_/vr_",extendedTranslationMap);
    return fR;
  }
  std::string AbstractText::prepare(const PSDI::SessionData  & _psd, const std::string & _raw,const std::string & _position){
    if(allowedToDisplayText(_psd,myUserName,_position)!=1){
      return "";
    }
    if((_psd.pEditReq=="y")||(_psd.pEditReq=="r")||(_psd.pEditReq=="w")){
      if((_psd.pEditReq=="y")||(_psd.pEditReq=="r")){
        if(_psd.isRoot=="no"){
          return "";
        }
      }
      if(allowedToModifyText(_psd,"","",_psd.my_un)==0){
        return "";
      }
      std::string argument=_raw;
      if(_psd.pEditReq=="r"){
        argument=initText;
      }
      std::string forKeeper="";
      CCFI::ArgsPowerModifyForm apmf;
      apmf.textToModify=argument;
      apmf.modifyMe=modifyMe();
      apmf.dbName=tName;
      apmf.linkForm="index.cgi?comEx=yes&";
      apmf.linkForm+=MWII::GL_WI.get_e_parPage();
      apmf.linkForm+="=";
      apmf.linkForm+=tName;
      if(_psd.pEditReq=="w"){
        return treatInserts(_psd,CCFI::createSimpleFormForTextEdit(apmf),GL_syntax.s_insertB,GL_syntax.s_insertE);
      }
      return treatInserts(_psd,CCFI::createCustomCommandForm(apmf),GL_syntax.s_insertB,GL_syntax.s_insertE);
    }
    std::string raw=convertVariablesToValues(_psd,_raw);
    if(documentType=="problem"){
      return treatInserts(_psd, prepareProblem(_psd,raw),GL_syntax.s_insertB,GL_syntax.s_insertE);
    }
    return treatInserts(_psd, raw,GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::getRawText() const{ return rawText; }
  std::string AbstractText::displayText(const PSDI::SessionData  & _psd, const std::string & _position ){
    return prepare(_psd,rawText,_position)+ sysDataIfNeededAndAllowed(_psd);
  }
  std::string AbstractText::getTextName() const{return tName;}
  std::string AbstractText::getTextExternalID() const{return tExternalId;}
  std::set<std::string> AbstractText::readPermissions() const{return permitRead;}
  std::set<std::string> AbstractText::writePermissions() const{return permitWrite;}
  long AbstractText::getSubTextRecursionDepth() const{return subTextRecursionDepth;}
  void AbstractText::setSubTextRecursionDepth(const long & _d){subTextRecursionDepth=_d;}
  std::string AbstractText::pageAttributesForForumScroller(const PSDI::SessionData & _psd) const{
    std::string fR="page="+_psd.pageRequested+"&";
    fR+="s1=u11&r1="+_psd.inFormReplace;
    return fR;
  }
  std::string AbstractText::forumScroller(const PSDI::SessionData & _psd,
                                              long & start, long & end, const long & numM,
                                              const long & _numDispl,
                                              const std::string & _numOnEachSide) const{
    std::string pa=pageAttributesForForumScroller(_psd);
    std::string stL=MWII::GL_WI.getStartOfList();
    long numDispl=_numDispl;
    if(numDispl<1){
      numDispl=3;
    }
    if(stL==""){
      end=numM;
      start=end-numDispl;
      if(start<0){start=0;}
    }
    else{
      start=BF::stringToInteger(stL);
      if(start<0){
        end=numM;start=end-numDispl;if(start<0){start=0;}
      }
      else{
        end=start+numDispl;
        if(end>numM){
          end=numM;
        }
      }
    }
    return BI::createScroller(start,end,numM, numDispl,pa,_numOnEachSide);
  }
  std::string AbstractText::createPowerMessageEdit(const PSDI::SessionData & _psd, const std::string & _mC){
    MEI::MElement mEl;
    if(MEI::initMElementFromCode(mEl,_mC)=="failed"){
      return "";
    }
    CCFI::ArgsPowerModifyForm apmf;
    apmf.textToModify=mEl.getRawText();
    apmf.linkForm="index.cgi?comEx=yes&";
    apmf.linkForm+=MWII::GL_WI.get_e_parPage();
    apmf.linkForm+="="+tName+"&s1=u11&r1=";
    apmf.linkForm+=_mC;
    apmf.modifyMe="modifyMessage";
    apmf.dbName=_mC;
    apmf.textNameLabel=MWII::GL_WI.getDefaultWebText("Message code:")+" ";
    return treatInserts(_psd,CCFI::createCustomCommandForm(apmf),GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::createPowerCouasEdit(const PSDI::SessionData & _psd, const std::string & _cC){
    CEI::CouasElement cEl(_psd);
    if(ICEI::initFromCode(_psd,cEl,_cC)=="failed"){
      return "";
    }
    CCFI::ArgsPowerModifyForm apmf;
    apmf.textToModify=CCFI::hideEncryptedTextInCouas(cEl.getRawText());
    apmf.linkForm="index.cgi?comEx=yes&";
    apmf.linkForm+=MWII::GL_WI.get_e_parPage();
    apmf.linkForm+="="+tName+"&s1=u11&r1=";
    apmf.linkForm+=_cC;
    apmf.modifyMe="modifyCourseAssignment";
    apmf.dbName=_cC;
    apmf.textNameLabel=MWII::GL_WI.getDefaultWebText("Course/assignment code:");
    return treatInserts(_psd,CCFI::createCustomCommandForm(apmf),GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::createPowerCertEdit(const PSDI::SessionData & _psd, const std::string & _cC){
    CERD::Certificate myCert;
    int succ=myCert.setFromTextName(_cC);
    if(succ==0){return "";}
    CCFI::ArgsPowerModifyForm apmf;
    apmf.textToModify=myCert.getTextData();
    apmf.linkForm="index.cgi?comEx=yes&";
    apmf.linkForm+=MWII::GL_WI.get_e_parPage();
    apmf.linkForm+="="+tName+"&s1=u11&r1=";
    apmf.linkForm+=_cC;
    apmf.modifyMe="modifyCertificate";
    apmf.dbName=_cC;
    apmf.textNameLabel=MWII::GL_WI.getDefaultWebText("Certificate code/name:");
    return treatInserts(_psd,CCFI::createCustomCommandForm(apmf),GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::createMessageDisplay(const PSDI::SessionData & _psd, const std::string & _mC){
    long numMessScroller=10;
    std::string numMessEachSideScroller="3";
    std::string startMessage=MWII::GL_WI.getStartOfList();
    std::string messCode=_mC;
    if(_psd.inFormReplace!=""){messCode=_psd.inFormReplace;}
    if(_psd.messEditReq=="r"){
      if(_psd.isRoot=="yes"){
        return createPowerMessageEdit( _psd,  messCode);
      }
      return "";
    }
    std::pair<std::string,int> allD;
    long pos;
    MEI::MElement mEl;
    if(MEI::initMElementFromCode(mEl,messCode)=="failed"){
      return "";
    }
    if((_psd.isRoot=="no")&&(mEl.allowedToRead(myUserName)==0)){
      return "";
    }
    long start=0,end=-1;
    std::string displElements="";
    std::string scroller="";
    if((mEl.type()=="tColl")||(mEl.type()=="mColl")){
      long sz=mEl.numCodes();
      scroller=forumScroller(_psd,start,end,sz,numMessScroller, numMessEachSideScroller);
      if((end<start)||(end>sz)){
        end=sz;
      }
      for(long i=start;i<end;++i){
        std::string mTextI="";
        MEI::MElement mEI;
        if(MEI::initMElementFromCode(mEI,mEl.code(i))=="success"){
          if((_psd.isRoot=="yes")||(mEI.allowedToRead(myUserName)==1)){
            int allowedToEdit=mEI.allowedToModifyMessage(myUserName);
            if(_psd.isRoot=="yes"){
              allowedToEdit=1;
            }
            if(mEl.type()=="tColl"){
              mTextI=mEI.prepareIndividualTopic(_psd,messCode,_psd.messEditCode,startMessage, allowedToEdit,_psd.pageRequested);
            }
            else{
              mTextI=mEI.prepareIndividualMessage(_psd,messCode,_psd.messEditCode,startMessage, allowedToEdit,_psd.pageRequested);
            }
          }
        }
        displElements+=mTextI;
      }
    }
    std::string fR="";
    fR+=mEl.navigationBar(_psd.pageRequested);
    fR+="<h2>"+mEl.title()+"</h2>";
    fR+=scroller;
    if((mEl.type()!="mColl")&&(mEl.type()!="tColl")){fR+=mEl.text();}
    fR+=displElements;
    if((_psd.isRoot=="yes")||(mEl.allowedToCreateMessage(myUserName))){
      fR+="<div><p>$\\;$</p></div>";
      fR+="<div class=\"card\"><div class=\"card-body\">";
      if(_psd.messEditCode=="nm"){
        fR+="<h3>"+MWII::GL_WI.getDefaultWebText("New Message")+"</h3>";
        fR+=mEl.newMessageBoxOrLink(messCode,1,_psd.pageRequested);
      }
      if(_psd.messEditCode==""){
        fR+=mEl.newMessageBoxOrLink(messCode,0,_psd.pageRequested);
      }
      fR+="</div></div>\n";
    }
    return treatInserts(_psd,fR,GL_syntax.s_insertB,GL_syntax.s_insertE);
  }
  std::string AbstractText::createCouasDisplay(const PSDI::SessionData & _psd, const std::string & _cC){
    std::string numMessScroller="10";
    std::string numMessEachSideScroller="3";
    std::string startMessage=MWII::GL_WI.getStartOfList();
    std::string couasCode=_cC;
    int allowedToGrade=0;
    if(_psd.inFormReplace!=""){couasCode=_psd.inFormReplace;}
    if(_psd.messEditReq=="r"){
      if(_psd.isRoot=="yes"){
        return createPowerCouasEdit(_psd,couasCode);
      }
      return "";
    }
    CEI::CouasElement cEl(_psd);
    if(ICEI::initFromCode(_psd,cEl,couasCode)=="failed"){
      std::string cma=CMTI::analysisAndMigrationWhenCouasDoesntExist(_psd,cEl);
      if(cma!="continue"){
        return cma;
      }
    }
    cEl.sortGradeData(_psd.sortCriterion);
    allowedToGrade=cEl.allowedToExecuteAutomaticGrading(_psd);
    std::string actionLink="",actLinkHelp="";
    std::vector<std::string> navLinks,navLabels;
    if((allowedToGrade==1)&&(_psd.couasEditReq!="g")&&(cEl.gradeFromFormula()==0)){
      CCLI::ConveniencePackageForCouas cpfc;
      if(translationVarToVal.size()>0){
        std::string mainDocNameForCourse="";
        std::map<std::string,std::string>::const_iterator itVV,itVVE,itRM,itRME;
        itVVE=translationVarToVal.end();
        itVV=translationVarToVal.find("docName");
        if(itVV!=itVVE){
          mainDocNameForCourse=itVV->second;
        }
        std::string mainRespRecName="";
        itRME=_psd.respMap.end();
        itRM=_psd.respMap.find("r1");
        if(itRM!=itRME){
          mainRespRecName=itRM->second;
        }
        cpfc=CCLI::convenience_get(mainDocNameForCourse,mainRespRecName,cEl.title());
      }
      FHI::FormField fld;fld.convertToTextInput("rrg",MWII::GL_WI.getDefaultWebText("Response receiver code:")+" ",cpfc.preFill,50);
      actionLink=BI::updatedLinkNewEnvVars(_psd.respMap,"ecr","r0");
      FHI::InputForm iF(actionLink,"runH","runL",MWII::GL_WI.getDefaultWebText("Grade"));
      iF.addChangeField(fld);
      actionLink="";
      actionLink+=iF.displayForm("0",MWII::GL_WI.getDefaultWebText("Upload grades"),"inline");
      if(cpfc.linkForSummary!="notFound"){
        actionLink+=HSF::createButtonLink(cpfc.linkForSummary,MWII::GL_WI.getDefaultWebText("Grading Room"));
      }
    }
    int submRespRecGr=0;
    if(_psd.couasEditReq=="r0"){
      submRespRecGr=1;
      navLinks.resize(2);navLabels.resize(2);
      navLinks[0]=BI::updatedLinkNewEnvVars(_psd.respMap,"ecr","r1");
      navLabels[0]=MWII::GL_WI.getDefaultWebText("Submit grade(s)");
      navLinks[1]=BI::updatedLinkNewEnvVars(_psd.respMap,"ecr","!remove!");
      navLabels[1]=MWII::GL_WI.getDefaultWebText("Cancel submission");
      actionLink=BI::createNavigationBar(navLinks,navLabels);
    }
    if(_psd.couasEditReq=="r1"){
      navLinks.resize(1);navLabels.resize(1);
      navLinks[0]=BI::updatedLinkNewEnvVars(_psd.respMap,"ecr","r8");
      navLabels[0]=MWII::GL_WI.getDefaultWebText("Back to grades");
      actionLink=BI::createNavigationBar(navLinks,navLabels);
      submRespRecGr=2;
    }
    if(submRespRecGr>0){
      if(allowedToGrade==1){
        return actionLink+cEl.gradeFromRespReceiver(_psd,submRespRecGr-1)+actionLink;
      }
    }
    return actionLink+cEl.displayCouas(_psd,numMessScroller,numMessEachSideScroller);
  }
  AICD::LatexReplacements createLatexReplacementStrings(){
    AICD::LatexReplacements res;
    res.websiteURL=MWII::GL_WI.getWSURL();
    long sz=8;
    res.htmlTs.resize(sz);
    res.latexTs.resize(sz);
    res.htmlTs[0]="\\begin{problem}";
    res.latexTs[0]=MWII::GL_WI.getDefaultWebText("\\noindent{\\bf Problem} ");
    res.htmlTs[1]="\\end{problem}";
    res.latexTs[1]=MWII::GL_WI.getDefaultWebText("%%endProblem");
    res.htmlTs[2]="\\begin{solution}";
    res.latexTs[2]=MWII::GL_WI.getDefaultWebText("\\noindent{\\bf Solution.} ");
    res.htmlTs[3]="\\end{solution}";
    res.latexTs[3]=MWII::GL_WI.getDefaultWebText("%%endSolution");
    res.htmlTs[4]="\\begin{box}";
    res.latexTs[4]=MWII::GL_WI.getDefaultWebText("\\begin{box}");
    res.htmlTs[5]="\\end{box}";
    res.latexTs[5]=MWII::GL_WI.getDefaultWebText("\\end{box}");
    res.htmlTs[6]="\\begin{proof}";
    res.latexTs[6]=MWII::GL_WI.getDefaultWebText("\\noindent{\\bf Proof.} ");
    res.htmlTs[7]="\\end{proof}";
    res.latexTs[7]=MWII::GL_WI.getDefaultWebText("%%endProof");
    return res;
  }
  int AbstractText::allowedToMakePDFForCertificate(const PSDI::SessionData & _psd, const std::string& usernameWhoIsAllowedToMakeCertificate) const{
    if(_psd.isRoot=="yes"){return 1;}
    if(_psd.my_un=="visitor"){return 0;}
    if(CERD::GL_CertificatesOptions.pdfsAllowed.length()>0){
      char firstCh=CERD::GL_CertificatesOptions.pdfsAllowed[0];
      if((firstCh=='y')||(firstCh=='Y')||(firstCh=='1')){
        if((usernameWhoIsAllowedToMakeCertificate=="all")||(usernameWhoIsAllowedToMakeCertificate==_psd.my_un)){
          return 1;
        }
      }
    }
    return 0;
  }
  std::string AbstractText::pdfForCertificate(const PSDI::SessionData & _psd, const std::string & _code, const std::string & _txts, const std::string& usernameWhoIsAllowedToMakeCertificate){
    if(allowedToMakePDFForCertificate(_psd,usernameWhoIsAllowedToMakeCertificate)==0){
      return "";
    }
    if(AICD::GL_ReplStrings.htmlTs.size()<1){
      AICD::GL_ReplStrings = createLatexReplacementStrings();
    }
    std::string lStart;
    lStart=MWII::GL_WI.getDefaultWebText("certPreambleLatexTemplate");
    if(lStart=="certPreambleLatexTemplate"){
      lStart=MWII::GL_WI.getDefaultWebText("examPreambleLatexTemplate");
      if(lStart=="examPreambleLatexTemplate"){
        lStart=MWII::GL_WI.getDefaultWebText("notesPreambleLatexTemplate");
      }
    }
    long pos; std::pair<std::string,int> allD;
    pos=0; allD=SF::extract(_txts,pos,"_lStart*!_","_/lStart*!_");
    std::string txts=_txts;
    if(allD.second==1){
      lStart=allD.first;
      pos=0;allD=SF::extractAndReplace(_txts,pos,"_lStart*!_","_/lStart*!_",0,"");
      if(allD.second==1){
        txts=allD.first;
      }
    }
    std::string latexSource;
    latexSource+=lStart+"\n\n \\begin{document}\n";
    latexSource+=txts;
    latexSource+="\n\n\\end{document} \n";
    std::string pdfName=MWII::GL_WI.getPublicPDFCertLoc()+"/cIC"+_code;
    IOF::deleteOldFiles(MWII::GL_WI.getPublicPDFCertLoc(),"pdf",CERD::GL_CertificatesOptions.maxTimeToKeepPDF);
    LMF::createPdfFromTex(latexSource,pdfName,MWII::GL_WI.getPublicPDFCertLoc());
    pdfName+=".pdf";
    std::string fR="<p></p><p>\n";
    fR+=HSF::createButtonLink(pdfName,"Download PDF");
    fR+="</p>\n";
    return fR;
  }
  std::string AbstractText::createCertDisplay(const PSDI::SessionData & _psd, const std::string & _cC){
    std::string certCode=_cC;
    if(_psd.inFormReplace!=""){certCode=_psd.inFormReplace;}
     if(_psd.messEditReq=="r"){
      if(_psd.isRoot=="yes"){
        return createPowerCertEdit(_psd,certCode);
      }
      return "";
    }
    CERD::Certificate cCert;
    int certExists=cCert.setFromExternalCode(certCode);
    if(certExists==0){return "";}
    std::vector<std::string> certComponents=SF::stringToVector(cCert.getTextData(),"_n*_","_/n*_");
    std::string latexTemplate, htmlTemplate, parametersRaw;
    if(certComponents.size()>2){
      htmlTemplate=certComponents[0];
      latexTemplate=certComponents[1];
      parametersRaw=certComponents[2];
    }
    else{
      if(certComponents.size()>1){
        latexTemplate=certComponents[0];
        htmlTemplate=latexTemplate;
        parametersRaw=certComponents[1];
      }
    }
    std::map<std::string,std::string> parameters=SF::stringToMap(parametersRaw,LI::GL_LN.st_sep_vrB,LI::GL_LN.st_sep_vrE,LI::GL_LN.st_sep_vlB,LI::GL_LN.st_sep_vlE);
    parameters["certId"]=certCode;
    std::string usernameWhoIsAllowedToMakeCertificate="all";
    std::map<std::string,std::string>::const_iterator it;
    it=parameters.find("whoIsAllowedToMakePDF");
    if(it!=parameters.end()){
      usernameWhoIsAllowedToMakeCertificate=it->second;
    }
    if(allowedToMakePDFForCertificate(_psd,usernameWhoIsAllowedToMakeCertificate)==1){
      it=parameters.find("pdfLinkIndicator");
      if(it!=parameters.end()){
        std::string res=it->second;
        if(res.length()>0){
          if((res[0]=='y')||(res[0]=='Y')||(res[0]=='1')){
            std::string linkURL="index.cgi?" +MWII::GL_WI.get_e_parPage()+"=certificates&s1=u11&r1="+certCode+"&certpdf=yes";
            htmlTemplate+="<p></p><p>"+HSF::createButtonLink(linkURL,"Generate PDF")+"</p>";
          }
        }
      }
    }
    it=(_psd).respMap.find("certpdf");
    if(it!=(_psd).respMap.end()){
      if((it->second=="y")||(it->second=="yes")||(it->second=="Y")||(it->second=="YES")){
        return pdfForCertificate(_psd,certCode,CERTCLI::prepareCertificate(latexTemplate,parameters),usernameWhoIsAllowedToMakeCertificate);
      }
    }
    return CERTCLI::prepareCertificate(htmlTemplate,parameters);
  }
  std::string lastTwoDigitsOfTheYear(const std::string & year){
    std::string res;
    long yyyy=BF::stringToInteger(year);
    long yy=yyyy%100;
    std::string prefix="";
    if(yy<10){
      prefix= "0";
    }
    return prefix+ std::to_string(yy);
  }
  std::string extractURLFromLink(const std::string & x){
    std::string res="!*!notLink";
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(x,pos,"_insert_","_/insert_");
    if(allD.second==0){
      return res;
    }
    std::vector<std::string> linkComponents=SF::stringToVector(allD.first,"_n*_","_/n*_");
    if(linkComponents.size()>2){
      return linkComponents[2];
    }
    return res;
  }
  std::vector<std::string> prepareSingleRow(const std::vector<std::string> & components, const std::vector<std::string> &_nm,const std::string &code, const std::string &folder){
    std::vector<std::string> res;
    long sz=_nm.size();
    res.resize(sz);
    if(components.size()!=2*sz-1){
      return res;
    }
    res[0]=components[0];
    std::string linkURL;
    for(long i=1;i<sz;++i){
      linkURL=extractURLFromLink(components[2*i]);
      if(linkURL!="!*!notLink"){
        res[i]="<a href=\"?page="+linkURL+"\">";
        res[i]+=_nm[i];
        res[i]+="</a>";
      }
      else{
        if(components[2*i]!="l"){
          res[i]=_nm[i];
        }
        else{
          res[i]="<a target=\"_new\" href=\""+folder+"/"+code+"/"+code;
          res[i]+=components[2*i-1];
          res[i]+=lastTwoDigitsOfTheYear(components[0])+".pdf\">";
          res[i]+=_nm[i];
          res[i]+="</a>";
        }
      }
    }
    return res;
  }
  std::string AbstractText::createItemTable(const PSDI::SessionData & _psd, const std::string & _td) const{
     std::string iTable;
     std::string categoryName="notFound",categoryCode="notFound",folder="notFound";
     long numItems=0;
     long pos;
     std::pair<std::string, int> allD;
     pos=0;allD=SF::extract(_td,pos,"_cName_","_/cName_");
     if(allD.second==1){
       categoryName=allD.first;
     }
     pos=0;allD=SF::extract(_td,pos,"_cCode_","_/cCode_");
     if(allD.second==1){
       categoryCode=allD.first;
     }
     pos=0;allD=SF::extract(_td,pos,"_folder_","_/folder_");
     if(allD.second==1){
       folder=allD.first;
     }
     pos=0;allD=SF::extract(_td,pos,"_num*_","_/num*_");
     if(allD.second==1){
       numItems=BF::stringToInteger(allD.first);
     }
     std::vector<std::string> colNames;
     pos=0;allD=SF::extract(_td,pos,"_colNames*_","_/colNames*_");
     if(allD.second==1){
       colNames=SF::stringToVector(allD.first,"_nm_","_/nm_");
     }
     std::vector<std::string> rows=SF::stringToVector(_td,"_row*_","_/row*_");
     std::stack<std::vector<std::string> > tbSt;
     long numRows=rows.size();
     std::vector<std::string> firstRow;
     if(numRows>0){
       firstRow=SF::stringToVector(rows[0],"_col*_","_/col*_");
       if(firstRow.size()>0){
         firstRow[0]=colNames[0];
         for(long i=0;i<firstRow.size();++i){
           if(firstRow[i]=="l"){
             firstRow[i]="g";
           }
         }
       }
       std::set<long> columnBreakers;
       columnBreakers.insert(0);
       long multipleColumns=0;
       if(numRows>8){
         columnBreakers.insert(numRows/3);
         columnBreakers.insert((2*numRows)/3);
         multipleColumns=1;
       }
       std::set<long>::const_iterator it,itE=columnBreakers.end();
       std::string tableText;
       for(long i=0;i<numRows;++i){
         it=columnBreakers.find(i);
         if(it!=itE){
           if(!tbSt.empty()){
             tbSt.push(prepareSingleRow(firstRow,colNames,categoryCode,folder));
             if(multipleColumns==0){
               tableText=HSF::tableFromStack(tbSt,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag())+tableText;
             }
             else{
               tableText="<div class=\"col-sm-4\">"+HSF::tableFromStack(tbSt,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag())+"</div>"+tableText;
             }
           }
         }
         tbSt.push(prepareSingleRow(SF::stringToVector(rows[numRows-i-1],"_col*_","_/col*_"),colNames,categoryCode,folder));
       }
       if(!tbSt.empty()){
         tbSt.push(prepareSingleRow(firstRow,colNames,categoryCode,folder));
         if(multipleColumns==0){
           tableText=HSF::tableFromStack(tbSt,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag())+tableText;
         }
         else{
           tableText="<div class=\"col-sm-4\">"+HSF::tableFromStack(tbSt,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag())+"</div>"+tableText;
         }
       }
       if(multipleColumns==1){
         tableText="<div class=\"row\">"+tableText+"</div>";
       }
       iTable+= tableText;
     }
     return iTable;
  }
}
#endif
