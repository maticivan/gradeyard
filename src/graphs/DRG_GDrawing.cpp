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


#ifndef _INCL_GDRAWING_CPP
#define _INCL_GDRAWING_CPP
#include <png++/png.hpp>

#include<iostream>
#include<cmath>
#include<vector>

typedef long int G3Dint;
typedef long double G3Ddouble;

namespace DRG{

  const G3Ddouble PRECISION=0.0000000001;
  const G3Ddouble XVECTDEFAULT=0.5;
  const G3Ddouble YVECTDEFAULT=sqrt(3.0)/2.0;


  class GeneralFunction{
  protected:
      std::vector<G3Ddouble> leftEndsDomain;
      std::vector<G3Ddouble> rightEndsDomain;
      G3Dint evaluationOK;
  public:
      GeneralFunction();
      GeneralFunction(std::vector<G3Ddouble>, std::vector<G3Ddouble>);
      G3Dint isLastEvaluationOK();
      virtual ~GeneralFunction();
      virtual G3Ddouble evaluate(G3Ddouble);

  };


  class LinearInterpolator : public GeneralFunction{

  protected:
      std::vector<G3Ddouble> xValues;
      std::vector<G3Ddouble> yValues;
      G3Ddouble threshold;

  public:

      LinearInterpolator():GeneralFunction(){
          continueWithConstructor();
      }
      LinearInterpolator(std::vector<G3Ddouble> _l, std::vector<G3Ddouble> _r):GeneralFunction(_l,_r){
          continueWithConstructor();
      }
      G3Dint continueWithConstructor();
      G3Dint addPoint(G3Ddouble, G3Ddouble);

      G3Ddouble evaluate(G3Ddouble);
      ~LinearInterpolator();
  };

  class G2DPolygon;
  class G3DPolyhedron;
  class Plot{
  private:

      size_t rMargin;
      size_t lMargin;
      size_t tMargin;
      size_t bMargin;
      size_t marginBase;

      png::image< png::rgb_pixel > *mainImage;
      size_t dimX;
      size_t dimY;
      size_t centerX;
      size_t centerY;
      size_t thicknessX;
      size_t thicknessY;

      size_t axisThicknessX;
      size_t axisThicknessY;


      size_t lastXPic;//will be set when drawing the picture;
      size_t lastYPic;
      std::string fileName;
      G3Dint bgR;
      G3Dint bgG;
      G3Dint bgB;
      G3Dint dCR;
      G3Dint dCG;
      G3Dint dCB;

      G3Dint drawingXBoundLInd;
      G3Ddouble drawingXBoundLVal;
      G3Dint drawingXBoundRInd;
      G3Ddouble drawingXBoundRVal;
      G3Dint drawingYBoundTInd;
      G3Ddouble drawingYBoundTVal;
      G3Dint drawingYBoundBInd;
      G3Ddouble drawingYBoundBVal;

      G3Ddouble xAxisLastNumber;
      G3Ddouble yAxisLastNumber;

      G3Dint axisArrowsInd;
      G3Dint axisMarksInd;
      G3Dint numberOfMarksOnX;
      G3Dint numberOfMarksOnY;


      G3Dint arrowParameterLength;
      G3Dint initializedProperlyInd;

  public:
      Plot(size_t =512, size_t=512, size_t=256, size_t=256);
      G3Dint initialize(size_t =512, size_t=512, size_t=256, size_t=256);
      G3Dint setOrigin(size_t,size_t);
      G3Dint changeBgColor(G3Dint, G3Dint, G3Dint);
      G3Dint changeDrawingColor(G3Dint, G3Dint, G3Dint);

      G3Dint addDrawingBound(G3Dint, G3Ddouble);

      G3Dint setAxisLengths(G3Ddouble, G3Ddouble);
      G3Dint setAxisArrows(G3Dint);
      G3Dint setNumberOfMarkers(G3Dint=-1, G3Dint=-1);
      G3Dint removeDrawingBounds();

      G3Dint setLineThickness(size_t =1, size_t=0);
      G3Dint setAxisThickness(size_t =1, size_t=0);
      G3Dint addPointAt(size_t,size_t,size_t =0, size_t=0, G3Dint =0);

      G3Dint drawCharacter(std::string, size_t,size_t);
      G3Dint drawXAxis();
      G3Dint drawYAxis();
      G3Dint drawFunction(GeneralFunction*);
      G3Dint drawLineSegment(G3Ddouble, G3Ddouble, G3Ddouble,G3Ddouble);


      G3Dint drawPolygon(const G2DPolygon &);
      G3Dint drawPolyhedron(const G3DPolyhedron &);
      G3Dint drawCubeNicely(const G3DPolyhedron &,
                            const G3DPolyhedron &,
                            const G3DPolyhedron &);//
      //  Draws the cube but not the faces
      //  that cannot be seen
      //  Arguments:
      //  faces1And3
      //  faces2And4
      //  faces5And6

      G3Dint visibleFaceOfCube(const G3DPolyhedron &);
      //  The input is a polyhedron _P that consists of two parallel faces
      //  of the cube.
      //  They are _P[0] and _P[1].
      //  One face is hidden by the other. The function outputs 1 if _P[1] is visible
      //  but _P[1] is hidden.
      //  It outputs 0 if _P[0] is visible but _P[1] is hidden.

      G3Dint setFileName(std::string);
      G3Dint putToFile();
      ~Plot();
  };

  class G3DLine;
  class G3DPoint{
  private:
      G3Ddouble xC_PM;
      G3Ddouble yC_PM;
      G3Ddouble zC_PM;
      G3Dint wellDefined_PM;
  public:
      G3DPoint(G3Ddouble =0.0, G3Ddouble =0.0, G3Ddouble=0.0);
      G3DPoint(const G3DPoint&);
      G3Ddouble x() const;
      G3Ddouble y() const;
      G3Ddouble z() const;
      G3Dint isWellDefined() const;
      G3Dint setX(const G3Ddouble =0.0);
      G3Dint setY(const G3Ddouble =0.0);
      G3Dint setZ(const G3Ddouble =0.0);
      G3Dint setWellDefined(const G3Dint =1);
      void operator=(const G3DPoint &);
      G3DPoint operator+(const G3DPoint &) const;
      G3DPoint operator-() const;
      G3DPoint operator-(const G3DPoint &) const;
      G3DPoint& operator+=(const G3DPoint &);
      G3DPoint& operator-=(const G3DPoint &);
      G3DPoint operator^(const G3DPoint &) const; //cross product
      G3DPoint& operator^=(const G3DPoint &);//cross product

      G3DPoint rotation(const G3DLine&, const G3Ddouble =0.0) const;
      //  Arguments: G3DLine axis, G3Ddouble theta
      //  Returns the point that is obtained by rotating the given point *this
      //  around the axis for an angle theta.

      G3Ddouble norm() const;

      friend G3DPoint operator*(const G3Ddouble &, const G3DPoint &);//scalar multiple
      friend G3Ddouble operator*(const G3DPoint &, const G3DPoint &);//dot product
  };

