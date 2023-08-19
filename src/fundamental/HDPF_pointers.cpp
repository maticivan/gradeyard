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


#ifndef _INCL_MYPointerS_CPP
#define _INCL_MYPointerS_CPP
#include "PS00F_pointersSetup.cpp"

namespace HDPF{
  // GLOBAL SETUP FILE LOADING
  PS00F::PSetup GLOBAL_PS;
  // END OF GLOBAL SETUP FILE LOADING
  template<class TTT>
  class Pointer{
  private:
      std::string nameOfFile="nullptrFN";
      PS00F::PSetup pS=GLOBAL_PS;
  public:
      void changeSetup(const PS00F::PSetup& );
      std::string getNameOfFile() const;
      void setNameOfFile(const std::string &);
      int isNullptr() const;
      // reads the file nameOfFile and returns the object stored in the file
      TTT deRefRead() const;
      // puts the object in the file nameOfFile
      int deRefWrite(const TTT &) const;//returns 1 if the object was successfully written to a file
      // find the new available fileName and places it in nameOfFile;
      int allocateMemory(const std::string &);
      //  int allocateMemory();
      // deletes the object from nameOfFile and destroys the file.
      int deAllocateMemory() const;
      std::string putIntoString() const;
      void loadFromString(const std::string &);
      std::string statusReportForDebugging() const;
  };
  template<class TTT> std::string Pointer<TTT>::statusReportForDebugging() const{
    std::string fR;
    fR= "name of file "+nameOfFile;
    return fR;
  }
  template<class TTT>
  int Pointer<TTT>::isNullptr()const{
      if(nameOfFile=="nullptrFN"){
          return 1;
      }
      return 0;
  }
  template<class TTT>
  void Pointer<TTT>::changeSetup(const PS00F::PSetup &c){
      pS=c;
  }
  template<class TTT>
  std::string Pointer<TTT>::getNameOfFile() const{return nameOfFile;}
  template<class TTT>
  void Pointer<TTT>::setNameOfFile(const std::string & x){nameOfFile=x;}
  template<class TTT>
  TTT Pointer<TTT>::deRefRead() const{
      TTT fR;
      std::string s=IOF::fileToString(nameOfFile);
      fR.loadFromString(s);
      return fR;
  }
  template<class TTT>
  int Pointer<TTT>::deRefWrite(const TTT & obj) const{
      if(nameOfFile=="nullptrFN"){
          return 0;
      }
      return obj.putToFile(nameOfFile);
  }
  template<class TTT>
  int Pointer<TTT>::allocateMemory(const std::string & _stFN){
      std::string fDynData,fReadyNames;
      std::string lastUsedName;
      std::string folder=_stFN;
      fDynData=folder+"/"+pS.getFileWithDynData();
      fReadyNames=folder+"/"+pS.getAvailableNamesF();
      std::string newFileContent=pS.getNextNameB()+"1"+pS.getNextNameE();
      std::string newAvFilesContent=" ";
      std::string oldFileContent,oldAvFilesCont;
      std::string lastFileName;
      int foundMemory=0;
      std::string maxSt=pS.getMaxString();
      std::string nextFileName;
      std::pair<std::string,int> avNameFree;long pos;
      while(foundMemory==0){
          std::ifstream fTest(fDynData);
          if(!fTest.good()){
              IOF::sys_mkdir(folder);
              IOF::toFile(fDynData,newFileContent);
              IOF::toFile(fReadyNames,newAvFilesContent);
              foundMemory=1;
              nameOfFile=folder+"/"+pS.getPrefixFilesPT()+"1."+pS.getExtension();
          }
          else{
              oldAvFilesCont=IOF::fileToString(fReadyNames);
              oldFileContent=IOF::fileToString(fDynData);
              pos=0;
              // Step 1. Read if there was a deleted file that made a name available
              avNameFree=SF::extract(oldAvFilesCont,pos,pS.getNextEmptyB(),pS.getNextEmptyE());
              pos=0;
              if(avNameFree.second==1){
                  nextFileName=avNameFree.first;
                  foundMemory=1;
                  oldAvFilesCont=(SF::eraseStuffBetween(oldAvFilesCont,pS.getNextEmptyB(),pS.getNextEmptyE())).first;
                  nameOfFile=folder+"/"+pS.getPrefixFilesPT()+nextFileName+"."+pS.getExtension();
                  IOF::toFile(fReadyNames,oldAvFilesCont);
              }
              else{// There are no free names.
                  nextFileName=(SF::extract(oldFileContent,pos,pS.getNextNameB(),pS.getNextNameE())).first;
                  if(nextFileName.size()<maxSt.size()){
                      foundMemory=1;
                  }
                  if((foundMemory==0)&&(nextFileName.size()==maxSt.size())){
                      if(nextFileName<maxSt){
                          foundMemory=1;
                      }
                  }
                  if(foundMemory==0){
                      folder += "/"+pS.getPrefixSubDirs();
                      fDynData=folder+"/"+pS.getFileWithDynData();
                      fReadyNames=folder+"/"+pS.getAvailableNamesF();
                  }
                  else{
                      nextFileName=SF::incrementString(nextFileName);
                      nameOfFile=folder+"/"+pS.getPrefixFilesPT()+nextFileName+"."+pS.getExtension();
                      oldFileContent=pS.getNextNameB()+nextFileName+pS.getNextNameE();
                      IOF::toFile(fDynData,oldFileContent);
                  }
              }
          }
      }
      return 1;
  }
  template<class TTT>
  int Pointer<TTT>::deAllocateMemory() const{
      std::ifstream fTest(nameOfFile);
      if(fTest.good()){
          std::string fReadyNames=IOF::nameOfFolder(nameOfFile)+"/"+pS.getAvailableNamesF();
          std::string oldRNames=IOF::fileToString(fReadyNames);
          oldRNames+=pS.getNextEmptyB()+IOF::justFileNameNoExtensionNoFolder(nameOfFile,pS.getPrefixFilesPT())+pS.getNextEmptyE();
          IOF::toFile(fReadyNames,oldRNames);
          IOF::sys_deleteFile(nameOfFile);
          return 1;
      }
      return 0;
  }
  template<class TTT>
  void Pointer<TTT>::loadFromString(const std::string &s){
    nameOfFile=s;
  }
  template<class TTT>
  std::string Pointer<TTT>::putIntoString() const{
    return nameOfFile;
  }
}

#endif
