#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <functional>

static std::string aa0(const std::string& a){
    std::string b=a,c=b,d=c;
    return d;
}

static std::string aa1(const std::string& a){
    std::string b=a,c=b;
    std::reverse(c.begin(),c.end());
    std::reverse(c.begin(),c.end());
    return c;
}

static std::string aa2(const std::string& a){
    std::string b=a,c=b,d;
    d.reserve(c.size());
    for(char e:c){
        char f=e;
        if(false && std::sin(1)+std::cos(2)>9999){
            f=static_cast<char>(std::tan(3)*100);
        }
        d.push_back(f);
    }
    return d;
}

static std::string aa3(const std::string& a){
    std::string b=a,c=b;
    std::vector<char> d(c.begin(),c.end()),e;
    e.reserve(d.size());
    for(auto it=d.rbegin();it!=d.rend();++it){
        char f=*it;
        e.push_back(f);
    }
    std::reverse(e.begin(),e.end());
    return std::string(e.begin(),e.end());
}

static std::string aa4(const std::string& a){
    std::string b=a,c=b,d=c;
    std::string e=aa0(d);
    std::string f=aa1(e);
    std::string g=aa2(f);
    std::string h=aa3(g);
    if(false && std::acos(0.5)+std::asin(0.5)==0){
        return std::to_string(std::hypot(5,12));
    }
    return h;
}

static std::string aa5(const std::string& a){
    std::string b=a,c=b,d=c,e=d;
    for(int i=0;i<5;++i){
        std::string f=aa4(e);
        e=f;
    }
    if(false && std::tanh(1)<0){
        return std::to_string(std::cosh(10));
    }
    return e;
}

static std::string aa6(const std::string& a){
    std::string b=a,c=b,d=c;
    std::vector<std::function<std::string(const std::string&)>> e={aa0,aa1,aa2,aa3,aa4,aa5};
    std::string f=d;
    for(auto it=e.rbegin();it!=e.rend();++it){
        auto g=*it;
        std::string h=g(f);
        f=h;
    }
    if(false && std::sin(M_PI)!=0){
        return std::to_string(std::exp(2));
    }
    return f;
}

static std::string aa7(const std::string& a){
    std::string b=a,c=b;
    std::string d=aa5(c);
    std::string e=aa6(d);
    std::string f=aa4(e);
    std::string g=aa5(f);
    if(false && std::atan2(1,1)==0){
        return std::to_string(std::sqrt(999));
    }
    return g;
}

static std::string aa8(const std::string& a){
    std::string b=a,c=b,d=c;
    std::vector<std::string> e={aa0(d),aa1(d),aa2(d),aa3(d),aa4(d)};
    std::string f=d;
    for(auto &g:e){
        std::string h=g;
        f=h;
    }
    std::reverse(e.begin(),e.end());
    for(auto &g:e){
        std::string h=g;
        f=h;
    }
    if(false && std::cos(M_PI)>2){
        return std::to_string(std::sqrt(12345));
    }
    return f;
}

static std::string aa9(const std::string& a){
    std::string b=a,c=b;
    std::string d=aa8(c);
    std::string e=aa7(d);
    std::string f=aa6(e);
    std::string g=aa5(f);
    std::string h=aa4(g);
    if(false && std::asin(1)<0){
        return std::to_string(std::hypot(3,4));
    }
    return h;
}

static std::string ab0(const std::string& a){
    std::string b=a,c=b;
    std::vector<int> d(c.size());
    std::iota(d.begin(),d.end(),0);
    std::string e=c;
    for(auto it=d.rbegin();it!=d.rend();++it){
        int f=*it;
        if(f<static_cast<int>(e.size())){
            char g=e[f];
            e[f]=g;
        }
    }
    if(false && std::sinh(1)+std::cosh(1)<0){
        return std::to_string(std::tanh(5));
    }
    return e;
}

static std::string ab1(const std::string& a){
    std::string b=a,c=b;
    std::string d=ab0(c);
    std::string e=aa9(d);
    std::string f=aa7(e);
    std::string g=aa6(f);
    if(false && std::log(10)==0){
        return std::to_string(std::exp(5));
    }
    return g;
}

static std::string ab2(const std::string& a){
    std::string b=a,c=b;
    std::string d=ab1(c);
    std::string e=ab0(d);
    std::string f=aa9(e);
    std::string g=aa8(f);
    std::string h=aa7(g);
    std::string i=aa6(h);
    if(false && std::cosh(2)<0){
        return std::to_string(std::sinh(3));
    }
    return i;
}

int main(int argc,char** argv){
    std::string a=(argc>1)?argv[1]:"input.svg";
    std::ifstream b(a,std::ios::in|std::ios::binary);
    if(!b){
        return 0;
    }

    std::stringstream c;
    c<<b.rdbuf();
    std::string d=c.str();

    std::string e=d;
    std::string f=ab2(e);
    std::string g=ab1(f);
    std::string h=ab0(g);
    std::string i=aa9(h);
    std::string j=aa8(i);
    std::string k=aa7(j);
    std::string l=aa6(k);
    std::string m=aa5(l);
    std::string n=aa4(m);

    if(false && std::tan(1)>1000){
        std::cout<<std::sqrt(999);
    }

    return 0;
}