  class G3DLine{
      //  The line is defined with the point and the vector
  private:
      G3DPoint PC_PM;
      G3DPoint vC_PM;
      G3Dint wellDefined_PM;
  public:
      G3DLine(const G3Dint =0);//defaults to creating one of the axis
      // If argument is 0 - we create x axis,
      // If argument is 1 - then y axis, if argument is 2, then z axis
      G3DLine(const G3DPoint &, const G3DPoint &); //
      // Arguments: point A and vector v
      // creates a line passing through A parallel to v

      G3DLine(const G3DLine&);

      G3DPoint A() const;
      G3DPoint v() const;
      G3Dint isWellDefined() const;
      G3Dint setA(const G3DPoint &);
      G3Dint setV(const G3DPoint &);
      G3Dint setWellDefined(const G3Dint =1);
      void operator=(const G3DLine &);
      G3DPoint intersection(const G3DLine &) const;
      // Argument: ell
      // Returns the intersection point with the line ell
      // The wellDefined parameter of the return G3DPoint has value 1 if the
      // intersection is a single point.
      //      -1: The value -1 means that ell in not well defined or
      //          the current line is not well defined.
      //      0:  There is no intersection
      //      2:  The two lines coincide

      G3Dint containsPoint(const G3DPoint &) const;
      // Returns 1 if the argument belongs to the line and 0 otherwise

  };


  class G3DPlane{
  private:
      // The plane is defined with the point and the normal vector
      G3DPoint AC_PM;
      G3DPoint nC_PM;
      G3Dint wellDefined_PM;
  public:
      G3DPlane(const G3Dint =0);// defaults to creating one of the coordinate planes
      // If argument is 0 - we create xy axis,
      // If argument is 1 - then yz plane, if argument is 2, then zx plane
      G3DPlane(const G3DPlane &);
      G3DPlane(const G3DPoint &, const G3DPoint &); //
      // Arguments: point A and vector n
      // creates a plane passing through A orthogonal to n

      G3DPlane(const G3DPoint &, const G3DLine &);//
      //  Arguments: Point A and line ell
      //  The constructor creates a plane passing through A and ell
      //  If A\in ell, then the plane will not be well defined and
      //      the wellDefined_PM will be set to 0.


      G3DPlane(const G3DPoint &, const G3DPoint &, const G3DPoint &);//
      //  Arguments: Points A, B, and C
      //  The constructor creates a plane passing through A, B, and C

      G3DPoint A() const;
      G3DPoint n() const;
      G3Dint isWellDefined() const;
      G3Dint setA(const G3DPoint &);
      G3Dint setN(const G3DPoint &);
      G3Dint setWellDefined(const G3Dint =1);
      void operator=(const G3DPlane &);

      G3Dint containsPoint(const G3DPoint &) const;

      G3DPoint intersection(const G3DLine &) const;//
      //  If there is no intersection then the method isWellDefined() will return 0
      //  when applied to the returned value.
      //  If the entire line is in the plane then isWellDefined() will return 2.
      //  If the intersection is only one point then isWellDefined() will return 1.

      G3DLine intersection(const G3DPlane &) const;
      //  If there is no intersection then the method isWellDefined() will return 0
      //  when applied to the returned value.
      //  If the planes coincide then isWellDefined() will return 2.
      //  If the planes instersect at only one point, then isWellDefined() will return
      //  1.

      G3Ddouble distanceFromPoint(const G3DPoint &) const;


  };

  class G2DPolygon{
  private:
      std::vector<G3Ddouble> xCoordinates;
      std::vector<G3Ddouble> yCoordinates;
  public:
      G2DPolygon();
      G2DPolygon(const G2DPolygon&);
      G2DPolygon(const std::vector<G3DPoint>&,
                 const G3DPoint &,
                 const G3DPoint &, const G3DPoint &);
      //  Creates 2D polygon from 3D polygon
      //  Arguments:
      //  1) vertices of the 3D polygon
      //  2) xAxis2D (ignores the third coordinate)
      //  3) yAxis2D (ignores the third coordinate)
      //  4) zAxis2D (ignores the third coordinate)

      G2DPolygon(const std::vector<G3DPoint>&);
      //  Creates 2D polygon from 3D polygon with default axis setup

      void operator=(const G2DPolygon&);

      G3Dint addPoint(const G3Ddouble, const G3Ddouble);


      std::vector<G3Ddouble> getX() const;
      std::vector<G3Ddouble> getY() const;
      ~G2DPolygon();

  };


  class G3DPolyhedron{
  public:
      std::vector< std::vector<G3DPoint> > faces;
      G3DPolyhedron rotation(const G3DLine&, const G3Ddouble =0.0) const;
      G3DPolyhedron translation(const G3DPoint & ) const;
      G3Dint overrideIndicator;
      G3Dint frontFace;
      G3DPolyhedron();

  };

  GeneralFunction::GeneralFunction(){
      evaluationOK=1;
      leftEndsDomain.clear();
      rightEndsDomain.clear();
  }
  GeneralFunction::GeneralFunction(std::vector<G3Ddouble> _leftEnds, std::vector<G3Ddouble> _rightEnds){
      evaluationOK=1;
      leftEndsDomain.clear();
      rightEndsDomain.clear();
      leftEndsDomain=_leftEnds;
      rightEndsDomain=_rightEnds;
  }
  GeneralFunction::~GeneralFunction(){
      leftEndsDomain.clear();
      rightEndsDomain.clear();
  }
  G3Ddouble GeneralFunction::evaluate(G3Ddouble x){
      return x;
  }
  G3Dint GeneralFunction::isLastEvaluationOK(){
      return evaluationOK;
  }

  G3Dint LinearInterpolator::continueWithConstructor(){
      xValues.clear();
      yValues.clear();
      threshold =0.00000001;
      return 1;
  }
  G3Dint LinearInterpolator::addPoint(G3Ddouble _x, G3Ddouble _y){

      G3Dint numberOfTerms=xValues.size();
      G3Dint EVERYTHING_OK=1;
      G3Dint WARNING_UNSORTED_SEQUENCE_INSERTION_IS_NOT_OPTIMIZED=7;
      G3Dint ERROR_NO_INSERTION_X_VALUE_TOO_CLOSE_TO_ALREADY_EXISTING_X=10;
      G3Dint ERROR_AND_WARNING=17;
      G3Dint forReturn=EVERYTHING_OK;
      if((numberOfTerms>0) &&(xValues[numberOfTerms-1]>_x)){
          forReturn = WARNING_UNSORTED_SEQUENCE_INSERTION_IS_NOT_OPTIMIZED;
          std::vector<G3Ddouble> newX;
          std::vector<G3Ddouble> newY;
          newX.clear();
          newY.clear();
          G3Dint counter=0;
          while(xValues[counter]<=_x){
              newX.push_back(xValues[counter]);
              newY.push_back(yValues[counter]);
              counter++;
          }

          if((counter==0)||((counter>0)&&(_x-xValues[counter-1]>threshold))){
              if(xValues[counter]-_x>threshold){
                  newX.push_back(_x);
                  newY.push_back(_y);
              }
              else{
                  forReturn=ERROR_AND_WARNING;

              }
          }
          else{
              forReturn=ERROR_AND_WARNING;

          }
          while(counter<numberOfTerms){
              newX.push_back(xValues[counter]);
              newY.push_back(yValues[counter]);
              counter++;
          }
          xValues.clear();
          yValues.clear();
          xValues=newX;
          yValues=newY;
      }
      else{
          if((numberOfTerms==0)||((numberOfTerms>0)&&(_x-xValues[numberOfTerms-1]>threshold))){
              xValues.push_back(_x);
              yValues.push_back(_y);
          }
          else{
              forReturn=ERROR_NO_INSERTION_X_VALUE_TOO_CLOSE_TO_ALREADY_EXISTING_X;

          }
      }


      return forReturn;
  }

