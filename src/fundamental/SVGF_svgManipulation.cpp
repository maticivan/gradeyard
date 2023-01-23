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


#ifndef _INCL_SVGF_SVGMANIPULATION_CPP
#define _INCL_SVGF_SVGMANIPULATION_CPP
namespace SVGF{
  long getNextNonNegInteger(const std::string& st, long& pos){
    long num=0;
    long sz=st.length();
    while((pos<sz)&&((st[pos]<'0')||(st[pos]>'9')) ) {
      ++pos;
    }
    while((pos<sz)&&((st[pos]>='0')&&(st[pos]<='9'))){
      num*=10;
      num+=static_cast<long>(st[pos]-'0');
      ++pos;
    }
    return num;
  }
  std::vector<long> getAllNonNegIntegers(const std::string& st){
    std::stack<long> allInts;
    long sz=st.length();
    long nextInt=0;
    long pos=0;
    nextInt=getNextNonNegInteger(st,pos);
    allInts.push(nextInt);
    while(pos<sz){
      nextInt=getNextNonNegInteger(st,pos);
      allInts.push(nextInt);
    }
    return SF::stackToVector(allInts);
  }
  long scaleAndTranslatePoint(const long& p, const double & scale, const double& add){
    double in=static_cast<double>(p);
    in *= scale;
    in += add;
    long truncate=static_cast<long>(in);
    if( in-static_cast<double>(truncate) >= 0.5){
      truncate += 1;
    }
    return truncate;
  }
  std::string vectorToSVGCoordinates(const std::vector<long>& v, const std::vector<std::string>& separators){
    long sz=v.size();
    std::string res;
    long sSize=separators.size();
    if(sSize<1){
      return "ERROR: No separators";
    }
    for(long i=0;i<sz;++i){
      res+=std::to_string(v[i]);
      if(i!=sz-1){
        res+=separators[i%sSize];
      }
    }
    return res;
  }
  std::string vectorToSVGCoordinates(const std::vector<long>& v){
    std::vector<std::string> separators;
    separators.resize(2);
    separators[0]=",";separators[1]=" ";
    return vectorToSVGCoordinates(v,separators);
  }
  std::string scaleAndTranslatePoints(const std::string& in, const double & scaleX, const double & scaleY, const double & addX, const double & addY){
    std::vector<long> coordinates=getAllNonNegIntegers(in);
    long sz=coordinates.size();
    for(long i=0;i<sz;++i){
      if(i%2==0){
        coordinates[i]=scaleAndTranslatePoint(coordinates[i],scaleX,addX);
      }
      else{
        coordinates[i]=scaleAndTranslatePoint(coordinates[i],scaleY,addY);
      }
    }
    return vectorToSVGCoordinates(coordinates);
  }
  std::string scaleAndTranslateOneCoordinate(const std::string &in, const double& scale, const double& add, const std::string& open, const std::string& close){
    std::string out=in;
    std::pair<std::string,int> allD;
    long pos;
    pos=0; allD=SF::extract(out,pos,open,close);
    if(allD.second==0){
      return in;
    }
    long newVal=scaleAndTranslatePoint(BF::stringToInteger(allD.first),scale,add);
    out=SF::findAndReplace(out,open+allD.first+close,open+std::to_string(newVal)+close);
    return out;
  }
  std::string scaleAndTranslateLabels(const std::string& in, const double & scaleX, const double & scaleY, const double & addX, const double & addY){
    std::string out=in;
    out=scaleAndTranslateOneCoordinate(out,scaleX,addX,"x=\"","\"");
    out=scaleAndTranslateOneCoordinate(out,scaleY,addY,"y=\"","\"");
    return out;
  }
  std::string scaleAndTranslate(const std::string& type, const std::string& in, const double & scaleX, const double & scaleY, const double & addX, const double & addY){
    std::string openTag="<polyline points=\"";
    std::string closeTag="\"";
    if(type=="labels"){
      openTag="<text";
      closeTag=">";
    }
    std::string out=in;
    std::pair<std::string,long> allD; long pos;
    std::string transformed;
    pos=0; allD=SF::extract(out,pos,openTag,closeTag);
    while(allD.second==1){
      if(type=="points"){transformed=scaleAndTranslatePoints(allD.first,scaleX,scaleY,addX,addY);}
      if(type=="labels"){transformed=scaleAndTranslateLabels(allD.first,scaleX,scaleY,addX,addY);}
      out=SF::findAndReplace(out,openTag+allD.first+closeTag,"_*TRANSFMD*_"+transformed+"_/*TRANSFMD*_");
      pos=0;allD=SF::extract(out,pos,openTag,closeTag);
    }
    out=SF::findAndReplace(out,"_*TRANSFMD*_",openTag);
    out=SF::findAndReplace(out,"_/*TRANSFMD*_",closeTag);
    return out;
  }
  struct SVGDetails{
  public:
    long width, height;
  };
  std::string removeSVGTags(const std::string& svgIn){
    std::string withoutTags=svgIn;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(withoutTags,pos,"<svg",">");
    if(allD.second==0){
      return svgIn;
    }
    withoutTags=SF::findAndReplace(withoutTags,"<svg"+allD.first+">","");
    withoutTags=SF::findAndReplace(withoutTags,"</svg>","");
    return withoutTags;
  }
  SVGDetails getSVGDetails(const std::string& svgIn){
    SVGDetails res;
    res.width=0;res.height=0;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(svgIn,pos,"<svg",">");
    if(allD.second==0){
      return res;
    }
    std::string rawInfo=allD.first;
    pos=0; allD=SF::extract(rawInfo,pos,"width=\"","\"");
    if(allD.second==0){
      return res;
    }
    pos=0;
    res.width=getNextNonNegInteger(allD.first,pos);
    pos=0;allD=SF::extract(rawInfo,pos,"height=\"","\"");
    if(allD.second==0){
      return res;
    }
    pos=0;
    res.height=getNextNonNegInteger(allD.first,pos);
    return res;
  }
  std::string addTwoSVGs(const std::string& first, const std::string& second, const double& scaleX, const double& scaleY){
    SVGDetails details1=getSVGDetails(first);
    SVGDetails details2=getSVGDetails(second);
    long addX=details1.width;
    details1.width = scaleAndTranslatePoint(details2.width,scaleX,static_cast<double>(details1.width));
    details2.height= scaleAndTranslatePoint(details2.height,scaleY,0.0);
    if(details2.height>details1.height){
      details1.height=details2.height;
    }
    std::string result;
    result+="<svg width=\""+std::to_string(details1.width)+"\" height=\""+std::to_string(details1.height)+"\"";
    result+=" class=\"bi bi-images\">";
    result+=removeSVGTags(first);
    std::string afterPoints=scaleAndTranslate("points",removeSVGTags(second),scaleX,scaleY,static_cast<double>(addX),0.0);
    std::string afterLabels=scaleAndTranslate("labels",removeSVGTags(afterPoints),scaleX,scaleY,static_cast<double>(addX),0.0);
    result+=afterLabels;
    result+="</svg>";
    return result;
  }
  std::string addAllSVGs(const std::string& input, const double& scaleX, const double& scaleY){
    std::vector<std::string> svgs=SF::stringToVector(input,"_n*_","_/n*_");
    long sz=svgs.size();
    if(sz<1){
      return "";
    }
    std::string res=SVGF::addTwoSVGs("",svgs[0],scaleX,scaleY);
    for(long i=1;i<sz;++i){
      res=SVGF::addTwoSVGs(res,svgs[i],scaleX,scaleY);
    }
    return res;
  }
}
#endif
