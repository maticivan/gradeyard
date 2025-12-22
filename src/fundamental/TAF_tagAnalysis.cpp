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

#ifndef _INCL_TAF_TAG_ANALYSIS_CPP
#define _INCL_TAF_TAG_ANALYSIS_CPP

namespace TAF{
struct TagSeparation{
public:
    std::vector<std::string> outside;
    std::vector<std::string> inside;
    std::string openT;
    std::string closeT;
    void fromString(const std::string&,const std::string& ,const std::string& );
    int isGood() const;
    std::string toString() const;
};
int TagSeparation::isGood() const{
    if (outside.size()==inside.size()+1){return 1;}
    return 0;
}
std::string TagSeparation::toString() const{
    //if(!isGood()){return "";}
    std::string res;
    long i=0;
    while( (i<inside.size()) || (i<outside.size()) ){
        if(i<outside.size()){res+=outside[i];}
        if(i<inside.size()){res+=openT+inside[i]+closeT;}
        ++i;
    }
    return res;
}
void TagSeparation::fromString(const std::string& _in,
                               const std::string& _oT, const std::string& _cT){
    openT=_oT;
    closeT=_cT;
    long pos,oldPos; std::pair<std::string,int> allD;
    std::stack<std::string> oS,iS;
    pos=0; allD=SF::getEverythingBefore(_in,pos,_oT);
    oS.push(allD.first);
    while( (pos<_in.length()) && allD.second){
        allD=SF::getEverythingBefore(_in,pos,_cT);
        iS.push(allD.first);
        if( (pos<_in.length()) && allD.second){
            oldPos=pos;
            allD=SF::getEverythingBefore(_in,pos,_oT);
            if(allD.second){
                oS.push(allD.first);
            }
            else{
                std::string fP;
                pos=oldPos;
                while(pos<_in.length()){
                    fP+=_in[pos];++pos;
                }
                oS.push(fP);
            }
        }
    }
    outside=SF::stackToVector(oS);inside=SF::stackToVector(iS);
}
}
#endif
