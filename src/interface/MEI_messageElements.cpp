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


#ifndef _INCL_WI_MElement_CPP
#define _INCL_WI_MElement_CPP

namespace MEI{
  MElement::MElement(const std:: string & rawT, const std::string & mSE, const std::string & mE,const std::string & mC){
    initialize(rawT,mSE,mE,mC);
  }
  int MElement::initialize(const std:: string & rawT,const std::string & mSE, const std::string & mE,const std::string & mC){
    mUserN="";mFirstN="";mLastN="";rawText=rawT;
    mCreated="";mText="";mType="";mElements="";mtTitle="";
    nmCodes=0; myCode=mC;
    mElCodes.resize(nmCodes);
    permitRead.clear();permitWrite.clear();
    PBD::createPermitSet(permitRead, mSE,s_individualPermissionB,  s_individualPermissionE,"read");
    PBD::createPermitSet(permitWrite, mSE,s_individualPermissionB,  s_individualPermissionE,"write");
    if(mE==""){
      return 0;
    }
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(mE,pos,st_sepUNameB,st_sepUNameE);
    if(allD.second==1){
      mUserN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepFNameB,st_sepFNameE);
    if(allD.second==1){
      mFirstN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepLNameB,st_sepLNameE);
    if(allD.second==1){
      mLastN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepNavB,st_sepNavE);
    if(allD.second==1){
      navigationSt=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepCreatedB,st_sepCreatedE);
    if(allD.second==1){
      mCreated=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepMTextB,st_sepMTextE);
    if(allD.second==1){
      mText=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepMessTypeB,st_sepMessTypeE);
    if(allD.second==1){
      mType=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepElementsB,st_sepElementsE);
    if(allD.second==1){
      mElements=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,st_sepTitleB,st_sepTitleE);
    if(allD.second==1){
        mtTitle=allD.first;
    }
    mElCodes=SF::stringToVector(mElements,"_n_","_/n_");
    nmCodes=mElCodes.size();
    mNavEls=SF::stringToVector(navigationSt,"_n_","_/n_");
    nmNavEls=mNavEls.size();

    pos=0;allD=SF::extract(mE,pos,st_sepAddMessB,st_sepAddMessE);
    if(allD.second==1){
        PBD::createPermitSet(permitAdd, allD.first,s_individualPermissionB,  s_individualPermissionE);
    }

    return 1;
  }
  int MElement::allowedToRead(const std::string & _uN) const{
    return PBD::permitExists_N("read"+myCode, permitRead,_uN);
  }
  int MElement::allowedToWrite(const std::string & _uN) const{
    return PBD::permitExists_N("write"+myCode,  permitWrite,_uN);
  }
  int MElement::allowedToCreateMessage(const std::string & _uN) const{
    if(_uN=="visitor"){return 0;}
    return PBD::permitExists_N("createMess"+myCode, permitAdd, _uN);
  }
  int MElement::allowedToModifyMessage(const std::string & _uN) const{
    std::set<std::string> munS;
    munS.insert(mUserN);
    return PBD::permitExists_N("boss"+mUserN, munS, _uN);
  }
  long MElement::numCodes() const{
    return nmCodes;
  }
  long MElement::numNavEls() const{
    return nmNavEls;
  }
  std::string MElement::getRawText() const{
    return rawText;
  }
  std::string MElement::code(const long & i) const{
    if(i<0){
      return "notFound";
    }
    if(i>nmCodes){
      return "notFound";
    }
    return mElCodes[i];
  }
  std::string MElement::navEl(const long & i) const{
    if(i<0){
      return "notFound";
    }
    if(i>nmNavEls){
      return "notFound";
    }
    return mNavEls[i];
  }
  std::string MElement::type() const{
    return mType;
  }
  std::string MElement::title() const{
    return mtTitle;
  }
  std::string MElement::text() const{
    return mText;
  }
  std::string MElement::navString() const{
    return navigationSt;
  }
  std::string MElement::navigationBar(const std::string & _page) const{
    std::string fR="";
    std::string tCode,tTitle;
    long pos;
    std::pair<std::string,int> allD;
    std::string prevDispl="";
    for(long i=0;i<nmNavEls;++i){
      tCode=mNavEls[i];
      tTitle=mNavEls[i];
      pos=0;allD=SF::extract(tCode,pos,"_!code_","_/!code_");
      if(allD.second==1){
        tCode=allD.first;
      }
      pos=0;allD=SF::extract(tTitle,pos,"_!title_","_/!title_");
      if(allD.second==1){
        tTitle=allD.first;
      }
      if(prevDispl!=""){
        fR+=" $\\to$ ";
      }
      prevDispl="";
      if(tTitle!=""){
        prevDispl=createLinkToTopic(tCode,tTitle,_page);
      }

      fR+=prevDispl;
    }
    return fR;
  }