  G3Ddouble LinearInterpolator::evaluate(G3Ddouble x){

      G3Ddouble forReturn;
      //  Step 1:
      //          Binary search to place x between two values in xValues
      //          At the end we will have
      //              xValues[leftIndex] <= x <= xValues[rightIndex]
      G3Dint leftIndex, rightIndex, middleIndex;
      leftIndex=0; rightIndex=xValues.size();

      //      Step 1.1: Check whether binary search will be successful
      evaluationOK=1;
      if(rightIndex==0){
          evaluationOK=0;
          return 0.0;
      }
      rightIndex--;
      if(xValues[rightIndex]<x){
          evaluationOK=2;
          return yValues[rightIndex];
      }
      if(xValues[leftIndex]>x){
          evaluationOK=3;
          return yValues[leftIndex];
      }

      //      Step 1.2:   If we are still within the function, then binary search
      //                  will result in a success.
      //
      //                  We perform the binary search now.
      //

      while(leftIndex<rightIndex-1){
          middleIndex=(leftIndex+rightIndex)/2;
          //  Since leftIndex<rightIndex-1,
          //      we are guaranteed that
          //      leftIndex < middleIndex < rightIndex
          if(xValues[middleIndex]<x){
              leftIndex=middleIndex;
          }
          else{
              rightIndex=middleIndex;
          }
      }
      //  We are guaranteed the following two things:
      //      1)  leftIndex=rightIndex-1
      //      2)  xValues[leftIndex] <= x <= xValues[rightIndex]
      //

      if(xValues[rightIndex]-xValues[leftIndex]<threshold){
          forReturn=xValues[rightIndex];
      }
      else{
          forReturn=yValues[rightIndex]-yValues[leftIndex];
          forReturn/=(xValues[rightIndex]-xValues[leftIndex]);
          forReturn*=(x-xValues[leftIndex]);
          forReturn+=yValues[leftIndex];
      }


      return forReturn;
  }
  LinearInterpolator::~LinearInterpolator(){
      xValues.clear();
      yValues.clear();
  }

  G3Dint Plot::initialize(size_t _dimX, size_t _dimY,
                           size_t _centerX, size_t _centerY){
      dimX=_dimX;
      dimY=_dimY;
      centerX=dimX/2;
      centerY=dimY/2;
      if(initializedProperlyInd!=0){
          delete mainImage;
      }
      mainImage= new png::image< png::rgb_pixel >(dimX,dimY);
      initializedProperlyInd=1;
      fileName="didNotSetTheName.png";
      bgR=255;
      bgG=255;
      bgB=255;
      dCR=0;
      dCG=0;
      dCB=0;

      rMargin=5;
      lMargin=5;
      tMargin=5;
      bMargin=5;
      marginBase=50;
      thicknessX=1;
      thicknessY=1;

      axisThicknessX=2;
      axisThicknessY=2;


      xAxisLastNumber=5.0;
      yAxisLastNumber=5.0;

      axisArrowsInd=1;
      axisMarksInd=1;
      numberOfMarksOnX=5;
      numberOfMarksOnY=5;
      arrowParameterLength=5;
      removeDrawingBounds();

      for (size_t y = 0; y < (*mainImage).get_height(); ++y)
      {
          for (size_t x = 0; x < (*mainImage).get_width(); ++x)
          {
              (*mainImage)[y][x] = png::rgb_pixel(bgR, bgG, bgB);

          }
      }
      return 1;
  }

  Plot::Plot(size_t _dimX, size_t _dimY, size_t _centerX, size_t _centerY){
      initializedProperlyInd=0;
      initialize(_dimX,_dimY,_centerX,_centerY);
  }

  G3Dint Plot::setOrigin(size_t _x, size_t _y){
      centerX=_x;
      centerY=_y;
      return 1;
  }
  G3Dint Plot::changeBgColor(G3Dint _r, G3Dint _g, G3Dint _b){
      bgR=_r;
      bgG=_g;
      bgB=_b;
      for (size_t y = 0; y < (*mainImage).get_height(); ++y)
      {
          for (size_t x = 0; x < (*mainImage).get_width(); ++x)
          {
              (*mainImage)[y][x] = png::rgb_pixel(bgR, bgG, bgB);

          }
      }
      return 1;
  }
  G3Dint Plot::changeDrawingColor(G3Dint _r, G3Dint _g, G3Dint _b){
      dCR=_r;
      dCG=_g;
      dCB=_b;
      return 1;
  }

  G3Dint Plot::setLineThickness(size_t _x, size_t _y){
      if(_x==0){
          _x=1;
      }
      if (_y==0){
          _y=_x;
      }
      thicknessX=_x;
      thicknessY=_y;
      return 1;
  }
  G3Dint Plot::setAxisThickness(size_t _x, size_t _y){
      if(_x==0){
          _x=1;
      }
      if (_y==0){
          _y=_x;
      }
      axisThicknessX=_x;
      axisThicknessY=_y;
      return 1;
  }
  G3Dint Plot::setAxisArrows(G3Dint _a){
      axisArrowsInd=_a;
      return 1;
  }
  G3Dint Plot::removeDrawingBounds(){

      drawingXBoundLInd=0;
      drawingXBoundRInd=0;
      drawingYBoundTInd=0;
      drawingYBoundBInd=0;
      return 1;
  }



  G3Dint Plot::addDrawingBound(G3Dint which, G3Ddouble bound){
      if (which==0){
          drawingXBoundLInd=1;
          drawingXBoundLVal=bound;
      }
      if (which==1){
          drawingXBoundRInd=1;
          drawingXBoundRVal=bound;
      }
      if (which==2){
          drawingYBoundBInd=1;
          drawingYBoundBVal=bound;
      }
      if (which==3){
          drawingYBoundTInd=1;
          drawingYBoundTVal=bound;
      }
      return 1;
  }

  G3Dint Plot::setAxisLengths(G3Ddouble _xB, G3Ddouble _yB){
      xAxisLastNumber=_xB;
      yAxisLastNumber=_yB;
      return 1;
  }

