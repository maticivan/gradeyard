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


#ifndef _INCL_IF_IO_CPP
#define _INCL_IF_IO_CPP


namespace IOF{

  int sys_deleteFolderAndSubfolders(const std::string &folderName){
    std::string command="rm -rf "+folderName;
    system(command.c_str());
    return 1;
  }
  int sys_deleteFile(const std::string &nameOfFile){
    std::map<std::string,std::string>::iterator it,itE;
    itE=GF::GL_OPENED_FILES.end();
    it=GF::GL_OPENED_FILES.find(nameOfFile);
    if(it!=itE){
      GF::GL_OPENED_FILES.erase(it);
    }
    std::ifstream fTest(nameOfFile);
    if(fTest.good()){
      std::string systemCommand="rm ";
      systemCommand+=nameOfFile;
      system(systemCommand.c_str());
    }
    return 1;
  }
  int sys_clearFolder(const std::string &folderName,const std::string & extension="*"){
    std::string slashIfMissing="/";
    std::string dotIfMissing=".";
    if((folderName.length()>0) &&(folderName[folderName.length()-1]=='/')){
      slashIfMissing="";
    }
    if((extension.length()>0) && (extension[0]=='.') ){
      dotIfMissing="";
    }
    std::string command="rm "+folderName+slashIfMissing+"*"+dotIfMissing+extension;
    system(command.c_str());
    return 1;
  }
  std::vector<std::string> listFiles(const std::string &dirPath){
    std::vector<std::string> fR;
    long sz=0;
    for (const auto & entry : std::experimental::filesystem::directory_iterator(dirPath)){
      ++sz;
    }
    fR.resize(sz);
    long i=0;
    for (const auto & entry : std::experimental::filesystem::directory_iterator(dirPath)){
      fR[i]= entry.path();
      ++i;
    }
    return fR;
  }
  int fileExists(const std::string &fN){
      std::ifstream fTest(fN);
      if(fTest.good()){
          return 1;
      }
      return 0;
  }
  std::string fileToString(const std::string &filename,const long & getFromDiskNotFromCache=0){
      if(getFromDiskNotFromCache==0){
        std::map<std::string,std::string>::iterator it,itE;
        itE=GF::GL_OPENED_FILES.end();
        it=GF::GL_OPENED_FILES.find(filename);
        if(it!=itE){
          return it->second;
        }
      }
      if(!fileExists(filename)){
        std::map<std::string,std::string>::iterator it,itE;
        itE=GF::GL_OPENED_FILES.end();
        it=GF::GL_OPENED_FILES.find(filename);
        if(it!=itE){
          GF::GL_OPENED_FILES.erase(it);
        }
        return("fileNotFound");
      }
      std::ifstream t(filename);
      t.seekg(0, std::ios::end);
      size_t size = t.tellg();
      std::string buffer(size, ' ');
      t.seekg(0);
      t.read(&buffer[0], size);
      if(GF::GL_OPENED_FILES.size()<GF::GL_MAXFILESINRAM){
        GF::GL_OPENED_FILES[filename]=buffer;
      }
      return buffer;
  }
  int sys_changeNameOfFile(const std::string & f1, const std::string & f2){
    if(fileExists(f2)){
      return 0;
    }
    if(!fileExists(f1)){
      return 1;
    }
    std::string systemCommand="mv "+f1+" "+f2;
    system(systemCommand.c_str());
    fileToString(f1,1);//refreshing RAM
    fileToString(f2,1);//refreshing RAM
    return 1;
  }
  int sys_changePermissions(const std::string & fileName, const std::string & permission="664"){
    std::string systemCommand="chmod "+permission+" "+fileName;
    system(systemCommand.c_str());
    return 1;
  }
  int sys_copyFile(const std::string & originalFile, const std::string & newFile){
    std::string systemCommand="cp "+originalFile+" "+newFile;
    system(systemCommand.c_str());
    return 1;
  }
  int sys_copyAllFilesButIgnoreThoseThatExist(const std::string & src, const std::string & dst){
    std::string command="rsync -a -v -q --ignore-existing "+src+"/*.* "+dst+"/";
    system(command.c_str());
    return 1;
  }
  int sys_runLatexCommand(const std::string & _lComm){
    std::string lComm=_lComm+" > /dev/null 2>&1";
    system(lComm.c_str());
    return 1;
  }
  int sys_executeCommandThatDoesNotPrint(const std::string & command){
    system(command.c_str());
    return 1;
  }
  std::string sys_executeCommandThatPrints(const std::string & command, const std::string & filePathToSaveData){
    std::string command1=command+" > "+filePathToSaveData;
    system(command1.c_str());
    std::string result=IOF::fileToString(filePathToSaveData,1);
    IOF::sys_deleteFile(filePathToSaveData);
    return result;
  }
  int toFile(const std::string & filename, const std::vector<std::string> & s){
      sys_deleteFile(filename);
      std::ofstream mfile;
      mfile.open(filename);
      long l=s.size();
      for(long i=0;i<l;i++){
          mfile<<s[i]<<"\n";
      }
      mfile<<std::endl;
      mfile.close();
      sys_changePermissions(filename);
      fileToString(filename,1);// refreshing RAM
      return 1;
  }
  int toFile(const std::string &filename, const  std::string  & s){
      sys_deleteFile(filename);
      std::ofstream mfile;
      mfile.open(filename);
      mfile<<s;
      mfile.close();
      sys_changePermissions(filename);
      fileToString(filename,1);// refreshing RAM
      return 1;
  }
  int toFile(const std::string & filename, const  std::set<std::string>  & s){
      sys_deleteFile(filename);
      std::ofstream mfile;
      mfile.open(filename);
      std::set<std::string>::iterator it;
      for(it=s.begin();it!=s.end();++it){
        mfile<<*it<<"\n";
      }
      mfile.close();
      sys_changePermissions(filename);
      fileToString(filename,1);// refreshing RAM
      return 1;
  }
  std::time_t timeOfCreation(const std::string & fName){
    std::experimental::filesystem::file_time_type lwt= std::experimental::filesystem::last_write_time(fName);
    std::time_t ttlwt= std::experimental::filesystem::file_time_type::clock::to_time_t(lwt);
    return ttlwt;
  }
  std::vector<std::time_t> timeOfCreation(const std::vector<std::string> & fNs){
    std::vector<std::time_t> fR;
    long sz=fNs.size();
    if(sz>0){
      fR.resize(sz);
      for(long i=0;i<sz;++i){
        fR[i]=timeOfCreation(fNs[i]);
      }
    }
    return fR;
  }
  std::string nameOfFolder(const std::string & fullNameOfFile){
      std::string rnameOfFolder="";
      long sz=fullNameOfFile.size();
      while(sz>0){
          --sz;
          rnameOfFolder += fullNameOfFile[sz];
      }
      rnameOfFolder="!"+rnameOfFolder;
      rnameOfFolder=rnameOfFolder+"/";
      rnameOfFolder=(SF::eraseStuffBetween(rnameOfFolder,"!","/")).first;
      std::string nF="";
      sz=rnameOfFolder.size()-1;
      while(sz>0){
          --sz;
          nF +=rnameOfFolder[sz];
      }
      return nF;
  }
  int legalFileName(const std::string & in){
    std::set<std::string> badStrings;
    badStrings.insert("/");
    badStrings.insert(".");
    badStrings.insert("\"");
    badStrings.insert("\n");
    badStrings.insert("\t");
    badStrings.insert("\'");
    badStrings.insert("@");
    badStrings.insert("!");
    badStrings.insert("~");
    badStrings.insert("`");
    badStrings.insert("$");
    badStrings.insert("%");
    badStrings.insert("^");
    badStrings.insert("\\");
    badStrings.insert("+");
    badStrings.insert(",");
    badStrings.insert(";");
    badStrings.insert(":");
    badStrings.insert("?");
    badStrings.insert("<");
    badStrings.insert(">");
    if(MFRF::find(in,badStrings).second==-1){
      return 1;
    }
    return 0;
  }
  std::string justFileNameNoExtensionNoFolder(const std::string & nameOfFile,
                                              const std::string & beginningToIgnore=""){
      std::string folder=nameOfFolder(nameOfFile);
      std::string justName="";
      long pos=folder.size();
      long totalS=nameOfFile.size();
      while((pos<totalS)&&(nameOfFile[pos]!='.')){
          justName+=nameOfFile[pos];
          ++pos;
      }
      if(justName[0]=='/'){
          std::string temp=justName;
          justName="";
          pos=temp.size();
          while(pos>1){
              --pos;
              justName=temp[pos]+justName;
          }
      }
      pos=beginningToIgnore.size();
      if(pos>0){
          std::string temp2=justName;
          justName="";
          long sz=temp2.size();
          while(pos<sz){
              justName+=temp2[pos];
              ++pos;
          }
      }
      return justName;
  }
  std::string extensionOfFile(const std::string & fName){
    std::string foldN=nameOfFolder(fName);
    long pos=foldN.size();
    if((pos<fName.size())&&(fName[pos]=='/')){
      ++pos;
    }
    std::string nFWExt=justFileNameNoExtensionNoFolder(fName);
    pos+=nFWExt.size();
    if((pos<fName.size())&&(fName[pos]=='.')){
      ++pos;
    }
    long sz=fName.size();
    std::string fR="";
    while(pos<sz){
      fR+=fName[pos];
      ++pos;
    }
    return fR;
  }

