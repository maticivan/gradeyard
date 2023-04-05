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


#ifndef _INCL_ASMRIFF_ASMREADINSTRUCTIONS_CPP
#define _INCL_ASMRIFF_ASMREADINSTRUCTIONS_CPP
namespace ASMRIF{
  class Arm64InstrArg{
  public:
    std::string argName;
    int derefIndicator;
    long immediateVal; //argName must be "immediate" for this attribute to be of consequence
    std::string shift;
    long immediateShift; //shiftName must be "immediate" for this attribute to work
    Arm64InstrArg();
  };
  Arm64InstrArg::Arm64InstrArg(){
    argName="";derefIndicator=0;immediateVal=0;shift="";immediateShift=0;
  }
  std::string toString(const Arm64InstrArg& inArg){
    std::string res;
    if(inArg.argName=="immediate"){
      res+=std::to_string(inArg.immediateVal);
    }
    else{
      if(inArg.derefIndicator){
        res+="*(";
      }
      res+=inArg.argName;
      if(inArg.derefIndicator){
        if(inArg.shift!=""){
          res+="+";
          if(inArg.shift=="immediate"){
            res+=std::to_string(inArg.immediateShift);
          }
          else{
            res+=inArg.shift;
          }
        }
        res+=")";
      }
    }
    return res;
  }
  class Arm64Instruction{
  public:
    std::string name;
    Arm64InstrArg arg1;
    Arm64InstrArg arg2;
    Arm64InstrArg arg3;
    Arm64InstrArg arg4;
  };
  std::string toString(const Arm64Instruction& in){
    std::string res;
    res+=in.name+":\t";
    res+=toString(in.arg1)+"|:|\t";
    res+=toString(in.arg2)+"|:|\t";
    res+=toString(in.arg3)+"|:|\t";
    res+=toString(in.arg4);
    return res;
  }
  Arm64Instruction shiftArgumentsDown(const Arm64Instruction& in){
    Arm64Instruction out=in;
    out.arg4=in.arg3;
    out.arg3=in.arg2;
    out.arg2=in.arg1;
    return out;
  }
  std::string instructionsToString(const std::vector<Arm64Instruction>& v){
    std::string res;
    long sz=v.size();
    for(long i=0;i<sz;++i){
      res+=toString(v[i])+"\n";
    }
    return res;
  }
  int isEmptyLine(const std::string& x){
    std::string y=x;
    y=SF::findAndReplace(y,"\n","");
    y=SF::findAndReplace(y,"\t","");
    y=SF::findAndReplace(y," ","");
    if(y==""){return 1;}
    return 0;
  }