  G3Dint Plot::addPointAt(size_t _x,size_t _y, size_t _prevX,
                           size_t _prevY, G3Dint smoothConnectionIndicator){



      if(smoothConnectionIndicator==0){
          size_t leftX, bottomY,rightX,topY;


          leftX=_x;
          bottomY=_y;

          rightX=_x+thicknessX;
          topY=_y+thicknessY;
          if(rightX>=dimX){
              rightX=dimX-1;
          }
          if(topY>=dimY){
              topY=dimY-1;
          }
          size_t x,  y;
          for(x=leftX;x<rightX;++x){
              for(y=bottomY;y<topY;++y){

                  if(dimY>= y+1){
                      (*mainImage)[dimY-1-y][x]=png::rgb_pixel(dCR, dCG, dCB);
                  }
              }
          }

      }
      else{

          if( (_prevY+1>=_y)&&(_y+1>=_prevY)){
              // y's are connected correctly
              if(_prevX+1<_x){
                  for(size_t x= _prevX;x<_x;++x){
                      addPointAt(x,_y);
                  }
              }
              if(_x+1<_prevX){
                  for(size_t x=_x;x<_prevX;++x){
                      addPointAt(x,_y);
                  }
              }
          }
          if( (_prevX+1>=_x)&&(_x+1>=_prevX)){
              // x's are connected correctly

              if(_prevY+1<_y){
                  for(size_t y= _prevY;y<_y;++y){

                      addPointAt(_x,y);
                  }
              }

              if(_y+1<_prevY){
                  for(size_t y=_y;y<_prevY;++y){
                      addPointAt(_x,y);
                  }
              }
          }
          addPointAt(_x,_y);

      }
      return 1;
  }
  G3Dint Plot::setNumberOfMarkers(G3Dint x, G3Dint y){
      if(y<=0){
          y=x;
      }
      if(x>0){
          numberOfMarksOnX=x;
          numberOfMarksOnY=y;
          axisMarksInd=1;
      }
      else{
          axisMarksInd=0;
      }
      return 1;
  }


  G3Dint Plot::drawCharacter(std::string name, size_t tLCornerX, size_t tLCornerY){

      G3Dint forReturn=1;
      std::string fileInput;
      if(name=="x"){
          fileInput="letters/letter_x.png";
      }
      if(name=="y"){
          fileInput="letters/letter_y.png";
      }
      png::image< png::rgb_pixel > imageCharacter(fileInput);
      size_t characterWidth=imageCharacter.get_width();
      size_t characterHeight=imageCharacter.get_height();
      size_t startX, startY;
      startX=tLCornerX;
      startX+=characterWidth;
      startY=tLCornerY;
      startY+=characterHeight;
      if(startX>dimX){
          startX=dimX-characterWidth-1;
      }
      if(startY>dimY){
          startY=dimY-characterHeight-1;
      }
      size_t x,y;
      for(x=0;x<characterWidth;++x){
          for(y=0;y<characterHeight;++y){
              (*mainImage)[startY+y][startX+ x]=imageCharacter[y][x];
          }
      }



      return forReturn;
  }

  G3Dint Plot::drawXAxis(){

      size_t saveXThickness=thicknessX;
      size_t saveYThickness=thicknessY;
      lastXPic=dimX-rMargin - marginBase* thicknessX;
      thicknessX=axisThicknessX;
      thicknessY=axisThicknessY;


      size_t prevX=0;
      size_t prevY=centerY;
      size_t x=dimX;
      if(axisArrowsInd==1){
          x-=axisThicknessX * arrowParameterLength;
      }
      size_t y=centerY;

      addPointAt(x,y,prevX,prevY,1);

      if(axisMarksInd==1){
          G3Dint alreadyDrawn=numberOfMarksOnX;
          size_t markSpacing= (lastXPic-centerX)/(size_t(numberOfMarksOnX));
          size_t pos;
          pos=centerX;
          size_t aCenterY=centerY+(axisThicknessY/2);
          size_t v=2* axisThicknessY;
          while(pos<lastXPic){
              pos+=markSpacing;

              addPointAt(pos,aCenterY-v,pos,aCenterY+v,1);
              alreadyDrawn--;
              if(alreadyDrawn==0){
                  pos=lastXPic;
              }
          }
          pos=centerX;
          while(pos>markSpacing){
              pos-=markSpacing;
              addPointAt(pos,aCenterY-v,pos,aCenterY+v,1);

          }
      }


      if(axisArrowsInd==1){
          thicknessX=1;
          thicknessY=1;
          size_t drawingX;
          size_t arrowLen=axisThicknessX * arrowParameterLength;
          size_t v;
          size_t aCenterY=centerY+(axisThicknessY/2);
          for(size_t j=0;j<arrowLen;j++){
              drawingX=x+j;
              v=(arrowLen-j)/2;
              addPointAt(drawingX,aCenterY+v,drawingX,aCenterY-v,1);
          }

          drawCharacter("x",x,(dimY- aCenterY)+v);

      }


      thicknessX=saveXThickness;
      thicknessY=saveYThickness;
      return 1;
  }

  G3Dint Plot::drawYAxis(){


      size_t saveXThickness=thicknessX;
      size_t saveYThickness=thicknessY;
      lastYPic=dimY-tMargin - marginBase*thicknessY;
      thicknessX=axisThicknessX;
      thicknessY=axisThicknessY;

      size_t prevX=centerX;
      size_t prevY=0;
      size_t y=dimY;
      if(axisArrowsInd==1){
          y-=axisThicknessY * arrowParameterLength;
      }
      size_t x=centerX;

      addPointAt(x,y,prevX,prevY,1);

      if(axisMarksInd==1){

          G3Dint alreadyDrawn=numberOfMarksOnY;
          size_t markSpacing= (lastYPic-centerY)/(size_t(numberOfMarksOnY));
          size_t pos;
          pos=centerY;
          size_t aCenterX=centerX+(axisThicknessX/2);
          size_t v=2* axisThicknessX;
          while(pos<lastYPic){
              pos+=markSpacing;
              addPointAt(aCenterX-v,pos, aCenterX+v,pos,1);
              alreadyDrawn--;
              if(alreadyDrawn==0){
                  pos=lastYPic;
              }
          }
          pos=centerY;
          while(pos>markSpacing){
              pos-=markSpacing;
              addPointAt(aCenterX-v,pos, aCenterX+v,pos,1);

          }
      }

      if(axisArrowsInd==1){
          thicknessX=1;
          thicknessY=1;
          size_t drawingY;
          size_t arrowLen=axisThicknessY * arrowParameterLength;
          size_t v;
          size_t aCenterX=centerX+(axisThicknessX/2);
          for(size_t j=0;j<arrowLen;j++){
              drawingY=y+j;
              v=(arrowLen-j)/2;
              addPointAt(aCenterX+v,drawingY,aCenterX-v,drawingY,1);
          }
          drawCharacter("y",aCenterX-100,prevY-50);
      }


      thicknessX=saveXThickness;
      thicknessY=saveYThickness;
      return 1;

  }

