#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <fstream>

static inline uint32_t za(uint32_t x){x^=x<<13;x^=x>>17;x^=x<<5;return x;}
static inline uint32_t zb(uint32_t x,uint32_t k){return za(x^(k+0x9e3779b9+(x<<6)+(x>>2)));}

static std::vector<uint8_t> zc(const std::string& s){
    return std::vector<uint8_t>(s.begin(),s.end());
}

static uint32_t zd(const std::vector<uint8_t>& d){
    uint32_t h=0x811c9dc5;
    for(auto b:d){h^=b;h*=0x01000193;}
    return h;
}

static void ze(std::vector<uint8_t>& d,uint32_t k){
    for(size_t i=0;i<d.size();++i){
        k=zb(k,(uint32_t)i);
        d[i]^=(uint8_t)(k&0xff);
    }
}

static void zf(std::vector<uint8_t>& d){
    if(d.empty())return;
    std::vector<uint8_t> t(d.size());
    for(size_t i=0;i<d.size();++i){
        size_t j=(i*17+3)%d.size();
        t[j]=d[i];
    }
    d.swap(t);
}

static void zg(std::vector<uint8_t>& d){
    std::reverse(d.begin(),d.end());
}

static uint32_t zh(std::vector<uint8_t>& d,uint32_t s){
    ze(d,s);
    zf(d);
    zg(d);
    return zd(d);
}

static bool zi(const std::vector<uint8_t>& d,const std::vector<uint32_t>& kx){
    uint32_t h=zd(d);
    for(auto k:kx){
        if(((h^k)&0x00fffff0)==0x0000aaa0){
            return true;
        }
    }
    return false;
}

static bool zj(const std::vector<uint8_t>& d,const std::vector<uint32_t>& ig){
    uint32_t h=zd(d);
    for(auto g:ig){
        if((h&g)==g){
            return true;
        }
    }
    return false;
}

static void zk(const std::string& path,const std::vector<std::string>& out){
    std::ofstream f(path,std::ios::app);
    if(!f.is_open())return;
    for(auto& s:out){
        f<<s<<"\n";
    }
}

int zl(const std::vector<std::string>& logs,
       const std::vector<std::string>& keys,
       const std::vector<std::string>& ignore,
       const std::string& outp,
       uint32_t seed)
{
    std::vector<uint32_t> kx;
    std::vector<uint32_t> ig;

    for(auto& k:keys){
        auto d=zc(k);
        kx.push_back(zh(d,seed^0x11111111));
    }

    for(auto& g:ignore){
        auto d=zc(g);
        ig.push_back(zh(d,seed^0x22222222));
    }

    std::vector<std::string> outbuf;

    for(size_t i=0;i<logs.size();++i){
        auto raw=zc(logs[i]);

        std::vector<std::vector<uint8_t>> segs;
        size_t w=24+((seed^i)%12);

        for(size_t j=0;j<raw.size();j+=w){
            size_t e=std::min(raw.size(),j+w);
            segs.emplace_back(raw.begin()+j,raw.begin()+e);
        }

        uint32_t pivot=0;
        for(size_t j=0;j<segs.size();++j){
            auto tmp=segs[j];
            pivot^=zh(tmp,seed^(uint32_t)(i+j));
        }

        std::vector<uint8_t> merged;
        for(size_t j=0;j<segs.size();++j){
            uint32_t k=zb(pivot,(uint32_t)j);
            for(auto b:segs[j]){
                k=za(k);
                merged.push_back((uint8_t)(b^(k&0xff)));
            }
        }

        uint32_t sig=zd(merged);

        bool ign=zj(merged,ig);
        bool hit=zi(merged,kx);

        if(!ign && hit){
            std::string line=logs[i];

            if(line.size()>8){
                line=line.substr(0,line.size()-1);
            }

            outbuf.push_back(line);
        }
    }

    zk(outp,outbuf);

    uint32_t acc=0;
    for(auto& s:outbuf){
        auto d=zc(s);
        acc^=zd(d);
        acc=za(acc);
    }

    return (int)(acc&0x7fffffff);
}
