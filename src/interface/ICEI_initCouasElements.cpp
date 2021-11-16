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


#ifndef _INCL_WI_initCouasElements_CPP
#define _INCL_WI_initCouasElements_CPP
namespace ICEI{


  std::string initFromRawText(const PSDI::SessionData & _psd,CEI::CouasElement & cEl, const std::string & rawText,const std::string &cCode){


    int scc=cEl.initialize(_psd, rawText, cCode);
    if(scc==0){
      return "failed";
    }
    return "success";
  }
  std::string initFromCode(const PSDI::SessionData & _psd,CEI::CouasElement & cEl, const std::string &cCode){
    CAD::CouAs myC;
    int couasExists=myC.setFromExternalCode(cCode);
    if(couasExists==0){
      return "failed";
    }
    std::string rawText=myC.getTextData();
    return initFromRawText(_psd,cEl,rawText,cCode);
  }

  std::set<std::string> missingGradesOfStudent(const CEI::CouasListElAtt & st, const std::set<std::string> & neededGrades){
    std::set<std::string> fR;
    std::set<std::string>::const_iterator it=neededGrades.begin(),itE=neededGrades.end();
    while(it!=itE){
      if(st.getGradeStr(*it)==""){
        fR.insert(*it);
      }
      ++it;
    }
    return fR;
  }
  std::set<std::string> missingGrades(const std::map<std::string,CEI::CouasListElAtt> & allSt, const std::set<std::string> & neededGrades){
    std::set<std::string>  fR,tmp;
    std::map<std::string,CEI::CouasListElAtt>::const_iterator it=allSt.begin(),itE=allSt.end();
    std::set<std::string>::const_iterator itS,itSE;
    while(it!=itE){
      tmp=missingGradesOfStudent(it->second,neededGrades);
      itSE=tmp.end();
      itS=tmp.begin();
      while(itS!=itSE){
        fR.insert(*itS);
        ++itS;
      }
      ++it;
    }
    return fR;
  }
  GEI::GEvalData createEvalDataForGrade(const std::string & grName, const std::map<std::string,std::string> & currentFormulas){
    GEI::GEvalData fR;
    fR.name=grName;
    fR.type="";
    fR.evaluationFormulas=currentFormulas;
    std::map<std::string,std::string>::const_iterator it,itE=currentFormulas.end();

    return fR;
  }
  std::map<std::string,std::string> getExplicitGrades(const std::map<std::string,CEI::CouasListElAtt> & mM){
    std::map<std::string,std::string> fR;
    std::map<std::string,CEI::CouasListElAtt>::const_iterator it=mM.begin(),itE=mM.end();
    std::string grStr;
    while(it!=itE){
      grStr= (it->second).getGradeStr(LI::GL_LN.defaultGradeName);
      if(grStr!=""){
        fR[it->first]=grStr;
      }
      ++it;
    }
    return fR;
  }
  std::set<std::string> codesOfDocumentsThatNeedToBeRead(const GEI::GEvalData & gm){
    std::set<std::string> fR;
    std::map<std::string,std::string>::const_iterator it1=gm.evaluationFormulas.begin(),it1E=gm.evaluationFormulas.end();
    std::string allFSt="";
    while(it1!=it1E){
      allFSt+=(it1->second);
      ++it1;
    }
    std::vector<std::string> v=SF::stringToVector(allFSt,LI::GL_LN.st_sepGetFromB,LI::GL_LN.st_sepGetFromE);
    long sz=v.size();
    for(long i=0;i<sz;++i){
      fR.insert(v[i]);
    }

    return fR;

  }