  G3Dint Plot::drawFunction(GeneralFunction *f){
      G3Ddouble xFun, yFun;
      G3Ddouble onePixelH, onePixelW;
      size_t xPic, yPic;
      G3Dint yPicInt;
      lastXPic=dimX-rMargin - marginBase* thicknessX;
      size_t lastXPicB=lastXPic;
      size_t firstXPic=lMargin;

      G3Dint drawingIndicator;

      size_t lastPixelDrawnX, lastPixelDrawnY;
      G3Dint drawnAlready=0;

      onePixelW=xAxisLastNumber/(G3Ddouble(lastXPic-centerX));
      onePixelH=yAxisLastNumber/(G3Ddouble(dimY-tMargin - marginBase*thicknessY-centerY));
      lastYPic=dimY-tMargin - marginBase*thicknessY;

      lastXPicB++;


      for(xPic=firstXPic;xPic<lastXPicB;xPic++){
          drawingIndicator=1;

          xFun=(G3Ddouble(xPic)-G3Ddouble(centerX))* onePixelW;
          if((drawingXBoundLInd==1)&&(xFun<drawingXBoundLVal)){
              drawingIndicator=0;
          }
          if((drawingXBoundRInd==1)&&(xFun>drawingXBoundRVal)){
              drawingIndicator=0;
          }
          if(drawingIndicator!=0){
              yFun=f->evaluate(xFun);
              if (f->isLastEvaluationOK()==1){
                  yPicInt=G3Dint(yFun/onePixelH);
                  if((drawingYBoundBInd==1)&&(yFun<drawingYBoundBVal)){
                      drawingIndicator=0;
                  }
                  if((drawingYBoundTInd==1)&&(yFun>drawingYBoundTVal)){
                      drawingIndicator=0;
                  }
                  if(drawingIndicator==1){
                      yPicInt+=centerY;
                      if(yPicInt<0){
                          drawingIndicator=0;
                      }
                      else{
                          yPic=size_t(yPicInt);
                          if((yPic>=bMargin)&&(yPic<=dimY- tMargin)){
                              addPointAt(xPic,yPic,lastPixelDrawnX,lastPixelDrawnY,drawnAlready);



                              drawnAlready=1;
                              lastPixelDrawnX=xPic;
                              lastPixelDrawnY=yPic;
                          }
                      }
                  }
              }
          }
      }



      return 1;
  }

  G3Dint Plot::drawLineSegment(G3Ddouble x1, G3Ddouble y1, G3Ddouble x2, G3Ddouble y2){
      G3Ddouble xFun, yFun;
      G3Ddouble onePixelH, onePixelW;
      size_t xPic, yPic;
      G3Dint yPicInt;
      lastXPic=dimX-rMargin - marginBase* thicknessX;
      size_t lastXPicB=lastXPic;
      size_t firstXPic=lMargin;

      G3Dint drawingIndicator;

      size_t lastPixelDrawnX, lastPixelDrawnY;
      G3Dint drawnAlready=0;

      onePixelW=xAxisLastNumber/(G3Ddouble(lastXPic-centerX));
      onePixelH=yAxisLastNumber/(G3Ddouble(dimY-tMargin - marginBase*thicknessY-centerY));
      lastYPic=dimY-tMargin - marginBase*thicknessY;

      lastXPicB++;


          drawingIndicator=1;

      xFun=x1;
      xPic=G3Dint(xFun/onePixelW+G3Ddouble(centerX) );

          if((drawingXBoundLInd==1)&&(xFun<drawingXBoundLVal)){
              drawingIndicator=0;
          }
          if((drawingXBoundRInd==1)&&(xFun>drawingXBoundRVal)){
              drawingIndicator=0;
          }
          if(drawingIndicator!=0){
              yFun=y1;
                  yPicInt=G3Dint(yFun/onePixelH);
                  if((drawingYBoundBInd==1)&&(yFun<drawingYBoundBVal)){
                      drawingIndicator=0;
                  }
                  if((drawingYBoundTInd==1)&&(yFun>drawingYBoundTVal)){
                      drawingIndicator=0;
                  }
                  if(drawingIndicator==1){
                      yPicInt+=centerY;
                      if(yPicInt<0){
                          drawingIndicator=0;
                      }
                      else{
                          yPic=size_t(yPicInt);
                          if((yPic>=bMargin)&&(yPic<=dimY- tMargin)){
                              addPointAt(xPic,yPic,lastPixelDrawnX,lastPixelDrawnY,drawnAlready);


                              drawnAlready=1;
                              lastPixelDrawnX=xPic;
                              lastPixelDrawnY=yPic;

                              yFun=y2;
                              yPicInt=G3Dint(yFun/onePixelH);

                              if((drawingYBoundBInd==1)&&(yFun<drawingYBoundBVal)){
                                  drawingIndicator=0;
                              }
                              if((drawingYBoundTInd==1)&&(yFun>drawingYBoundTVal)){
                                  drawingIndicator=0;
                              }

                              if(drawingIndicator==1){
                                  yPicInt+=centerY;
                                  if(yPicInt<0){
                                      drawingIndicator=0;
                                  }
                                  else{
                                      yPic=size_t(yPicInt);
                                      if((yPic>=bMargin)&&(yPic<=dimY- tMargin)){
                                          addPointAt(xPic,yPic,lastPixelDrawnX,lastPixelDrawnY,drawnAlready);


                                          drawnAlready=1;
                                          lastPixelDrawnX=xPic;
                                          lastPixelDrawnY=yPic;
                                      }
                                  }
                              }
                          }
                      }
                  }
      }



      return 1;
  }

  G3Dint Plot::setFileName(std::string _name){
      fileName=_name;
      return 1;
  }
  G3Dint Plot::putToFile(){
      (*mainImage).write(fileName);
      return 1;
  }
  Plot::~Plot(){
      delete mainImage;
  }




  template<class T> G3Dint changeValues(T *x, T*y){
      T temp;
      temp=*x;
      *x=*y;
      *y=temp;
      return 1;
  }


  G3DPoint::G3DPoint(G3Ddouble _x, G3Ddouble _y, G3Ddouble _z){
      xC_PM=_x;
      yC_PM=_y;
      zC_PM=_z;
      wellDefined_PM=1;
  }

  G3DPoint::G3DPoint(const G3DPoint &_from){
      xC_PM=_from.x();
      yC_PM=_from.y();
      zC_PM=_from.z();
      wellDefined_PM=_from.isWellDefined();
  }
  G3Ddouble G3DPoint::x() const{
      return xC_PM;
  }
  G3Ddouble G3DPoint::y() const{
      return yC_PM;
  }
  G3Ddouble G3DPoint::z() const{
      return zC_PM;
  }
  G3Dint G3DPoint::isWellDefined() const{
      return wellDefined_PM;
  }

  G3Dint G3DPoint::setX(const G3Ddouble _t){
      xC_PM=_t;
      return 1;
  }
  G3Dint G3DPoint::setY(const G3Ddouble _t){
      yC_PM=_t;
      return 1;
  }

  G3Dint G3DPoint::setZ(const G3Ddouble _t){
      zC_PM=_t;
      return 1;
  }

  G3Dint G3DPoint::setWellDefined(const G3Dint x){
      wellDefined_PM=x;
      return 1;
  }

  void G3DPoint::operator=(const G3DPoint &_P){
      xC_PM=_P.x();
      yC_PM=_P.y();
      zC_PM=_P.z();
      wellDefined_PM=_P.isWellDefined();

  }


  G3DPoint G3DPoint::operator+(const G3DPoint &_Q) const{
      G3Ddouble nx,ny,nz;
      nx=xC_PM+_Q.x();
      ny=yC_PM+_Q.y();
      nz=zC_PM+_Q.z();
      return G3DPoint(nx,ny,nz);
  }


