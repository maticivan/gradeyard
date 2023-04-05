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


#ifndef _INCL_DockerContainerExecution_CPP
#define _INCL_DockerContainerExecution_CPP
namespace DCEI{
  struct DCEI_globalConstants{
  public:
    std::string separatorBetweenBigNumbersInNames;
    std::vector<std::string> dangerousCPPCommands;
    std::vector<std::string> dangerousCPPWords;
    std::vector<std::string> dangerousPythonCommands;
    DCEI_globalConstants();
  } GL_DCEI_const;
  DCEI_globalConstants::DCEI_globalConstants(){
    separatorBetweenBigNumbersInNames="d";
    long numDangerousCPPCommands=3;
    dangerousCPPCommands.resize(numDangerousCPPCommands);
    dangerousCPPCommands[0]="system";
    dangerousCPPCommands[1]="exec";
    dangerousCPPCommands[2]="fork";
    long numDangerousCPPWords=1;
    dangerousCPPWords.resize(numDangerousCPPWords);
    dangerousCPPWords[0]="reinterpret_cast";
    long numDangerousPythonCommands=6;
    dangerousPythonCommands.resize(numDangerousPythonCommands);
    dangerousPythonCommands[0]="system";
    dangerousPythonCommands[1]="subprocess";
    dangerousPythonCommands[2]="open";
    dangerousPythonCommands[3]="write";
    dangerousPythonCommands[4]="fork";
    dangerousPythonCommands[5]="chdir";
  }
  struct ContainerExecutionData{
  public:
    std::vector<std::string> language;
    std::vector<std::string> compilerFlags;
    std::vector<std::string> sourceCode;
    std::vector<std::vector<std::string> > inputData;
    std::string sourceFNBase;
    std::string inDataFNBase;
    std::string outDataFNBase;
    std::string errDataFNBase;
    std::string dockerCommand;
    std::string folderInContainer;
    std::string myLinuxUserName;
    std::string bashExecFileName;
    long myLinuxUserId;
  };
  std::string myMountFolder(const std::string & myUserName){
    return DD::GL_DBS.getMountFolder()+"/un_"+myUserName;
  }
  int createTempFolderForMounting(const std::string &myUserName){
    IOF::sys_createFolderIfDoesNotExist(myMountFolder(myUserName),"readme.txt","Do not edit this folder");
    return 1;
  }
  std::pair<std::string, long> usernameAndId(const PSDI::SessionData & _psd){
    std::string filePathToSaveData=myMountFolder(_psd.my_un)+"/"+"redirectCommand.txt";
    std::pair<std::string,long> res;
    res.first=IOF::sys_executeCommandThatPrints("whoami",filePathToSaveData);
    res.second=BF::stringToInteger(IOF::sys_executeCommandThatPrints("id -u $whoami",filePathToSaveData));
    return res;
  }
  std::string copyFromOneFolderToAnother(const std::string &from, const std::string &to, const std::string & fNameFrom, const std::string & fNameTo="" ){
    if(fNameTo==""){
      return "cp "+from+"/"+fNameFrom+" "+to+"/"+fNameFrom+"\n";
    }
    return "cp "+from+"/"+fNameFrom+" "+to+"/"+fNameTo+"\n";
  }
  std::string fileFullName(const std::string &baseName, const std::string &ext, const long &mod1, const long &mod2=-1){
    std::string fR=baseName+BF::padded(mod1+1,BF::GLOBAL_PRIME_SEQUENCES.paddingNumB,"0");
    if(mod2>-1){
      fR+=GL_DCEI_const.separatorBetweenBigNumbersInNames+BF::padded(mod2+1,BF::GLOBAL_PRIME_SEQUENCES.paddingNumB,"0");
    }
    fR+=ext;
    return fR;
  }
  int putTextsIntoFiles(const std::string & folderName,const std::vector<std::string> &tx, const std::string &fileBaseName, const std::string & addDot, const std::vector<std::string> &ext){
    long sz=tx.size();
    std::string tempFName;
    for(long i=0;i<sz;++i){
      tempFName=folderName+"/"+fileFullName(fileBaseName,addDot+ext[i],i);
      IOF::toFile(tempFName,tx[i]);
    }
    return 1;
  }
  int putTextsIntoFiles(const std::string & folderName,const std::vector<std::string> &tx, const std::string &fileBaseName, const std::string & ext){
    std::vector<std::string> extV;
    long sz=tx.size();
    extV.resize(sz);
    for(long i=0;i<sz;++i){
      extV[i]=ext;
    }
    return putTextsIntoFiles(folderName,tx,fileBaseName,"",extV);
  }
  int deleteFiles(const std::string & folderName, const long & sz, const std::string &fileBaseName, const std::string & addDot, const std::vector<std::string> &ext){
    std::string tempFName;
    for(long i=0;i<sz;++i){
      tempFName=folderName+"/"+fileFullName(fileBaseName,addDot+ext[i],i);
      IOF::sys_deleteFile(tempFName);
    }
    return 1;
  }
  int deleteFiles(const std::string & folderName,const long & sz, const std::string &fileBaseName, const std::string & ext){
    std::vector<std::string> extV;
    extV.resize(sz);
    for(long i=0;i<sz;++i){
      extV[i]=ext;
    }
    return deleteFiles(folderName,sz,fileBaseName,"",extV);
  }
  std::string bashCommToCompileAndExecute(const std::string & compilerCommand,
                                          const std::string & inN,
                                          const std::string & outN,
                                          const std::string & errN,
                                          const std::string & executionPrefix,
                                          const std::string & binName,
                                          const long & i,
                                          const long &numTsts,
                                          const long &timeOut,
                                          const long &cppIndicator
                                         ){
    std::string txtResFile, txtErrorFile, txtInFile;
    std::string bRes=compilerCommand;
    bRes+="if [[ $? != 0 ]]; then\n";
    for(long j=0;j<numTsts;++j){
      txtResFile=fileFullName(outN,".txt",i,j);
      txtErrorFile=fileFullName(errN,".txt",i,j);
      bRes+="   outBegin=\"_error*|_yes_/error*|__eTxt*|_\"\n";
      bRes+="   outEnd=\"_/eTxt*|_\"\n";
      bRes+="   out=\"${outBegin} ${output} ${outEnd}\"\n";
      bRes+="   echo $out > "+txtErrorFile+"\n";
      bRes+="   echo \""+GF::GL_errorDidNotCompile+"\" > "+txtResFile+"\n";
    }
    bRes+="else\n";
    for(long j=0;j<numTsts;++j){
      txtInFile=fileFullName(inN,".txt",i,j);
      txtResFile=fileFullName(outN,".txt",i,j);
      txtErrorFile=fileFullName(errN,".txt",i,j);
      bRes+="   out3=$(timeout " + std::to_string(timeOut)+" "+executionPrefix;
      bRes+=binName;
      bRes+=" < "+txtInFile;
      bRes+=" > "+txtResFile+" 2>"+txtResFile+")\n";
      bRes+="   echo \"compiled\" > "+txtErrorFile+"\n";
    }
    if(cppIndicator==1){
      bRes+="   rm ";
      bRes+=binName+"\n";
    }
    bRes+="fi\n";
    return bRes;
    return bRes;
  }
  std::string bashCommandsToCompileAndExecuteCpp(const std::string & cppSourceFile,
                                                 const std::string & compilerFlags,
                                                 const std::string & inN,
                                                 const std::string & outN,
                                                 const std::string & errN,
                                                 const std::string &binB,
                                                 const long & i,
                                                 const long &numTsts,
                                                 const long &timeOut){
    std::string binFName=fileFullName(binB,"",i);
    std::string cComm="output=$(c++ "+cppSourceFile+" -o "+binFName +" "+compilerFlags+" 2>&1)\n";
    return bashCommToCompileAndExecute(cComm, inN,outN,errN,"./",binFName,i,numTsts,timeOut,1);
  }
  std::string bashCommandsToCompileAndExecutePython3(const std::string & pySourceFile,
                                                     const std::string & inN,
                                                     const std::string & outN,
                                                     const std::string & errN,
                                                     const std::string & binB,
                                                     const long & i,
                                                     const long &numTsts,
                                                     const long &timeOut){
    std::string cComm="output=$(python3 -m py_compile "+pySourceFile+" 2>&1)\n";
    return bashCommToCompileAndExecute(cComm, inN,outN,errN,"python3 ",pySourceFile,i,numTsts,timeOut,0);
  }
  std::string overwriteBigOutputs(const std::string & fNBase, const std::vector<std::vector<std::string> > & iD, const long & maxSize){
    std::string bRes="";
    long sz=iD.size();
    long szI;
    std::string fName;
    bRes+="compsize="+std::to_string(maxSize)+"\n";
    for(long i=0;i<sz;++i){
      szI=iD[i].size();
      for(long j=0;j<szI;++j){
        fName=fileFullName(fNBase,".txt",i,j);
        bRes+="file="+fName+"\n";
        bRes+="fsize=$(wc -c <\"$file\")\n";
        bRes+="if [ $fsize -ge $compsize ]; then\n";
        bRes+="   ";
        bRes+="echo \""+GF::GL_errorOutputTooBig+"\" > "+fName+"\n";
        bRes+="fi\n";
      }
    }
    return bRes;
  }
  std::string createBashForExecution(const ContainerExecutionData& ced,
                                     const long & timeOutTime=3,
                                     const std::string & folderKnownToSource="/home7"
                                     ){
    std::string folderHiddenFromSource=ced.folderInContainer;
    std::string bRes="";
    long numSrc=ced.sourceCode.size();
    std::string sourceFile;
    std::string inputFile;
    long numTst;
    std::string wordThatStartWithDot;
    for(long i=0;i<numSrc;++i){
      wordThatStartWithDot=".";
      wordThatStartWithDot+=ced.language[i];
      sourceFile=fileFullName(ced.sourceFNBase,wordThatStartWithDot,i);
      bRes+=copyFromOneFolderToAnother(folderHiddenFromSource,folderKnownToSource,sourceFile);
      numTst=ced.inputData[i].size();
      for(long j=0;j<numTst;++j){
        inputFile=fileFullName(ced.inDataFNBase,".txt",i,j);
        bRes+=copyFromOneFolderToAnother(folderHiddenFromSource,folderKnownToSource,inputFile);
      }
    }
    bRes+="cd "+folderKnownToSource+"\n";
    for(long i=0;i<numSrc;++i){
      if(ced.language[i]=="cpp"){
        wordThatStartWithDot=".";
        wordThatStartWithDot+=ced.language[i];
        bRes+=bashCommandsToCompileAndExecuteCpp(
          fileFullName(ced.sourceFNBase,wordThatStartWithDot,i),
          ced.compilerFlags[i],
          ced.inDataFNBase,
          ced.outDataFNBase,
          ced.errDataFNBase,
          ced.sourceFNBase,i,
          ced.inputData[i].size(),
          timeOutTime
        );
      }
      if(ced.language[i]=="py"){
        wordThatStartWithDot=".";
        wordThatStartWithDot+=ced.language[i];
        bRes+=bashCommandsToCompileAndExecutePython3(
          fileFullName(ced.sourceFNBase,wordThatStartWithDot,i),
          ced.inDataFNBase,ced.outDataFNBase,ced.errDataFNBase,
          ced.sourceFNBase,i,
          ced.inputData[i].size(),
          timeOutTime
        );
      }
    }
    bRes+=overwriteBigOutputs(ced.outDataFNBase,ced.inputData,1025);
    long numOuts=ced.inputData.size();
    long numOutsI;
    for(long i=0;i<numOuts;++i){
      numOutsI=ced.inputData[i].size();
      for(long j=0;j<numOutsI;++j){
        bRes+=copyFromOneFolderToAnother(folderKnownToSource,folderHiddenFromSource,fileFullName(ced.outDataFNBase,".txt",i,j));
        bRes+=copyFromOneFolderToAnother(folderKnownToSource,folderHiddenFromSource,fileFullName(ced.errDataFNBase,".txt",i,j));
      }
    }
    return bRes;
  }
  int executeInContainerAndWriteFiles(const PSDI::SessionData & _psd, const ContainerExecutionData& ced){
    putTextsIntoFiles(myMountFolder(_psd.my_un),ced.sourceCode,ced.sourceFNBase,".",ced.language);
    long numFiles=ced.inputData.size();
    for(long i=0;i<numFiles;++i){
      putTextsIntoFiles(myMountFolder(_psd.my_un),ced.inputData[i],fileFullName(ced.inDataFNBase,GL_DCEI_const.separatorBetweenBigNumbersInNames,i),".txt");
    }
    std::string bExecFileFullPath=myMountFolder(_psd.my_un)+"/"+ced.bashExecFileName;
    IOF::toFile(bExecFileFullPath,createBashForExecution(ced));
    IOF::sys_changePermissions(bExecFileFullPath,"775");
    IOF::sys_executeCommandThatDoesNotPrint(ced.dockerCommand);
    return 1;
  }
  std::vector<std::vector<std::string> > collectOutputAndDeleteFiles(const PSDI::SessionData & _psd, const ContainerExecutionData & ced){
    std::vector<std::vector<std::string> > res;
    long numFiles=ced.inputData.size();
    res.resize(numFiles);
    long szI;
    std::string mFolder=myMountFolder(_psd.my_un)+"/";
    std::string outFileName,errFileName;
    for(long i=0;i<numFiles;++i){
      szI=ced.inputData[i].size();
      res[i].resize(szI);
      for(long j=0;j<szI;++j){
        outFileName=mFolder+ fileFullName(ced.outDataFNBase,".txt",i,j);
        errFileName=mFolder+ fileFullName(ced.errDataFNBase,".txt",i,j);
        res[i][j]=IOF::fileToString(outFileName,1);
        if(SF::findAndReplace(res[i][j],GF::GL_errorDidNotCompile,"")==res[i][j]){
          res[i][j]=SF::findAndReplace(res[i][j],"<",".");
          res[i][j]=SF::findAndReplace(res[i][j],">",",");
          res[i][j]=SF::findAndReplace(res[i][j],"_"," ");
        }
        else{
          res[i][j]=IOF::fileToString(errFileName,1);
        }
      }
    }
    IOF::sys_deleteFolderAndSubfolders(myMountFolder(_psd.my_un));
    return res;
  }
  std::pair<std::string,std::string> replaceDangerWords(const std::string & _in, const std::vector<std::string> & listDangers, const std::string & startReplWord){
    std::pair<std::string,std::string>out;
    out.first=_in;
    std::string replacementWord=startReplWord;
    long numDangers=listDangers.size();
    for(long i=0;i<numDangers;++i){
      replacementWord=SF::findYForNonSubstring(out.first,replacementWord,"b","e");
      out.first=SF::findAndReplace(out.first,listDangers[i],"b"+replacementWord+"e");
    }
    out.second=replacementWord;
    return out;
  }
  std::string makeCPPSourceSafe(const std::string &_src,const std::vector<std::string> &incl){
    std::string out=_src;
    std::string replacementWord="aaa";
    std::pair<std::string,std::string> improvedCode;
    improvedCode=replaceDangerWords(out,GL_DCEI_const.dangerousCPPCommands,replacementWord);
    out=improvedCode.first;
    replacementWord=improvedCode.second;
    improvedCode=replaceDangerWords(out,GL_DCEI_const.dangerousCPPWords,replacementWord);
    out=improvedCode.first;
    replacementWord=improvedCode.second;
    out=SF::findAndReplace(out,"#","//#");
    long isz=incl.size();
    if(isz>0){
      while(isz>0){
        --isz;
        out=incl[isz]+"\n"+out;
      }
    }
    else{
      out="#include<iostream>\n"+out;
    }
    return out;
  }
  std::string makePython3SourceSafe(const std::string &_src,const std::vector<std::string> &incl){
    std::string out=_src;
    std::string replacementWord="aaa";
    std::pair<std::string,std::string> improvedCode;
    improvedCode=replaceDangerWords(out,GL_DCEI_const.dangerousPythonCommands,replacementWord);
    out=improvedCode.first;
    replacementWord=improvedCode.second;
    out=SF::findAndReplace(out,"from","#from");
    out=SF::findAndReplace(out,"import","#import");
    long isz=incl.size();
    if(isz>0){
      while(isz>0){
        --isz;
        out=incl[isz]+"\n"+out;
      }
    }
    return out;
  }
  std::vector<std::string> improveCPPIncludes(const std::vector<std::string> &oldIncl){
    std::vector<std::string> newIncl=oldIncl;
    long sz=oldIncl.size();
    std::string tmp;
    for(long i=0;i<sz;++i){
      tmp=SF::findAndReplace(oldIncl[i],"#","");
      if(tmp==oldIncl[i]){
        newIncl[i]="#include<"+tmp+">";
      }
    }
    return newIncl;
  }
  std::vector<std::string> improvePythonIncludes(const std::vector<std::string> &oldIncl){
    std::vector<std::string> newIncl=oldIncl;
    long sz=oldIncl.size();
    std::string tmp;
    for(long i=0;i<sz;++i){
      tmp=SF::findAndReplace(oldIncl[i],"from","");
      if(tmp==oldIncl[i]){
        tmp=SF::findAndReplace(oldIncl[i],"import","");
        if(tmp==oldIncl[i]){
          newIncl[i]="import "+tmp;
        }
      }
    }
    return newIncl;
  }
  std::string makeSourceSafe(const std::string & _src,const std::string & _lang,const std::vector<std::string> & incl,const std::vector<std::string> & custIncl){
    std::string src=_src;
    long cisz=custIncl.size();
    for(long i=0;i<cisz;++i){
      src=custIncl[i]+"\n"+src;
    }
    if(_lang=="cpp"){
      return makeCPPSourceSafe(src,improveCPPIncludes(incl));
    }
    if(_lang=="py"){
      return makePython3SourceSafe(src,improvePythonIncludes(incl));
    }
    return src;
  }
  std::vector<std::string> makeSourcesSafe(const std::vector<std::string> & _src, const std::vector<std::string> &_lang,const std::vector<std::vector<std::string> > &_incl,const std::vector<std::vector<std::string> > &_custIncl){
    std::vector<std::string> res;
    long sz=_src.size();
    res.resize(sz);
    for(long i=0;i<sz;++i){
      res[i]=makeSourceSafe(_src[i],_lang[i],_incl[i],_custIncl[i]);
    }
    return res;
  }
  std::pair<std::vector<std::vector<std::string> >,int> executePrograms(const PSDI::SessionData & _psd, const std::vector<std::string> & _sources, const std::vector<std::string> & _languages, const std::vector<std::string> & _cFlags, const std::vector<std::vector<std::string> > & includes, const std::vector<std::vector<std::string> > & customIncludes, const std::vector<std::vector<std::string> > & _inputData){
    std::pair<std::vector<std::vector<std::string> >,int> res;
    res.second=0;
    createTempFolderForMounting(_psd.my_un);
    long sz=_sources.size();
    long langSz=_languages.size();
    long inDSz=_inputData.size();
    long inclSz=includes.size();
    long cInclSz=customIncludes.size();
    long test=0;
    test+= (sz-langSz)*(sz-langSz);
    test+=(sz-inDSz)*(sz-inDSz);
    test+=(sz-inclSz)*(sz-inclSz);
    test+=(sz-cInclSz)*(sz-cInclSz);
    if(test>0){return res;}
    res.second=1;
    ContainerExecutionData mainCED;
    std::pair<std::string,long> unid=usernameAndId(_psd);
    mainCED.myLinuxUserName=unid.first;
    mainCED.myLinuxUserId=unid.second;
    mainCED.language=_languages;
    mainCED.compilerFlags=_cFlags;
    mainCED.sourceCode=makeSourcesSafe(_sources,_languages,includes,customIncludes);
    mainCED.inputData=_inputData;
    mainCED.sourceFNBase="sfile";
    mainCED.inDataFNBase="ind";
    mainCED.outDataFNBase="outd";
    mainCED.errDataFNBase="errd";
    mainCED.bashExecFileName="befile";
    mainCED.folderInContainer="/fmountPoint/f"+DD::GL_DBS.getInitExtension();
    mainCED.dockerCommand="docker run --rm --name test_container_001 -v ";
    mainCED.dockerCommand+=myMountFolder(_psd.my_un)+":";
    mainCED.dockerCommand+=mainCED.folderInContainer+" -u "+std::to_string(mainCED.myLinuxUserId)+" ";
    mainCED.dockerCommand+=DD::GL_DBS.getImageName()+" /bin/bash "+mainCED.folderInContainer+"/"+mainCED.bashExecFileName;
    res.second= executeInContainerAndWriteFiles(_psd,mainCED);
    res.first=collectOutputAndDeleteFiles(_psd,mainCED);
    return res;
  }
}
#endif
