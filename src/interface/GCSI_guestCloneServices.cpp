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


#ifndef _INCL_GUESTCLONESERVICES_CPP
#define _INCL_GUESTCLONESERVICES_CPP

namespace GCSI{
  int deleteClonedWebsite(const std::string & userWithWebsite){
    IOF::sys_deleteFolderAndSubfolders(DD::GL_DBS.getGuestClonesRelLoc()+userWithWebsite);
    IOF::sys_deleteFolderAndSubfolders(DD::GL_DBS.getClonesMainDBFolder()+"/"+userWithWebsite);
    return 1;
  }
  int preparePublicFolder(const std::string & userWithWebsite, const std::string & encPassword){
    std::string setupText=IOF::fileToString( MWII::GL_WI.getClonesSystemFolder()+"/"+ GL_MAIN_SETUP_FILE_NAME);
    setupText=SF::findAndReplace(setupText,
                            "_mainFolderDB*!_"+DD::GL_DBS.getMainFolder()+"_/mainFolderDB*!_",
                            "_mainFolderDB*!_"+DD::GL_DBS.getClonesMainDBFolder()+"/"+userWithWebsite +"_/mainFolderDB*!_");
    setupText=SF::findAndReplace(setupText,
                            "_websiteURL*!_"+MWII::GL_WI.getWSURL()+"_/websiteURL*!_",
                            "_websiteURL*!_"+MWII::GL_WI.getWSURL()+"/"+DD::GL_DBS.getGuestClonesRelLoc()+userWithWebsite+"_/websiteURL*!_");
    setupText=SF::findAndReplace(setupText,
                            "_cookiePath*!_"+MWII::GL_WI.getCookiePath()+"_/cookiePath*!_",
                            "_cookiePath*!_"+MWII::GL_WI.getCookiePath()+"/"+DD::GL_DBS.getGuestClonesRelLoc()+userWithWebsite+"_/cookiePath*!_");
    setupText=SF::findAndReplace(setupText,"_cookiePath*!_//","_cookiePath*!_/");
    setupText=SF::findAndReplace(setupText,
                            "_cookieName*!_"+MWII::GL_WI.getCookieName()+"_/cookieName*!_",
                            "_cookieName*!_"+MWII::GL_WI.getCookieName()+userWithWebsite+"_/cookieName*!_");
    std::string wFolder=DD::GL_DBS.getGuestClonesRelLoc()+userWithWebsite;
    int folderCreated=IOF::sys_createFolderIfDoesNotExist(wFolder,GL_MAIN_SETUP_FILE_NAME,setupText);
    IOF::sys_copyAllFilesButIgnoreThoseThatExist( MWII::GL_WI.getClonesSystemFolder(),wFolder);
    std::string fToUpdate, fContent;
    fToUpdate=wFolder;
    fToUpdate+="/mainText"+DD::GL_DBS.getInitExtension()+".txt";
    fContent=IOF::fileToString(fToUpdate,1);
    fContent=SF::findAndReplace(fContent,"_THIS*WEBSITE*URL*_",MWII::GL_WI.getWSURL());
    fContent=SF::findAndReplace(fContent,">Home</a>",">"+userWithWebsite+"</a>");
    IOF::toFile(fToUpdate,fContent);
    fToUpdate=wFolder;
    fToUpdate+="/uns"+DD::GL_DBS.getInitExtension()+".txt";
    fContent=IOF::fileToString(fToUpdate,1);
    std::vector<std::string> individualUsers=SF::stringToVector(fContent,"_nextCommand!*!!_","_/nextCommand!*!!_");
    long sz=individualUsers.size();
    if(sz>1){
      std::string lastUserData=individualUsers[sz-1];
      fContent="";
      for(long i=0;i<sz-1;++i){
        fContent += "_nextCommand!*!!_\n"+individualUsers[i]+"_/nextCommand!*!!_\n\n";
      }

      fContent+="_nextCommand!*!!_\n";
      std::vector<std::string> itemsInLastData=SF::stringToVector(lastUserData,"_n*!!***!_","_/n*!!***!_");
      long correctSizeForItemsInInsertUserCommand=4;
      if(itemsInLastData.size()==correctSizeForItemsInInsertUserCommand){
        itemsInLastData[1]=BF::padded(BF::stringToInteger(itemsInLastData[1]),100000000,"0");
        itemsInLastData[1]="0000000"+itemsInLastData[1];
        itemsInLastData[2]=userWithWebsite;
        itemsInLastData[3]="_password!*_"+encPassword+"_/password!*_";
        itemsInLastData[3]+="_firstName!*_Adm02_/firstName!*__lastName!*_Adm02_/lastName!*_";
        itemsInLastData[3]+="_email!*_adm02_/email!*__isRoot!*_yes_/isRoot!*__aEC!*_yes_/aEC!*_";
        for(long i=0;i<correctSizeForItemsInInsertUserCommand;++i){
          fContent+="\n";
          fContent+="_n*!!***!_"+itemsInLastData[i]+"_/n*!!***!_";
          fContent+="\n";
        }
        fContent+="_/nextCommand!*!!_\n";
        IOF::toFile(fToUpdate,fContent);
        fToUpdate=wFolder;
        fToUpdate+="/eIDT"+DD::GL_DBS.getInitExtension()+".txt";
        fContent=IOF::fileToString(fToUpdate,1);
        fContent+="_nextCommand!*!!_ \n\n _n*!!***!_insert_/n*!!***!_ \n\n _n*!!***!_"+userWithWebsite;
        fContent+="ID_/n*!!***!_\n_n*!!***!_"+ itemsInLastData[1]+"_/n*!!***!_ \n\n_n*!!***!_\n\n _/n*!!***!_\n_/nextCommand!*!!_\n";
        IOF::toFile(fToUpdate,fContent);
        fToUpdate=wFolder;
        fToUpdate+="/bossDB"+DD::GL_DBS.getInitExtension()+".txt";
        fContent=IOF::fileToString(fToUpdate,1);
        std::string beforeNum;
        std::string afterNum;
        beforeNum="_nextCommand!*!!_ \n\n_n*!!***!_insert_/n*!!***!_\n\n_n*!!***!_";
        beforeNum+=itemsInLastData[1];
        beforeNum+="_/n*!!***!_\n_n*!!***!_000000000000000";
        afterNum="_/n*!!***!_\n\n_n*!!***!_\n\n_/n*!!***!_\n_/nextCommand!*!!_  \n";
        for(long i=1;i<5;++i){
          fContent+=beforeNum+std::to_string(i)+afterNum;
        }
        IOF::toFile(fToUpdate,fContent);
      }
    }
    return 1;
  }
  int indHTMLExistsForGuest(const std::string &guestUName){
    return IOF::fileExists(guestUName+"/index.html");
  }
  int createClonedWebsite(const std::string &guestUName, const std::string & encPassword){
    if(IOF::legalFileName(guestUName)==0){
      return 0;
    }
    if(guestUName!=SF::toLowerCase(guestUName)){
      return 0;// only usernames that are pure lowercase characters can have clones.
      // The URLs are not case sensitive
    }
    if(DD::GL_DBS.getGuestClonesRelLoc()==""){
      if(indHTMLExistsForGuest(guestUName)){
        return 0;
      }
    }

    deleteClonedWebsite(guestUName);
    preparePublicFolder(guestUName,encPassword);
    return 1;
  }
}
#endif
