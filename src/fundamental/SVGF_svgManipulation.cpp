//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
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
struct SVGCounters{
private:
    std::vector<std::string> titles;
    std::vector<std::string> descs;
    long counter;
public:
    SVGCounters();
    long getCounter() const;
    void increment(const std::string& ="");
    std::string getTitle(const std::string& ="") const;
    std::string getDesc(const std::string& ="") const;
    std::string getTitleId(const std::string& ="") const;
    std::string getDescId(const std::string& ="") const;
} GL_SVGCntrs;
SVGCounters::SVGCounters(){
    counter=0;
    long num = 20;
    titles.resize(num);
    descs.resize(num);

    long i = 0;
    titles[i] = "Graphic referenced in text";
    descs[i]  = "This graphic is referenced and explained in the surrounding text.";

    ++i;
    titles[i] = "Illustrative graphic";
    descs[i]  = "This image provides a visual illustration of content discussed nearby.";

    ++i;
    titles[i] = "Visual representation";
    descs[i]  = "This visual representation corresponds to information described in the text.";

    ++i;
    titles[i] = "Supporting graphic";
    descs[i]  = "This graphic supports and complements the surrounding written explanation.";

    ++i;
    titles[i] = "Diagram referenced in text";
    descs[i]  = "This diagram is referenced in the nearby text and illustrates the described idea.";

    ++i;
    titles[i] = "Illustration";
    descs[i]  = "This illustration accompanies the text and reflects the discussed content.";

    ++i;
    titles[i] = "Graphical element";
    descs[i]  = "This graphical element corresponds to concepts explained in the text.";

    ++i;
    titles[i] = "Visual aid";
    descs[i]  = "This visual aid is intended to assist understanding of the surrounding text.";

    ++i;
    titles[i] = "Conceptual diagram";
    descs[i]  = "This diagram visually represents a concept explained in the text.";

    ++i;
    titles[i] = "Explanatory graphic";
    descs[i]  = "This graphic provides a visual explanation related to nearby content.";

    ++i;
    titles[i] = "Illustrative diagram";
    descs[i]  = "This diagram illustrates information that is described in the text.";

    ++i;
    titles[i] = "Accompanying graphic";
    descs[i]  = "This image accompanies the text and visually reflects its subject.";

    ++i;
    titles[i] = "Reference graphic";
    descs[i]  = "This graphic is referenced by and explained in the surrounding material.";

    ++i;
    titles[i] = "Visual diagram";
    descs[i]  = "This visual diagram corresponds to the explanation provided in the text.";

    ++i;
    titles[i] = "Supporting illustration";
    descs[i]  = "This illustration supports the written explanation nearby.";

    ++i;
    titles[i] = "Text-related graphic";
    descs[i]  = "This graphic relates directly to information presented in the text.";

    ++i;
    titles[i] = "Graphical illustration";
    descs[i]  = "This graphical illustration reflects concepts discussed in the text.";

    ++i;
    titles[i] = "Explanatory diagram";
    descs[i]  = "This diagram visually explains content described in the surrounding text.";

    ++i;
    titles[i] = "Content-related graphic";
    descs[i]  = "This image is related to and explained by the nearby written content.";

    ++i;
    titles[i] = "Illustrative visual";
    descs[i]  = "This visual illustration corresponds to the discussion in the text.";

}
long SVGCounters::getCounter() const{return counter;}
void SVGCounters::increment(const std::string& t){
    if(t==""){++counter;}
}
std::string SVGCounters::getTitle(const std::string& t) const{
    if(t==""){return titles[counter % (titles.size())];}
    return t;
}
std::string SVGCounters::getDesc(const std::string& t) const{
    if(t==""){return descs[counter % (descs.size())];}
    return t;
}
std::string SVGCounters::getTitleId(const std::string& t) const{
    if(t==""){return "svgt"+std::to_string(counter);}
    return t;
}
std::string SVGCounters::getDescId(const std::string& t) const{
    if(t==""){return "svgd"+std::to_string(counter);}
    return t;
}
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
    std::string role, ariaLabel,title,desc,titleId,descId;
  };
  void addToEraseMapOpenTag(std::map<std::string,std::string> & replMap, const std::string& mT,
                            const std::string& open, const std::string& close){
      std::pair<std::string,int> allD; long pos;
      pos=0; allD=SF::extract(mT,pos,open,close);
      if(allD.second==1){
          replMap[open+allD.first+close]="";
      }
  }
  void addToEraseMapEverything(std::map<std::string,std::string> & replMap,const std::string& _in, const std::string& kw){
     std::pair<std::string,int> allD; long pos;
     std::string open="<"+kw;
     std::string close="</"+kw+">";
     pos=0; allD=SF::extract(_in,pos,open,close);
     if(allD.second==1){
         replMap[open+allD.first+close]="";
     }
  }
  std::string removeSVGTags(const std::string& svgIn){
    std::string withoutTags=svgIn;
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(withoutTags,pos,"<svg",">");
    if(allD.second==0){
      return svgIn;
    }
      std::map<std::string,std::string> replMap;
      addToEraseMapOpenTag(replMap,withoutTags,"<svg",">");
      addToEraseMapEverything(replMap,withoutTags,"title");
      addToEraseMapEverything(replMap,withoutTags,"desc");
      replMap["</svg>"]="";
      withoutTags=MFRF::findAndReplace(withoutTags,replMap);
      return withoutTags;
    }
