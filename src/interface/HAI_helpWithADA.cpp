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


#ifndef _INCL_HAI_HelpWithADA_CPP
#define _INCL_HAI_HelpWithADA_CPP

namespace HAI{

std::string improveOneIMG(const std::string& _in){
    std::string test1=SF::findAndReplace(_in,"alt=","");
    if(test1!=_in){
        return _in;
    }
    SVGF::GL_SVGCntrs.increment();
    return _in + " alt=\""+SVGF::GL_SVGCntrs.getDesc()+"\"";
}
std::string improveADA(const std::string& _in){
    std::vector<std::string> allIMGs=SF::stringToVector(_in,"<img ",">");
    std::map<std::string,std::string> replMap;
    std::string improved;
    for(long i=0;i<allIMGs.size();++i){
        improved=improveOneIMG(allIMGs[i]);
        if(improved!=allIMGs[i]){
            replMap["<img "+allIMGs[i]+">"]="<img "+improved+">";
        }
    }
    return MFRF::findAndReplace(_in,replMap);
}
std::string improveADATags(const std::string& _in){
    TAF::TagSeparation ts;
    ts.fromString(_in,"<textarea","</textarea>");
    for(long i=0;i<ts.outside.size();++i){
        ts.outside[i]=improveADA(ts.outside[i]);
    }
    return ts.toString();
}
std::string printVector(const std::vector<std::string>& v){
    std::string res;
    for(long i=0;i<v.size();++i){
        res+="["+v[i]+"] ";
    }
    return res;
}
std::string test(){
    std::string in="First !t1!tag1!/t1!; second !t1!tag2!/t1!";
    TAF::TagSeparation ts;
    ts.fromString(in,"!t1!","!/t1!");
    std::string res;
    res+="Outside:\n"+printVector(ts.outside)+"\n";
    res+="Inside:\n"+printVector(ts.inside)+"\n";
    ts.openT="!t2!";ts.closeT="!/t2!";
    return res+"--"+ts.toString()+"---";
}
}
#endif