  std::set<std::string> codesOfDocumentsThatNeedToBeRead(const std::map<std::string,GEI::GEvalData> & mm){
    std::set<std::string> fR,tmp;
    std::map<std::string,GEI::GEvalData>::const_iterator it=mm.begin(),itE=mm.end();
    std::set<std::string>::const_iterator itS,itSE;
    while(it!=itE){
      tmp=codesOfDocumentsThatNeedToBeRead(it->second);
      itS=tmp.begin();itSE=tmp.end();
      while(itS!=itSE){
        fR.insert(*itS);
        ++itS;
      }
      ++it;
    }
    return fR;
  }
  std::string prepareFormulasFromSingleGED(const CEI::CouasListElAtt &st, const GEI::GEvalData & ged){
    std::string fR="";
    if(ged.evaluationFormulas.size()==0){
      return "";
    }
    std::map<std::string,std::string>::const_iterator it,itE;
    itE=ged.evaluationFormulas.end();
    it=ged.evaluationFormulas.find(st.getUName());
    if(it!=itE){
      fR=it->second;
    }
    else{
      it=ged.evaluationFormulas.find("all");
      if(it!=itE){
        fR=it->second;
      }
    }
    return fR;
  }
  std::string evaluateFormulas(const CEI::CouasListElAtt &st, const GEI::GEvalData & ged,
                              const std::map<std::string, std::map<std::string,CEI::CouasListElAtt>  > & recursiveCalculations){
    std::string fR="";
    fR+=prepareFormulasFromSingleGED(st, ged);
    std::map<std::string, std::map<std::string,CEI::CouasListElAtt>  >::const_iterator it1,it1E;
    std::map<std::string,CEI::CouasListElAtt>::const_iterator itI,itIE;
    it1=recursiveCalculations.begin();it1E=recursiveCalculations.end();
    std::string to_repl, to_search;
    double numGr;
    while(it1!=it1E){
      to_repl="";
      to_search=it1->first;
      to_search=LI::GL_LN.st_sepGetFromB+to_search+LI::GL_LN.st_sepGetFromE;
      itIE=(it1->second).end();
      itI=(it1->second).find(st.getUName());
      if(itI!=itIE){
        to_repl=  (itI->second).getGradeStr(LI::GL_LN.defaultGradeName);
        numGr= (itI->second).getGradeNum(LI::GL_LN.defaultGradeName);
        if(numGr<BF::GLOBAL_EPSILON){
          to_repl="0";
        }
      }
      fR=SF::findAndReplace(fR,to_search,to_repl);
      ++it1;
    }
    std::string oldFR=fR;
    if(oldFR==""){
      return "";
    }
    fR="_gradesNeeded!*_";
    std::set<std::string>::const_iterator itS,itSE;
    itS=(ged.gradesToOutput).begin();itSE=(ged.gradesToOutput).end();
    while(itS!=itSE){
      fR+="_k*!_"+SF::findAndReplace(SF::findAndReplace((*itS),"{","{_"),"}","_}")+"_/k*!_"+"_v*!_"+(*itS)+"_/v*!_";
      ++itS;
    }


    fR+="_/gradesNeeded!*_";
    fR+="_searchReplace_";
    fR+="_v*a_";
    fR+=oldFR;
    fR+="_/v*a_";
    fR+="_v*0_";
    fR+="{irrelevant11}=11@@";
    fR+="_/v*0_";
    fR+="_/searchReplace_";
    fR=TWDVF::singleVersion(fR,0);
    fR=SF::findAndReplace(fR,"{_","{");
    fR=SF::findAndReplace(fR,"_}","}");
    return fR;
  }