void getStringAndParameter(std::string& st, std::string& par,
                           const std::string& _in,
                           const std::string& tName,
                           const std::string& pName){
    st="";par="";
    std::pair<std::string,int> allD; long pos;
    pos=0; allD=SF::extract(_in,pos,"<"+tName,"</"+tName+">");
    if(allD.second==0){return;}
    std::string sNew=allD.first+"< ";
    pos=0; allD=SF::extract(sNew,pos,">","<");
    if(allD.second==1){st=allD.first;}
    pos=0; allD=SF::extract(sNew,pos,pName+"=\"","\"");
    if(allD.second==1){par=allD.first;}
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
    pos=0;allD=SF::extract(rawInfo,pos,"role=\"","\"");
    res.role="img";
    if(allD.second==1){
        res.role=allD.first;
    }
      getStringAndParameter(res.title,res.titleId,svgIn,"title","id");
      getStringAndParameter(res.desc,res.descId,svgIn,"desc","id");
      pos=0;allD=SF::extract(rawInfo,pos,"aria-labelledby=\"","\"");
      res.ariaLabel=res.titleId+" "+res.descId;
      if(allD.second==1){
          res.ariaLabel=allD.first;
      }
    return res;
  }
std::string printTagForSVG(const std::string& tagName, const std::string& tagValue, const std::string& idVal){
    return "<"+tagName+" id=\""+idVal+"\">"+tagValue+"</"+tagName+">";
}
std::string createOpenSVGTag(const SVGDetails& details){
    std::string res;
    res+="<svg width=\""+std::to_string(details.width)+"\" height=\""+std::to_string(details.height)+"\"";
    res+=" class=\"bi bi-images\"";
    res+=" role=\""+details.role+"\"";
    res+=" aria-labelledby=\""+details.ariaLabel+"\"";
    res+=">";
    GL_SVGCntrs.increment(details.titleId);
    res+=printTagForSVG("title",GL_SVGCntrs.getTitle(details.title),GL_SVGCntrs.getTitleId(details.titleId));
    res+=printTagForSVG("desc",GL_SVGCntrs.getDesc(details.desc),GL_SVGCntrs.getDescId(details.descId));
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
    if(details1.titleId==""){
        long wSave=details1.width;long hSave=details1.height;
        details1=details2;
        details1.width=wSave;details1.height=hSave;
    }
    std::string result=createOpenSVGTag(details1);
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
