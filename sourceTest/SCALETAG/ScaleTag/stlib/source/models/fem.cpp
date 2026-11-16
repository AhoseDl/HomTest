#include<fem.h>
bool findValue(std::valarray<size_t>& vec, size_t i){
    auto resIt=std::find(std::begin(vec), std::end(vec), i);
    if(resIt==std::end(vec)) return false; else return true;
}