  std::string getNextDeref(const std::string& st, long& pos){
    std::string res;
    long sz=st.length();
    while((pos<sz)&&(st[pos]!=']')){
      res+=st[pos];++pos;
    }
    if((pos<sz)&&(st[pos]==']')){
      res+=']';++pos;
    }
    return res;
  }
  std::string getNextBeforeSep(const std::string& st, long& pos, const char& sep){
    std::string res;
    long sz=st.length();
    if((pos<sz)&&(st[pos]=='[')){
      return getNextDeref(st,pos);
    }
    while((pos<sz)&&(st[pos]!=sep)){
      res+=st[pos];
      ++pos;
    }
    if((pos<sz)&&(st[pos]==sep)){
      ++pos;
    }
    return res;
  }
  long baseConversion(const std::string& st,long pos, const long& base){
    long res=0;
    long sz=st.length();
    while(pos<sz){
      res*=base;
      if((st[pos]>='0')&&(st[pos]<='9')){
        res+=st[pos]-'0';
      }
      else{
        if((st[pos]>='a')&&(st[pos]<='z')){
          res+=10+st[pos]-'a';
        }
        else{
          if((st[pos]>='A')&&(st[pos]<='Z')){
            res+=10+st[pos]-'A';
          }
        }
      }
      ++pos;
    }
    return res;
  }
  long baseConversionForStringThatStartsWith0(const std::string& rSt){
    if(rSt.length()<2){
      return 0;
    }
    if((rSt[1]=='b')||(rSt[1]=='B')){
      return baseConversion(rSt,2,2);
    }
    if((rSt[1]=='x')||(rSt[1]=='X')){
      return baseConversion(rSt,2,16);
    }
    if((rSt[1]=='o')||(rSt[1]=='O')){
      return baseConversion(rSt,2,8);
    }
    return baseConversion(rSt,1,8);
  }
  Arm64InstrArg getNextArgBeforeSep(const std::string& st, long& pos, const char& sep){
    Arm64InstrArg res;
    std::string rSt=getNextBeforeSep(st,pos,sep);
    if(rSt==""){
      res.argName="error01";
      return res;
    }
    if(rSt[0]=='0'){
      res.argName="immediate";
      res.immediateVal=baseConversionForStringThatStartsWith0(rSt);
      return res;
    }
    if(BF::isNumeric(rSt)){
      res.argName="immediate";
      res.immediateVal=BF::stringToInteger(rSt);
      return res;
    }
    if(rSt[0]!='['){
      res.argName=rSt;
      return res;
    }
    res.derefIndicator=1;
    std::string shiftTest=SF::findAndReplace(rSt,",","");
    long pos2=1;
    if(shiftTest==rSt){
      res.argName=getNextBeforeSep(rSt,pos2,']');
      return res;
    }
    std::string nextSt=getNextBeforeSep(rSt,pos2,',');
    if(nextSt==""){
      res.argName="error02";return res;
    }
    if(nextSt[0]=='-'){
      res.argName="error03";return res;
    }
    if((nextSt[0]>='0')&&(nextSt[0]<='9')){
      res.argName="error04";return res;
    }
    res.argName=nextSt;
    nextSt=getNextBeforeSep(rSt,pos2,']');
    if(nextSt==""){
      res.argName="error05";return res;
    }
    if(nextSt[0]=='0'){
      res.shift="immediate";
      res.immediateShift=baseConversionForStringThatStartsWith0(nextSt);
      return res;
    }
    if(BF::isNumeric(nextSt)){
      res.shift="immediate";
      res.immediateShift=BF::stringToInteger(nextSt);
      return res;
    }
    res.shift=nextSt;
    return res;
  }
  struct PreIndexAndPostIndexInstructionData{
  public:
    int preIndexIndicator;
    int postIndexIndicator;
    Arm64Instruction preIndexInstruction;
    Arm64Instruction postIndexInstruction;
    PreIndexAndPostIndexInstructionData();
  };
  PreIndexAndPostIndexInstructionData::PreIndexAndPostIndexInstructionData(){
    preIndexIndicator=0; postIndexIndicator=0;
  }
  void updateStackWithInstructionAndPrePostIndex(std::stack<Arm64Instruction>& instStack, const PreIndexAndPostIndexInstructionData& ppiData, const Arm64Instruction& inst){
    if(ppiData.preIndexIndicator){
      instStack.push(ppiData.preIndexInstruction);
    }
    instStack.push(inst);
    if(ppiData.postIndexIndicator){
      instStack.push(ppiData.postIndexInstruction);
    }
  }
  void expandInstructionStack(std::stack<Arm64Instruction>& , const std::string& );
  void updatePreIndexing(std::string& st, PreIndexAndPostIndexInstructionData& ppiData){
    if(st!=SF::findAndReplace(st,"]!","")){
      std::pair<std::string,int> allD; long pos;
      pos=0;allD=SF::extract(st,pos,"[","]!");
      if(allD.second==1){
        std::stack<Arm64Instruction> st1;
        expandInstructionStack(st1,"add "+ allD.first);
        ppiData.preIndexIndicator=1;
        ppiData.preIndexInstruction=shiftArgumentsDown(st1.top());
        st=SF::findAndReplace(st,"["+allD.first+"]!","["+ppiData.preIndexInstruction.arg1.argName+"]");
      }
    }
  }
  void updatePostIndexing(std::string& st, PreIndexAndPostIndexInstructionData& ppiData){
    if(st!=SF::findAndReplace(st,"],","")){
      std::pair<std::string,int> allD; long pos;
      std::string argumentName;
      pos=0;allD=SF::extract(st,pos,"[","],");
      if(allD.second==0){return;}
      argumentName=allD.first;
      pos=0;allD=SF::getEverythingBefore(st,pos,"],");
      if(allD.second==1){
        std::string stNew=allD.first+"]";
        std::string stAfter;
        long sz=st.length();
        while(pos<sz){
          stAfter+=st[pos];
          ++pos;
        }
        std::stack<Arm64Instruction> st1;
        expandInstructionStack(st1,"add "+ argumentName+","+argumentName+","+stAfter);
        ppiData.postIndexIndicator=1;
        ppiData.postIndexInstruction=st1.top();
        st=stNew;
      }
    }
  }
  void separateShifts(std::string & st){
    st=SF::findAndReplace(st,"lsl ","lsl|");
    st=SF::findAndReplace(st,"lsr ","lsr|");
    st=SF::findAndReplace(st,"asr ","asr|");
    st=SF::findAndReplace(st,"ror ","ror|");
  }
  void restoreShifts(std::string & st){
    st=SF::findAndReplace(st,"lsl|","lsl ");
    st=SF::findAndReplace(st,"lsr|","lsr ");
    st=SF::findAndReplace(st,"asr|","asr ");
    st=SF::findAndReplace(st,"ror|","ror ");
  }
  void expandInstructionStack(std::stack<Arm64Instruction>& instStack, const std::string& _st){
    Arm64Instruction res;
    std::string st=_st;
    st=BF::cleanSpaces(st);
    std::string labelChecker=BF::cleanAllSpaces(st);
    if(labelChecker.length()>0){
      if(labelChecker[labelChecker.length()-1]==':'){
        res.name="label";
        res.arg1.argName=SF::findAndReplace(labelChecker,":","");
        instStack.push(res);
        return;
      }
    }
    st=SF::findAndReplace(st,"#","");
    st=SF::findAndReplace(st,"\t"," ");
    std::string nextSt;
    long pos=0;
    long sz=st.length();
    nextSt=getNextBeforeSep(st,pos,' ');
    if(nextSt==""){
      res.name="error|"+_st+"|";
      instStack.push(res);
      return;
    }
    res.name=SF::toLowerCase(nextSt);
    std::string stOld=st;
    st="";
    while(pos<sz){
      st+=stOld[pos];++pos;
    }
    separateShifts(st);
    st=BF::cleanAllSpaces(st);
    restoreShifts(st);
    if(res.name[0]!='b'){
      st=SF::toLowerCase(st);
    }
    PreIndexAndPostIndexInstructionData ppiData;
    updatePreIndexing(st,ppiData);
    updatePostIndexing(st,ppiData);
    pos=0; sz=st.length();
    if(pos>=sz){
      updateStackWithInstructionAndPrePostIndex(instStack,ppiData,res);
      return;
    }
    res.arg1=getNextArgBeforeSep(st,pos,',');
    if(pos>=sz){
      updateStackWithInstructionAndPrePostIndex(instStack,ppiData,res);
      return;
    }
    res.arg2=getNextArgBeforeSep(st,pos,',');
    if(pos>=sz){
      updateStackWithInstructionAndPrePostIndex(instStack,ppiData,res);
      return;
    }
    res.arg3=getNextArgBeforeSep(st,pos,',');
    if(pos>=sz){
      updateStackWithInstructionAndPrePostIndex(instStack,ppiData,res);
      return;
    }
    res.arg4=getNextArgBeforeSep(st,pos,',');
    updateStackWithInstructionAndPrePostIndex(instStack,ppiData,res);
    return;
  }
  std::string removeComments(const std::string& in, const std::string &cBegin, const std::string &cEnd, const std::string& replWith=""){
    std::string out=in;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extractAndReplace(out,pos,cBegin,cEnd,0,replWith);
    while(allD.second==1){
      out=allD.first;
      pos=0; allD=SF::extractAndReplace(out,pos,cBegin,cEnd,0,replWith);
    }
    return out;
  }
  std::vector<Arm64Instruction> getInstructions(const std::string& in){
    std::string out=in;
    out=removeComments(out,"/*","*/");
    out=removeComments(out,"//","\n","\n");
    out="\n"+out;
    out+="\n";
    out=SF::findAndReplace(out,"\n","_/nInstr*|_ _nInstr*|_");
    std::vector<std::string> instLines=SF::stringToVector(out,"_nInstr*|_","_/nInstr*|_");
    long sz=instLines.size();
    std::stack<Arm64Instruction> instStack;
    for(long i=0;i<sz;++i){
      instLines[i]=BF::cleanSpaces(instLines[i]);
      if(!isEmptyLine(instLines[i])){
        expandInstructionStack(instStack,instLines[i]);
      }
    }

    return SF::stackToVector(instStack);
  }


  std::string transformProgram(const std::string& in){
    std::string out=in;
    std::pair<std::string,int> allD; long pos;
    pos=0;allD=SF::extract(out,pos,"_asm_","_/asm_");
    if(allD.second){
      out=allD.first;
    }
    std::vector<Arm64Instruction> instructions=getInstructions(out);
    out=instructionsToString(instructions);
    return out;
  }
}
#endif
