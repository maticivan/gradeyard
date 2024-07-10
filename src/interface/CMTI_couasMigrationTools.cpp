//    GradeYard learning management system
//
//    Copyright (C) 2024 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_CMTI_couasMigrationTools_CPP
#define _INCL_CMTI_couasMigrationTools_CPP

namespace CMTI{ 
  std::string GL_Failed="failed";
  int isCodeForFinal(const std::string& c){
    if(c=="{finalCouas}"){return 1;}
    return 0;
  }
  long identifyFirstInteger(const std::string& c){
    long res=0;
    long ln=c.length();
    long i=0;
    while((i<ln)&&((c[i]<'0')||(c[i]>'9'))){++i;}
    if(i>=ln){return -1;}
    while((i<ln)&&(c[i]>='0')&&(c[i]<='9')){
      res*=10;res+=static_cast<long>(c[i]-'0');++i;
    }
    return res;
  }
  std::string attemptAssignment(const std::string& c,const char& firstLetterL, const char& firstLetterU, const std::string& prefix){
    long aNum=identifyFirstInteger(c);
    if(aNum<1){return GL_Failed;}
    if(c.length()<3){return GL_Failed;}
    if(c[0]!='{'){return GL_Failed;}
    if((c[1]!=firstLetterL)&&(c[1]!=firstLetterU)){return GL_Failed;}
    return prefix+BF::padded(aNum,10,"0");
  }
  void updateBijectionBetweenCouasAndRR(std::pair<std::map<std::string,std::string>, std::map<std::string,std::string> >& b,
                                   const std::string& couasCode="all", const std::string& rrCode="all"){

    //
    if((couasCode=="all")&&(rrCode=="all")){
      std::map<std::string,std::string>::const_iterator itC=b.first.begin(), itCE=b.first.end();
      while(itC!=itCE){
        b.second[itC->second]=itC->first;
        ++itC;
      }
      return;
    }
    if(isCodeForFinal(rrCode)){
      b.first[couasCode]="F";
      return;
    }
    std::string attempt;
    attempt=attemptAssignment(rrCode,'h','H',"H");
    if(attempt!=GL_Failed){
      b.first[couasCode]=attempt;
    }
    attempt=attemptAssignment(rrCode,'q','Q',"Q");
    if(attempt!=GL_Failed){
      b.first[couasCode]=attempt;
    }
    attempt=attemptAssignment(rrCode,'m','M',"M");
    if(attempt!=GL_Failed){
      b.first[couasCode]=attempt;
    }
    return;
  }
  std::string createGradingForCourse(const PSDI::SessionData& psd, 
                                     const std::string& _couasData, 
                                     std::stack<std::string>& recoveryNames, 
                                     std::stack<std::string>& recoveryCommands,
                                     std::pair<std::map<std::string,std::string>, std::map<std::string,std::string> >& bijectionCouasRR){
    //WARNING: not implemented properly yet: permission checking is too restrictive 
    std::string uNM=psd.my_un;
    std::string uFN=psd.myFirstName;
    std::string uLN=psd.myLastName;
    std::string nSt="";
    std::string intUserId=(psd.myWU).getInternalId();;
    std::vector<std::string> cToCreate=SF::stringToVector(_couasData,"_couas!*_","_/couas!*_");
    std::vector<std::string> undeclaredVarsV; long uvsz;
    long sz=cToCreate.size();
    if(sz<1){
      return "!failed! No individual couas items.";
    }
    std::map<std::string,std::string> variableToContent,variableToCode;
    std::set<std::string> undeclaredVariables;
    std::string currentVar,currentContent;
    long pos;std::pair<std::string,int> allD;
    for(long i=0;i<sz;++i){
      pos=0;allD=SF::extract(cToCreate[i],pos,LI::GL_LN.st_sepCNmTextB,LI::GL_LN.st_sepCNmTextE);
      if(allD.second==1){
        currentVar=allD.first;
        pos=0;allD=SF::extract(cToCreate[i],pos,LI::GL_LN.st_sepCTextB,LI::GL_LN.st_sepCTextE);
        if(allD.second==1){
          currentContent=allD.first;
          variableToContent[currentVar]=currentContent;
          undeclaredVarsV=SF::stringToVector(currentContent,LI::GL_LN.st_sepGetFromB,LI::GL_LN.st_sepGetFromE);
          uvsz=undeclaredVarsV.size();
          for(long j=0;j<uvsz;++j){
            undeclaredVariables.insert(undeclaredVarsV[j]);
          }
        }
      }
    }
    std::map<std::string,std::string>::const_iterator it,itE;
    it=variableToContent.begin();itE=variableToContent.end();
    while(it!=itE){
      undeclaredVariables.erase(it->first);
      ++it;
    }
    if(undeclaredVariables.size()>0){
      return "!failed! There are variables that are not declared.";
    }
    int succ;
    it=variableToContent.begin();
    std::map<std::string,CAD::CouAs> mCM;
    while(it!=itE){
      CAD::CouAs myCouas;
      succ=myCouas.createCouAs(intUserId);
      if(succ==0){
        return "!failed!: Something may be bad with the database. The userId did exist when checked but could not create course / assignment.";
      }
      mCM[it->first]=myCouas;
      variableToCode[it->first]=myCouas.getExternalCodeFromInternalId();
      ++it;
    }
    it=variableToContent.begin();
    while(it!=itE){
      variableToContent[it->first]=SF::updateVarsWithVals(it->second,variableToCode,"","");
      ++it;
    }
    TMF::Timer tm;
    std::string mDataProcessed;
    it=variableToContent.begin();
    std::string succList="";
    std::string mainCourseCode="";
    std::string cCode,bossCode;
    while(it!=itE){
      mDataProcessed=TDI::couasTextString(tm.timeString(),uNM, uFN,uLN,it->second);
      (mCM[it->first]).setTextData(TDI::prepareTextForTextTableRaw(psd,mDataProcessed,"!noOldData!"));
      (mCM[it->first]).putInDB();
      cCode=(mCM[it->first]).getExternalCodeFromInternalId();
      succList+=" "+cCode;
      if(it->first=="{mainCouas}"){
        mainCourseCode="_mainCouas_"+cCode+"_/mainCouas_ ";
      }
      updateBijectionBetweenCouasAndRR(bijectionCouasRR,cCode,it->first);
      bossCode="notFound";
      pos=0;allD=SF::extract(mDataProcessed,pos,"_bossC!*_","_/bossC!*_");
      if(allD.second==1){
        bossCode=allD.first;
      }
      recoveryNames.push("courseCreation");
      recoveryCommands.push(BMD::deleteCouasOrMessage("deleteCourseAssignment",cCode,bossCode));
      ++it;
    }
    updateBijectionBetweenCouasAndRR(bijectionCouasRR);
    return "!success!:"+succList+mainCourseCode;
  }


