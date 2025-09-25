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

#ifndef _INCL_WI_FileFormCenter_CPP
#define _INCL_WI_FileFormCenter_CPP

namespace FFI{
  MatrixForDisplay::MatrixForDisplay(){
    numRows=0;
    numCols=0;
    matrixFromTemplateIndicator=0;
  }
  std::string determineVariableType(const std::string& in){
    std::string out=MFRF::toLowerCase(in);
    if(out=="int"){out="long";}
    if(out=="float"){out="double";}
    if( (out!="long") && (out!="double") ){
      out="string";
    }
    return out;
  }
  long determineNumberOfDecimals(const std::string& in){
    long res = BF::stringToInteger(in);
    if(res<0){res=2;}
    return res;
  }
  int PairRC::operator<(const PairRC& b)const{
    if(row<b.row){return 1;}
    if(row>b.row){return 0;}
    if(col<b.col){return 1;}
    return 0;
  }
  MatrixInfo::MatrixInfo(){
    nRows=-1;nCols=-1;coordinates.row=-1;coordinates.col=-1;
    matrixCellIndicator=0;matrixTemplateIndicator=0;nDecimalsIfDouble=2;
    type="string";
  }
  int goodCoordinate(const long& c,const long & upperLimit){
    if(c<0){return 0;}
    if(c>upperLimit){return 0;}
    return 1;
  }
  MatrixInfo mInfoFromMap(const std::map<std::string,std::string>& vMap,const std::string& _nm){
    std::map<std::string,std::string>::const_iterator it,itE=vMap.end();
    MatrixInfo res;
    res.matrixName="";
    res.varName=_nm;
    it=vMap.find(GL_matrixName);
    if(it!=itE){
      res.matrixName=it->second;
    }
    it=vMap.find(GL_rowPosition);
    if(it!=itE){
      res.coordinates.row=BF::stringToInteger(it->second);
    }
    it=vMap.find(GL_colPosition);
    if(it!=itE){
      res.coordinates.col=BF::stringToInteger(it->second);
    }
    it=vMap.find(GL_matrixTotalNumRows);
    if(it!=itE){
      res.nRows=BF::stringToInteger(it->second);
    }
    it=vMap.find(GL_matrixTotalNumCols);
    if(it!=itE){
      res.nCols=BF::stringToInteger(it->second);
    }
    it=vMap.find(GL_vType);
    if(it!=itE){
      res.type=determineVariableType(it->second);
    }
    it=vMap.find(GL_numDecimals);
    if(it!=itE){
      res.nDecimalsIfDouble=determineNumberOfDecimals(it->second);
    }
    it=vMap.find(GL_qLink);
    if(it!=itE){
      res.quickJumpLink=it->second;
    }
    res.displayWidth=GL_defaultDisplayWidthForMatrix;
    it=vMap.find(GL_displayWidth);
    if(it!=itE){
      res.displayWidth=BF::stringToInteger(it->second);
      if(res.displayWidth<3){
        res.displayWidth=GL_defaultDisplayWidthForMatrix;
      }
    }
    if((goodCoordinate(res.coordinates.row,GL_maxMatrixCoordinate))&&(goodCoordinate(res.coordinates.col,GL_maxMatrixCoordinate))){
      res.matrixCellIndicator=1;
    }
    if((goodCoordinate(res.nRows,GL_maxMatrixCoordinate+1))&&(goodCoordinate(res.nCols,GL_maxMatrixCoordinate+1))){
      res.matrixTemplateIndicator=1;
    }
    return res;
  }
  VariableInForm::VariableInForm(){
    dblValue=0.0;
    lValue=0;
    type="string";
    nDecimals=-1;
  }
  VariableInForm formVariableFromString(const std::string& raw,const std::string& _nm){
    VariableInForm res;
    std::map<std::string,std::string> vMap=SF::stringToMap(raw,"[k]","[/k]","[v]","[/v]");
    std::map<std::string,std::string>::iterator it,itE;
    long parametersFound=0;
    itE=vMap.end();
    res.type="string";
    it=vMap.find(GL_vType);
    if(it!=itE){
      ++parametersFound;
      res.type=determineVariableType(it->second);
    }
    it=vMap.find(GL_descriptionLabel);
    if(it!=itE){
      ++parametersFound;
      res.description=it->second;
    }
    it=vMap.find(GL_displayWidth);
    res.displayWidth=GL_defaultDisplayWidth;
    if(it!=itE){
      res.displayWidth=BF::stringToInteger(it->second);
      if(res.displayWidth<3){
        res.displayWidth=GL_defaultDisplayWidth;
      }
    }
    res.matrixData=mInfoFromMap(vMap,_nm);
    it=vMap.find(GL_numDecimals);
    if(it!=itE){
      res.nDecimals=determineNumberOfDecimals(it->second);
    }
    it=vMap.find(GL_qLink);
    if(it!=itE){
      res.quickJumpLink=it->second;
    }
    it=vMap.find(GL_codeInsertionLabel);
    if(it!=itE){
      res.codeInsertionKey=it->second;
    }
    it=vMap.find(GL_vValue);
    if(it!=itE){
      ++parametersFound;
      res.strValue=it->second;
      if(res.type=="double"){
        res.dblValue=BF::stringToDouble(it->second);
      }
      if(res.type=="long"){
        res.lValue=BF::stringToInteger(it->second);
      }
    }
    res.varName=_nm;
    if(parametersFound<1){
      res.strValue=raw;
    }
    return res;
  }
  void updateVariable(VariableInForm& dest, const VariableInForm& src){
    dest.strValue=src.strValue;
    if(dest.type=="double"){
      dest.dblValue=BF::stringToDouble( src.strValue );
    }
    if(dest.type=="long"){
      dest.lValue=BF::stringToInteger( src.strValue );
    }
  }
  CodeExecutionParameters::CodeExecutionParameters(){
    sources.resize(1);languages.resize(1);cFlags.resize(1);
    includes.resize(1);dbIncludes.resize(1);inputTestCases.resize(1);
    inputTestCases[0].resize(1);
    (inputTestCases[0])[0]="";
    cFlags[0]="-std=c++11";
  }
  FormComponents::FormComponents(){
    maxNumberOfFiles=100;
  }
  void FormComponents::update(){
    userFolderPath=DD::GL_DBS.getFileFormsFolder()+"/";
    userFolderPath+=GL_openingSeparatorInFileName+userName+GL_closingSeparatorInFileName;
    userFolderPath+="/";
    submissionFolderPath=userFolderPath;
    submissionFolderPath+=GL_openingSeparatorInFileName+templateName+GL_closingSeparatorInFileName;
    submissionFolderPath+="/";
    regFilePath=submissionFolderPath;
    regFilePath+=GL_openingSeparatorInFileName+fileName+GL_closingSeparatorInFileName;
    regFilePath+=".txt";
  }
  std::string FormComponents::get_templateName() const{return templateName;}
  std::string FormComponents::get_userName() const{return userName;}
  std::string FormComponents::get_fileName() const{return fileName;}
  std::string FormComponents::get_regFilePath() const{return regFilePath;}
  std::string FormComponents::get_userFolderPath() const{return userFolderPath;}
  std::string FormComponents::get_submissionFolderPath() const{return submissionFolderPath;}
  void FormComponents::set_templateName(const std::string& x){
    templateName=x; update();
  }
  void FormComponents::set_userName(const std::string& x){
    userName=x; update();
  }
  void FormComponents::set_fileName(const std::string& x){
    fileName=x; update();
  }
  void FormComponents::set_parameters(const std::string& x, const std::string& y, const std::string& z){
    templateName=x;userName=y;fileName=z; update();
  }
  std::string valueFromStringMap(const std::map<std::string,std::string>& m, const std::string& k){
    std::map<std::string,std::string>::const_iterator it=m.find(k);
    if(it!=m.end()){
      return it->second;
    }
    return "";
  }
  std::vector<std::string> getVariablesOrdering(const std::string &in){
    std::vector<std::string> res=SF::stringToVector(in,"[p]","[/p]");
    long sz=res.size();
    std::pair<std::string,int> allD; long pos;
    for(long i=0;i<sz;++i){
      pos=0;allD=SF::extract(res[i],pos,"[k]","[/k]");
      if(allD.second==1){
        res[i]=allD.first;
      }
    }
    return res;
  }
  void loadVariablesFromString( std::map<std::string,VariableInForm>& m, const std::string& in){
    VariableInForm fVar;
    std::map<std::string,std::string> fMap=SF::stringToMap(in,"[k]","[/k]","[v]","[/v]");
    std::map<std::string,std::string>::iterator it,itE;
    itE=fMap.end();
    it=fMap.begin();
    std::map<std::string, VariableInForm>::iterator itMM,itMME;
    while(it!=itE){
      fVar=formVariableFromString(it->second,it->first);
      itMM=m.find(it->first);
      if(itMM==m.end()){
        m[it->first]=fVar;
      }
      else{
        updateVariable(itMM->second,fVar);
      }
      ++it;
    }
  }
  std::string createStructure(const FormComponents& fC, const std::string& req){
    std::string res;
    res+="struct Q_"+req+GL_endStructureStrName+"{\n";
    res+="public:\n";
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    it=fC.calculatedVariables.begin();
    itE=fC.calculatedVariables.end();
    if(req=="input"){
      it=fC.fillOutVariables.begin();
      itE=fC.fillOutVariables.end();
    }
    std::string tp;
    std::string initVal;
    std::string quotMod;
    while(it!=itE){
      if((it->second).codeInsertionKey==""){
        tp=(it->second).type;
        if(tp=="string"){tp="std::string";}
        if(req=="input"){
          initVal="";
          quotMod="";
          if(tp=="std::string"){quotMod="\"";}
          if((it->second).strValue!=""){
            initVal="="+quotMod+(it->second).strValue+quotMod;
          }
        }
        res+="\t"+tp+" _"+BF::cleanAllSpaces((it->second).varName)+" "+initVal+";\n";
      }
      ++it;
    }
    res+="} "+GL_globalPrefix+req+";\n";
    return res;
  }
  std::string createPrintingFunction(const FormComponents& fC){
    std::string res;
    res+="std::string q_out_"+GL_endStructureStrName+"(const Q_output"+GL_endStructureStrName+"& x){\n";
    res+="\tstd::string fR;\n";
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    it=fC.calculatedVariables.begin();
    itE=fC.calculatedVariables.end();
    std::string vName;
    std::string conversionB;
    std::string conversionE;
    while(it!=itE){
      vName=BF::cleanAllSpaces((it->second).varName);
      res+="\tfR+=\"[p][k]"+vName+"[/k]\\n\";\n";
      res+="\tfR+=\"[v]\";\n";
      conversionB="";conversionE="";
      if(((it->second).type=="double")||((it->second).type=="long")) {
        conversionB="std::to_string(";
        conversionE=")";
      }
      res+="\tfR+="+conversionB+GL_globalPrefix+"output._"+vName+conversionE+";\n";
      res+="\tfR+=\"[/v]\\n[/p]\\n\";\n";
      ++it;
    }
    res+="\treturn fR;";
    res+="\n}\n";
    return res;
  }
  std::string createIOStructures(const FormComponents& fC){
    std::string ioStructures;
    ioStructures+=createStructure(fC,"input");
    ioStructures+=createStructure(fC,"output");
    ioStructures+=createPrintingFunction(fC);
    return ioStructures;
  }
  void prepareInputAndOutputInSource(FormComponents& fC){
    std::map<std::string,std::string> replMap;
    replMap["[#]IOStructures[/#]"]=createIOStructures(fC);
    replMap["[#]printOutput[/#]"]="std::cout<<q_out_"+GL_endStructureStrName+"("+GL_globalPrefix+"output)<<std::endl;";
    (fC.cep).sources[0]=MFRF::findAndReplace((fC.cep).sources[0],replMap);
  }
  void updateOneVariable(FormComponents& fC, const std::string& rawVariable){
    std::vector<std::string> commandParameters=SF::stringToVectorSimpleSeparator(rawVariable,";");
    if(commandParameters.size()<1){return;}
    VariableInForm vDiscovered;
    vDiscovered.varName=commandParameters[0];
    vDiscovered.type="double";
    if(commandParameters.size()>1){
      vDiscovered.type=MFRF::toLowerCase(commandParameters[1]);
    }
    vDiscovered.nDecimals=2;
    if((vDiscovered.type=="double")&&(commandParameters.size()>2)){
      vDiscovered.nDecimals=determineNumberOfDecimals(commandParameters[2]);
    }
    (fC.calculatedVariables)[vDiscovered.varName]=vDiscovered;
  }
  void updateCodeInsertions(FormComponents& fC){
    std::map<std::string,std::string>::iterator it,itE;
    std::map<std::string,VariableInForm>::const_iterator itV,itVE;
    itVE=(fC.fillOutVariables).end();
    it=(fC.mapCodeInsertions).begin();
    itE=(fC.mapCodeInsertions).end();
    while(it!=itE){
      itV=(fC.fillOutVariables).find(it->second);
      if(itV!=itVE){
        it->second=(itV->second).strValue;
      }
      ++it;
    }
  }
  std::string updateCodeInsertionsAndOutputVariables(FormComponents& fC,const std::string& in){
    std::pair<std::stack<std::string>,std::string> updatedSource
         = SF::stringToStackAndRemoveItems(in,"[var]","[/var]");
    if((updatedSource.first).size()<1){return in;}
    while(!(updatedSource.first).empty()){
      updateOneVariable(fC,BF::cleanAllSpaces((updatedSource.first).top()));
      (updatedSource.first).pop();
    }
    return MFRF::findAndReplace(updatedSource.second,fC.mapCodeInsertions);
  }
  std::string createCellVariableName(const std::string& base, const long& r, const long& c,const std::string &oT = GL_openTagForVNames, const std::string& cT = GL_closeTagForVNames){
    std::string res=base;
    res+=oT+BF::padded(r,GL_pad,"0")+cT;
    res+=oT+BF::padded(c,GL_pad,"0")+cT;
    return res;
  }
  std::string expandDisplMatrix(const std::map<std::string,std::string> &m){
    std::string res;
    std::map<std::string,std::string>::const_iterator it;
    it=m.find("dim");
    if(it==m.end()){return "";}
    std::string dimSt=it->second;
    long dim=BF::stringToInteger(dimSt);
    it=m.find("name");
    if(it==m.end()){return "";}
    std::string name=it->second;
    std::string oT=GL_openTagForVNames;
    std::string cT=GL_closeTagForVNames;
    it=m.find("openTag");
    if(it!=m.end()){oT=it->second;}
    it=m.find("closeTag");
    if(it!=m.end()){cT=it->second;}
    it=m.find("type");
    if(it==m.end()){return "";}
    if(it->second=="html"){
      res+="<tr>"+GL_fParameters.tdDarkCell+"</td>";
      for(long c=0;c<dim;++c){
        res+=GL_fParameters.tdDarkCell+std::to_string(c)+"</td>";
      }
      for(long r=0;r<dim;++r){
        res+="<tr>\n";
        res+=GL_fParameters.tdDarkCell+std::to_string(r)+"</td>";
        for(long c=0;c<dim;++c){
          res+="<td>[k]";
          res+=createCellVariableName(name,r,c,oT,cT);
          res+="[/k]</td>";
        }
        res+="</tr>\n";
      }
    }
    if(it->second=="cpp"){
      it=m.find("vType");
      if(it==m.end()){return "";}
      std::string vType=it->second;
      it=m.find("position");
      if(it==m.end()){return "";}
      std::string posCode=it->second;
      it=m.find("direction");
      std::string direction="output";
      if(it==m.end()){
        if((name.length()>0)&&((name[0]=='i')||(name[0]=='I'))){
          direction="input";
        }
      }
      else{
        direction=it->second;
      }
      std::string vn="A";
      it=m.find("varName");
      if(it!=m.end()){vn=it->second;}
      if(posCode=="before"){
        res+="std::vector<std::vector<"+vType+"> > "+vn+";\n";
        res+=vn+".resize(dim);\n";
        res+="for(long i=0;i<dim;++i){"+vn+"[i].resize(dim);"+"}";
        if(vType=="double"){
          res+="for(long r=0;r<dim;++r){\n";
          res+="  for(long c=0;c<dim;++c){\n";
          res+="    "+vn+"[r][c]=0.0;\n";
          res+="  }\n}\n";
        }
        if(direction=="input"){
          for(long r=0;r<dim;++r){
            for(long c=0;c<dim;++c){
              res+=vn+"["+std::to_string(r)+"]["+std::to_string(c)+"]=";
              res+=GL_globalPrefix+"input._"+createCellVariableName(name,r,c,oT,cT)+";\n";
            }
          }
        }
      }
      if(posCode=="after"){
        for(long r=0;r<dim;++r){
          for(long c=0;c<dim;++c){
            res+="[var]"+createCellVariableName(name,r,c,oT,cT)+";string[/var]\n";
          }
        }
        for(long r=0;r<dim;++r){
          for(long c=0;c<dim;++c){
            res+="[var]"+createCellVariableName(name,r,c,oT,cT)+";string[/var]\n";
          }
        }
        std::string precision="3";
        it=m.find("precision");
        if(it!=m.end()){precision=it->second;}
        for(long r=0;r<dim;++r){
          for(long c=0;c<dim;++c){
            res+=GL_globalPrefix+"output._"+createCellVariableName(name,r,c,oT,cT)+"=";
            res+="BF::double_to_string_withPrecision("+vn+"["+std::to_string(r)+"]["+std::to_string(c)+"]";
            res+=",precision);\n";
          }
        }
      }
    }
    return res;
  }
  std::string expandDisplMatrices(const std::string& in){
    std::string out;
    std::pair<std::string,int> allD; long pos; long posSave=0;
    pos=0;allD=SF::getEverythingBefore(in,pos,GL_displMatrixB);
    while(allD.second==1){
      out+=allD.first;
      allD=SF::getEverythingBefore(in,pos,GL_displMatrixE);
      posSave=pos;
      if(allD.second==1){
        out+=expandDisplMatrix(SF::stringToMap(allD.first,"[k]","[/k]","[v]","[/v]"));
      }
      allD=SF::getEverythingBefore(in,pos,GL_displMatrixB);
    }
    SF::copyTheRestOfTheStringAndGiveUp(in,posSave,out,1);
    return out;
  }
  void prepareSource(FormComponents& fC, const std::string& raw){
    updateCodeInsertions(fC);
    std::map<std::string,std::string> mMap=SF::stringToMap(raw,"[k]","[/k]","[v]","[/v]");
    std::map<std::string,std::string>::iterator it,itE;
    itE=mMap.end();
    it=mMap.find("language");
    if((it==itE)||(it->second!="cpp")){return;}
    it=mMap.find("source");
    if(it==itE){return;}
    (fC.cep).languages[0]="cpp";
    (fC.cep).sources[0]=updateCodeInsertionsAndOutputVariables(fC,
                                                               SF::updateRepetitiveText(it->second));
    std::set<std::string> inclSet;
    it=mMap.find("includes");
    if(it!=itE){
      inclSet=SF::stringToSet(it->second,"[p]","[/p]");
    }
    inclSet.insert("iostream");
    (fC.cep).includes[0]=SF::setToVector(inclSet);
    it=mMap.find("dbIncludes");
    if(it!=itE){
      (fC.cep).dbIncludes[0]=SF::stringToVector(it->second,"[p]","[/p]");
      long dbinsz=((fC.cep).dbIncludes[0]).size();
      for(long i=0;i<dbinsz;++i){
          ((fC.cep).dbIncludes[0])[i]=updateCodeInsertionsAndOutputVariables(fC,DCEI::lastCode(TMD::rawTextData(((fC.cep).dbIncludes[0])[i])));
      }
    }
    it=mMap.find("compilerFlags");
    if(it!=itE){
      (fC.cep).cFlags[0]=it->second;
    }
    prepareInputAndOutputInSource(fC);
  }
  std::string addMoreDataFromInclude(const PSDI::SessionData& _psd,const std::string& x){
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(x,pos,"[include]","[/include]");
    if(allD.second){
      std::vector<std::string> vFiles=SF::stringToVector(x,"[p]","[/p]");
      std::string res;
      long sz=vFiles.size();
      for(long i=0;i<sz;++i){
        res+=MPTI::getTextIfAllowed(_psd,_psd.my_un,vFiles[i]);
      }
      return res;
    }
    return "";
  }
  std::string createItemForCellVariable(const std::string& key, const std::string& val){
    return "\n[p]\n[k]"+key+"[/k]\n[v]"+val+"[/v]\n[/p]\n";
  }
  PairRC getCoordinatesFromName(const std::string& n){
    PairRC res;
    std::vector<std::string> v=SF::stringToVector(n,GL_openTagForVNames,GL_closeTagForVNames);
    if(v.size()>1){
      res.row=BF::stringToInteger(v[0]);
      res.col=BF::stringToInteger(v[1]);
    }
    return res;
  }
  std::string createCellVariable(const MatrixInfo& m, const long& r, const long& c){
    std::string res="\n[p]\n";
    res+="[k]"+createCellVariableName(m.matrixName,r,c)+"[/k]\n[v]";
    res+=createItemForCellVariable(GL_descriptionLabel,"");
    res+=createItemForCellVariable(GL_vType,m.type);
    res+=createItemForCellVariable(GL_rowPosition,std::to_string(r));
    res+=createItemForCellVariable(GL_colPosition,std::to_string(c));
    res+=createItemForCellVariable(GL_matrixName,m.matrixName);
    res+=createItemForCellVariable(GL_qLink,m.quickJumpLink);
    if(m.type=="double"){res+=createItemForCellVariable(GL_numDecimals,std::to_string(m.nDecimalsIfDouble));}
    res+=createItemForCellVariable(GL_displayWidth,std::to_string(m.displayWidth));
    res+="\n[/v]\n[/p]\n";
    return res;
  }
  std::string expandIfMatrix(FormComponents& fC, const std::string& in){
    std::map<std::string,std::string> mainMap=SF::stringToMap(in,"[k]","[/k]","[v]","[/v]");
    std::map<std::string,std::string>::const_iterator it=mainMap.find(GL_matrixTemplateLabel);
    if(it==mainMap.end()){return "\n[p]\n"+in+"\n[/p]\n";}
    MatrixInfo mTemplateData=mInfoFromMap(SF::stringToMap(it->second,"[k]","[/k]","[v]","[/v]"),GL_matrixTemplateLabel);
    if(mTemplateData.matrixTemplateIndicator==0){return "\n[p]\n"+in+"\n[/p]\n";}
    fC.mTemplates.insert(mTemplateData.matrixName);
    std::string out;
    for(long r=0;r<mTemplateData.nRows;++r){
      for(long c=0;c<mTemplateData.nCols;++c){
        out+=createCellVariable(mTemplateData,r,c);
      }
    }
    return out;
  }
  std::string preProcessCompactMatrixRequests(FormComponents& fC, const std::string& in){
    std::stack<std::string> vStack=SF::stringToStack(in,"[p]","[/p]");
    if(vStack.size()<1){return in;}
    std::string out;
    while(!vStack.empty()){
      out = expandIfMatrix(fC, vStack.top())+out;
      vStack.pop();
    }
    return out;
  }
  void prepareMatrixDisplaysAndCodeInsertions(FormComponents& fC){
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    std::map<std::string,MatrixForDisplay>::iterator itM;
    itE=(fC.fillOutVariables).end();
    it=(fC.fillOutVariables).begin();
    while(it!=itE){
      if((it->second).codeInsertionKey!=""){
        (fC.mapCodeInsertions)[(it->second).codeInsertionKey]=it->first;
      }
      MatrixInfo const & mFromVariable=(it->second).matrixData;
      if(mFromVariable.matrixCellIndicator){
        itM=(fC.matrices).find(mFromVariable.matrixName);
        MatrixForDisplay mToDisplay;
        mToDisplay.name=mFromVariable.matrixName;
        if( itM!=(fC.matrices).end() ){
          mToDisplay=itM->second;
        }
        else{
          if(fC.mTemplates.find(mFromVariable.matrixName)!=fC.mTemplates.end()){
            mToDisplay.matrixFromTemplateIndicator=1;
          }
        }
        (mToDisplay.cells)[mFromVariable.coordinates]=mFromVariable.varName;
        if(mFromVariable.coordinates.row+1>mToDisplay.numRows){
          mToDisplay.numRows=mFromVariable.coordinates.row+1;
        }
        if(mFromVariable.coordinates.col+1>mToDisplay.numCols){
          mToDisplay.numCols=mFromVariable.coordinates.col+1;
        }
        if(mFromVariable.quickJumpLink!=""){
          mToDisplay.quickJumpLink=mFromVariable.quickJumpLink;
        }
        (fC.matrices)[mFromVariable.matrixName]=mToDisplay;
      }
      ++it;
    }
  }
  void loadParameters(const PSDI::SessionData& _psd, FormComponents& fC, const std::string& x){
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(x,pos,"[parameters]","[/parameters]");
    if(allD.second){
      std::map<std::string,std::string> pMap=SF::stringToMap(allD.first,"[k]","[/k]","[v]","[/v]");
      std::map<std::string,std::string>::const_iterator it;
      it=pMap.find(GL_svBTitle);
      if(it!=pMap.end()){
        GL_fParameters.saveButtonTitle=it->second;
      }
      it=pMap.find(GL_calcBTitle);
      if(it!=pMap.end()){
        GL_fParameters.calculateButtonTitle=it->second;
      }
      it=pMap.find(GL_editBTitle);
      if(it!=pMap.end()){
        GL_fParameters.editButtonTitle=it->second;
      }
      return;
    }
    return;
  }
  void loadDefaultVariablesFromTemplate(const PSDI::SessionData& _psd, FormComponents& fC, const std::string& _rawTemplate){
    loadParameters(_psd,fC,_rawTemplate);
    std::string rawTemplate=addMoreDataFromInclude(_psd,_rawTemplate)+_rawTemplate;
    std::map<std::string,std::string> mMap=SF::stringToMap(rawTemplate,"[k]","[/k]","[v]","[/v]");
    std::map<std::string,std::string>::iterator it,itE;
    itE=mMap.end();
    it=mMap.find("maxNumberOfFiles");
    if(it!=itE){fC.maxNumberOfFiles=BF::stringToInteger(it->second);}
    it=mMap.find(GL_foSectionName);
    if(it!=itE){
      std::string fillOutSection=preProcessCompactMatrixRequests(fC,SF::updateRepetitiveText(it->second));
      loadVariablesFromString(fC.fillOutVariables,fillOutSection);
      fC.fillOutOrdering=getVariablesOrdering(fillOutSection);
      prepareMatrixDisplaysAndCodeInsertions(fC);
    }
    it=mMap.find(GL_calcSectionName);
    if(it!=itE){loadVariablesFromString(fC.calculatedVariables,it->second);}
    it=mMap.find(GL_progSectionName);
    if(it!=itE){(fC.cep).rawSource=it->second;}
    it=mMap.find("htmlSource");
    if(it!=itE){
      fC.htmlSrcDoc=it->second;
      fC.htmlSrcCont=MPTI::getTextIfAllowed(_psd,_psd.my_un,fC.htmlSrcDoc);
        fC.htmlSrcCont=SF::updateRepetitiveText(fC.htmlSrcCont);
    }
    it=mMap.find("latexSource");
    if(it!=itE){
      fC.latexSrcDoc=it->second;
      fC.latexSrcCont=MPTI::getTextIfAllowed(_psd,_psd.my_un,fC.latexSrcDoc);
        fC.latexSrcCont=SF::updateRepetitiveText(fC.latexSrcCont);
    }
  }
  std::string determineSpecialRequest(const PSDI::SessionData& _psd){
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=(_psd.respMap).end();
    it=(_psd.respMap).find(GL_nameForSpecialRequest);
    if(it==itE){return "no";}
    return it->second;
  }
  long recoverIndex(const std::string& src, const std::string& startToRemove){
    long sz1=src.size();
    long sz2=startToRemove.size();
    if(sz2>sz1){return -1;}
    long i=0;
    while(i<sz2){
      if(src[i]!=startToRemove[i]){
        return -1;
      }
      ++i;
    }
    std::string rem;
    while(i<sz1){
      rem+=src[i];
      ++i;
    }
    return BF::stringToInteger(rem)-GL_shift;
  }
  int updateVariablesFromMap(FormComponents& fC, const std::map<std::string,VariableInForm>& vPost){
    std::map<std::string,VariableInForm>::iterator iVOld,iVOldE;
    std::map<std::string,VariableInForm>::const_iterator iVPost,iVPostE;
    iVPostE=vPost.end();
    iVPost=vPost.begin();
    iVOldE=(fC.fillOutVariables).end();
    int updateMade=0;
    while(iVPost!=iVPostE){
      iVOld=(fC.fillOutVariables).find(iVPost->first);
      if(iVOld!=iVOldE){
        updateMade=1;
        updateVariable(iVOld->second,iVPost->second);
      }
      ++iVPost;
    }
    return updateMade;
  }
  int updateVariablesFromWebsitePostConvenientSubmission(const PSDI::SessionData& _psd, FormComponents& fC){
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=(_psd.respMap).end();
    it=(_psd.respMap).find(GL_respVariableConvForm+BF::padded(1,GL_pad,"0"));
    if(it==itE) return 0;
    if(IOF::legalFileName(it->second)){
      fC.set_fileName(it->second);
    }
    ++it;
    long i=-1;
    if(it!=itE){
      i=recoverIndex(it->first,GL_respVariableConvForm);
    }
    std::map<std::string,VariableInForm> vPost;
    VariableInForm w;
    long sz=(fC.fillOutOrdering).size();
    while((it!=itE)&&(i>-1)&&(i<sz)){
      w.varName=(fC).fillOutOrdering[i];
      w.strValue=it->second;
      vPost[w.varName]=w;
      ++it;
      if(it!=itE){
        i=recoverIndex(it->first,GL_respVariableConvForm);
      }
    }
    return updateVariablesFromMap(fC,vPost);
  }
  int updateVariablesFromWebsitePostSubmission(const PSDI::SessionData & _psd,FormComponents& fC){
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=(_psd.respMap).end();
    it=(_psd.respMap).find(GL_respVariableName);
    if(it==itE) return updateVariablesFromWebsitePostConvenientSubmission(_psd,fC);
    std::map<std::string,std::string> mainMap=SF::stringToMap(it->second,"[k]","[/k]","[v]","[/v]");
    itE=mainMap.end();
    it=mainMap.find("fileName");
    if(it==itE) return 0;
    if(IOF::legalFileName(it->second)){
      fC.set_fileName(it->second);
    }
    it=mainMap.find(GL_foSectionName);
    if(it==itE) return 0;
    std::map<std::string,VariableInForm> vPost;
    loadVariablesFromString(vPost,it->second);
    return updateVariablesFromMap(fC,vPost);
  }
  std::string prepareValue(const VariableInForm& v){
    if(v.type=="string") return v.strValue;
    if(v.type=="double") return BF::doubleToString(v.dblValue);
    return std::to_string(v.lValue);
  }
  std::string prepareDescription(const VariableInForm& v){
    if(v.description=="") return "";
    return "\nDescription:\n"+v.description+"\n";
  }
  std::string varFieldsToString(const VariableInForm& v){
    std::string t;
    t+=createItemForCellVariable(GL_vType,v.type);
    t+=createItemForCellVariable(GL_vValue,v.strValue);
    return t;
  }
  std::string variableToString(const VariableInForm& v, const std::string&requestType){
    std::string t;
    t+="\n***** FORM FIELD *****\n";
    t+="[p]\n\t\t[k]"+v.varName+"[/k]\n";
    t+="\t\t[v]";
    if(requestType=="inputForm"){
      t+=prepareValue(v);
    }
    else{
      t+=varFieldsToString(v);
    }
    t+="[/v]\n";
    if(requestType=="inputForm"){
      t+=prepareDescription(v);
      t+="\nType: "+v.type+"\n";
    }
    t+="[/p]\n";
    return t;
  }
  std::string variablesToString(const std::vector<std::string>& ord,const std::map<std::string,VariableInForm>& v, const std::string& requestType){
    std::string t;
    t+="\n";
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    itE=v.end();
    long i=0;
    long sz=ord.size();
    while(i<sz){
      it=v.find(ord[i]);
      if(it!=itE){
        t+=variableToString(it->second,requestType);
      }
      ++i;
    }
    return t;
  }
  int deleteFolderIfNoFormsInside(const std::string& fName){
    std::vector<std::string> filesInFolder= IOF::listFiles(fName) ;
    if(filesInFolder.size()<2){
      IOF::sys_deleteFolderAndSubfolders(fName);
    }
    return 1;
  }
  int createDeleteFormFile(const FormComponents& fC, const std::string& req,const std::string& content){
    IOF::sys_deleteFile(fC.get_regFilePath());
    if(req=="delete"){
      if(IOF::sys_folderExists(fC.get_userFolderPath())==0) return 1;
      if(IOF::sys_folderExists(fC.get_submissionFolderPath())==0){
        deleteFolderIfNoFormsInside(fC.get_userFolderPath());
        return 1;
      }
      deleteFolderIfNoFormsInside(fC.get_submissionFolderPath());
      deleteFolderIfNoFormsInside(fC.get_userFolderPath());
      return 1;
    }
    IOF::sys_createFolderIfDoesNotExist(fC.get_userFolderPath(),"readme.txt","Do not edit this folder");
    IOF::sys_createFolderIfDoesNotExist(fC.get_submissionFolderPath(),"readme.txt","Do not edit this folder");
    std::vector<std::string> filesInFolder=IOF::listFiles(fC.get_submissionFolderPath());
    if(filesInFolder.size()<fC.maxNumberOfFiles+1){
      IOF::toFile(fC.get_regFilePath(),content);
    }
    return 1;
  }
  void updateFiles(const FormComponents& fC){
    if(fC.specialRequest==GL_deleteReq){
      createDeleteFormFile(fC,"delete","");
      return;
    }
    createDeleteFormFile(fC,"create",variablesToString(fC.fillOutOrdering,fC.fillOutVariables,"fileContent"));
  }
  void executeSource(const PSDI::SessionData& _psd, FormComponents& fC){
    std::pair<std::vector<std::vector<std::string> >,int>
    aGOutput=DCEI::executePrograms(_psd, fC.cep.sources,fC.cep.languages,fC.cep.cFlags,
                                   fC.cep.includes,fC.cep.dbIncludes,fC.cep.inputTestCases,GL_maxOSizeDocker);
    if((aGOutput.second==0)||(aGOutput.first.size()!=1)||(((aGOutput.first)[0]).size()!=1)){
      return;
    }
    fC.cep.rawExecutionResult=((aGOutput.first)[0])[0];
    std::map<std::string,std::string> replMap;
    replMap[".tr,"]="<tr>";
    replMap["./tr,"]="</tr>";
    replMap[".td,"]="<td>";
    replMap[".td class=\"text-end\","]="<td class=\"text-end\">";
    replMap["./td,"]="</td>";
    replMap[".b,"]="<b>";
    replMap["./b,"]="</b>";
    replMap[".i,"]="<i>";
    replMap["./i,"]="</i>";
    fC.cep.rawExecutionResult=MFRF::findAndReplace(fC.cep.rawExecutionResult,replMap);
  }
  void updateMap(std::map<std::string,std::string>& m, const std::map<std::string,VariableInForm>& v){
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    itE=v.end();
    it=v.begin();
    std::string displVal;
    while(it!=itE){
      displVal=(it->second).strValue;
      if(((it->second).type=="double") && ((it->second).nDecimals>-1) ) {
        displVal=BF::forceDecimals(displVal,(it->second).nDecimals);
      }
      m["[k]"+it->first+"[/k]"]=displVal;
      ++it;
    }
  }
  std::string displayErrorCpp(const std::string& err){
    std::string res;
    res="<h1>ERROR IN THE PROGRAM</h1>\n";
    res+="<pre>"+SF::insertReasonableLineBreaks(err)+"</pre>";
    return res;
  }
  int allowedToMakePDFForForm(const PSDI::SessionData & _psd, const std::string& usernameWhoIsAllowedToMakePDF){
    if(_psd.isRoot=="yes"){return 1;}
    if(_psd.my_un=="visitor"){return 0;}
    if(GL_PDFFormOptions.pdfsAllowed.length()>0){
      char firstCh=GL_PDFFormOptions.pdfsAllowed[0];
      if((firstCh=='y')||(firstCh=='Y')||(firstCh=='1')){
        if((usernameWhoIsAllowedToMakePDF=="all")||(usernameWhoIsAllowedToMakePDF==_psd.my_un)){
          return 1;
        }
      }
    }
    return 0;
  }
  std::string pdfForForms(const PSDI::SessionData & _psd, const std::string & _code, const std::string & latexSrc, const std::string& usernameWhoIsAllowedToMakePDF){
    if(allowedToMakePDFForForm(_psd,usernameWhoIsAllowedToMakePDF)==0){
      return "";
    }
    std::string lStart;
    lStart=MWII::GL_WI.getDefaultWebText("formPreambleLatexTemplate");
    if(lStart=="formPreambleLatexTemplate"){
      lStart=MWII::GL_WI.getDefaultWebText("certPreambleLatexTemplate");
      if(lStart=="certPreambleLatexTemplate"){
        lStart=MWII::GL_WI.getDefaultWebText("examPreambleLatexTemplate");
        if(lStart=="examPreambleLatexTemplate"){
          lStart=MWII::GL_WI.getDefaultWebText("notesPreambleLatexTemplate");
        }
      }
    }
    std::string latexSource;
    latexSource+=lStart+"\n\n";
    latexSource+=latexSrc;
    std::string pdfName=MWII::GL_WI.getPublicPDFFormLoc()+"/aE"+_code;
    IOF::deleteOldFiles(MWII::GL_WI.getPublicPDFFormLoc(),"pdf",GL_PDFFormOptions.maxTimeToKeepPDF);
    LMF::createPdfFromTex(latexSource,pdfName,MWII::GL_WI.getPublicPDFFormLoc());
    pdfName+=".pdf";
    return pdfName;
  }
  std::vector<std::map<std::string,std::string> > pagesFromStrings(const std::vector<std::string>& individualPages){
    std::vector<std::map<std::string,std::string> > res;
    long sz=individualPages.size();
    if(sz<1){
      return res;
    }
    res.resize(sz);
    for(long i=0;i<sz;++i){
      res[i]=SF::stringToMap(individualPages[i],"[k]","[/k]","[v]","[/v]");
    }
    return res;
  }
  std::string removeUnwantedPages(const std::string& in){
    std::vector<std::map<std::string,std::string> > pMaps= pagesFromStrings(SF::stringToVector(in,"[p]","[/p]"));
    std::string res;
    long sz=pMaps.size();
    std::map<std::string,std::string>::const_iterator it;
    int printed=0;
    for(long i=0;i<sz;++i){
      it=(pMaps[i]).find("skipPage");
      if( (it!=(pMaps[i]).end()) && (it->second=="no") ){
        it=(pMaps[i]).find("pageContent");
        if(it!=(pMaps[i]).end()){
          if(printed){
            res+="\n\\pagebreak\n";
          }
          printed=1;
          res+=it->second;
        }
      }
    }
    return res;
  }
  std::string editLatexPages(const std::string& in){
    std::pair<std::string,int> allD;
    long pos;
    pos=0; allD=SF::extract(in,pos,"[pages]","[/pages]");
    if(allD.second==0){
      return in;
    }
    pos=0;allD=SF::extractAndReplace(in,pos,"[pages]","[/pages]",0,removeUnwantedPages(allD.first));
    if(allD.second==0){
      return in;
    }
    return allD.first;
  }
  void updateDisplays(const PSDI::SessionData & _psd, FormComponents& fC){
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(fC.cep.rawExecutionResult,pos,"_error*|_","_/error*|_");
    if(allD.second==1){
      if(allD.first=="yes"){
        pos=0;allD=SF::extract(fC.cep.rawExecutionResult,pos,"_eTxt*|_","_/eTxt*|_");
        if(allD.second==1){
          fC.htmlSrcCont=displayErrorCpp(allD.first);
        }
        else{
          fC.htmlSrcCont=displayErrorCpp("Error in c++ program.");
        }
      }
      else{
        fC.htmlSrcCont=displayErrorCpp("Error in c++ program.");
      }
    }
    else{
      loadVariablesFromString(fC.calculatedVariables,fC.cep.rawExecutionResult);
      std::map<std::string,std::string> replMap;
      updateMap(replMap,fC.fillOutVariables);
      updateMap(replMap,fC.calculatedVariables);
      if(fC.latexSrcCont!=""){
        fC.latexSrcCont=MFRF::findAndReplace(fC.latexSrcCont,replMap);
        fC.latexSrcCont=editLatexPages(fC.latexSrcCont);
        replMap[GL_urlForPDFPlacer]=pdfForForms(_psd,_psd.my_un+"q"+RNDF::genRandCode(7),fC.latexSrcCont,_psd.my_un);
      }
      fC.htmlSrcCont=MFRF::findAndReplace(expandDisplMatrices(fC.htmlSrcCont),replMap);
    }
  }
  FormComponents determineComponents(const PSDI::SessionData & _psd,
                                     const std::string& rawTemplate,
                                     const std::string& templateName,
                                     const std::string& _parameter01,
                                     const std::string& parameter02){
    FormComponents fRes;
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(rawTemplate,pos,"<h1>","</h1>");
    fRes.title="Form Input";
    if(allD.second){fRes.title=allD.first;}
    fRes.mainPageName=_psd.pageRequested;
    std::string fn=_parameter01;
    if(IOF::legalFileName(fn)==0){fn="DefaultFile";}
    std::string uName=_psd.my_un;
    if((parameter02!="na")&&(parameter02!="")){
      if(_psd.isRoot=="yes"){
        if(IOF::legalFileName(parameter02)){
          uName=parameter02;
        }
      }
    }
    fRes.set_parameters(templateName,uName,fn);
    fRes.specialRequest=determineSpecialRequest(_psd);
    loadDefaultVariablesFromTemplate(_psd,fRes,rawTemplate);
    loadVariablesFromString(fRes.fillOutVariables,IOF::fileToString(fRes.get_regFilePath()));
    int updateMade=updateVariablesFromWebsitePostSubmission(_psd,fRes);
    if((updateMade)||(fRes.specialRequest==GL_deleteReq)){
      updateFiles(fRes);
    }
    if(fRes.specialRequest==GL_displayReq){
      fRes.cep.rawSource=expandDisplMatrices(fRes.cep.rawSource);
      prepareSource(fRes,fRes.cep.rawSource);
      executeSource(_psd,fRes);
      updateDisplays(_psd,fRes);
    }
    return fRes;
  }
  std::string debuggingPrint(const VariableInForm& vif){
    std::string res;
    res+="\t\tvarName=\t"+vif.varName+"\n";
    res+="\t\tdescription=\t"+vif.description+"\n";
    res+="\t\ttype=\t"+vif.type+"\n";
    res+="\t\tstrValue=\t"+vif.strValue+"\n";
    res+="\t\tdblValue=\t"+std::to_string(vif.dblValue)+"\n";
    res+="\t\tlValue=\t"+std::to_string(vif.lValue)+"\n";
    return res;
  }
  std::string debuggingPrint(const std::map<std::string,VariableInForm>& m){
    std::string res;
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    itE=m.end();
    it=m.begin();
    while(it!=itE){
      res+="<pre>"+it->first+"->\n";
      res+=debuggingPrint(it->second);
      res+="</pre>\n";
      ++it;
    }
    return res;
  }
  std::string debuggingPrint(const FormComponents& fC){
    std::string res;
    res+="<pre>mainPageName: "+fC.mainPageName+"</pre>\n";
    res+="<pre>templateName: "+fC.get_templateName()+"</pre>\n";
    res+="<pre>userName: "+fC.get_userName()+"</pre>\n";
    res+="<pre>fileName: "+fC.get_fileName()+"</pre>\n";
    res+="<pre>regFilePath: "+fC.get_regFilePath()+"</pre>\n";
    res+="<pre>userFolderPath: "+fC.get_userFolderPath()+"</pre>\n";
    res+="<pre>submissionFolderPath: "+fC.get_submissionFolderPath()+"</pre>\n";
    res+="<pre>maxNumberOfFiles: "+std::to_string(fC.maxNumberOfFiles)+"</pre>\n";
    res+="<pre>***fillOutVariables***</pre>\n";
    res+=debuggingPrint(fC.fillOutVariables);
    res+=debuggingPrint(fC.calculatedVariables);
    if(((fC.cep).sources).size()>0){
    res+="<pre>***SOURCE CODE***</pre>\n";
      res+="<pre>"+(fC.cep).sources[0]+"</pre>\n";
      res+="<pre>***EXECUTION RESULT***</pre>\n";
      res+="<pre>"+(fC.cep).rawExecutionResult+"</pre>\n";
    }
    if(fC.htmlSrcCont!=""){
      res+="<pre>***HTML SOURCE***</pre>\n";
      res+="<pre>"+fC.htmlSrcCont+"</pre>\n";
    }
    if(fC.latexSrcCont!=""){
      res+="<pre>***LATEX SOURCE***</pre>\n";
      res+="<pre>"+fC.latexSrcCont+"</pre>\n";
    }
    return res;
  }
  std::string preFilledText(const FormComponents& fC){
    std::string t;
    t+="[p][k]fileName[/k][v]"+fC.get_fileName()+"[/v][/p]\n";
    t+="[p][k]"+GL_foSectionName+"[/k][v]"+variablesToString(fC.fillOutOrdering,fC.fillOutVariables,"inputForm")+"[/v][/p]\n";
    return t;
  }
  std::string webParFromMap(const PSDI::SessionData& _psd,const std::string &p){
    std::map<std::string,std::string>::const_iterator it=(_psd.respMap).find(p);
    if(it!=(_psd.respMap).end()){
      return it->second;
    }
    return "";
  }
  std::string webParameter(const PSDI::SessionData& _psd,const std::string &p,const std::string& _wP=""){
    std::string wP=_wP;
    if(wP==""){wP=webParFromMap(_psd,p);}
    if(wP!=""){
      return "&"+p+"="+wP;
    }
    return "";
  }
  std::string webParameterPair(const PSDI::SessionData& _psd,const std::string &k, const std::string& v, const std::string& watchFor, const std::string& overWrite){
    std::string resK=webParFromMap(_psd,k);
    std::string resV=webParFromMap(_psd,v);
    if((watchFor!="")&&(resK==watchFor)){
      resV=overWrite;
    }
    return webParameter(_psd,k,resK)+webParameter(_psd,v,resV);
  }
  std::string webParameters(const PSDI::SessionData& _psd,const std::string& watchFor="", const std::string& overWrite=""){
    std::string res;
    std::vector<std::string> kV, vV;
    long sz=3; kV.resize(sz); vV.resize(sz);
    kV[0]="ev0"; vV[0]="el0"; kV[1]="ev1"; vV[1]="el1"; kV[2]="ev2"; vV[2]="el2";
    for(long i=0;i<sz;++i){
      res+=webParameterPair(_psd,kV[i],vV[i],watchFor,overWrite);
    }
    return res;
  }
  std::string webParametersRobust(const PSDI::SessionData& _psd, const FormComponents& fC){
    std::string watchFor="fl01";
    std::string fn=fC.get_fileName();
    if(fn==""){watchFor="";}
    std::string attempt1=webParameters(_psd,watchFor,fn);
    if(attempt1!="") return attempt1;
    std::string attempt2="&ev0=fTmpl&el0="+fC.get_templateName();
    attempt2+="&ev1=fl01&el1="+fn;
    return attempt2;
  }
  std::string formForEditingAdvanced(const PSDI::SessionData & _psd, const FormComponents& fC){
    CCFI::ArgsPowerModifyForm apmf;
    apmf.linkForm="index.cgi?page="+fC.mainPageName+webParametersRobust(_psd,fC);
    apmf.editMessage=MWII::GL_WI.getDefaultWebText("Fill out the form by editing the command below");
    apmf.fieldName=GL_respVariableName;
    apmf.textToModify=preFilledText(fC);
    return CCFI::createSimpleForm(apmf);
  }
  void createQuestionForForm(QuestionCollector& qC,const VariableInForm& v,const long &i){
    (qC.v[i]).quickJumpLink=v.quickJumpLink;
    (qC.v[i]).assignedLink=BF::padded(i+GL_shift,GL_pad,"0");
    (qC.assignedLinkTranslator)[v.varName]=i;
    (qC.v[i]).textWidth=v.displayWidth;
    if(v.matrixData.matrixCellIndicator){
      (qC.v[i]).parentMatrix=v.matrixData.matrixName;
    }
    (qC.v[i]).questionDisplayForMatrix=v.description;
    if(v.codeInsertionKey!=""){
      (qC.v[i]).textAreaIndicator="yes";
    }
    if(v.description.length()>2){
      (qC.v[i]).questionDisplay=v.description;
    }
    else{
      (qC.v[i]).questionDisplay+=v.varName+" ("+v.type;
      if(v.type=="double"){
        if(v.nDecimals>-1){
          (qC.v[i]).questionDisplay+=" with "+std::to_string(v.nDecimals)+" decimal places";
        }
      }
      (qC.v[i]).questionDisplay+=")<br>"+v.description;
    }
    (qC.v[i]).preFilledText=prepareValue(v);
  }
  QuestionCollector createQuestionsCollector(const PSDI::SessionData& _psd, const FormComponents& fC){
    QuestionCollector res;
    std::map<std::string,VariableInForm>::const_iterator it,itE;
    itE=(fC.fillOutVariables).end();
    long i=0;
    long sz=(fC.fillOutOrdering).size();
    (res.v).resize(sz);
    while(i<sz){
      it=(fC.fillOutVariables).find((fC.fillOutOrdering)[i]);
      if(it!=itE){
        createQuestionForForm(res,it->second,i);
      }
      ++i;
    }
    return res;
  }
  std::string addLinkBar(const std::string&in, const std::string& lB){
    std::string res="<div class=\"row\">\n";
    res+="<div class=\"col-md-3\">\n";
    res+="<table class=\"table table-striped\">";
    res+="<tbody>\n";
    res+=lB;
    res+="</tbody>\n</table>\n</div>\n<div class=\"col-md-9\">\n";
    res+=in;
    res+="</div></div>\n";
    return res;
  }
  std::string textInputField(const std::string& lName, const std::string& preFilled, const std::string& width, const std::string& tAreaInd){
    std::string res;
    if(tAreaInd=="yes"){
      res+=GL_fParameters.openingAreaInput1+lName+GL_fParameters.openingAreaInput2;
      res+=preFilled+GL_fParameters.closingArea;
    }
    else{
      res+=GL_fParameters.openingTextInput1+lName+GL_fParameters.openingTextInput2;
      res+=preFilled+GL_fParameters.closingText1+width+GL_fParameters.closingText2;
    }
    return res;
  }
  void updateQuickJumpLinks(std::string& fR, std::string& linkBar, long& counter, const std::string& plCode, const std::string& assignedLink, const std::string& quickJLink){
    ++counter;
    linkBar+="<tr><td>";
    linkBar+=std::to_string(counter);
    linkBar+="</td><td><a href=\"#"+plCode+assignedLink+"\">"+quickJLink+"</a></li>";
    linkBar+="</td></tr>\n";
    fR+="\n<h2 id=\""+plCode+assignedLink+"\">";
    fR+=std::to_string(counter)+". "+quickJLink;
    if(counter>1){
      fR+=" <a href=\"#"+plCode+"top\">(Back to top)</a>\n";
    }
    fR+="</h2>\n";
  }
  void prepareMatrixOfInputs(std::string& fR, std::string& linkBar, long& counter, const std::string& plCode, const MatrixForDisplay& mFD, const QuestionCollector& qC){
    PairRC p;
    std::map<PairRC,std::string>::const_iterator itCells;
    std::map<std::string,long>::const_iterator itI;
    long i;
    if(mFD.quickJumpLink!=""){
      updateQuickJumpLinks(fR,linkBar,counter,plCode,mFD.name+std::to_string(counter+1),mFD.quickJumpLink);
    }
    fR+=GL_fParameters.tableBegin;
    std::string tdTag="<td>";
    if(mFD.matrixFromTemplateIndicator){
      tdTag="<td align=\"center\">";
    }
    if(mFD.matrixFromTemplateIndicator){
      fR+="\n<tr>";
      fR+=GL_fParameters.tdDarkCell+" </td>";
      for(long c=0;c<mFD.numCols;++c){
        fR+=GL_fParameters.tdDarkCell+std::to_string(c)+" </td>";
      }
      fR+="\n</tr>\n";
    }
    for(long r=0;r<mFD.numRows;++r){
      fR+="\n<tr>";
      if(mFD.matrixFromTemplateIndicator){
        fR+=GL_fParameters.tdDarkCell+std::to_string(r)+" </td>";
      }
      for(long c=0;c<mFD.numCols;++c){
        fR+=tdTag;
        p.row=r;p.col=c;
        itCells=(mFD.cells).find(p);
        if(itCells!=(mFD.cells).end()){
          itI=(qC.assignedLinkTranslator).find(itCells->second);
          if(itI!=(qC.assignedLinkTranslator).end()){
            i=itI->second;
            fR+=(qC.v)[i].questionDisplayForMatrix;
            fR+=textInputField(GL_respVariableConvForm+(qC.v)[i].assignedLink,(qC.v)[i].preFilledText,std::to_string((qC.v)[i].textWidth),"no");
          }
        }
        fR+="</td>";
      }
      fR+="</tr>";
    }
    fR+=GL_fParameters.tableEnd;
  }
  void prepareMatrixOfInputs(std::string& fR, std::string& linkBar, long& counter, const std::string& plCode, std::map<std::string,MatrixForDisplay>& mFD, const QuestionCollector& qC,const long& i){
    std::map<std::string,MatrixForDisplay>::const_iterator it=mFD.find((qC.v)[i].parentMatrix);
    if(it==mFD.end()){return;}
    prepareMatrixOfInputs(fR, linkBar, counter, plCode, it->second,qC);
    mFD.erase((qC.v)[i].parentMatrix);
  }
  std::string createConvenientForm(std::map<std::string,MatrixForDisplay>& mFD, const CCFI::ArgsPowerModifyForm & apmf,const QuestionCollector& qC,const std::string& fName, const std::string& title){
    std::string linkBar;
    std::string plCode="pc"+RNDF::genRandCode(7)+"d";
    std::string fR;
    fR+="_title*_"+title+"_/title*_\n";
    fR+="<h1 id=\""+plCode+"top"+"\">"+title+"</h1>\n";
    fR+=GL_fParameters.openingTagForm1;
    fR+=apmf.linkForm;
    fR+=GL_fParameters.openingTagForm2;
    fR+="\\begin{box}\n"+GL_fParameters.saveAsMessage;
    fR+=textInputField(GL_respVariableConvForm+BF::padded(1,GL_pad,"0"),fName,"50","no");
    fR+="\\end{box}\n";
    fR+="<p></p>";
    long sz=(qC.v).size();
    long counter=0;
    for(long i=0;i<sz;++i){
      if((qC.v)[i].parentMatrix!=""){
        prepareMatrixOfInputs(fR,linkBar,counter,plCode,mFD, qC,i);
      }
      else{
        if((qC.v)[i].quickJumpLink!=""){
          updateQuickJumpLinks(fR,linkBar,counter,plCode, (qC.v)[i].assignedLink, (qC.v)[i].quickJumpLink);
        }
        fR+="\\begin{box}\n "+(qC.v)[i].questionDisplay;
        fR+=textInputField(GL_respVariableConvForm+(qC.v)[i].assignedLink,(qC.v)[i].preFilledText,std::to_string((qC.v)[i].textWidth),(qC.v)[i].textAreaIndicator);
        fR+="\\end{box}\n";
        fR+="<p></p>";
      }
    }
    fR+=GL_fParameters.closingTagForm1+GL_fParameters.saveButtonTitle+GL_fParameters.closingTagForm2+"\n";
    if(linkBar!=""){
      fR=addLinkBar(fR,linkBar);
    }
    return fR;
  }
  std::string formForEditingConvenient(const PSDI::SessionData & _psd, const FormComponents& fC){
    CCFI::ArgsPowerModifyForm apmf;
    apmf.linkForm="index.cgi?page="+fC.mainPageName;
    apmf.linkForm+=webParametersRobust(_psd,fC);
    apmf.editMessage=MWII::GL_WI.getDefaultWebText("Fill out the form by editing the command below");
    apmf.fieldName=GL_respVariableName;
    std::map<std::string,MatrixForDisplay> mFD=fC.matrices;
    QuestionCollector qC=createQuestionsCollector(_psd,fC);
    return createConvenientForm(mFD,apmf,qC,fC.get_fileName(),fC.title);
  }
  std::string messageDeleted(const PSDI::SessionData & _psd,
                             const std::string& rawTemplate,
                             const std::string& templateName,
                             const std::string& parameter01,
                             const std::string& parameter02){
    std::string res;
    res+="<p></p><h2>Deleted</h2>\n";
    return res;
  }
  std::string topLink(const PSDI::SessionData & _psd, const FormComponents& fC){
    std::string url="index.cgi?page="+_psd.pageRequested;
    url+=webParametersRobust(_psd,fC);
    std::string label=GL_fParameters.editButtonTitle;
    if(fC.specialRequest!=GL_displayReq){
      url+="&"+GL_nameForSpecialRequest+"="+GL_displayReq;
      label=GL_fParameters.calculateButtonTitle;
    }
    return "<p>"+HSF::createButtonLink(url,label,"standardSize")+"</p>\n";
  }
  std::string formForViewing(const PSDI::SessionData& _psd, const FormComponents& fC){
    if(fC.htmlSrcCont==""){return debuggingPrint(fC);}
    return fC.htmlSrcCont;
  }
  std::string userDisplay(const PSDI::SessionData & _psd,
                          const std::string& rawTemplate,
                          const std::string& templateName,
                          const std::string& parameter01,
                          const std::string& parameter02,
                          const std::string& requestType){
    if(IOF::legalFileName(templateName)==0){return "";}
    FormComponents fC=determineComponents(_psd,rawTemplate,templateName,parameter01,parameter02);
    if(fC.specialRequest==GL_deleteReq){
      return messageDeleted(_psd,rawTemplate,templateName,parameter01,parameter02);
    }
    std::string res;
    res+=topLink(_psd,fC);
    if(fC.specialRequest==GL_displayReq){
      res+=formForViewing(_psd,fC);
      return res;
    }
    if(requestType=="advanced"){
      res+=formForEditingAdvanced(_psd,fC);
    }
    else{
      res+=formForEditingConvenient(_psd,fC);
    }
    return res;
  }
  std::string formDisplay(const PSDI::SessionData & _psd,
                              const std::string& rawTemplate,
                              const std::string& requestType,
                              const std::string& templateName,
                              const std::string& parameter01,
                              const std::string& parameter02){
    if(_psd.my_un=="visitor"){return GL_mustBeLoggedIn;}
    std::string res;
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(rawTemplate,pos,"_textData!!_","_/textData!!_");
    std::string templ=rawTemplate;
    if(allD.second==1){
      templ=allD.first;
    }
    if(requestType=="userDisplay"){ return userDisplay(_psd,templ,templateName,parameter01,parameter02,"advanced");}
    if(requestType=="userConvenientDisplay"){ return userDisplay(_psd,templ,templateName,parameter01,parameter02,"convenient");}
    return "";
  }
  SSF::StatSeq<std::string> getNames(const std::vector<std::string> & v){
    SSF::StatSeq<std::string> res;
    long sz=v.size();
    std::string fName;
    long pos; std::pair<std::string, int> allD;
    for(long i=0;i<sz;++i){
      fName=IOF::justFileNameNoExtensionNoFolder(v[i]);
      pos=0;allD=SF::extract(fName,pos,GL_openingSeparatorInFileName,GL_closingSeparatorInFileName);
      if(allD.second){
        res.insert(allD.first);
      }
    }
    return res;
  }
  std::string createBackLink(const PSDI::SessionData& _psd, const std::string &uname,const std::string& tmpl=""){
    std::string link="index.cgi?page="+_psd.pageRequested;
    link+="&ev0=unm&el0="+uname;
    std::string label=uname;
    if(tmpl!=""){
      link+="&ev1=tmn&el1="+tmpl;
      label=tmpl;
    }
    return HSF::createButtonLink(link,label,"standardSize");
  }
  std::string createDeleteLink(const PSDI::SessionData& _psd, const std::string& label, const std::string & request, const std::string& uname, const std::string & tmpl){
    if((_psd.my_un!=uname)&&(_psd.isRoot!="yes")){return "";}
    std::string res;
    std::string link;
    if(request=="allForms"){
      link="index.cgi?page=mFileForm&ev0=fTmpl&el0="+tmpl;
      link+="&ev1=fl01&el1="+label;
      link+="&ev2=u02&el2="+uname;
      link+="&"+GL_nameForSpecialRequest+"="+GL_deleteReq;
    }
    res+="_hideReveal__revealTitle_"+MWII::GL_WI.getDefaultWebText("delete")+"_/revealTitle__hideTitle_";
    res+=MWII::GL_WI.getDefaultWebText("cancel")+"_/hideTitle_";
    res+="<a href=\""+link+"\">Delete</a>";
    res+="_/hideReveal_";
    return res;
  }
  std::string createLink(const PSDI::SessionData& _psd, const std::string& label, const std::string & request, const std::string& uname, const std::string & tmpl){
    std::string res;
    if(request=="allForms"){
      res+="<a href=\"index.cgi?page=mFileForm&ev0=fTmpl&el0="+tmpl;
      res+="&ev1=fl01&el1="+label;
      if(_psd.my_un!=uname){
        res+="&ev2=u02&el2="+uname;
      }
    }
    else{
      res+="<a href=\"index.cgi?page="+_psd.pageRequested;
      res+="&ev0=unm&el0=";
      if(request=="allUsers"){res+=label;}
      if(request=="allTemplates"){
        res+=uname;
        res+="&ev1=tmn&el1="+label;
      }
    }
    res+="\">"+label+"</a>";
    return res;
  }
  std::string finalTableDisplay(const PSDI::SessionData& _psd,
                                const long& maxNumDispl,
                                const SSF::StatSeq<std::string>& names,
                                const std::string& request,
                                const std::string& uname,
                                const std::string& tmpl){
    std::string res;
    res+="<p>";
    if((request=="allTemplates")||(request=="allForms")){
      res+=createBackLink(_psd,uname);
    }
    if(request=="allForms"){
      res+=createBackLink(_psd,uname,tmpl);
    }
    res+="</p>";
    std::string sc=MWII::GL_WI.getSortCriterion();
    long numTotal=names.size();
    long start=BF::stringToInteger(MWII::GL_WI.getStartOfList());
    if((start<0)||(start>numTotal-1)){start=0;}
    if((sc!="0")&&(sc!="1")){sc="0";}
    long numb=maxNumDispl;
    if(numb<0){numb=0;}
    long lsz=2;
    std::vector<std::string> topLine,mLine;
    topLine.resize(lsz);mLine.resize(lsz);
    std::stack<std::vector<std::string> > allLines;
    long end=start+numb;
    if((end<0)||(end>numTotal)){
      end=numTotal;
    }
    topLine[0]="Name";topLine[1]="Action";
    for(long i=start;i<end;++i){
      mLine[0]=createLink(_psd, names[i],request,uname,tmpl);
      if(request=="allForms"){
        mLine[1]=createDeleteLink(_psd,names[i],request,uname,tmpl);
      }
      allLines.push(mLine);
    }
    SF::flipTheStack(allLines);
    allLines.push(topLine);
    std::string pageCall="page="+_psd.pageRequested;
    pageCall+=webParameters(_psd);
    return res+BI::createScroller(start,end,numTotal,numb,pageCall,"5",sc)+HSF::tableFromStack(allLines,MWII::GL_WI.getTableOpenTag(),MWII::GL_WI.getTheadOpenTag());
  }
  std::string allFormsForUsers(const PSDI::SessionData& _psd,const long& maxNumDispl, const std::string& _un, const std::string& tmpl){
    std::string uname=_un;
    if(_psd.isRoot!="yes"){
      uname=_psd.my_un;
    }
    if(IOF::legalFileName(uname)==0){uname=_psd.my_un;}
    std::string folderName=DD::GL_DBS.getFileFormsFolder()+"/";
    folderName+=GL_openingSeparatorInFileName+uname+GL_closingSeparatorInFileName+"/";
    SSF::StatSeq<std::string> formsForUser;
    if(IOF::sys_folderExists(folderName)){
      folderName+=GL_openingSeparatorInFileName+tmpl+GL_closingSeparatorInFileName+"/";
      if(IOF::sys_folderExists(folderName)){
        formsForUser=getNames(IOF::listFiles(folderName));
      }
    }
    return finalTableDisplay(_psd,maxNumDispl,formsForUser,"allForms",uname,tmpl);
  }
  std::string allFormsForUsers(const PSDI::SessionData& _psd,const long& maxNumDispl, const std::string& _un){
    std::string uname=_un;
    if(_psd.isRoot!="yes"){
      uname=_psd.my_un;
    }
    if(IOF::legalFileName(uname)==0){uname=_psd.my_un;}
    std::string folderName=DD::GL_DBS.getFileFormsFolder()+"/";
    folderName+=GL_openingSeparatorInFileName+uname+GL_closingSeparatorInFileName+"/";
    SSF::StatSeq<std::string> templatesForUser;
    if(IOF::sys_folderExists(folderName)){
      templatesForUser=getNames(IOF::listFiles(folderName));
    }
    return finalTableDisplay(_psd,maxNumDispl,templatesForUser,"allTemplates",uname,"");
  }
  std::string allFormsForUsers(const PSDI::SessionData& _psd,const long& maxNumDispl){
    if(_psd.isRoot!="yes"){
      return allFormsForUsers(_psd,maxNumDispl,_psd.my_un);
    }
    SSF::StatSeq<std::string> userNameFolders=getNames(IOF::listFiles(DD::GL_DBS.getFileFormsFolder()+"/"));
    return finalTableDisplay(_psd,maxNumDispl,userNameFolders,"allUsers","","");
  }
  std::string listOfFormsDisplay(const PSDI::SessionData & _psd,
                                 const std::string& requestType,
                                 const std::string& userName,
                                 const std::string& templateName,
                                 const std::string& parameter02){
    if(_psd.my_un=="visitor"){return GL_mustBeLoggedIn;}
    std::string res;
    if(requestType=="userList"){
      long maxDisplayOnPage=BF::stringToInteger(parameter02);
      if(maxDisplayOnPage<3){maxDisplayOnPage=3;}
      if((userName.size()>2)&&(userName[0]=='_')&&(userName[1]=='v')&&(userName[2]=='r')){
        return allFormsForUsers(_psd,maxDisplayOnPage);
      }
      if((templateName.size()>2)&&(templateName[0]=='_')&&(templateName[1]=='v')&&(templateName[2]=='r')){
        return allFormsForUsers(_psd,maxDisplayOnPage,userName);
      }
      if(IOF::legalFileName(templateName)==0){
        return allFormsForUsers(_psd,maxDisplayOnPage,userName);
      }
      return allFormsForUsers(_psd,maxDisplayOnPage,userName,templateName);
    }
    res+="<pre>"+requestType+"</pre>";
    res+="<pre>"+userName+"</pre>";
    res+="<pre>"+templateName+"</pre>";
    res+="<pre>"+parameter02+"</pre>";
    return res;
  }
}
#endif
