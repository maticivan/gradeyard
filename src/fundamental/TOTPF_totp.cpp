//    GradeYard learning management system
//
//    Copyright (C) 2025 Ivan Matic, https://gradeyard.com
//
//
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
//    Portions of this file were generated or adapted with assistance from ChatGPT.
//
//    You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
//    along with this program.  If not, see https://www.gnu.org/licenses/.

#ifndef _INCL_TOTPF_CPP
#define _INCL_TOTPF_CPP

 
namespace TOTPF{

int eval32(char c){
    if( (c>='A') && (c<='Z') ){
        return c-'A';
    }
    if( (c>='a') && (c<='z') ){
        return c-'a';
    }
    if( (c>'1') && (c<'8') ){
        return 26+c-'2';
    }
    return -1;
}
std::vector<uint8_t> base32_decode(const std::string& s) {
    std::vector<uint8_t> out;
    int buffer = 0, bitsLeft = 0;
    for(long i=0;i<s.length();++i){
        if(! (s[i]=='=' || s[i]==' ' || s[i]=='\t' || s[i]=='\n' || s[i]=='\r') ){
            int v = eval32(s[i]);
            if (v>0){
                buffer = (buffer<<5) | v;
                bitsLeft += 5;
                if (bitsLeft >= 8) {
                    bitsLeft -= 8;
                    out.push_back((buffer >> bitsLeft) & 0xFF);
                }
            }
        }
    }
    return out;
}
struct SHA1 {
    uint32_t h0=0x67452301, h1=0xEFCDAB89, h2=0x98BADCFE, h3=0x10325476, h4=0xC3D2E1F0;
    uint64_t total_len = 0;
    std::array<uint8_t,64> block{};
    size_t block_len = 0;
    static uint32_t adjust32(uint32_t x, int n){
        return (x<<n) | (x>>(32-n));
    }
    void process_block(const uint8_t* p){
        uint32_t w[80];
        for (int i=0;i<16;i++){
            w[i] = (uint32_t)p[4*i]<<24 | (uint32_t)p[4*i+1]<<16 | (uint32_t)p[4*i+2]<<8 | (uint32_t)p[4*i+3];
        }
        for (int i=16;i<80;i++) w[i] = adjust32(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);
        uint32_t a=h0,b=h1,c=h2,d=h3,e=h4;
        for (int i=0;i<80;i++){
            uint32_t f,k;
            if (i<20){ f=(b&c)|((~b)&d); k=0x5A827999; }
            else if (i<40){ f=b^c^d; k=0x6ED9EBA1; }
            else if (i<60){ f=(b&c)|(b&d)|(c&d); k=0x8F1BBCDC; }
            else { f=b^c^d; k=0xCA62C1D6; }
            uint32_t temp = adjust32(a,5)+f+e+k+w[i];
            e=d; d=c; c=adjust32(b,30); b=a; a=temp;
        }
        h0+=a; h1+=b; h2+=c; h3+=d; h4+=e;
    }
    void update(const uint8_t* data, size_t len){
        total_len += len;
        while (len){
            size_t n = std::min(len, 64 - block_len);
            memcpy(block.data()+block_len, data, n);
            block_len += n; data += n; len -= n;
            if (block_len==64){
                process_block(block.data());
                block_len=0;
            }
        }
    }
    void update(const std::vector<uint8_t>& v){ update(v.data(), v.size()); }
    std::array<uint8_t,20> finish(){
        uint64_t bit_len = total_len*8;
        uint8_t one = 0x80, zero=0x00;
        update(&one,1);
        uint8_t padZeros[64]={0};
        size_t padNeeded = (block_len<=56) ? (56 - block_len) : (56 + 64 - block_len);
        update(padZeros, padNeeded);
        uint8_t lenBytes[8];
        for (int i=7;i>=0;i--){ lenBytes[i] = bit_len & 0xFF; bit_len >>=8; }
        update(lenBytes,8);
        std::array<uint8_t,20> out{};
        uint32_t hs[5]={h0,h1,h2,h3,h4};
        for (int i=0;i<5;i++){
            out[4*i]= (hs[i]>>24)&0xFF;
            out[4*i+1]= (hs[i]>>16)&0xFF;
            out[4*i+2]= (hs[i]>>8)&0xFF;
            out[4*i+3]= hs[i]&0xFF;
        }
        return out;
    }
};
std::array<uint8_t,20> sha1(const std::vector<uint8_t>& m){
    SHA1 s; s.update(m); return s.finish();
}
std::array<uint8_t,20> hmac_sha1(const std::vector<uint8_t>& key, const std::vector<uint8_t>& msg){
    const size_t B=64;
    std::vector<uint8_t> k = key;
    if (k.size()>B){
        std::array<uint8_t,20> d = sha1(k);
        k.assign(d.begin(), d.end());
    }
    if (k.size()<B) k.resize(B, 0x00);
    std::vector<uint8_t> okey(B), ikey(B);
    for (size_t i=0;i<B;i++){ okey[i] = k[i] ^ 0x5c; ikey[i] = k[i] ^ 0x36; }
    std::vector<uint8_t> inner(ikey);
    inner.insert(inner.end(), msg.begin(), msg.end());
    std::array<uint8_t,20> inner_hash = sha1(inner);
    std::vector<uint8_t> outer(okey);
    outer.insert(outer.end(), inner_hash.begin(), inner_hash.end());
    return sha1(outer);
}
uint32_t hotp(const std::vector<uint8_t>& key, uint64_t counter, int digits=6){
    std::vector<uint8_t> msg(8);
    for (int i=7;i>=0;i--){ msg[i] = counter & 0xFF; counter >>=8; }
    std::array<uint8_t,20> mac = hmac_sha1(key, msg);
    int offset = mac[19] & 0x0F;
    uint32_t code = ((mac[offset] & 0x7F) << 24)
                  | ((mac[offset+1] & 0xFF) << 16)
                  | ((mac[offset+2] & 0xFF) << 8)
                  |  (mac[offset+3] & 0xFF);
    uint32_t mod = 1;
    for (int i=0;i<digits;i++) mod *= 10;
    return code % mod;
}
std::string totp(const std::string& base32_secret, time_t t, int digits=6, int period=30){
    std::vector<uint8_t> key = base32_decode(base32_secret);
    uint64_t counter = static_cast<uint64_t>(t / period);
    uint32_t code = hotp(key, counter, digits);
    std::string s = std::to_string(code);
    if ((int)s.size()<digits) s = std::string(digits - s.size(),'0') + s;
    return s;
}
std::vector<std::string> totpV(const std::string& base32_secret, time_t t, int digits=6, int period=30, long n=1){
    std::vector<uint8_t> key = base32_decode(base32_secret);
    uint64_t counter = static_cast<uint64_t>(t / period);
    std::vector<std::string> res;
    if((n<1)||(n>50)){n=1;}
    res.resize(n);uint32_t code;
    for(long i=0;i<n;++i){
        code = hotp(key, counter+i, digits);
        res[i]=std::to_string(code);;
        if(res[i].size()<digits){
            res[i]=std::string(digits-res[i].size(),'0')+res[i];
        }
    }
    return res;
}
    std::string prepareOneTOTP(const std::string& label, const std::string& secret){
        std::string res="<h4>"+label+"</h4>\n";
        time_t now = time(nullptr);
        long numCodesToDisplay=3;
        std::vector<std::string> nextFew=totpV(secret, now, 6, 30,numCodesToDisplay);
        for(long i=0;i<nextFew.size();++i){
            res+=nextFew[i]+"\t";
        }
        return res;
    }
    std::string prepareTOTPDisplay(const std::string &_in){
        long maxNumberOfTOTPS=20;
        std::map<std::string,std::string> mAll=SF::stringToMap(_in,"_k_","_/k_","_v_","_/v_");
        if(mAll.size()<1){
            mAll["default"]=_in;
        }
        std::string itemStart="\n\\begin{box}";
        std::string itemEnd="\\end{box}\n";
        std::string res;
        std::map<std::string,std::string>::const_iterator it=mAll.begin();
        long i=0;
        while((i<maxNumberOfTOTPS)&&(it!=mAll.end())){
            res+=itemStart+prepareOneTOTP(it->first,it->second)+itemEnd;
            ++i;++it;
        }
        return res;
    }

}
#endif
