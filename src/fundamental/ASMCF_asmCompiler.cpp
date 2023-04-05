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


#ifndef _INCL_ASMCF_ASMCOMPILER_CPP
#define _INCL_ASMCF_ASMCOMPILER_CPP
namespace ASMCF{


  struct ASMRules{
  public:
    std::string labelPrefix;
    std::string asmObjectName;
    std::string pointerPrefix;
    std::string sepB;
    std::string sepE;
    std::map<std::string,std::string> branchingMap;
    std::map<std::string,std::string> simpleInstructionsMap;
  };
  ASMRules stringToRules(const std::string& in){
    ASMRules res;
    res.labelPrefix="l"+RNDF::genRandCode(5);
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(in,pos,"_branchingMap_","_/branchingMap_");
    SF::varValPairs(allD.first,"_vVPair_", "_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",res.branchingMap);
    pos=0;allD=SF::extract(in,pos,"_simpleInstructionsMap_","_/simpleInstructionsMap_");
    SF::varValPairs(allD.first,"_vVPair_", "_/vVPair_","_vr_","_/vr_","_vl_","_/vl_",res.simpleInstructionsMap);
    res.asmObjectName="sim";
    pos=0;allD=SF::extract(in,pos,"_asmObjectName_","_/asmObjectName_");
    if(allD.second){res.asmObjectName=allD.first;}
    res.pointerPrefix="a_";
    pos=0;allD=SF::extract(in,pos,"_pointerPrefix_","_/pointerPrefix_");
    if(allD.second){res.pointerPrefix=allD.first;}
    res.sepB="_asm_";
    pos=0;allD=SF::extract(in,pos,"_asmSeparatorB_","_/asmSeparatorB_");
    if(allD.second){res.sepB=allD.first;}
    res.sepE="_/asm_";
    pos=0;allD=SF::extract(in,pos,"_asmSeparatorE_","_/asmSeparatorE_");
    if(allD.second){res.sepE=allD.first;}
    return res;
  }
  std::string branchingToCPP(const ASMRIF::Arm64Instruction& asmInst, const ASMRules& rules){
    std::string label=rules.labelPrefix+asmInst.arg1.argName;
    if(asmInst.name=="b"){
      return "goto "+label+";";
    }
    std::map<std::string,std::string>::const_iterator it;
    it=rules.branchingMap.find(asmInst.name);
    if(it==rules.branchingMap.end()){
      return "Error: Branching instruction "+asmInst.name+" not supported.";
    }
    return it->second+"{goto "+label+";}";
  }
  std::string simpleInstructionToCPP(const ASMRIF::Arm64Instruction& asmInst, const ASMRules& rules){
    std::map<std::string,std::string>::const_iterator it;
    it=rules.simpleInstructionsMap.find(asmInst.name);
    if(it==rules.simpleInstructionsMap.end()){
      return "";
    }
    return it->second;
  }
  std::string addOneArgument(const ASMRIF::Arm64Instruction& asmInst, const std::string &in, const std::string& findName, const ASMRIF::Arm64InstrArg& argRepl, const ASMRules& rules){
    if(argRepl.derefIndicator){
      std::string replString;
      std::string shiftAddition;
      if(argRepl.shift!=""){
        if(argRepl.shift=="immediate"){
          shiftAddition="+("+std::to_string(argRepl.immediateShift)+")";
        }
        else{
          shiftAddition="+(*("+rules.asmObjectName+"."+rules.pointerPrefix+argRepl.shift+"))";
        }
      }
      std::string pointerValue="(*("+rules.asmObjectName+"."+rules.pointerPrefix+argRepl.argName+"))";
      pointerValue+=shiftAddition;
      std::string castingType="(long*)";
      if(argRepl.argName[0]=='w'){
        castingType="(int*)";
      }
      std::string castedPointer=castingType+"("+pointerValue+")";
      replString="(*("+castedPointer+"))";
      return SF::findAndReplace(in,findName,replString);
    }
    if(argRepl.argName=="immediate"){
      return SF::findAndReplace(in,findName,"("+std::to_string(argRepl.immediateVal)+")");
    }
    return SF::findAndReplace(in,findName,"(*("+rules.asmObjectName+"."+rules.pointerPrefix+argRepl.argName+"))");
  }
  std::string addArguments(const ASMRIF::Arm64Instruction& asmInst,const std::string &in, const ASMRules& rules){
    std::string out=in;
    out=addOneArgument(asmInst,out,"_*arg1*_",asmInst.arg1, rules);
    out=addOneArgument(asmInst,out,"_*arg2*_",asmInst.arg2, rules);
    out=addOneArgument(asmInst,out,"_*arg3*_",asmInst.arg3, rules);
    out=addOneArgument(asmInst,out,"_*arg4*_",asmInst.arg4, rules);
    return out;
  }
  std::string asmToCPP(const ASMRIF::Arm64Instruction& asmInst, const ASMRules& rules){
    if(asmInst.name=="label"){
      return rules.labelPrefix+asmInst.arg1.argName+":";
    }
    if(asmInst.name[0]=='b'){
      return branchingToCPP(asmInst,rules);
    }
    std::string attemptInst=simpleInstructionToCPP(asmInst,rules);
    if(attemptInst!=""){
      return addArguments(asmInst,attemptInst,rules);
    }
    return "Error: Instruction "+asmInst.name+" not supported.";
  }
  std::string asmToCPP(const std::vector<ASMRIF::Arm64Instruction>& asmInst, const ASMRules& rules){
    std::string res;
    long sz=asmInst.size();
    for(long i=0;i<sz;++i){
      res+=asmToCPP(asmInst[i],rules)+"\n";
    }
    return res;
  }
  std::string createCPPCode(const std::string& input, const ASMRules& rules){
    std::string out=input;
    std::pair<std::string,int> allD;
    long pos;
    pos=0; allD=SF::extract(out,pos,rules.sepB,rules.sepE);
    if(allD.second==0){
      return out;
    }
    std::vector<ASMRIF::Arm64Instruction> allInstructions=ASMRIF::getInstructions(allD.first);
    pos=0; allD=SF::extractAndReplace(out,pos,rules.sepB,rules.sepE,0, asmToCPP(allInstructions, rules));
    if(allD.second==0){
      return out;
    }
    return allD.first;
  }
  std::string assemblerComponentToCPP(const std::string &src, const std::string& stRules){
    ASMRules ar=stringToRules(stRules);
    return createCPPCode(src,ar);
  } 
}
#endif