  G3DPoint G3DPoint::operator-(const G3DPoint &_Q) const{
      G3Ddouble nx,ny,nz;
      nx=xC_PM-_Q.x();
      ny=yC_PM-_Q.y();
      nz=zC_PM-_Q.z();
      return G3DPoint(nx,ny,nz);
  }


  G3DPoint G3DPoint::operator-() const{
      G3Ddouble nx,ny,nz;
      nx=-xC_PM;
      ny=-yC_PM;
      nz=-zC_PM;
      return G3DPoint(nx,ny,nz);
  }

  G3DPoint& G3DPoint::operator+=(const G3DPoint &_Q){
      xC_PM+=_Q.x();
      yC_PM+=_Q.y();
      zC_PM+=_Q.z();
      return *this;
  }
  G3DPoint& G3DPoint::operator-=(const G3DPoint &_Q){
      xC_PM-=_Q.x();
      yC_PM-=_Q.y();
      zC_PM-=_Q.z();
      return *this;
  }

  G3Ddouble G3DPoint::norm() const{
      G3Ddouble normVar=0.0;
      normVar+=xC_PM*xC_PM;
      normVar+=yC_PM*yC_PM;
      normVar+=zC_PM*zC_PM;
      normVar=sqrt(normVar);
      return normVar;
  }

  G3DPoint operator*(const G3Ddouble &alpha, const G3DPoint &P){
      G3Ddouble nx, ny,nz;
      nx=alpha*P.x();
      ny=alpha*P.y();
      nz=alpha*P.z();
      return G3DPoint(nx,ny,nz);

  }
  G3Ddouble operator*(const G3DPoint &P, const G3DPoint &Q){
      G3Ddouble fR;
      fR=(P.x() * Q.x());
      fR+=(P.y() * Q.y());
      fR+=(P.z() * Q.z());
      return fR;

  }

  G3DPoint G3DPoint::operator^(const G3DPoint &_Q) const{
      G3Ddouble nx,ny,nz;
      nx=(yC_PM*_Q.z())-(zC_PM*_Q.y());
      ny=(zC_PM*_Q.x())-(xC_PM*_Q.z());
      nz=(xC_PM*_Q.y())-(yC_PM*_Q.x());
      return G3DPoint(nx,ny,nz);
  }

  G3DPoint& G3DPoint::operator^=(const G3DPoint &_Q){
      G3Ddouble nx,ny,nz;
      nx=(yC_PM*_Q.z())-(zC_PM*_Q.y());
      ny=(zC_PM*_Q.x())-(xC_PM*_Q.z());
      nz=(xC_PM*_Q.y())-(yC_PM*_Q.x());
      xC_PM=nx;
      yC_PM=ny;
      zC_PM=nz;
      return *this;
  }

  G3DLine::G3DLine(const G3Dint x){
      PC_PM=G3DPoint(0.0,0.0,0.0);
      vC_PM=G3DPoint(1.0,0.0,0.0);
      if(x==1){
          vC_PM=G3DPoint(0.0,1.0,0.0);
      }
      if(x==2){
          vC_PM=G3DPoint(0.0,0.0,1.0);
      }
      wellDefined_PM=1;
  }

  G3DLine::G3DLine(const G3DPoint &A, const G3DPoint &v){
      PC_PM=A;
      vC_PM=v;
      wellDefined_PM=1;

      G3Ddouble normVar=0.0;
      normVar+=v.x() *v.x();
      normVar+=v.y() *v.y();
      normVar+=v.z() *v.z();
      if(normVar<PRECISION){
          wellDefined_PM=0;
      }

  }
  G3DPoint G3DLine::A() const{
      return PC_PM;
  }
  G3DPoint G3DLine::v() const{
      return vC_PM;
  }
  G3Dint G3DLine::isWellDefined() const{
      return wellDefined_PM;
  }
  G3Dint G3DLine::setA(const G3DPoint &_P){
      PC_PM=_P;
      return 1;
  }
  G3Dint G3DLine::setV(const G3DPoint &_v){
      vC_PM=_v;
      G3Ddouble normVar=0.0;
      normVar+=_v.x() *_v.x();
      normVar+=_v.y() *_v.y();
      normVar+=_v.z() *_v.z();
      if(normVar<PRECISION){
          wellDefined_PM=0;
      }

      return 1;
  }
  G3Dint G3DLine::setWellDefined(const G3Dint x){
      wellDefined_PM=x;
      return 1;
  }
  G3DLine::G3DLine(const G3DLine &_from){
      PC_PM=_from.A();
      vC_PM=_from.v();
      wellDefined_PM=_from.isWellDefined();
  }
  void G3DLine::operator=(const G3DLine &_l){
      PC_PM=_l.A();
      vC_PM=_l.v();
      wellDefined_PM=_l.isWellDefined();
  }

  G3Dint G3DLine::containsPoint(const G3DPoint &P) const{
      G3DPoint crossProduct=(P-PC_PM)^vC_PM;

      G3Ddouble normVar=0.0;
      normVar+=crossProduct.x()*crossProduct.x();
      normVar+=crossProduct.y()*crossProduct.y();
      normVar+=crossProduct.z()*crossProduct.z();
      if(normVar<PRECISION){
          return 1;
      }
      return 0;



  }
  G3DPoint G3DLine::intersection(const G3DLine &_ell) const{
      G3Ddouble Ax, Ay,Az, Bx,By,Bz;
      G3Ddouble vx,vy,vz, wx,wy,wz;
      G3Ddouble s,t;
      G3DPoint B=_ell.A();
      G3DPoint w=_ell.v();


      Ax=PC_PM.x();
      Ay=PC_PM.y();
      Az=PC_PM.z();
      Bx=B.x();
      By=B.y();
      Bz=B.z();

      vx=vC_PM.x();
      vy=vC_PM.y();
      vz=vC_PM.z();
      wx=w.x();
      wy=w.y();
      wz=w.z();

      // The lines intersect if there is a pair (s,t)
      // such that (Ax+t*vx, Ay+t*vy, Az+t*vz)=(Bx+s*wx,By+s*wy,Bz+s*wz)
      G3Ddouble rx,ry,rz;
      rx=Bx-Ax;
      ry=By-Ay;
      rz=Bz-Az;

      G3DPoint forReturn(0.0,0.0,0.0);

      if(wellDefined_PM!=1){
          forReturn.setWellDefined(-1);

          return forReturn;
      }

      if (vx==0){
          if(vy==0){
              if(vz==0){
                  // wellDefined_PM=0; We should do this but can't because we are const

                  forReturn.setWellDefined(-1);

                  return forReturn;
              }
              else{
                  // vz is non-zero
                  // replace x and z

                  changeValues(&rx,&rz);
                  changeValues(&vx,&vz);
                  changeValues(&wx,&wz);

              }
          }
          else{
              // vy is non-zero
              // replace x and y
              changeValues(&rx,&ry);
              changeValues(&vx,&vy);
              changeValues(&wx,&wy);
          }
      }

      // We can be sure that vx!=0

      G3Ddouble sMultiplier, rightSide, firstS, secondS,differenceS;
      G3Dint firstEvaluationSuccessful=1, secondEvaluationSuccessful=1;
      sMultiplier=(wx * vy)/vx;
      sMultiplier-=wy;
      rightSide=rx*vy/vx;
      rightSide=ry-rightSide;
      if((sMultiplier<PRECISION)&&(sMultiplier>-PRECISION)){
          if((rightSide>PRECISION)||(rightSide<-PRECISION)){
              forReturn.setWellDefined(0);

              return forReturn;
          }
          firstEvaluationSuccessful=0;
      }
      else{
          firstS=rightSide/sMultiplier;
      }

      sMultiplier=(wx * vz)/vx;
      sMultiplier-=wz;
      rightSide=rx*vz/vx;
      rightSide=rz-rightSide;
      if((sMultiplier<PRECISION)&&(sMultiplier>-PRECISION)){
          if((rightSide>PRECISION)||(rightSide<-PRECISION)){
              forReturn.setWellDefined(0);

              return forReturn;
          }
          secondEvaluationSuccessful=0;
      }
      else{
          secondS=rightSide/sMultiplier;
      }
      if(firstEvaluationSuccessful*secondEvaluationSuccessful==1){
          differenceS=firstS-secondS;
          if((differenceS>PRECISION)||(differenceS<-PRECISION)){
              forReturn.setWellDefined(0);

              return forReturn;
          }
      }
      if(firstEvaluationSuccessful+secondEvaluationSuccessful==0){
          forReturn.setWellDefined(2);

          return forReturn;
      }



      return B+(secondS * w);
  }






