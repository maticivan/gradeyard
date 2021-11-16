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


#ifndef _INCL_DataFileManagement_CPP
#define _INCL_DataFileManagement_CPP


namespace DMF{
  std::string dataFileNameFromPointerData(const std::string &pFileName, const long &posInNode){
    std::string prefPT=HDPF::GLOBAL_PS.getPrefixFilesPT();
    std::string prefDT=HDPF::GLOBAL_PS.getPrefixFilesDT();
    std::string fR= IOF::nameOfFolder(pFileName)+"/"+prefDT+IOF::justFileNameNoExtensionNoFolder(pFileName,prefPT);
    fR+="_"+BF::padded(posInNode,1000,"0");
    fR+="."+HDPF::GLOBAL_PS.getExtension();
    return fR;
  }

  template<typename TTT>
  std::string dataFNameFromIRFN(const IRFNF::IndRecFName<TTT> & input){
    return dataFileNameFromPointerData(input.fileName,input.positionInNode);
  }

  template<typename TTT>
  int deleteDataFile(const HDPF::Pointer<HBF::Node<TTT> > & pOldNode, const long & posOldNode){
    if(pOldNode.isNullptr()){
      return 0;
    }
    std::string oldFileName=dataFileNameFromPointerData( pOldNode.getNameOfFile(),posOldNode);
    return IOF::sys_deleteFile(oldFileName);
  }

  template<typename TTT>
  int renameDataFile(const HDPF::Pointer<HBF::Node<TTT> > & pOldNode,
                     const long & posOldNode,
                     const HDPF::Pointer<HBF::Node<TTT> > & pNewNode,
                     const long & posNewNode){
    if(pOldNode.isNullptr()){
      return 0;
    }
    if(pNewNode.isNullptr()){
      return 0;
    }
    std::string oldFileName=dataFileNameFromPointerData( pOldNode.getNameOfFile(),posOldNode);
    std::string newFileName=dataFileNameFromPointerData( pNewNode.getNameOfFile(), posNewNode);

    return IOF::sys_changeNameOfFile(oldFileName, newFileName);
  }

  template<typename TTT>
  int shiftLeftDFNames(const HDPF::Pointer<HBF::Node<TTT> > & rootOld, const HDPF::Pointer<HBF::Node<TTT> > & rootNew,  const long & start, const long &end, const long &shiftSize){
    int success=1;
    for(long i=start;i<end;++i){
      success *= renameDataFile(rootOld,i,rootNew,i-shiftSize);
    }
    return success;
  }
  template<typename TTT>
  int shiftRightDFNames(const HDPF::Pointer<HBF::Node<TTT> > & rootOld, const HDPF::Pointer<HBF::Node<TTT> > & rootNew, const long & start, const long &end, const long &shiftSize){
    int success=1;
    long sm1=start-1;
    for(long i=end-1;i>sm1;--i){
      success *= renameDataFile(rootOld,i,rootNew,i+shiftSize);
    }
    return success;
  }
}



#endif
