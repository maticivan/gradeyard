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

#ifndef _INCL_IndexRecordFileName_CPP
#define _INCL_IndexRecordFileName_CPP




namespace IRFNF{
  template<typename TTT> class IndRecFName{
  public:
    long index;
    TTT record;
    HDPF::Pointer<HBF::Node<TTT> > aRecord;
    std::string fileName;
    long positionInNode;
    IndRecFName();
  };

  template<typename TTT> IndRecFName<TTT>::IndRecFName(){
    index = -1;
    positionInNode = -1;
    fileName="notFound";
  }

}


#endif