  std::pair<CEI::CouasListElAtt,int> updateGrades(const CEI::CouasListElAtt &st, const GEI::GEvalData & ged,
                                             const std::map<std::string, std::map<std::string,CEI::CouasListElAtt> > & recursiveCalculations){
    std::pair<CEI::CouasListElAtt,int> fR;
    fR.first=st;
    fR.second=0;
    std::map<std::string,std::string> resEvaluation=SF::stringToMap(evaluateFormulas(st,ged,recursiveCalculations),"_k*!_","_/k*!_","_v*!_","_/v*!_");
    std::map<std::string,std::string>::const_iterator it,itE;
    it=resEvaluation.begin();itE=resEvaluation.end();
    std::string oldGrade;
    while(it!=itE){
      oldGrade=st.getGradeStr(it->first);
      if(oldGrade==""){
        (fR.first).setGradeStr(it->first,it->second);
        fR.second=1;
      }
      ++it;
    }
    return fR;
  }
  std::map<std::string,CEI::CouasListElAtt> calculateGradesFromCode(const std::string & ,const std::string & , const long &  = 10);
  std::map<std::string,CEI::CouasListElAtt> calculateGradesFromRawText(const std::string &rawText,const std::string & masterKey,const long &remainingRecDepth=10){
    std::map<std::string,CEI::CouasListElAtt> fR;
    GEI::GEvalData ged;
    std::set<std::string> allStudents;
    if(remainingRecDepth<1){
      return fR;
    }

    CEI::CouasListElAtt tmp;
    long sz=0;

    std::pair<std::string,int> allD;
    long pos;

    pos=0;allD=SF::extract(rawText,pos,LI::GL_LN.st_sepElementsEncB,LI::GL_LN.st_sepElementsEncE);
    std::string gdr="";
    if(allD.second==0){
      pos=0;allD=SF::extract(rawText,pos,LI::GL_LN.st_sepElementsB,LI::GL_LN.st_sepElementsE);
      if(allD.second==1){
          gdr=allD.first;
      }
    }
    else{
      gdr=ENCF::symmetricDecrypt(allD.first,masterKey);
    }
    if(gdr!=""){
      std::vector<std::string> gdrv=SF::stringToVector(gdr,LI::GL_LN.st_sepIndElB,LI::GL_LN.st_sepIndElE);
      sz=gdrv.size();
      for(long i=0;i<sz;++i){
        tmp.setFromString(gdrv[i]);
        fR[tmp.getUName()]=tmp;
        allStudents.insert(tmp.getUName());
      }
    }
    if(remainingRecDepth>0){
        (ged.gradesToOutput).clear();
        pos=0;allD=SF::extract(rawText,pos,LI::GL_LN.st_sepGrDisplB,LI::GL_LN.st_sepGrDisplE);
        if(allD.second==1){
          ged.gradesToOutput=SF::stringToSet(allD.first,LI::GL_LN.st_sepValueB,LI::GL_LN.st_sepValueE);
        }
        if((ged.gradesToOutput).size()==0){
          ged.gradesToOutput.insert(LI::GL_LN.defaultGradeName);
        }
        (ged.evaluationFormulas).clear();
        pos=0;allD=SF::extract(rawText,pos,LI::GL_LN.st_sepGradeFormulaB,LI::GL_LN.st_sepGradeFormulaE);
        if(allD.second==1){
          ged.evaluationFormulas=SF::stringToMap(allD.first,LI::GL_LN.st_sepGradeFSB,LI::GL_LN.st_sepGradeFSE,LI::GL_LN.st_sepGradeFMB,LI::GL_LN.st_sepGradeFME);
        }

        ged.explicitGrades=getExplicitGrades(fR);
        std::set<std::string> missedGrades=missingGrades(fR,ged.gradesToOutput);
        if(missedGrades.size()>0){
          // We first identify all codes that need to be read

          std::set<std::string> toRead= codesOfDocumentsThatNeedToBeRead(ged);
          std::set<std::string>::const_iterator itS1,itS1E;
          itS1=toRead.begin();itS1E=toRead.end();
          std::map<std::string, std::map<std::string,CEI::CouasListElAtt> > recursiveCalculations;
          while(itS1!=itS1E){
            recursiveCalculations[*itS1]=calculateGradesFromCode(*itS1,masterKey, remainingRecDepth-1);
            ++itS1;
          }
          std::set<std::string>::const_iterator itSM,itSME;
          itSM=allStudents.begin();itSME=allStudents.end();
          while(itSM!=itSME){
            std::pair<CEI::CouasListElAtt,int> newGrades=updateGrades(fR[*itSM],ged,recursiveCalculations);
            if(newGrades.second==1){
              fR[*itSM]=newGrades.first;
            }
            ++itSM;
          }

        }
    }

    return fR;
  }
  std::map<std::string,CEI::CouasListElAtt>  calculateGradesFromCode(const std::string &cCode, const std::string & masterKey, const long & remainingRecDepth ){
    std::map<std::string,CEI::CouasListElAtt> fR;
    if(remainingRecDepth<1){
      return fR;
    }
    CAD::CouAs myC;
    int couasExists=myC.setFromExternalCode(cCode);
    if(couasExists==0){
      return fR;
    }
    std::string rawText=myC.getTextData();
    return calculateGradesFromRawText(rawText,masterKey, remainingRecDepth);
  }
}
#endif
