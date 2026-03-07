//    GradeYard learning management system
//
//    Copyright (C) 2026 Ivan Matic, https://gradeyard.com
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


#ifndef _INCL_htmlTagStorageForCodeBox_CPP
#define _INCL_htmlTagStorageForCodeBox_CPP

namespace HCBF{
  std::map<std::string, std::string > vectTagsToBeSaved(){
    std::map<std::string,std::string > res;
    res["insert_"]="verIns!**!_";
    res["variables_"]="verVar!**!_";
    res["\n"]="*newLn!**!";
    res["<"]="*less!**!";
    res[">"]="*grea!**!";
    return res;
  }
  std::map<std::string,std::string> verySafePlace=vectTagsToBeSaved();
  std::map<std::string,std::string> verySafePlaceInv=BF::inverse(verySafePlace);
}


#endif