  G3DPlane::G3DPlane(const G3Dint x){
      AC_PM=G3DPoint(0.0,0.0,0.0);
      nC_PM=G3DPoint(0.0,0.0,1.0);
      if(x==1){
          nC_PM=G3DPoint(1.0,0.0,0.0);
      }
      if(x==2){
          nC_PM=G3DPoint(0.0,1.0,0.0);
      }
      wellDefined_PM=1;
  }

  G3DPlane::G3DPlane(const G3DPoint &A, const G3DPoint &n){
      AC_PM=A;
      nC_PM=n;
      wellDefined_PM=1;
  }
  G3DPoint G3DPlane::A() const{
      return AC_PM;
  }
  G3DPoint G3DPlane::n() const{
      return nC_PM;
  }
  G3Dint G3DPlane::isWellDefined() const{
      return wellDefined_PM;
  }
  G3Dint G3DPlane::setA(const G3DPoint &_P){
      AC_PM=_P;
      return 1;
  }
  G3Dint G3DPlane::setN(const G3DPoint &_n){
      nC_PM=_n;
      return 1;
  }
  G3Dint G3DPlane::setWellDefined(const G3Dint x){
      wellDefined_PM=x;
      return 1;
  }
  G3DPlane::G3DPlane(const G3DPlane &_from){
      AC_PM=_from.A();
      nC_PM=_from.n();
      wellDefined_PM=_from.isWellDefined();
  }
  void G3DPlane::operator=(const G3DPlane &_p){
      AC_PM=_p.A();
      nC_PM=_p.n();
      wellDefined_PM=_p.isWellDefined();
  }

  G3DPlane::G3DPlane(const G3DPoint &A, const G3DLine &ell){
      G3DPoint X=ell.A();
      G3DPoint Y=X+ell.v();
      AC_PM=A;

      nC_PM= (X-A)^(Y-A);
      wellDefined_PM=1;
      G3Ddouble temp,normVar=0.0;
      temp=nC_PM.x();
      normVar+=temp*temp;
      temp=nC_PM.y();
      normVar+=temp*temp;
      temp=nC_PM.z();
      normVar+=temp*temp;
      if(normVar<PRECISION){
          wellDefined_PM=0;
      }

  }

  G3DPlane::G3DPlane(const G3DPoint &A, const G3DPoint &X, const G3DPoint &Y){
      wellDefined_PM=1;
      AC_PM=A;
      nC_PM=(X-A)^(Y-A);
      wellDefined_PM=1;
      G3Ddouble temp,normVar=0.0;
      temp=nC_PM.x();
      normVar+=temp*temp;
      temp=nC_PM.y();
      normVar+=temp*temp;
      temp=nC_PM.z();
      normVar+=temp*temp;
      if(normVar<PRECISION){
          wellDefined_PM=0;
      }

  }

  G3Dint G3DPlane::containsPoint(const G3DPoint &P) const{

      G3Ddouble test= (P-AC_PM)*nC_PM;
      if(test*test<PRECISION){
          return 1;
      }
      return 0;
  }


  G3Ddouble G3DPlane::distanceFromPoint(const G3DPoint &P) const{

      G3Ddouble fR=(P-AC_PM)*nC_PM;
      fR/=nC_PM.norm();
      if(fR<0.0){
          fR*=-1.0;
      }
      return fR;
  }


  G3DPoint G3DPlane::intersection(const G3DLine &ell) const{

      G3DPoint fR;
      fR.setWellDefined(1);

      if((wellDefined_PM!=1)||(ell.isWellDefined()!=1)){
          fR.setWellDefined(0);
          return fR;
      }

      if(containsPoint(ell.A())==1){
          // We need to check whether the entire line belongs to the plane
          G3DPoint anotherPointOnTheLine(ell.A()+ell.v());
          fR= ell.A();

          if(containsPoint(anotherPointOnTheLine)){
              fR.setWellDefined(2);
          }
          return fR;
      }
      G3Ddouble num,den;
      num=(AC_PM-ell.A())*nC_PM;
      den=ell.v()*nC_PM;
      if(den*den<PRECISION){

          fR.setWellDefined(0);
          return fR;
      }

      G3Ddouble s=num/den;
      fR=ell.A()+(s* ell.v());
      fR.setWellDefined(1);
      return fR;
  }

  G3DLine G3DPlane::intersection(const G3DPlane& gamma) const{
      G3DPoint vVect=nC_PM^gamma.n();

      G3Ddouble normVar=0.0;
      G3DLine fR;

      if((wellDefined_PM!=1)||(gamma.isWellDefined()!=1)){
          fR.setWellDefined(0);
          return fR;
      }

      normVar+=vVect.x()*vVect.x();
      normVar+=vVect.y()*vVect.y();
      normVar+=vVect.z()*vVect.z();
      if(normVar<PRECISION){
          G3Dint indicatorBelongs,ind2=2;
          indicatorBelongs=containsPoint(gamma.A());
          if(indicatorBelongs==0){
              ind2=0;
          }
          fR.setWellDefined(ind2);
          return fR;
      }

      G3DLine intersectingLine(gamma.A(),nC_PM);
      G3DPoint footOfPerp=intersection(intersectingLine);
      G3DLine perpLine(AC_PM,footOfPerp-AC_PM);
      G3DPoint pointOnLine=gamma.intersection(perpLine);

      fR=G3DLine(pointOnLine,vVect);
      return fR;


  }
  G3DPoint G3DPoint::rotation(const G3DLine &ell, const G3Ddouble theta) const{
      G3DPoint fR;

      G3DPoint P(*this);

      if(ell.containsPoint(P)){
          return P;
      }
      if(ell.isWellDefined()!=1){
          fR.setWellDefined(0);
          return fR;
      }
      G3DPlane alpha(P,ell.v());

      G3DPoint M=alpha.intersection(ell);

      G3DPoint vectorPM=M-P;

      G3Ddouble lengthPM=vectorPM.norm();
      G3Ddouble lengthNM=lengthPM * cos(theta);
      G3Ddouble lengthNP1=lengthPM*sin(theta);
      G3DPoint N=M+((-lengthNM/lengthPM) * vectorPM);
      G3DPoint vectorParallel=vectorPM^ell.v();
      G3Ddouble lengthVP=vectorParallel.norm();
      vectorParallel=(lengthNP1/lengthVP)*vectorParallel;

      fR=N+vectorParallel;

      return fR;


  }