  std::string buttonToUseCouasMigrationTools(const std::string& pageReq){
    std::string res;
    std::string tmp=MWII::GL_WI.getDefaultWebText("couasMigrationText01");
    if(tmp!="couasMigrationText01"){res+=tmp;}
    else{
      res+="\\begin{box} <p><b>WARNING: </b> Missing entry in the database with courses and assignments. </p><p><a href=\"?page=\n";
    }
    res+=pageReq;
    tmp=MWII::GL_WI.getDefaultWebText("couasMigrationText02");
    if(tmp!="couasMigrationText02"){res+=tmp;} 
    else{
      res+="&cmt001=yes\"><button type=\"button\" class=\"btn btn-outline-dark btn-sm\">Recovery attempt</button></a></p>\\end{box}\n";
    }
    return res;
  } 
  int Student::operator<(const Student& oth) const{
    if(last<oth.last){return 1;}
    if(oth.last<last){return 0;}
    if(first<oth.first){return 1;}
    if(oth.first<first){return 0;}
    if(username<oth.username){return 1;}
    if(oth.username<username){return 0;}
    if(email<oth.email){return 1;}
    return 0;
  }
  Student studentFromString(const std::string& st){
    Student res;
    std::vector<std::string> raw=SF::stringToVector(st,"_n*|_","_/n*|_");
    if(raw.size()>4){res.username=raw[4];}
    if(raw.size()>3){res.email=raw[3];}
    if(raw.size()>2){
      std::vector<std::string> nameComponents=SF::stringToVectorSimpleSeparator(raw[2]," ");
      long sz=nameComponents.size();
      std::string unlikelyLastName;
      if(sz>1){
        if((nameComponents[sz-1].length()<2) || (nameComponents[sz-1]=="I") || (nameComponents[sz-1]=="II") || (nameComponents[sz-1]=="III")){
          if(nameComponents[sz-1].length()>0){
            unlikelyLastName=nameComponents[sz-1];
          }
          --sz;
          if((unlikelyLastName==" ")||(unlikelyLastName=="\t")){
            unlikelyLastName="";
          }
          else{
            unlikelyLastName=" "+unlikelyLastName;
          }
        }
      }
      if(sz>1){
        res.last=nameComponents[sz-1];
        --sz;
        for(long i=0;i<sz;++i){
          if(i>0){res.first+=" ";}
          res.first+=nameComponents[i];
        }
        res.first+=unlikelyLastName;
      }
    }
    return res;
  } 
  RespRecNameComponents getRRNC(const std::string& start, const std::string& in){
    RespRecNameComponents res;
    res.success=0;
    long i=0;
    long stSz=start.length();
    long inSz=in.length();
    if(inSz<=stSz){return res;}
    while(i<stSz){
      if(start[i]!=in[i]){return res;}
      ++i;
    }
    while((i<inSz) && BF::isLetter(in[i])){
      res.examAbbreviation+=in[i];
      ++i;
    }
    while((i<inSz) && (in[i]>='0') && (in[i]<='9')){
      res.examNumber+=in[i];
      ++i;
    }
    while(i<inSz){
      res.extension+=in[i];
      ++i;
    }
    res.success=1;
    return res;
  }
  int isPrefix(const std::string& start, const std::string& in){
    long i=0;
    long stSz=start.length();
    long inSz=in.length();
    if(inSz<stSz){return 0;}
    while(i<stSz){
      if(start[i]!=in[i]){return 0;}
      ++i;
    }
    return 1;
  }
  void getSemesterCourseNameAndSection(const std::string& in, std::string& semester, std::string& cNS, int& succ){
    long len=in.length();
    semester="";cNS="";succ=0;
    if(len<3){return;}
    if((in[len-2]!='G')||(in[len-1]!='r')){return;}
    len-=2;
    long i=0;
    while((i<len) && BF::isLetter(in[i])){
      semester+=in[i];
      ++i;
    }
    if(i>=len){semester="";return;}
    while((i<len) && (in[i]>='0') && (in[i]<='9')){
      semester+=in[i];
      ++i;
    }
    if(i>=len){semester="";return;}
    while(i<len){
      cNS+=in[i];
      ++i;
    }
    succ=1;
    return;
  }
  std::set<std::string> getDBElementsWithPrefix(const std::string& dbName, const std::string& prefix, const long& limit=200){
    std::set<std::string> res;
    MTF::Table& textTable=DD::GL_MAIN_DB.dbsM[dbName];
    TOSF::TSets indexRes=textTable.getIndTable("_i_1_ei_");
    long indSz=indexRes.size(); 
    std::vector<std::string> searchStart;
    searchStart.resize(1); searchStart[0]=prefix;
    long indLow=indexRes.lowerBoundRowIndex(searchStart);
    if(indLow<0){indLow=0;} 
    long sSize=0;
    while((indLow<indSz)&&(sSize<limit)){
      std::string nextEl=(indexRes.getKeysFromRow(indLow))[0];
      if(isPrefix(searchStart[0],nextEl)){
        res.insert(nextEl);
        ++sSize;
      }
      else{
        if(sSize>0){indLow=indSz;}
      }
      ++indLow;
    }
    return res;
  }
  void getAllExamNames(DocumentData& dd){
    std::string pref=dd.semester+dd.courseNameAndSection;
    std::set<std::string> sPref=getDBElementsWithPrefix("response",pref);
    std::stack<RespRecNameComponents> stH,stQ,stM,stF;
    std::set<std::string>::const_iterator it=sPref.begin(),itE=sPref.end();
    while(it!=itE){
      RespRecNameComponents rrcomp=getRRNC(pref,*it);
      if(rrcomp.examAbbreviation=="EH"){stH.push(rrcomp);} 
      if(rrcomp.examAbbreviation=="EQ"){stQ.push(rrcomp);} 
      if(rrcomp.examAbbreviation=="EM"){stM.push(rrcomp);} 
      if(rrcomp.examAbbreviation=="EF"){stF.push(rrcomp);}         
      ++it;
    } 
    dd.homeworks=SF::stackToVector(stH);
    dd.quizzes=SF::stackToVector(stQ);
    dd.midterms=SF::stackToVector(stM);
    dd.finals=SF::stackToVector(stF);
  }
  void getAllStudents(DocumentData& dd,const RespRecNameComponents& rrnc){
    std::string rrname=dd.semester+dd.courseNameAndSection+rrnc.examAbbreviation+rrnc.examNumber;
    RMD::Response sf;
    sf.setFromTextName(rrname); 
    std::vector<std::string> rawStData=SF::stringToVector(sf.getTextData(),"_st*|_","_/st*|_");
    for(long i=0;i<rawStData.size();++i){
      dd.students.insert(studentFromString(rawStData[i]));
    }
  }
  void getAllStudents(DocumentData& dd, const std::vector<RespRecNameComponents>& v){
    for(long i=0;i<v.size();++i){
      if(v[i].extension==""){
        getAllStudents(dd,v[i]);
      }
    }
  }
  void getAllStudents(DocumentData& dd){
    getAllStudents(dd,dd.homeworks);
    getAllStudents(dd,dd.quizzes);
    getAllStudents(dd,dd.midterms);
    getAllStudents(dd,dd.finals);
  }
  DocumentData getData(const PSDI::SessionData& _psd){
    DocumentData res;
    res.success=0;
    getSemesterCourseNameAndSection(_psd.pageRequested,res.semester,res.courseNameAndSection,res.success);
    getAllExamNames(res);
    getAllStudents(res);
    return res;
  }
  std::vector<PASF::StudentData> convertData(const std::set<Student>& st){
    std::vector<PASF::StudentData> res;
    res.resize(st.size());
    std::set<Student>::const_iterator it,itE;
    itE=st.end();
    it=st.begin();
    long i=0;
    while(it!=itE){
      res[i].firstName=(*it).first;
      res[i].lastName=(*it).last;
      res[i].userName=(*it).username;
      res[i].email=(*it).email;
      ++it;
      ++i;
    }
    return res;
  }
  template<typename T>
  T* removeConstFromAddress(const T& x){
    void* a=(void*)(&x);
    T* constRemoved=(T*)(a);
    return constRemoved;
  }
  std::string updateMainGradingDocumentInVarMap(const PSDI::SessionData & _psd, const std::string& semester, const std::string& cNS, const std::string& newCode){
    TMD::MText varDocument;
    int scc=varDocument.setFromTextName(semester+cNS+"Var");
    if(scc==0){return "error: No Var document";}
    std::string rawText=varDocument.getTextData();
    std::map<std::string,std::string> mMap=SF::stringToMap(rawText,"_vr_","_/vr_","_vl_","_/vl_");
    std::map<std::string,std::string>::const_iterator it=mMap.find("mainGradingDocument");
    if(it==mMap.end()){
      rawText=SF::findAndReplace(rawText,"_vVPair_"," _vVPair_ _vr_mainGradingDocument_/vr_ _vl_"+newCode+"_/vl_ _/vVPair_ _vVPair_");
    }
    else{
      rawText=SF::findAndReplace(rawText,"_vl_"+it->second+"_/vl_","_vl_"+newCode+"_/vl_");
    }
    varDocument.setTextData(rawText);
    varDocument.putInDB();
    return "";
  }
  std::string getTemplateFromString(const std::string& textName, const std::map<std::string,std::string>& sReplaceMap){
    TMD::MText varDocument;
    int scc=varDocument.setFromTextName(textName);
    if(scc==0){return "";}
    std::string rawText=varDocument.getTextData();
    std::map<std::string,std::string> mT=SF::stringToMap(rawText,"_templateName_","_/templateName_","_templateContent_","_/templateContent_");
    std::map<std::string,std::string>::const_iterator it=mT.find("gradingForCourse");
    if(it==mT.end()){
      return "";
    }
    return MFRF::findAndReplace(it->second,sReplaceMap);
  }
  void updateTemplateMap(const std::string& name, std::map<std::string,std::string>& oldM,const std::map<std::string,std::string>&  sReplaceMap){
    std::set<std::string> sNames=getDBElementsWithPrefix("mainText",name);
    std::set<std::string>::const_iterator it=sNames.begin(),itE=sNames.end();
    while(it!=itE){
      oldM[*it]=getTemplateFromString(*it,sReplaceMap);
      ++it;
    }
  }
  std::map<std::string,std::string> getMapOfTemplates(const std::map<std::string,std::string>& sReplaceMap){
    std::map<std::string,std::string> res;
    updateTemplateMap("templateTwoMidterms",res,sReplaceMap);
    updateTemplateMap("templateThreeMidterms",res,sReplaceMap);
    updateTemplateMap("templateNoMidterms",res,sReplaceMap);
    return res;
  }
  std::string createLabel(const long& numericPos, const std::string& abbreviation, const std::string& varName, const long& paddingNum=1){
    std::string res="_label!__pos_";
    if(paddingNum==1){res+=std::to_string(numericPos);}
    else{res+=BF::padded(numericPos,paddingNum,"0");}
    res+="_/pos__disp_";
    res+=abbreviation;
    res+="_/disp__/label!__value!_{";
    res+=varName;
    res+="}_/value!_\n";
    return res;
  }
  std::string properDecimal(const long& n){
    std::string res="0.";
    if(n<10){res+="0";}
    res+=std::to_string(n);
    return res;
  }
  std::string couasCategory(const long& num,const std::string& couasNm, const std::string& sing, const std::string& plu, const std::string& abbr, const std::string& label, const std::string& label2, const long& padd){
    std::string res;
    res+="_couas!*_\n_couasName!*_{"+couasNm+"}_/couasName!*__couasText!*_\n";
    res+="_title!*_"+plu+"_/title!*__bossC!*_{mainCouas}_/bossC!*__bossN!*_Grade summary_/bossN!*_\n";
    res+="_grDisplay!*_\n";
    long numericPos=0;
    while(numericPos<num){
      res+=createLabel(numericPos,abbr+std::to_string(numericPos+1),label+std::to_string(numericPos+1),padd);
      ++numericPos;
    }
    res+=createLabel(numericPos,"T","result",padd);
    res+="_/grDisplay!*_\n_grFormulas!*__fScope!*_all_/fScope!*__fmath!*_\n";
    long eachHW=100/num;
    long rem=100%num;
    long currentHWScore;
    numericPos=0;
    res+="{result}=";
    while(numericPos<num){
      currentHWScore=eachHW;
      if(numericPos<rem){++currentHWScore;}
      if(numericPos>0){res+="+";}
      res+=properDecimal(currentHWScore)+"*{"+label+std::to_string(numericPos+1)+"}";
      ++numericPos;
    }
    res+="@@\n";
    numericPos=0;
    while(numericPos<num){
      ++numericPos;
      res+="{"+label+std::to_string(numericPos)+"}=_getFrom_{"+label2+std::to_string(numericPos)+"Couas}_/getFrom_@@\n";
    }
    res+="_/fmath!*_\n_/grFormulas!*_\n_/couasText!*__/couas!*_\n\n";
    numericPos=0;
    while(numericPos<num){
      ++numericPos;
      res+="_couas!*_\n_couasName!*_{"+label2+std::to_string(numericPos)+"Couas}_/couasName!*__couasText!*_\n";
      res+="_title!*_"+sing+" "+std::to_string(numericPos)+"_/title!*__bossC!*_{"+couasNm+"}_/bossC!*_";
      res+="_bossN!*_All "+plu+"_/bossN!*_\n_grDisplay!*__/grDisplay!*_\n_grFormulas!*__/grFormulas!*_\n";
      res+="_/couasText!*__/couas!*_\n";
    }
    return res;
  }
  std::string customTemplate(const std::string& cTitle,long numHW, long numMT){
    std::string res;
    res+="_couas!*_\n_couasName!*_{mainCouas}_/couasName!*__couasText!*_\n";
    res+="_title!*_"+cTitle+"_/title!*_\n";
    long numericPos=0;
    res+="_grDisplay!*_";
    long hWeight=0;
    long mWeight=0;
    if(numHW>0){hWeight=25;res+=createLabel(numericPos,"H","homeworks"); ++numericPos;}
    if(numMT>0){mWeight=50;res+=createLabel(numericPos,"E","midterms"); ++numericPos;}
    if((mWeight<1)&&(hWeight>0)){hWeight=50;}
    long fWeight=100-mWeight-hWeight;
    res+=createLabel(numericPos,"F","final");++numericPos;
    res+=createLabel(numericPos,"T","result");++numericPos;
    res+=createLabel(numericPos,"G","letterG");++numericPos;
    res+="_/grDisplay!*_\n";
    res+="_grFormulas!*__fScope!*_all_/fScope!*__fmath!*_\n";
    res+="{result}=";
    if(hWeight>0){res+=properDecimal(hWeight)+"*({homeworks}&100)+";}
    if(mWeight>0){res+=properDecimal(mWeight)+"*({midterms}&100)+";}
    res+=properDecimal(fWeight)+"*({final}&100)@@\n";
    if(numHW>0){res+="{homeworks}=_getFrom_{hwCouas}_/getFrom_@@ \n";}
    if(numMT>0){res+="{midterms}=_getFrom_{mtCouas}_/getFrom_@@ \n";}
    res+="{final}=_getFrom_{finalCouas}_/getFrom_@@\n";
    res+="{letterG}=_getFrom_{letterGCouas}_/getFrom_@@\n";
    res+="_/fmath!*_\n";
    res+="_/grFormulas!*_\n";
    res+="_/couasText!*__/couas!*_\n";
    if(numHW>0){
      res+=couasCategory(numHW,"hwCouas","Homework","Homeworks","H","hw","h",10);
    }
    if(numMT>0){
      res+=couasCategory(numMT,"mtCouas","Midterm","Midterms","M","mw","mw",1);
    }
    res+="_couas!*_\n_couasName!*_{letterGCouas}_/couasName!*__couasText!*_\n";
    res+="_title!*_G_/title!*__bossC!*_{mainCouas}_/bossC!*__bossN!*_Grade summary_/bossN!*_\n";
    res+="_grDisplay!*__/grDisplay!*_\n";
    res+="_grFormulas!*__/grFormulas!*_\n_/couasText!*__/couas!*_\n";
    res+="\n\n_couas!*_\n_couasName!*_{finalCouas}_/couasName!*__couasText!*_\n";
    res+="_title!*_F_/title!*__bossC!*_{mainCouas}_/bossC!*__bossN!*_Grade summary_/bossN!*_\n";
    res+="_grDisplay!*__/grDisplay!*_\n_grFormulas!*__/grFormulas!*_\n_/couasText!*__/couas!*_\n";
    return res;
  }
  std::string enrollStudents(const PSDI::SessionData & _psd, const std::string & _couas, const std::vector<PASF::StudentData> & students, const long & recursionDepth=10){
    if(recursionDepth<1){
      return "";
    }
    CEI::CouasElement cEl(_psd);
    if(ICEI::initFromCode(_psd,cEl,_couas)==GL_Failed){
      return "";
    }
    std::string modifiedText=cEl.addStudents(_psd,students);
    if(modifiedText!=""){
      MCWCPI::modifyCouasWithoutCheckingPermissions(_psd,_couas,modifiedText);
    }
    long pos;std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(cEl.getRawText(),pos, LI::GL_LN.st_sepGradeFormulaB,LI::GL_LN.st_sepGradeFormulaE);
    if(allD.second==1){
      std::vector<std::string> vForRec=SF::stringToVector(allD.first,LI::GL_LN.st_sepGetFromB, LI::GL_LN.st_sepGetFromE);
      long sz=vForRec.size();
      for(long i=0;i<sz;++i){
        enrollStudents(_psd,vForRec[i],students,recursionDepth-1);
      }
    }
    return "";
  }
  std::string assignGradesToCoaaFromRR(const PSDI::SessionData& _psd, const std::string& couasCode, const std::string& rrCode){
    std::string res;
    CEI::CouasElement cEl(_psd);
    if(ICEI::initFromCode(_psd,cEl,couasCode)=="failed"){
      GF::GL_DEB_MESSAGES.addMessage("<pre>Failed with ("+couasCode+","+rrCode+")</pre>");
      return "";
    }
    PSDI::SessionData psdWithModifiedRRCode=_psd;
    psdWithModifiedRRCode.rrgrader=rrCode;
    cEl.gradeFromRespReceiver(psdWithModifiedRRCode,1);
    return "";
  }
  std::string assignGradesToCoaaFromRR(const PSDI::SessionData & _psd, const std::string& semester, const std::string& cNS, const std::pair<std::map<std::string,std::string>,std::map<std::string,std::string> >& bijection){
    std::string res;
    std::map<std::string,std::string>::const_iterator it,itE;
    it=bijection.first.begin();
    itE=bijection.first.end();
    while(it!=itE){ 
      res+=assignGradesToCoaaFromRR(_psd,it->first,semester+cNS+"E"+it->second);
      ++it;
    }
    return res;
  }
  void chooseTheMostAppropriateTemplate(const DocumentData& mainDocData, const std::map<std::string,std::string>& mapOfTemplates, std::string& currentTemplate){
    std::map<std::string,std::string>::const_iterator it,itE=mapOfTemplates.end();
    if((mainDocData.homeworks.size()==14)&&(mainDocData.midterms.size()==0)){
      it=mapOfTemplates.find("templateNoMidterms7Homeworks");
      if(it!=itE){currentTemplate=it->second;}
      return;
    }
    if((mainDocData.homeworks.size()>16)&&(mainDocData.midterms.size()==4)){
      it=mapOfTemplates.find("templateTwoMidterms");
      if(it!=itE){currentTemplate=it->second;}
      return;
    }
    if((mainDocData.homeworks.size()>16)&&(mainDocData.midterms.size()==4)){
      it=mapOfTemplates.find("templateTwoMidterms");
      if(it!=itE){currentTemplate=it->second;}
      return;
    }
    if(mainDocData.midterms.size()==6){
      it=mapOfTemplates.find("templateThreeMidterms");
      if(it!=itE){currentTemplate=it->second;}
      return;
    }
    if((mainDocData.homeworks.size()==16)&&(mainDocData.midterms.size()==4)){
      it=mapOfTemplates.find("templateTwoMidtermsWeakHW");
      if(it!=itE){currentTemplate=it->second;}
      return;
    }
    return;
  }
  std::string analysisAndMigrationWhenCouasDoesntExist(const PSDI::SessionData & _psd, CEI::CouasElement& cEl){
    if(_psd.isRoot!="yes"){return "";}
    std::map<std::string,std::string>::const_iterator itRM;
    itRM=_psd.respMap.find("cmt001");
    if(itRM==_psd.respMap.end()){
      return buttonToUseCouasMigrationTools(_psd.pageRequested);
    }
    if(itRM->second!="yes"){
      return buttonToUseCouasMigrationTools(_psd.pageRequested);
    }
    DocumentData mainDocData=getData(_psd);
    std::map<std::string,std::string> sReplMap;
    sReplMap["_vr_semester_/vr_"]=mainDocData.semester;
    sReplMap["_vr_courseNumber_/vr_"]=mainDocData.courseNameAndSection;
    std::map<std::string,std::string> mapOfTemplates=getMapOfTemplates(sReplMap);
    std::string templateM=customTemplate(mainDocData.semester+" "+mainDocData.courseNameAndSection,mainDocData.homeworks.size()/2,mainDocData.midterms.size()/2);
    mapOfTemplates["defaultTemplate"]=templateM;
    std::string res="<h2>Recovery completed</h2>";
    std::map<std::string,std::string>::const_iterator itMT,itMTE;
    itMT=mapOfTemplates.begin();itMTE=mapOfTemplates.end();
    chooseTheMostAppropriateTemplate(mainDocData,mapOfTemplates,templateM);
    std::stack<std::string>* aRecNames=removeConstFromAddress(_psd.recoveryOperationNames);
    std::stack<std::string>* aRecComm=removeConstFromAddress(_psd.recoveryOperationCommands);
    std::pair<std::map<std::string,std::string>, std::map<std::string,std::string> > bijectionCouasRR;
    std::string outMessage=createGradingForCourse(_psd,templateM,*aRecNames,*aRecComm,bijectionCouasRR);
    long pos; std::pair<std::string,int> allD;
    pos=0;allD=SF::extract(outMessage,pos,"_mainCouas_","_/mainCouas_");
    if(allD.second==0){
      return res+="<pre>!failed!: grading documents were not created properly</pre><pre>"+outMessage+"</pre>\n";
    }
    std::string couasId=allD.first;
    std::string updateRes=updateMainGradingDocumentInVarMap(_psd,mainDocData.semester,mainDocData.courseNameAndSection,couasId);
    res+=updateRes;
    std::vector<PASF::StudentData> students=convertData(mainDocData.students);
    res+= enrollStudents(_psd,couasId,students) ;
    res+= assignGradesToCoaaFromRR(_psd,mainDocData.semester, mainDocData.courseNameAndSection,bijectionCouasRR);
    return res;

  }
}
#endif