  std::string MElement::createLinkToTopic(const std::string & tNum, const std::string & tName,const std::string & _page) const{
    return HSF::createButtonLink(MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+tNum,tName);
  }
  std::string MElement::createMTEditLink(const std::string & prevCode, const std::string &mToEdit, const std::string & linkName, const std::string & startM, const std::string & _page) const{
    if(prevCode=="adv"){
      return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+mToEdit+"&emr=r"+"\">"+linkName+ "</A>";
    }
    std::string stInsertion="";
    if(startM!=""){
      stInsertion="&st="+startM;
    }
    return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+prevCode+stInsertion+"&mei="+mToEdit+"\">"+linkName+ "</A>";
  }
  std::string MElement::prepareEditBox(const std::string & prevCode, const std::string &mToEdit, const std::string & startM,const std::string &_page) const{
    std::string fR="";
    std::string stInsertion="";
    if(startM!=""){
      stInsertion="&st="+startM;
    }
    fR+="<div class=\"card-body\">";
    fR+="_insert_";
    fR+="_n*_formInitialization_/n*_";
    fR+="_n*_mMess_/n*_";
    fR+="_n*_index.cgi?page="+_page+"&s1=u11&r1="+prevCode+stInsertion+"&msd="+mToEdit;
    fR+="_/n*_";
    fR+="_/insert_";
    fR+="_insert_";
    fR+="_n*_textAreaField_/n*_";
    fR+="_n*_mMess_/n*_";
    fR+="_n*_nmt_/n*_";
    fR+="_n*_ _/n*_";
    fR+="_n*_";
    if(mToEdit!="nm"){ fR+=mText;}
    fR+="_/n*_";
    fR+="_/insert_";
    fR+="_insert_";
    fR+="_n*_formPlacement_/n*_";
    fR+="_n*_mMess_/n*_";
    fR+="_/insert_ ";
    fR+="</div>";
    return fR;
  }

  std::string MElement::newMessageBoxOrLink(const std::string &prevCode,const long & messageOrLinkChoice,const std::string &_page) const{
    if(messageOrLinkChoice==1){return prepareEditBox(prevCode,"nm","",_page);}
    return createMTEditLink(prevCode,"nm",MWII::GL_WI.getDefaultWebText("New Message"),"",_page);
  }
  std::string prepareMCreatedString(const PSDI::SessionData & _psd, const std::string & in){
    std::string out=in;
    if(_psd.displayDaysInWeek.size()==TMF::GLOBAL_NUM_DAYS_IN_WEEK){
      for(long i=0;i<TMF::GLOBAL_NUM_DAYS_IN_WEEK;++i){
        out=SF::findAndReplace(out,TMF::GLOBAL_DAYS_OF_WEEK_A[i],_psd.displayDaysInWeek[i]);
      }
    }
    if(_psd.displayMonthsInYear.size()==TMF::GLOBAL_NUM_MONTHS_IN_YEAR){
      for(long i=0;i<TMF::GLOBAL_NUM_MONTHS_IN_YEAR;++i){
        out=SF::findAndReplace(out,TMF::GLOBAL_MONTHS_A[i],_psd.displayMonthsInYear[i]);
      }
    }
    return out;
  }
  std::string MElement::prepareMT(const PSDI::SessionData & _psd, const std::string & prevCode, const std::string & mToEdit, const std::string &displText, const std::string &startM, const int & editLinkInd,const std::string &_page) const{
    std::string fR="";
    std::string fB="<div class=\"card bg-light text-dark\">";
    fB+="<div class=\"card-body\">";
    std::string fE="</div></div>\n";
    std::string uNameDispl=mUserN;
    std::string fNameDispl=mFirstN;
    std::string lNameDispl=mLastN;
    std::string mCreatedDispl=prepareMCreatedString(_psd,mCreated);
    if((mType=="mColl")||(mType=="tColl")){
      uNameDispl="";fNameDispl="";lNameDispl="";mCreatedDispl="";
      fB="";fE="";
    }
    std::string editLinkDispl="";
    std::string editBoxDispl="";
    if(myCode==mToEdit){
      if(editLinkInd==1){editBoxDispl+=prepareEditBox(prevCode, mToEdit,startM,_page );}
    }
    else{
      if(editLinkInd==1){editLinkDispl=createMTEditLink(prevCode,myCode,"edit",startM,_page);}
    }

    fR+="<div> $${ }^{ }$$</div>";

    fR+="<div class=\"row\">";
    fR+="<div class=\"col-sm-4 \"><small>"+fNameDispl+" "+lNameDispl+"</small></div>";
    fR+="<div class=\"col-sm-4 \"><small>"+mCreatedDispl+"</small></div>";
    fR+="<div class=\"col-sm-4  \"><small>"+editLinkDispl+"</small></div>";
    fR+="</div>";

    fR+=fB+displText+fE;

    fR+=editBoxDispl;
    return fR;
  }
  std::string MElement::prepareIndividualTopic(const PSDI::SessionData &_psd, const std::string & prevCode, const std::string & mToEdit, const std::string &startM, const int & editLinkInd,const std::string &_page) const{
    return prepareMT(_psd,prevCode,mToEdit,createLinkToTopic(myCode,mtTitle,_page),startM,editLinkInd,_page);
  }

  std::string MElement::prepareIndividualMessage(const PSDI::SessionData &_psd, const std::string & prevCode, const std::string & mToEdit, const std::string &startM, const int & editLinkInd,const std::string & _page) const{
    return prepareMT(_psd,prevCode,mToEdit,DISPPF::prepareForHTMLDisplay( mText),startM,editLinkInd,_page);
  }
  std::string initMElementFromRawText(MElement & mEl, const std::string & rawText,const std::string &messCode){
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(rawText,pos,"_textData!!_","_/textData!!_");
    if(allD.second==0){
      return "failed";
    }
    std::string mTData=allD.first;
    pos=0;allD=SF::extract(rawText,pos,"_systemData!!_","_/systemData!!_");
    if(allD.second==0){
      return "failed";
    }
    std::string mSData=allD.first;

    mEl.initialize(rawText,mSData,mTData,messCode);
    return "success";
  }
  std::string initMElementFromCode(MElement & mEl, const std::string &messCode){
    MD::Message bm;
    int messageExists=bm.setFromExternalCode(messCode);
    if(messageExists==0){
      return "failed";
    }
    std::string rawText=bm.getTextData();
    return initMElementFromRawText(mEl,rawText,messCode);
  }
}

#endif
