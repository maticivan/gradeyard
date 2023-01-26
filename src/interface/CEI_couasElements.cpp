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


#ifndef _INCL_WI_CouasElement_CPP
#define _INCL_WI_CouasElement_CPP

namespace CEI{
  std::string CouasListElAtt::getUName() const{return uName;}
  std::string CouasListElAtt::getFName() const{return fName;}
  std::string CouasListElAtt::getLName() const{return lName;}
  std::string CouasListElAtt::getCode() const{return code;}
  std::string CouasListElAtt::getGradeStr(const std::string & grLabel) const{
    std::map<std::string,std::string>::const_iterator it,itE=grades.end();
    it=grades.find(grLabel);
    if(it==itE){
      return "";
    }
    std::string grRet=it->second;
    if((grRet=="")||(grRet==" ")||(grRet=="  ")){
      return "";
    }
    return grRet;
  }
  double CouasListElAtt::getGradeNum(const std::string & grLabel) const{
    std::string grade=getGradeStr(grLabel);
    double grNum=HSF::letterToNumGrade(grade);
    if(grNum<-999.0){
      grNum=0.0;
      grNum=BF::stringToDouble(grade);
    }
    return grNum;
  }
  void CouasListElAtt::setUName(const std::string & _x){uName=_x;}
  void CouasListElAtt::setFName(const std::string & _x){fName=_x;}
  void CouasListElAtt::setLName(const std::string & _x){lName=_x;}
  void CouasListElAtt::setCode(const std::string & _x){code=_x;}
  void CouasListElAtt::setGradeStr(const std::string & grLabel, const std::string & _x){
    grades[grLabel]=_x;
  }
  int CouasListElAtt::setFromString(const std::string & rawT){
    int fR=0;
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.st_clea_sepUNameB,LI::GL_LN.st_clea_sepUNameE); uName="";
    if(allD.second==1){
      ++fR;uName=allD.first;
    }
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.st_clea_sepFNameB,LI::GL_LN.st_clea_sepFNameE);fName="";
    if(allD.second==1){
      ++fR;fName=allD.first;
    }
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.st_clea_sepLNameB,LI::GL_LN.st_clea_sepLNameE);lName="";
    if(allD.second==1){
      ++fR;lName=allD.first;
    }
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.st_clea_sepCodeB,LI::GL_LN.st_clea_sepCodeE);code="";
    if(allD.second==1){
      ++fR;code=allD.first;
    }
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.st_clea_sepGradeB,LI::GL_LN.st_clea_sepGradeE);
    if(allD.second==1){
      ++fR;
      setGradeStr(LI::GL_LN.defaultGradeName,allD.first);
    }
    return fR;
  }
  std::string CouasListElAtt::putIntoString(const int & compactStringIndicator) const{
    std::string fR="";
    fR+=LI::GL_LN.st_clea_sepUNameB+uName+LI::GL_LN.st_clea_sepUNameE;
    if((compactStringIndicator==0)||(compactStringIndicator==5)||(compactStringIndicator==50)){
      fR+=LI::GL_LN.st_clea_sepFNameB+fName+LI::GL_LN.st_clea_sepFNameE;
      fR+=LI::GL_LN.st_clea_sepLNameB+lName+LI::GL_LN.st_clea_sepLNameE;
    }
    fR+=LI::GL_LN.st_clea_sepCodeB+code+LI::GL_LN.st_clea_sepCodeE;
    std::string grStr=getGradeStr(LI::GL_LN.defaultGradeName);
    if(compactStringIndicator==5){
      // put spaces instead of numZeros
      if(grStr=="0"){
        grStr="";
      }
    }
    if(compactStringIndicator==50){
      // put spaces always because the grade should be evaluated using formula
      grStr="";

    }
    fR+=LI::GL_LN.st_clea_sepGradeB+grStr+LI::GL_LN.st_clea_sepGradeE;
    return fR;
  }
  int CouasListElAtt::operator<(const CouasListElAtt & oth) const{
    if(lName<oth.lName){
      return 1;
    }
    if(lName>oth.lName){
      return 0;
    }
    if(fName<oth.fName){
      return 1;
    }
    if(fName>oth.fName){
      return 0;
    }
    if(uName<oth.uName){
      return 1;
    }
    return 0;
  }
  std::string CouasAttributes::prepareGradeFormula(const PSDI::SessionData & _psd,const std::string & rawFormula, const std::set<std::string> & grIncomplete, const long &remainingRecDepth) const{
    if(remainingRecDepth==0){
      return "";
    }
    std::string fR;
    std::vector<std::string> codesOfAssignmentsToGetGradesFrom=SF::stringToVector(rawFormula,LI::GL_LN.st_sepGetFromB,LI::GL_LN.st_sepGetFromE);
    long numA=codesOfAssignmentsToGetGradesFrom.size();
    for(long i=0;i<numA;++i){
      CouasElement cEl_i(_psd);
      std::string scc=ICEI::initFromCode(_psd,cEl_i,codesOfAssignmentsToGetGradesFrom[i]);
      if(scc!="failed"){
      }
    }
    return fR;
  }
  std::string CouasAttributes::setFromStringEasySteps(const std::string &rawT){
    mUserN="";mFirstN="";mLastN="";rawText=rawT;
    mCreated="";mText="";eType="";mElements="";mtTitle="";bossCode="";bossName="";
    std::pair<std::string,int> allD;
    long pos;
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.s_tDataB,LI::GL_LN.s_tDataE);
    if(allD.second==0){
      return "failed";
    }
    std::string mE=allD.first;
    pos=0;allD=SF::extract(rawT,pos,LI::GL_LN.s_sysDataB,LI::GL_LN.s_sysDataE);
    if(allD.second==0){
      return "failed";
    }
    std::string mSData=allD.first;
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepUNameB,LI::GL_LN.st_sepUNameE);
    if(allD.second==1){
      mUserN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepFNameB,LI::GL_LN.st_sepFNameE);
    if(allD.second==1){
      mFirstN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepLNameB,LI::GL_LN.st_sepLNameE);
    if(allD.second==1){
      mLastN=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepCreatedB,LI::GL_LN.st_sepCreatedE);
    if(allD.second==1){
      mCreated=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepCTextB,LI::GL_LN.st_sepCTextE);
    if(allD.second==1){
      mText=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepMessTypeB,LI::GL_LN.st_sepMessTypeE);
    if(allD.second==1){
      eType=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepTitleB,LI::GL_LN.st_sepTitleE);
    if(allD.second==1){
        mtTitle=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepBossCB,LI::GL_LN.st_sepBossCE);
    if(allD.second==1){
        bossCode=allD.first;
    }
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepBossNB,LI::GL_LN.st_sepBossNE);
    if(allD.second==1){
        bossName=allD.first;
    }
    grDisplays.clear();
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepGrDisplB,LI::GL_LN.st_sepGrDisplE);
    if(allD.second==1){
      grDisplays=SF::stringToMap(allD.first,LI::GL_LN.st_sepLabelB,LI::GL_LN.st_sepLabelE,LI::GL_LN.st_sepValueB,LI::GL_LN.st_sepValueE);
    }
    grFormulas.clear();
    pos=0;allD=SF::extract(mE,pos,LI::GL_LN.st_sepGradeFormulaB,LI::GL_LN.st_sepGradeFormulaE);
    if(allD.second==1){
      grFormulas=SF::stringToMap(mE,LI::GL_LN.st_sepGradeFSB,LI::GL_LN.st_sepGradeFSE,LI::GL_LN.st_sepGradeFMB,LI::GL_LN.st_sepGradeFME);
    }
    return mE;
  }
  int CouasAttributes::setFromString(const std::string &rawT, const std::map<std::string,CouasListElAtt> & gMap){
    std::string mE=setFromStringEasySteps(rawT);
    if(mE=="failed"){
      return 0;
    }
    std::pair<std::string,int> allD;
    gradeData.clear();
    long sz=gMap.size();
    if(sz>0){
      std::map<std::string,CouasListElAtt>::const_iterator it=gMap.begin(),itE=gMap.end();
      gradeData.resize(sz);
      long i=0;
      while(it!=itE){
        gradeData[i]=it->second;
        ++it;++i;
      }
    }
    return 1;
  }
  std::string CouasAttributes::putIntoString(const int & compactStringIndicator, const std::string & masterKey) const{
    std::string preparedText="";
    if(eType!=""){
      preparedText+=LI::GL_LN.st_sepMessTypeB+eType+LI::GL_LN.st_sepMessTypeE;
    }
    if(mtTitle!=""){
      preparedText+=LI::GL_LN.st_sepTitleB+mtTitle+LI::GL_LN.st_sepTitleE;
    }
    if(bossCode!=""){
      preparedText+=LI::GL_LN.st_sepBossCB+bossCode+LI::GL_LN.st_sepBossCE;
    }
    if(bossName!=""){
      preparedText+=LI::GL_LN.st_sepBossNB+bossName+LI::GL_LN.st_sepBossNE;
    }
    preparedText+="\n";
    preparedText+=LI::GL_LN.st_sepGrDisplB+ SF::mapToString(grDisplays,LI::GL_LN.st_sepLabelB,LI::GL_LN.st_sepLabelE,LI::GL_LN.st_sepValueB,LI::GL_LN.st_sepValueE,"\n")+LI::GL_LN.st_sepGrDisplE+"\n";
    preparedText+=LI::GL_LN.st_sepGradeFormulaB+ SF::mapToString(grFormulas,LI::GL_LN.st_sepGradeFSB,LI::GL_LN.st_sepGradeFSE,LI::GL_LN.st_sepGradeFMB,LI::GL_LN.st_sepGradeFME,"\n")+LI::GL_LN.st_sepGradeFormulaE+"\n";
    std::string elsData="";
    long sz=gradeData.size();
    for(long i=0;i<sz;++i){
      elsData+= LI::GL_LN.st_sepIndElB+(gradeData[i]).putIntoString(compactStringIndicator)+LI::GL_LN.st_sepIndElE;
      elsData+="\n";
    }
    if(masterKey==""){
      preparedText+="\n"+LI::GL_LN.st_sepElementsB+elsData+LI::GL_LN.st_sepElementsE+LI::GL_LN.st_sepCTextE;
    }
    else{
      elsData=ENCF::symmetricEncrypt(elsData + "_salt*_"+RNDF::genRandCode(15)+"_/salt*_",masterKey);
      preparedText+="\n"+LI::GL_LN.st_sepElementsEncB+elsData+LI::GL_LN.st_sepElementsEncE+LI::GL_LN.st_sepCTextE;
    }
    return TDI::couasTextString( mCreated,  mUserN, mFirstN,  mLastN,  preparedText);
  }
  std::map<std::string,CouasListElAtt> gradeDataToMap(const std::vector<CouasListElAtt> &gd){
    std::map<std::string,CouasListElAtt> fR;
    long sz=gd.size();
    for(long i=0;i<sz;++i){
      fR[(gd[i]).getUName()]=gd[i];
    }
    return fR;
  }
  std::vector<CouasListElAtt> mapToGradeData(const std::map<std::string,CouasListElAtt> & m){
    std::map<std::string,CouasListElAtt>::const_iterator it,itE;
    itE=m.end();
    it=m.begin();
    std::vector<CouasListElAtt> fR;
    fR.resize(m.size());
    long i=0;
    while(it!=itE){
      fR[i]=it->second;
      ++it;++i;
    }
    return fR;
  }
  void CouasAttributes::sortGradeData(const std::string & _criterion){
    std::string criterion=_criterion;
    if(criterion=="grade"){
      criterion="_gr_"+LI::GL_LN.defaultGradeName+"_/gr_";
    }
    long sz=gradeData.size();
    if(sz>0){
      std::vector<std::string> crit;
      crit.resize(sz);
      long done=0;
      if(criterion=="lastName"){
        for(long i=0;i<sz;++i){
          crit[i]=(gradeData[i]).getLName();
        }
        done=1;
      }
      if((done==0)&& (criterion=="firstName")){
        for(long i=0;i<sz;++i){
          crit[i]=(gradeData[i]).getFName();
        }
        done=1;
      }
      if((done==0)&&(criterion=="userName")){
        for(long i=0;i<sz;++i){
          crit[i]=(gradeData[i]).getUName();
        }
        done=1;
      }
      if((done==0)&&(criterion=="code")){
        for(long i=0;i<sz;++i){
          crit[i]=(gradeData[i]).getCode();
        }
        done=1;
      }
      if(done==1){
        SF::sortWithFollower(crit,gradeData);
      }
      else{
        long pos;std::pair<std::string,int> allD;
        pos=0;allD=SF::extract(criterion,pos,"_gr_","_/gr_");

        if(allD.second==1){
          std::vector<double> crGr;
          crGr.resize(sz);
          for(long i=0;i<sz;++i){
            crGr[i]=1000.0-(gradeData[i]).getGradeNum(allD.first);
          }
          SF::sortWithFollower(crGr,gradeData);
        }
        else{
          std::sort(gradeData.begin(),gradeData.end());
        }
      }
    }
  }
  CouasElement::CouasElement(const PSDI::SessionData & _psd,const std:: string & mE, const std::string & mC){
    initialize(_psd,mE,mC);
  }
  int CouasElement::initialize(const PSDI::SessionData & _psd,const std:: string & mE,const std::string & mC){
    if(mE==""){
      return 0;
    }
    coaa.myCode=mC;
    std::map<std::string,CouasListElAtt> gradeMap=ICEI::calculateGradesFromRawText(mE,_psd.masterKey,10) ;
    return coaa.setFromString(mE,gradeMap);
  }
  int CouasElement::gradeFromFormula() const{
    if (coaa.grFormulas.size()>0){
      return 1;
    }
    return 0;
  }
  std::string CouasElement::createMTEditLink(const std::string & reqC, const std::string &mToEdit, const std::string & linkName, const std::string & _page) const{
    if(reqC=="adv"){
      return "<A href=\""+MWII::GL_WI.getWSURL()+"/index.cgi?page="+_page+"&s1=u11&r1="+mToEdit+"&emr=r"+"\">"+linkName+ "</A>";
    }
    return mToEdit;
  }
  std::string CouasElement::getRawText() const{return coaa.rawText;}
  std::string CouasElement::type() const{return coaa.eType;}
  std::string CouasElement::title() const{return coaa.mtTitle;}
  std::string CouasElement::text() const{return coaa.mText;}
  int CouasElement::allowedToSeeAllGrades(const PSDI::SessionData & _psd) const{
    // WARNING: too restrictive
    if(_psd.isRoot=="yes"){
      return 1;
    }
    return 0;
  }
  int CouasElement::allowedToExecuteAutomaticGrading(const PSDI::SessionData & _psd) const{
    // WARNING: too restrictive
    if(_psd.isRoot=="yes"){
      return 1;
    }
    return 0;
  }
  int CouasElement::allowedToGrade(const PSDI::SessionData & _psd) const{
    // WARNING: to restrictive
    if(_psd.isRoot=="yes"){
      return 1;
    }
    return 0;
  }
  void CouasElement::sortGradeData(const std::string & cr){
    coaa.sortGradeData(cr);
  }
  std::map<std::string,std::string> CouasElement::displVarsThatNeedDBAccess(const std::map<std::string,std::string> & grd,const std::map<std::string,std::string> & grf) const{
    std::map<std::string,std::string>  fR;
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=grf.end();
    it=grf.find("all");
    if(it==itE){
      return fR;
    }
    if(grd.size()==0){
      return fR;
    }
    std::string mainFormula=it->second;
    mainFormula+="@@";
    long pos,posIn;std::pair<std::string,int> allD,allDIn;
    long tl=mainFormula.length();
    pos=0;
    std::string lhs,rhs;
    while(pos<tl){
      allD=SF::getEverythingBefore(mainFormula,pos,"=");
      if(allD.second==1){
        lhs=allD.first;
        allD=SF::getEverythingBefore(mainFormula,pos,"@@");
        if(allD.second==1){
          rhs=allD.first;
          posIn=0;allDIn=SF::extract(rhs,posIn,LI::GL_LN.st_sepGetFromB,LI::GL_LN.st_sepGetFromE);
          if(allDIn.second==1){
            rhs=allDIn.first;
            posIn=0;allDIn=SF::extract(lhs,posIn,"{","}");
            if(allDIn.second==1){
              lhs="{"+allDIn.first+"}";
              fR[lhs]=rhs;
            }
          }
        }
      }
    }
    return fR;
  }
  std::vector<std::string> CouasElement::prepareTopLineWithSortingLinks(const PSDI::SessionData & _psd,const std::vector<std::string> & tL) const{
    std::vector<std::string> fR,sortCrs;
    long sz=tL.size();
    if(sz<3){return fR;}
    fR.resize(sz);sortCrs.resize(sz);
    int accessAll=allowedToSeeAllGrades(_psd);
    sortCrs[0]="default";
    sortCrs[1]="userName";
    long base_lsz=2;
    long gradeEditIndex=-1;
    std::map<std::string,std::string> dVTNDBA;
    if((coaa.grDisplays).size()==0){
      sortCrs[base_lsz]="grade";
      gradeEditIndex=base_lsz;
    }
    else{
      std::map<std::string,std::string>::const_iterator itG,itGE;
      itG=(coaa.grDisplays).begin();itGE=(coaa.grDisplays).end();
      long j=base_lsz;
      while(itG!=itGE){
        sortCrs[j]="_gr_"+itG->second+"_/gr_";
        ++itG;++j;
      }
      dVTNDBA=displVarsThatNeedDBAccess(coaa.grDisplays,coaa.grFormulas);
    }
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if(start<0){start=0;}
    long cnt=0;
    std::string currentSc=_psd.sortCriterion;
    while(cnt<sz){
      if(currentSc==sortCrs[cnt]){
        cnt=sz+5;
      }
      ++cnt;
    }
    if(cnt<sz+5){
      currentSc="default";
    }
    std::set<std::string>::const_iterator it,itE;
    std::string linkToAssgn=MWII::GL_WI.get_e_parPage()+"="+_psd.pageRequested+"&s1=u11&r1=";
    std::string linkStDf=linkToAssgn+coaa.myCode+"&sc=";
    std::map<std::string,std::string>::const_iterator itD,itDE,itDS,itDSE;
    itDE=dVTNDBA.end();itDS=(coaa.grDisplays).begin();itDSE=(coaa.grDisplays).end();
    long pos;std::pair<std::string,int> allD;
    std::string displVar,displName;
    for(long i=0;i<sz;++i){
      fR[i]=tL[i];
      if((i>base_lsz-1)&&(itDS!=itDSE)){
        pos=0;allD=SF::extract(itDS->first,pos,LI::GL_LN.st_sepAbbrevDispB, LI::GL_LN.st_sepAbbrevDispE);
        if(allD.second==1){
          fR[i]=allD.first;
          displVar=itDS->second;
          itD=dVTNDBA.find(displVar);
          if(itD!=itDE){
            displName=fR[i];
            fR[i]="";
            fR[i]+="<a href=\"";
            fR[i]+=MWII::GL_WI.getWSURL()+"/index.cgi?";
            fR[i]+=linkToAssgn;
            fR[i]+=itD->second;
            fR[i]+="\">";
            fR[i]+="<button type=\"button\" class=\"btn btn-outline-dark btn-sm\">";
            if((displName.length()==2)&&(displName[0]=='H')){
              std::string newDN="";
              newDN+=displName[1];
              displName=newDN;
            }
            else{
              if((displName.length()==3)&&(displName[0]=='H')){
                std::string newDN="";
                newDN+=displName[1]; newDN+=displName[2];
                displName=newDN;
              }
            }
            fR[i]+=displName;
            fR[i]+="</button>";
            fR[i]+="</a>";
          }
        }
        ++itDS;
      }
      if( (currentSc!=sortCrs[i])&&(accessAll==1)){
        fR[i]+="<a style=\"color:yellow\" href=\"";
        fR[i]+=MWII::GL_WI.getWSURL()+"/index.cgi?";
        fR[i]+=linkStDf;
        fR[i]+=sortCrs[i];
        fR[i]+="\">";
        fR[i]+="<button type=\"button\" class=\"btn btn-outline-dark btn-sm\">";
        if(i>=base_lsz){
          fR[i]+="$\\downarrow$";
        }
        else{
          fR[i]+="$\\uparrow$";
        }
        fR[i]+="</button>";
        fR[i]+="</a>";
      }
      if(i==gradeEditIndex){
        if((allowedToGrade(_psd))&&(_psd.couasEditReq=="no")){
          fR[i]+=" <a href=\"";
          fR[i]+=MWII::GL_WI.getWSURL()+"/index.cgi?";
          fR[i]+=linkStDf;
          fR[i]+=currentSc;
          fR[i]+="&ecr=g";
          if(start!=0){
            fR[i]+="&st="+std::to_string(start);
          }
          fR[i]+="\"><button type=\"button\" class=\"btn btn-outline-dark btn-sm\">edit</button>";
          fR[i]+="</a>";
        }
      }
    }
    return fR;
  }
  std::map<std::string,std::pair<std::vector<std::string>,std::string> > rawGradesAndStatusFromRespReceiver(CouasAttributes & m_coaaI, const PSDI::SessionData & _psd,const std::string &respRecCode, const int & submit, const int & solvingStatus){
    std::map<std::string,std::pair<std::vector<std::string>, std::string> >fR;
    std::string myUserName=_psd.my_un;
    RTI::Response rrt(respRecCode,"no",myUserName);
    if(rrt.isInitialized()!=1){
      return fR;
    }
    RTI::ResponderInfo res=rrt.infoFromResponseText(_psd,"df");
    std::vector<RTI::ResponderInfo> rV;
    std::vector<std::string> rsV;
    std::vector<RTI::LocationOfDocuments> ldocs=rrt.getLocations(_psd);
    long sz=ldocs.size();
    long maxgcs=0,currentgcs;
    if(sz>0){
      rV.resize(sz);rsV.resize(sz);std::string indPost;
      for(long i=0;i<sz;++i){
        RTI::Response srt_i((ldocs[i]).gradeLocation,"no",myUserName);
        if(srt_i.isInitialized()!=1){
          (rV[i]).solverUserName="notFound";
        }
        else{
          rV[i]=srt_i.infoFromResponseText(_psd,"df",1);
          currentgcs=((rV[i]).gradersComments).size();
          if(currentgcs>maxgcs){
            maxgcs=currentgcs;
          }
        }
        rsV[i]="no";
        if(solvingStatus==1){
          RTI::Response solv_i((ldocs[i]).testLocation,"no",myUserName);
          if(solv_i.isInitialized()==1){
            if(solv_i.infoFromResponseText(_psd,"df",2).indicatorOfPOST==1){
              rsV[i]="yes";
            }
          }
        }
      }
    }
    else{
      if(res.documentType=="gradeOfResponse"){
        rV.resize(1);
        rV[0]=res;
        sz=1;
        maxgcs=(res.gradersComments).size();
      }
    }
    std::map<std::string,CouasListElAtt> mapGr;
    std::map<std::string,CouasListElAtt>::const_iterator itMM,itMME;
    if(submit==1){
      mapGr=gradeDataToMap(m_coaaI.gradeData);
    }
    if(maxgcs>0){
      std::vector<std::string> mLine,emptyLine,alternativeLine;
      std::pair<std::vector<std::string>,std::string> pairML;
      long lsz=3+maxgcs; emptyLine.resize(lsz);
      for(long j=0;j<lsz;++j){
        emptyLine[j]="-";
      }
      std::stack<std::vector<std::string> > allLines;
      std::map<std::string,std::string>::const_iterator it,itE;
      long pos;std::pair<std::string,int> allD;
      double score;
      int workIsDone=0;
      long qNum;
      for(long i=0;i<sz;++i){
        mLine=emptyLine;
        long cs=((rV[i]).gradersComments).size();
        long j=2;
        score=-999.9;
        if(cs>0){
          score=0.0;
          itE=((rV[i]).gradersComments).end();
          it=((rV[i]).gradersComments).begin();
          alternativeLine=mLine;
          while(it!=itE){
            pos=0;allD=SF::extract(it->second,pos,"_score_","_/score_");
            if(allD.second==1){
              mLine[j]=allD.first;
              score+=BF::stringToDouble(mLine[j]);
              qNum=BF::stringToIntegerRemoveStart(it->first)+1;
              if( (qNum>1) && (qNum<lsz-1) ){
                alternativeLine[qNum]=mLine[j];
              }
            }
            ++it;++j;
          }
          if(j!=lsz-1){
            mLine=alternativeLine;
          }
        }
        mLine[0]=(rV[i]).idInfoData;
        mLine[1]=(rV[i]).solverUserName;
        mLine[lsz-1]=BF::doubleToString(score);
        if(score>-900.0){
          if(submit==1){
            itMME=mapGr.end();
            itMM=mapGr.find((rV[i]).solverUserName);
            if(itMM!=itMME){
              (mapGr[(rV[i]).solverUserName]).setGradeStr(LI::GL_LN.defaultGradeName,mLine[lsz-1]);
              workIsDone=1;
            }
            else{
              mLine[lsz-1]+=" *";
            }
          }
        }
        else{
          mLine[lsz-1]="N/A";
        }
        pairML.first=mLine;
        pairML.second=rsV[i];
        fR[(rV[i]).solverUserName]=pairML;
      }
      if(workIsDone==1){
        m_coaaI.gradeData=mapToGradeData(mapGr);
        std::string newGrData=m_coaaI.putIntoString(0,_psd.masterKey);
        MCWCPI::modifyCouasWithoutCheckingPermissions(_psd,m_coaaI.myCode,newGrData);
      }
    }
    return fR;
  }
  std::string CouasElement::gradeFromRespReceiver(const PSDI::SessionData & _psd,const int & submit){
    std::map<std::string,std::pair<std::vector<std::string>,std::string> > resm=rawGradesAndStatusFromRespReceiver(coaa,_psd,_psd.rrgrader,submit,0);
    long msz=resm.size();
    if(msz==0){
      return "";
    }
    std::map<std::string,std::pair<std::vector<std::string>,std::string> >::const_iterator itRM,itRME;
    itRM=resm.begin();itRME=resm.end();
    long lsz=((itRM->second).first).size();
    std::vector<std::string> topLine,mLine,emptyLine;
    topLine.resize(lsz);emptyLine.resize(lsz);
    for(long j=0;j<lsz;++j){
      emptyLine[j]="-";
    }
    std::stack<std::vector<std::string> > allLines, allLinesCSV;
    topLine[0]=MWII::GL_WI.getDefaultWebText("Name");topLine[1]=MWII::GL_WI.getDefaultWebText("username");
    for(long j=2;j<lsz-1;++j){
      topLine[j]=std::to_string(j-1);
    }
    topLine[lsz-1]="$\\Sigma$";
    std::string fR="", fRCSV="";
    if(lsz>3){
      while(itRM!=itRME){
        mLine=(itRM->second).first;
        allLines.push(mLine);
        ++itRM;
      }
      SF::flipTheStack(allLines);
      allLinesCSV=allLines;
      fRCSV=HSF::tableOrCSVFromStack(allLinesCSV,",",MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
      allLines.push(topLine);
      fR= HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
    }
    return fR+fRCSV;
  }
  std::string CouasElement::bossLink(const PSDI::SessionData & _psd, const std::string & bC,const std::string &_bN) const{
    std::map<std::string,std::string> nenvv;
    nenvv["s1"]="u11";
    nenvv["r1"]=bC;
    std::string bN=_bN;
    if(_bN==""){
      bN=bC;
    }
    return HSF::createButtonLink(BI::updatedLinkNewEnvVars(_psd.respMap,nenvv),MWII::GL_WI.getDefaultWebText("Back to")+" "+bN);
  }
  std::string CouasElement::displayCouas(const PSDI::SessionData & _psd,const std::string & _numInList, const std::string & _numOnEachSide) const{
    std::string csvInd="no";
    int accessAll=allowedToSeeAllGrades(_psd);
    if(accessAll!=0){
      std::map<std::string,std::string>::const_iterator itRM,itRME;
      itRME=(_psd.respMap).end();
      itRM=(_psd.respMap).find("csvD");
      if(itRM!=itRME){
        csvInd= itRM->second;
      }
    }
    long nT=(coaa.gradeData).size();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(_numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    if(accessAll==0){
      start=0;end=nT;
    }
    std::string formInit="",formEnd="";
    std::string couasEditRequest=_psd.couasEditReq;
    long customDisplayNumItems=(coaa.grDisplays).size();
    if((couasEditRequest=="g")&&(customDisplayNumItems==0)){
      if(allowedToGrade(_psd)==0){
        return "";
      }
      formInit+="<div class=\"messageBox\"><FORM ACTION=\"";
      formInit+=MWII::GL_WI.getWSURL()+"/index.cgi?ecr=y&"+MWII::GL_WI.get_e_parPage()+"="+_psd.pageRequested+"&s1=u11&r1="+coaa.myCode+"&sc="+_psd.sortCriterion;
      formInit+="&st="+std::to_string(start);
      formInit+="&"+st_nil+"="+std::to_string(numb);
      formInit+="\" ENCTYPE=\"multipart/form-data";
      formInit+="\" METHOD=\"POST\">\n";
      formEnd+=BI::submButton("al",MWII::GL_WI.getDefaultWebText("Submit"));
      formEnd+="</div></FORM> \n";
    }
    long base_lsz=2;
    long lsz=base_lsz;
    std::vector<std::string> topLine,mLine;
    std::stack<std::vector<std::string> > allLines, allLinesCSV;
    lsz+=customDisplayNumItems;
    if(customDisplayNumItems==0){
      ++lsz;
    }
    topLine.resize(lsz);mLine.resize(lsz);
    topLine[0]=MWII::GL_WI.getDefaultWebText("Name");
    topLine[1]=MWII::GL_WI.getDefaultWebText("userName");
    std::map<std::string,std::string>::const_iterator itG,itGE;
    if(customDisplayNumItems==0){
      topLine[lsz-1]=MWII::GL_WI.getDefaultWebText("grade");
    }
    else{
      long j=0;
      itG=(coaa.grDisplays).begin();itGE=(coaa.grDisplays).end();
      while(itG!=itGE){
        topLine[base_lsz+j]=itG->first;
        ++itG;++j;
      }
    }
    if(end-start>0){
      topLine=prepareTopLineWithSortingLinks(_psd,topLine);
    }
    std::pair<std::string,int> allD;
    long pos;
    std::string labelField;CouasListElAtt clea;
    long totalLinesAdded=0;
    for(long i=0;i<nT;++i){
      if ( (csvInd=="yes") || ( (i>=start)&&(i<end))  ){
        clea=(coaa.gradeData)[i];
        if( (_psd.my_un==clea.getUName()) || (accessAll==1)) {
          mLine[0]=clea.getLName()+", "+clea.getFName();
          mLine[1]=clea.getUName();
          if(customDisplayNumItems==0){
            mLine[lsz-1]=clea.getGradeStr("{result}");
            if(couasEditRequest=="g"){
              labelField="G_"+clea.getCode();
              mLine[lsz-1]=BI::textInputField(labelField,"",5)+" "+mLine[lsz-1];
            }
          }
          else{
            long j=0;
            itG=(coaa.grDisplays).begin();itGE=(coaa.grDisplays).end();
            while(itG!=itGE){
              mLine[base_lsz+j]=clea.getGradeStr(itG->second);
              if(mLine[base_lsz+j].length()>6){
                mLine[base_lsz+j]=BF::eraseTrailingZeros(BF::double_to_string_withPrecision(BF::stringToDouble(mLine[base_lsz+j]),2));
              }
              ++itG;++j;
            }
          }
          if((i>=start)&&(i<end)){allLines.push(mLine);++totalLinesAdded;}
          allLinesCSV.push(mLine);
        }
      }
    }
    if(totalLinesAdded<1){return "";}
    SF::flipTheStack(allLines);
    SF::flipTheStack(allLinesCSV);
    allLines.push(topLine);
    std::string linkSt=MWII::GL_WI.get_e_parPage()+"="+_psd.pageRequested+"&s1=u11&r1="+coaa.myCode;
    if(_psd.sortCriterion!="default"){
      linkSt+="&sc="+_psd.sortCriterion;
    }
    std::string scroller="";
    std::string extraEndSc="";
    if((csvInd!="yes")&&(accessAll==1)){
      extraEndSc=HSF::createButtonLink("index.cgi?"+linkSt+"&csvD=yes",MWII::GL_WI.getDefaultWebText("Data in CSV"),"standardSize");
    }
    if(accessAll==1){scroller= BI::createScrollerWithExtraEnd(start,end,nT,numb,linkSt,_numOnEachSide,"-1",0,extraEndSc);}
    std::string mainTable=formInit+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag())+formEnd;
    std::string fR="";
    if(coaa.mtTitle!=""){
      fR+="<h2>"+coaa.mtTitle+"</h2>";
    }
    fR+= scroller+mainTable;
    if(coaa.bossCode!=""){
      std::string bLn=bossLink(_psd,coaa.bossCode,coaa.bossName);
      fR=bLn+fR+bLn;
    }
    if(csvInd=="yes"){
      fR+=HSF::tableOrCSVFromStack(allLinesCSV,",",MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
    }
    return fR;
  }
  std::string CouasElement::updateGrades(const PSDI::SessionData & _psd, const std::map<std::string,std::string> &allResponses) {
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=allResponses.end();
    it=allResponses.find(st_nil);
    std::string numInList="0";
    if(it!=itE){
      numInList=it->second;
    }
    long nT=(coaa.gradeData).size();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>nT-1)){start=0;}
    long numb=BF::stringToInteger(numInList);
    if(numb<0){numb=0;}
    long end=start+numb;
    if((end<0)||(end>nT)){
      end=nT;
    }
    std::string tempCd;CouasListElAtt clea;std::string newGr;
    int changesMade=0;
    for(long i=start;i<end;++i){
      clea=(coaa.gradeData)[i];
      tempCd="G_"+clea.getCode();
      it=allResponses.find(tempCd);
      if(it!=itE){
        newGr=it->second;
        if(newGr!=""){
          changesMade=1;
          (coaa.gradeData)[i].setGradeStr(LI::GL_LN.defaultGradeName,newGr);
        }
      }
    }
    if(changesMade==1){
      return coaa.putIntoString(0,_psd.masterKey);
    }
    return "failed";
  }
  std::string CouasElement::addStudents(const PSDI::SessionData & _psd,const std::vector<PASF::StudentData> & students){
    std::map<std::string,CouasListElAtt> grDataAll,newAdditions;
    long numSt=(coaa.gradeData).size();
    for(long i=0;i<numSt;++i){
      grDataAll[(coaa.gradeData)[i].getUName()]=(coaa.gradeData)[i];
    }
    std::map<std::string,CouasListElAtt>::const_iterator it,itE;
    long numNewSt=students.size();
    CouasListElAtt tmp;
    int messageForGradeErasing=5;
    if(coaa.grFormulas.size()>0){
      messageForGradeErasing=50;
    }
    for(long i=0;i<numNewSt;++i){
      itE=grDataAll.end();
      it=grDataAll.find(students[i].userName);
      if(it==itE){
        tmp.setUName(students[i].userName);
        tmp.setFName(students[i].firstName);
        tmp.setLName(students[i].lastName);
        CD::Counter cId(cntrCouasCode,cntrCC0,cntrCC1,cntrCCZ);
        cId.increase();
        tmp.setCode(cId.getCodeWord());
        grDataAll[students[i].userName]=tmp;
        newAdditions[students[i].userName]=tmp;
      }
    }
    if(newAdditions.size()>0){
      coaa.gradeData.resize(grDataAll.size());
      it=grDataAll.begin();itE=grDataAll.end();
      long i=0;
      while(it!=itE){
        (coaa.gradeData)[i]=it->second;
        ++it;++i;
      }
      return coaa.putIntoString(messageForGradeErasing,_psd.masterKey);
    }
    return "";
  }
}
#endif
