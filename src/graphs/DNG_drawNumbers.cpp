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

#ifndef _INCL_drawNumberS_CPP
#define _INCL_drawNumberS_CPP

namespace DNG{
  std::vector<DRG::G3DPoint> drawNumberOne(){
    std::vector<DRG::G3DPoint> oneFace;
    oneFace.clear();
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,9.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,9.0/16.0));
    return oneFace;
  }

  std::vector<DRG::G3DPoint> drawNumberThree(){
    std::vector<DRG::G3DPoint> oneFace;

    oneFace.clear();
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,9.0/16.0));
    return oneFace;
  }

  std::vector<DRG::G3DPoint> drawNumberTwo(){
    std::vector<DRG::G3DPoint> oneFace;
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,9.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,9.0/16.0));
    return oneFace;
  }
  std::vector<DRG::G3DPoint> drawNumberFour(){
    std::vector<DRG::G3DPoint> oneFace;
    oneFace.clear(); 
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,10.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,10.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,13.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,13.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,6.0/16.0));
    return oneFace;
  }
  std::vector<DRG::G3DPoint> drawNumberFive(){
    std::vector<DRG::G3DPoint> oneFace;
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,13.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,3.0/16.0));

    return oneFace;
  }
  std::vector<DRG::G3DPoint> drawNumberSix(){
    std::vector<DRG::G3DPoint> oneFace;
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,9.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,9.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,7.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,8.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,11.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,3.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,5.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,9.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,6.0/16.0,11.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,8.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,9.0/16.0,13.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,12.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,14.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,12.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,10.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,4.0/16.0,6.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,5.0/16.0,4.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,7.0/16.0,2.0/16.0));
    oneFace.push_back(DRG::G3DPoint(0.0,10.0/16.0,2.0/16.0));
    return oneFace;
  }


  std::vector<DRG::G3DPoint> drawNumber(const std::string &_i){
    std::vector<DRG::G3DPoint> fR;
    if(_i=="1"){
      return drawNumberOne();
    }
    if(_i=="2"){
      return drawNumberTwo();
    }
    if(_i=="3"){
      return drawNumberThree();
    }
    if(_i=="4"){
      return drawNumberFour();
    }
    if(_i=="5"){
      return drawNumberFive();
    }
    if(_i=="6"){
      return drawNumberSix();
    }
    return fR;
  }
}
#endif
