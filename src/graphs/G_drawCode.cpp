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


#ifndef _INCL_DRAW_CODE_CPP
#define _INCL_DRAW_CODE_CPP

namespace G{

  std::pair<std::string, std::string> prepareChallengeCode(const std::string & aN, const long & len){
    long sizeOfPlaceToKeep=10;
    long sz=aN.length();
    std::string code;
    std::string placeToKeep;
    for(long i=0;i<len;++i){
      code += aN[RNDF::randNum(sz)];
    }
    placeToKeep=RNDF::genRandCode(sizeOfPlaceToKeep);
    return std::pair<std::string, std::string>(code,placeToKeep);
  }

  long polyhedronToPNG(DRG::G3DPolyhedron &f_1,const std::string &fileName){



      DRG::Plot plot;


      G3Dint dimension,xCenter,yCenter;
      //dimension=1024;
      //xCenter=380;
      //yCenter=380;
      //yCenter=256;
      dimension=1024;
      xCenter=280;
      yCenter=70;
      plot.initialize(dimension,350);
      plot.setOrigin(xCenter,yCenter);
      plot.setLineThickness(5);
      plot.setAxisLengths(2.5,0.12);


      plot.drawPolyhedron(f_1);
      plot.setFileName(fileName+".png");
      plot.putToFile();
      return 1;
  }
  long polyhedronToGIF(DRG::G3DPolyhedron & p, const std::string &_f){
    polyhedronToPNG(p,_f);
    std::string sys_command1="convert "+_f+".png"+" "+_f+".gif";
    std::string sys_command2="rm "+_f+".png";
    system(sys_command1.c_str());
    system(sys_command2.c_str());
    return 1;
  }

  DRG::G3DPolyhedron code2Polyhedron(const std::string &  x){
    std::vector<DRG::G3DPoint> oneNumberDrawing;

    DRG::G3DPolyhedron fR,temp;
    DRG::G3DPoint transVect(0.0,-1.5,0.0);
    DRG::G3DPoint subtrVect0(0.0,0.35,-0.2);
    DRG::G3DPoint subtrVect1(0.0,0.35,-0.2);
    DRG::G3DPoint subtrVect2(0.0,0.35,0.2);
    DRG::G3DPoint subtrVect3(0.0,0.35,0.2);
    std::vector<DRG::G3DPoint> altVects;
    altVects.resize(4);
    altVects[0]=subtrVect0;
    altVects[1]=subtrVect1;
    altVects[2]=subtrVect2;
    altVects[3]=subtrVect3;
    long sza=altVects.size();
    long i=0;
    std::string fD;
    long ln=x.length();
    temp.faces.resize(1);
    while(i<ln){
      fD="";
      fD+=x[i];
      oneNumberDrawing=DNG::drawNumber(fD);


      temp.faces[0]=oneNumberDrawing;
      temp=temp.translation(transVect);
      transVect+=altVects[i % sza];
      ++i;
      fR.faces.push_back(temp.faces[0]);
    }
    return fR;
  }
  long code2Image(const std::string &x, const std::string &fN){
    DRG::G3DPolyhedron p=code2Polyhedron(x);
    return polyhedronToGIF(p,fN);

  }
  long code2Image(const long & _x, const std::string &fN){
    long x=_x;
    if(x<0){x*=-1;}
    std::string xSt=std::to_string(x);
    return code2Image(xSt,fN);
  }
}
#endif
