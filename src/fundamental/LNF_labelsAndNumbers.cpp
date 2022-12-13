//    GradeYard learning management system
//
//    Copyright (C) 2022 Ivan Matic, https://gradeyard.com
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
#ifndef _INCL_labelsAndNumbers_CPP
#define _INCL_labelsAndNumbers_CPP

namespace LNF{
  std::string collectLabels(std::map<std::string,std::map<std::string,long> > & mainMap, const std::string _input){
    std::string input=_input+"\\label{endL|endL}";
    std::string output="";
    long pos=0,posIn;
    std::pair<std::string,int> allD1,allD2,allDCat,allDLab;
    std::string category,label;
    allD1=SF::getEverythingBefore(input,pos,"\\label{");
    allD2=SF::getEverythingBefore(input,pos,"}");
    std::string labelRaw;
    std::map<std::string,std::map<std::string,long> >::iterator it,itE;
    long currentNum;
    while((allD2.second==1)&&(allD2.first!="endL|endL")){
      posIn=0;
      labelRaw=allD2.first;
      labelRaw+="|nothing!*|nothing!*";
      allDCat=SF::getEverythingBefore(labelRaw,posIn,"|");
      allDLab=SF::getEverythingBefore(labelRaw,posIn,"|");
      if(allDLab.first=="nothing!*"){
        category="default";label=allDCat.first;
      }
      else{
        category=allDCat.first;label=allDLab.first;
      }
      itE=mainMap.end();
      it=mainMap.find(category);
      if(it==itE){
        std::map<std::string,long> start;
        start[label]=1;
        mainMap[category]=start;
        currentNum=1;
      }
      else{
        //Step 0: Check if the label already exists:
        if( (it->second).find(label) != (it->second).end() ){
          currentNum=-777;
        }
        else{
          //Step 1: Insertion of a dummy element in map
          (it->second)[label]=0;
          //Step 2: Finding out the correct number;
          currentNum=(it->second).size();
          //Step 2: Correction of the label with correct number;
          (it->second)[label]=currentNum;
        }
      }
      output+=allD1.first;
      output+=std::to_string(currentNum);
      allD1=SF::getEverythingBefore(input,pos,"\\label{");
      allD2=SF::getEverythingBefore(input,pos,"}");
    }
    output+=allD1.first;
    return output;
  }
  std::string replaceReferences(const std::map<std::string,std::map<std::string,long> > & mainMap,const std::string & _input){
    std::map<std::string,std::map<std::string,long> >::const_iterator itM,itME=mainMap.cend();
    std::map<std::string,long>::const_iterator itS,itSE;
    std::string output="";
    std::string input=_input+"\\ref{endL|endL}";
    long pos=0,pos1,pos2,posIn;
    std::pair<std::string,int> allD1,allD2,allDCat,allDLab;
    std::string category,label,labelRaw;
    pos1=pos;
    allD1=SF::getEverythingBefore(input,pos,"\\ref{");
    allD2=SF::getEverythingBefore(input,pos,"}");
    long currentNum=0;
    while((allD2.second==1)&&(allD2.first!="endL|endL")){
      posIn=0;
      labelRaw=allD2.first;
      labelRaw+="|nothing!*|nothing!*";
      allDCat=SF::getEverythingBefore(labelRaw,posIn,"|");
      allDLab=SF::getEverythingBefore(labelRaw,posIn,"|");
      if(allDLab.first=="nothing!*"){
        category="default";label=allDCat.first;
      }
      else{
        category=allDCat.first;label=allDLab.first;
      }
      currentNum=0;
      itM=mainMap.find(category);
      if(itM!=itME){
        itS=(itM->second).find(label);
        if(itS!=(itM->second).end()){
          currentNum=itS->second;
        }
      }
      output+=allD1.first;
      output+=std::to_string(currentNum);
      allD1=SF::getEverythingBefore(input,pos,"\\ref{");
      allD2=SF::getEverythingBefore(input,pos,"}");
    }
    output+=allD1.first;
    return output;
  }
  std::string labelsAndNumbers(const std::string & input){
    std::map<std::string,std::map<std::string,long> > mainCategoryMap;
    std::string output=collectLabels(mainCategoryMap,input);
    output=replaceReferences(mainCategoryMap,output);
    return output;
  }
}

#endif
