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


#ifndef _INCL_WI_FORMHANDLING_CPP
#define _INCL_WI_FORMHANDLING_CPP
namespace FHI{

  std::string FormField::getType() const{
    return type;
  }
  std::string FormField::getAnswerLocation() const{
    return answerKeep;
  }
  FormField::FormField(){
    type="notSet";//form is still not valid
  }
  void FormField::convertToTextArea(const std::string & _nd,const std::string & _tb, const std::string & _pf, const long & _r, const long & _c){
    type="textArea";
    nameField=_nd;
    textBefore=_tb;
    textAfter="";
    preFilledValue=_pf;
    setDimensions(_r,_c);
  }
  void FormField::convertToRadioButtons(const std::string & _nd,const std::string & _tb, const std::string & _ta, const std::string & _aC, const std::string & _ps){
    type="radioButtons";
    nameField=_nd;
    textBefore=_tb;textAfter=_ta;
    preSelection=_ps;
    std::vector<std::string> allChV=SF::stringToVector(_aC,"_rb*_","_/rb*_");
    std::vector<std::string> smV;
    long sz=allChV.size();
    allChoicesValues.resize(sz);allChoicesDescriptions.resize(sz);
    for(long i=0;i<sz;++i){
      smV=SF::stringToVector(allChV[i],"_vl*_", "_/vl*_");
      if(smV.size()>1){
        allChoicesValues[i]=smV[0];allChoicesDescriptions[i]=smV[1];
      }
      else{
        allChoicesValues[i]="";allChoicesDescriptions[i]="";
      }
    }
  }
  void FormField::convertToTextInput(const std::string & _nd,const std::string & _tb, const std::string & _pf, const long & _c){
    type="textInput";
    nameField=_nd;
    textBefore=_tb;textAfter="";
    preFilledValue=_pf;
    setDimensions(_c,_c);
  }
  void FormField::convertToFileInput(const std::string & _nd,const std::string & _tb){
    type="fileInput";
    nameField=_nd;
    textBefore=_tb;textAfter="";
  }
  void FormField::convertToAntiSpamChallenge(const std::string & _nd ,const std::string & _tb, const long & _c, const long & _clen){
    type="antiSpamChallenge";
    nameField=_nd;
    textBefore=_tb;textAfter="";
    availableDrawings="123456";
    codeLength=_clen;
    if((codeLength<1)||(codeLength>10)){codeLength=8;}
    std::pair<std::string,std::string> qAndA=G::prepareChallengeCode(availableDrawings,codeLength);
    challengeCode=qAndA.first;
    answerKeep=qAndA.second;
    IOF::deleteOldFiles(DD::GL_DBS.getChallengeImStorage(),"gif",300);
    IOF::deleteOldFiles(DD::GL_DBS.getChallengeAnswStorage(),"dat",1000);
    fileAnswerKeep=DD::GL_DBS.getChallengeAnswStorage()+"/"+"a"+answerKeep+".dat";
    fileQuestionImg=DD::GL_DBS.getChallengeImStorage()+"/"+"q"+answerKeep;
    G::code2Image(challengeCode,fileQuestionImg);
    IOF::toFile(fileAnswerKeep,challengeCode);
    setDimensions(_c,_c);
  }

  void FormField::setTextBefore(const std::string & _tb){
    textBefore=_tb;
  }
  void FormField::setTextAfter(const std::string & _ta){
    textAfter=_ta;
  }
  void FormField::setDimensions(const long & _r, const long & _c){ numR=_r;numC=_c;}
  std::string FormField::displayTextAreaField() const{
    std::string fR;

    fR+= BI::textAreaField(nameField,preFilledValue,numR,numC);

    return fR;
  }

  std::string FormField::displayTextInputField() const{
    std::string fR;
    fR+=BI::textInputField(nameField, preFilledValue, numC);
    return fR;
  }
  std::string FormField::displayRadioButtonsField() const{
    std::string fR;
    fR+=BI::radioButtonsField(nameField, allChoicesValues, allChoicesDescriptions,preSelection);
    return fR;
  }
  std::string FormField::displayFileInputField() const{
    std::string fR;


    fR+="<input type = \"file\" name = \"";
    fR+=nameField;
    fR+="\" />\n";


    return fR;
  }


