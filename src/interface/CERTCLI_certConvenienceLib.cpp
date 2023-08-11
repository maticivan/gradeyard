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


#ifndef _INCL_WI_CERT_CONVENIENCE_LIB_CPP
#define _INCL_WI_CERT_CONVENIENCE_LIB_CPP

namespace CERTCLI{
  void updateExamAttributesWithProblemOrCertInformation(SII::ExamAttributes& eA, TMD::MText& tmp, std::string pData, const long & i, const std::string& additionForCertificate){
    long pos,pos2; std::pair<std::string,int> allD,allD2;
    int exists;
    std::string tmpTextName;
    std::string td;
    pos=0;allD=SF::extract(pData,pos,"_p*|_","_/p*|_");
    (eA.problemLabels)[i]="notFound";
    (eA.possibleVersions)[i].resize(0);
    if(allD.second==1){
      pos2=0;allD2=SF::extract(allD.first,pos2,"_tx*|_","_/tx*|_");
      if(allD2.second==0){
        tmpTextName=allD.first;
        exists=tmp.setFromTextName(tmpTextName);
        if(exists==1){
          td=tmp.getTextData();
          pos=0;allD=SF::extract(td,pos,"_textData!!_","_/textData!!_");
          if(allD.second==1){
            pData=SF::findAndReplace(pData,"_p*|_"+tmpTextName+"_/p*|_",allD.first);
          }
        }
      }
      else{
        pData=SF::findAndReplace(pData,"_p*|_"+allD.first+"_/p*|_",allD.first);
      }
      (eA.possibleVersions)[i]=TWDVF::identifyVersions(pData);
      pos=0;allD=SF::extract(pData,pos,"_lb*|_","_/lb*|_");
      if(allD.second==1){
        (eA.problemLabels)[i]=allD.first;
      }
      else{
        (eA.problemLabels)[i]="Q"+additionForCertificate+BF::padded(i+1,10,"0");
        pData += "_lb*|_"+(eA.problemLabels)[i]+"_/lb*|_";
      }
      eA.textForRespReceiver+="_in*|_"+pData+"_/in*|_\n";
    }
  }
  std::pair<std::vector<std::string>,std::string> problemsAndCertificate(const std::vector<std::string>& rawDocs){
    std::pair<std::vector<std::string>,std::string> res;
    long oldSz=rawDocs.size();
    if(oldSz<1){return res;}
    std::stack<std::string> docsStack;
    std::pair<std::string,int> allD; long pos;
    for(long i=0;i<oldSz;++i){
      pos=0; allD=SF::extract(rawDocs[i],pos,"_pt*|_","_/pt*|_");
      if((allD.second==1)&&(allD.first=="certificate")){
        res.second=SF::findAndReplace(rawDocs[i],"_pt*|_certificate_/pt*|_","_pt*|_0_/pt*|_");
      }
      else{
        docsStack.push(rawDocs[i]);
      }
    }
    if(res.second==""){
      res.first=rawDocs;
    }
    else{
      res.first=SF::stackToVector(docsStack);
    }
    return res;
  }
  int doesLabelCorrespondToCertificate(const std::string& s){
    if((s.length()>1)&&(s[0]=='Q')&&(s[1]=='C')){return 1;}
    return 0;
  }
  std::string addUsernameAndFullName(const std::string& in, const std::string& username, const std::string& fullName){
    std::string out;
    std::vector<std::string> components=SF::stringToVector(in,"_n*_","_/n*_");
    long sz=components.size();
    if(sz>1){
      components[sz-1]+="\n_vVPair__vr_name_/vr__vl_"+fullName+"_/vl_ _/vVPair_\n";
      components[sz-1]+="_vVPair__vr_whoIsAllowedToMakePDF_/vr__vl_"+username+"_/vl__/vVPair_\n";
    }
    return SF::vectorToString(components,"_n*_","_/n*_\n");
  }
  std::map<std::string,std::string> getCertMapFromTemplate(const std::string& raw, const std::string& username, const std::string& fullName){
    std::map<std::string,std::string> rawMap, finalMap;
    rawMap=SF::stringToMap(raw,"_vr_","_/vr_","_vl_","_/vl_");
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=rawMap.end();
    it=rawMap.begin();
    while(it!=itE){
      finalMap[it->first]=addUsernameAndFullName(it->second,username,fullName);
      ++it;
    }
    return finalMap;
  }
  std::string displayCertificateOptions(const std::string& raw){
    std::string optionsDisplay;
    std::map<std::string,std::string> cOptionsMap=getCertMapFromTemplate(raw,"noUserName","noFullName");
    std::map<std::string,std::string>::const_iterator it,itE;
    it=cOptionsMap.begin(); itE=cOptionsMap.end();
    optionsDisplay+="<p>Possible certificates are: ";
    int writtenAlready=0;
    while(it!=itE){
      if(writtenAlready){
        optionsDisplay+=", ";
      }
      writtenAlready=1;
      optionsDisplay+="<i>"+it->first+"</i>";
      ++it;
    }
    optionsDisplay+=".</p>\n";
    return optionsDisplay;
  }
  std::string prepareCertificate(const std::string& rawText, const std::map<std::string,std::string> & valVarMap){
    return SF::replaceVariablesWithValues(rawText,"_vr_","_/vr_",valVarMap);
  }
  std::string sanitizeCertificateComment(const std::string & cComment){
    return BF::cleanSpaces(cComment,1);
  }
  std::string identifyCertificateTemplate(const std::vector<std::string>& v){
    long sz=v.size();
    std::string res="";
    std::pair<std::string,int> allD; long pos;
    while((sz>0)&&(res=="")){
      --sz;
      pos=0;allD=SF::extract(v[sz],pos,"_lb*|_","_/lb*|_");
      if(allD.second==1){
        if(doesLabelCorrespondToCertificate(allD.first)){
          res=v[sz];
        }
      }
    }
    return res;
  }
  std::string rawCertificateTemplates(const std::string& fDocument){
    RMD::Response rs;
    int textExists=rs.setFromTextName(fDocument);
    if(textExists==0){
      return "!failed!";
    }
    std::string rawText=rs.getTextData();
    std::vector<std::string> individualProblems=SF::stringToVector(rawText,"_in*|_","_/in*|_");
    return identifyCertificateTemplate(individualProblems);
  }
  std::string updateCertificate(const PSDI::SessionData & _psd, const std::string& rawRespRecText, const std::string & _certLabel, const RTI::ResponderInfo& res){
    std::string certLabel=_certLabel;
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(_certLabel,pos,"_comment_","_/comment_");
    if(allD.second==1){
      certLabel=allD.first;
    }
    std::string updatedRawText=rawRespRecText;
    std::string oldCertDescCode=res.certificateDescriptionCode;
    std::string oldCertIDCode=res.certificateIdCode;
    std::string nCertDescCode="notFound", nCertIDCode="notFound";
    if((oldCertIDCode!="notFound")&&(oldCertIDCode!="")){
      MCEWCPI::deleteCertNoPermissionCheck(oldCertIDCode);
    }
    std::string rawTemplates=rawCertificateTemplates(res.formulationDocument);
    std::map<std::string,std::string> certMap=getCertMapFromTemplate(rawTemplates,res.solverUserName,res.idInfoData);
    std::map<std::string,std::string>::const_iterator it;
    it=certMap.find(certLabel);
    if(it!=certMap.end()){
      std::string certText=it->second;
      std::string certificateCreationMessage=MCEWCPI::createCertNoPermissionCheck(_psd,"!*!",certText);
      if((certificateCreationMessage.length()>3)&&(certificateCreationMessage[0]=='!')&&(certificateCreationMessage[1]='s')&&(certificateCreationMessage[2]=='u')){
        nCertIDCode=BF::removeNonNumericStart(certificateCreationMessage);
        nCertDescCode=certLabel;
        pos=0;allD=SF::extractAndReplace(updatedRawText,pos,"_cdCode*|_","_/cdCode*|_",0,"_cdCode*|_"+nCertDescCode+"_/cdCode*|_");
        if(allD.second==1){
          updatedRawText=allD.first;
        }
        else{
          updatedRawText+=" _cdCode*|_"+nCertDescCode+"_/cdCode*|_";
        }
        pos=0;allD=SF::extractAndReplace(updatedRawText,pos,"_cidCode*|_","_/cidCode*|_",0,"_cidCode*|_"+nCertIDCode+"_/cidCode*|_");
        if(allD.second==1){
          updatedRawText=allD.first;
        }
        else{
          updatedRawText+=" _cidCode*|_"+nCertIDCode+"_/cidCode*|_";
        }
      }
    }
    return updatedRawText;
  }
}
#endif