  G2DPolygon::G2DPolygon(){
      xCoordinates.clear();
      yCoordinates.clear();

  }
  G2DPolygon::G2DPolygon(const G2DPolygon &_P){
      xCoordinates=_P.getX();
      yCoordinates=_P.getY();
  }
  void G2DPolygon::operator=(const G2DPolygon &_P){
      xCoordinates=_P.getX();
      yCoordinates=_P.getY();
  }
  std::vector<G3Ddouble> G2DPolygon::getX() const{
      return xCoordinates;
  }
  std::vector<G3Ddouble> G2DPolygon::getY() const{
      return yCoordinates;
  }

  G3Dint G2DPolygon::addPoint(const G3Ddouble x, const G3Ddouble y){
      xCoordinates.push_back(x);
      yCoordinates.push_back(y);
      return 1;
  }

  G2DPolygon::G2DPolygon(const std::vector<G3DPoint>& _pol3D,
                         const G3DPoint &_defX,
                         const G3DPoint &_defY,
                         const G3DPoint &_defZ){
      G3Dint numPoints=_pol3D.size();
      G3Dint i;
      G3DPoint tempPoint, temp1;
      for(i=0;i<numPoints;++i){
          tempPoint=_pol3D[i];
          temp1=(tempPoint.x()*_defX)+(tempPoint.y()*_defY)+(tempPoint.z()*_defZ);
          xCoordinates.push_back(temp1.x());
          yCoordinates.push_back(temp1.y());
      }
  }

  G2DPolygon::G2DPolygon(const std::vector<G3DPoint>& _pol3D){
      G3DPoint defaultX(-XVECTDEFAULT,-YVECTDEFAULT,0.0);
      G3DPoint defaultY(1.0,0.0,0.0);
      G3DPoint defaultZ(0.0,1.0,0.0);
      xCoordinates.clear();
      yCoordinates.clear();
      G2DPolygon defaultPol(_pol3D,defaultX,defaultY,defaultZ);
      xCoordinates=defaultPol.getX();
      yCoordinates=defaultPol.getY();




  }


  G2DPolygon::~G2DPolygon(){
      xCoordinates.clear();
      yCoordinates.clear();
  }



  G3Dint Plot::drawPolygon(const G2DPolygon & _pol){
      LinearInterpolator *lin;

      G3Dint i,j,n,retVal;
      std::vector<G3Ddouble> xC,yC;
      xC=_pol.getX();
      yC=_pol.getY();
      n=xC.size();
      for(i=0;i<n;++i){
          lin=new LinearInterpolator;
          lin->addPoint(xC[i],yC[i]);
          j=i+1;
          if(j==n){
              j=0;
          }
          retVal= lin->addPoint(xC[j],yC[j]);
          G3Dint messedUp=0;
          if(retVal<=7){
              G3Ddouble difference=xC[j]-xC[i], slope=yC[j]-yC[i];
              if(difference<0.0){
                  difference*=-1.0;
              }
              if(slope<0.0){
                  slope*=-1.0;
              }
              if((difference<0.01)||(slope/difference>10.0)){

                  if(xC[j]>xC[i]){
                      lin->addPoint(xC[j]+0.005,yC[j]);
                      lin->addPoint(xC[i]-0.005,yC[i]);
                  }
                  else{
                      lin->addPoint(xC[j]-0.005,yC[j]);
                      lin->addPoint(xC[i]+0.005,yC[i]);
                  }
                  messedUp=1;
              }

              drawFunction(lin);

          }
          else{
              drawLineSegment(xC[i],yC[i],xC[j],yC[j]);
          }
          delete lin;








      }
      return 1;
  }

  G3Dint Plot::drawPolyhedron(const G3DPolyhedron &_pol){
      G3Dint i,j,n;
      n=_pol.faces.size();
      G2DPolygon one2dFace;
      for(i=0;i<n;++i){
          one2dFace=G2DPolygon(_pol.faces[i]);
          drawPolygon(one2dFace);
      }

      return 1;
  }

  G3Dint Plot::visibleFaceOfCube(const G3DPolyhedron &_P){

      if(_P.overrideIndicator==1){
          return _P.frontFace;
      }

      G3Dint fR=0;
      G3DPlane P0=G3DPlane( _P.faces[0][0],_P.faces[0][1],_P.faces[0][2]);
      G3DPlane P1=G3DPlane( _P.faces[1][0],_P.faces[1][1],_P.faces[1][2]);

      G3DPoint Eyes(1000.0,1000.0,1000.0);
      G3Ddouble d0=P0.distanceFromPoint(Eyes);
      G3Ddouble d1=P1.distanceFromPoint(Eyes);

      if(d1<d0){
          fR=1;
      }

      return fR;
  }

  G3Dint Plot::drawCubeNicely(const G3DPolyhedron &_f1A3,
                                const G3DPolyhedron &_f2A4,
                                const G3DPolyhedron &_f5A6){

      G3DPolyhedron forDrawing;
      G3Dint visF;
      visF=visibleFaceOfCube(_f1A3);
      forDrawing.faces.push_back(_f1A3.faces[visF]);
      visF=visibleFaceOfCube(_f2A4);
      forDrawing.faces.push_back(_f2A4.faces[visF]);
      visF=visibleFaceOfCube(_f5A6);
      forDrawing.faces.push_back(_f5A6.faces[visF]);
      drawPolyhedron(forDrawing);
      return 1;
  }

  G3DPolyhedron G3DPolyhedron::rotation(const G3DLine& ell,
                                          const G3Ddouble theta) const{
      std::vector<G3DPoint> oneFace;
      G3DPolyhedron fR;
      G3Dint n=faces.size(),i,j,k;
      for(i=0;i<n;++i){
          k=faces[i].size();
          oneFace.clear();
          for(j=0;j<k;++j){
              oneFace.push_back(faces[i][j].rotation(ell,theta));
          }
          fR.faces.push_back(oneFace);
      }
      return fR;
  }
  G3DPolyhedron::G3DPolyhedron(){
      overrideIndicator=0;
  }
  G3DPolyhedron G3DPolyhedron::translation(const G3DPoint & _p) const{
    G3DPolyhedron p2=*this;
    long nf=(p2.faces).size();
    long ns;
    for(long i=0;i<nf;++i){
      ns=((p2.faces)[i]).size();
      for(long j=0;j<ns;++j){
        ((p2.faces)[i])[j] += _p;
      }
    }
    return p2;
  }
}
#endif