  std::string FormField::displayField(const std::string & _inlineInd) const{
    std::string fR="";
    if(_inlineInd!="inline"){ fR+="<div class=\"messageBox\"> <P>";}

    if(type=="antiSpamChallenge"){
      fR+="<br>\n";
      fR+="<IMG  width=\"400\"  height=\"200\"  src=\"";
      fR+=MWII::GL_WI.getWSURL()+"/"+DD::GL_DBS.getChallengeImStorage()+"/q"+answerKeep+".gif";
      fR+= "\"";
      fR+=" alt=\"Picture\">\n<br> ";
    }
    fR+=textBefore;
    if(_inlineInd!="inline"){fR+="</P><P>";}
    if(type=="notSet"){
      fR+= "cannotDisplay";
    }
    if(type=="textArea"){
      fR+= displayTextAreaField();
    }
    if(type=="textInput"){
      fR+= displayTextInputField();
    }
    if(type=="radioButtons"){
      fR+= displayRadioButtonsField();
    }
    if(type=="fileInput"){
      fR+= displayFileInputField();
    }
    if(type=="antiSpamChallenge"){
      fR+=displayTextInputField();
    }
    fR+=textAfter;
    if(_inlineInd!="inline"){ fR+="</P></div><P></P>";}
    return fR;
  }


  InputForm::InputForm(const std::string & _aL, const std::string & _aNH,const std::string & _aNL, const std::string & _bL){
    clearForm();
    nextFieldNum=0;
    setMainVariables(_aL,_aNH, _aNL,_bL);
  }
  void InputForm::setMainVariables(const std::string & _aL, const std::string & _aNH,const std::string & _aNL, const std::string & _bL){
    actionLink=_aL;actionNameH=_aNH;actionNameL=_aNL;buttonLabel=_bL;
  }
  void InputForm::addChangeField(const FormField & _f, const long & _pos){
    if(_pos>-1){
      fields[_pos]=_f;
      if(_pos>=nextFieldNum){
        nextFieldNum=_pos+1;
      }
    }
    else{
      fields[nextFieldNum]=_f;
      ++nextFieldNum;
    }
    if(_f.getType()=="antiSpamChallenge"){
      antiSpamAnswerKeep=_f.getAnswerLocation();
    }
  }
  void InputForm::clearForm(){
    fields.clear();
    antiSpamAnswerKeep="notFound";
    redirectInfo="notFound";
  }
  std::string InputForm::displayAllFields(const std::string & _inlineInd) const{
    std::string fR;
    std::map<long, FormField>::const_iterator it,itE;
    itE=fields.end();
    it=fields.begin();
    while(it!=itE){
      fR+= (it->second).displayField(_inlineInd) ;
      ++it;
    }
    return fR;
  }
  void InputForm::setRedirectInfo(const std::string & _s){
    redirectInfo=_s;
  }
  std::string InputForm::displaySubmitButton(const std::string &aN, const std::string &bL) const{
    return BI::submButton(aN,bL);
  }
  std::string InputForm::displayForm(const std::string & submitPosition, const std::string & _bL, const std::string & _inlineInd) const{

    // submitPosition: 0 - button only at the bottom
    // submitPosition: 1 - button only on the top
    // submitPosition: 2 - button both on top and bottom
    std::string spacerB=" ",spacerE=" ";
    if(_inlineInd!="inline"){
      spacerB="<div>";spacerE="</div>";
    }
    std::string fR;
    std::string bL=buttonLabel;
    if(_bL!="notFound"){
      bL=_bL;
    }

    fR = "<div class=\"messageBox\"><FORM ACTION=\""+actionLink;
    if(antiSpamAnswerKeep!="notFound"){
      fR+="&asc="+antiSpamAnswerKeep;
    }
    if(redirectInfo!="notFound"){
      fR+="&rdo="+redirectInfo;
    }
    fR+="\" ENCTYPE=\"";
    fR+=enc_mfd;
    fR+="\" METHOD=\"POST\">";
    if(submitPosition!="0"){

      fR+=spacerB+displaySubmitButton(actionNameH,bL)+spacerE;

    }
    fR+= displayAllFields(_inlineInd);
    if(submitPosition!="1"){
      fR+=spacerB+displaySubmitButton(actionNameL,bL)+spacerE;
    }
    fR+= "</FORM></div>";
    return fR;
  }

}

#endif