  std::vector<std::string> selectFilesWithExtension(const std::vector<std::string>& vFiles,
                                                    const std::string & _extension){
    std::vector<std::string> fR;
    long num=0;
    long sz=vFiles.size();
    for(long i=0;i<sz;++i){
      if(_extension==extensionOfFile(vFiles[i])){
        ++num;
      }
    }
    fR.resize(num);long counter=0;
    for(long i=0;i<sz;++i){
      if(_extension==extensionOfFile(vFiles[i])){
        fR[counter]=vFiles[i];++counter;
      }
    }
    return fR;
  }
  std::string deleteOldFiles(const std::string & _folderName, const std::string & _extension, const long & defOfOld){
    std::string fR="";
    std::vector<std::string> fList=listFiles( _folderName);
    fList=selectFilesWithExtension(fList,_extension);
    long sz=fList.size();
    std::time_t crT;
    TMF::Timer tm;long age;long tmNow=tm.timeNow();
    for(long i=0;i<sz;++i){
      crT=timeOfCreation(fList[i]);
      age=tmNow-crT;
      if(age>defOfOld){
        sys_deleteFile(fList[i]);
        fR+="; "+fList[i];
      }
    }
    return fR;
  }
  int sys_folderExists(const std::string &folderName){
    struct stat sb;
    if (stat(folderName.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 1;
    }
    return 0;
  }
  int sys_mkdir(const std::string &folderName){
      if(folderName==""){
        return 0;
      }
      if(sys_folderExists(folderName)){
        return 0;
      }
    std::string systemCommand="mkdir "+folderName;
    system(systemCommand.c_str());
    systemCommand="chgrp wadmins " +folderName;
    system(systemCommand.c_str());
    systemCommand="chmod 774 "+folderName;
    system(systemCommand.c_str());
    systemCommand="chmod g+s "+folderName;
    system(systemCommand.c_str());
    return 1;
  }
  int sys_createFolderIfDoesNotExist(const std::string &folderName,
                                     const std::string &fileToTestExistence="readme.txt",
                                     const std::string &contentOfFile="Do not edit this folder.",
                                     const std::string &anotherFileToAdd="noOtherFileToAdd",
                                     const std::string &contOther=" "){
    int fR=0;
    std::string tF=folderName;
    tF+="/"+fileToTestExistence;
    std::ifstream fTest(tF);
    if(!fTest.good()){
        sys_mkdir(folderName);
        toFile(tF,contentOfFile);
        if(anotherFileToAdd!="noOtherFileToAdd"){
          std::string tF2=folderName+"/"+anotherFileToAdd;
          toFile(tF2,contOther);
        }
        fR=1;
    }
    return fR;
  }
}
#endif
