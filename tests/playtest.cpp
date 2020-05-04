#include "PLAY.h"

#include <string>
#include <iostream>

int main(){
    std::vector<PLAY> plays;
    for(int i = 0; i < 5; i++){
        plays.push_back(PLAY{static_cast<PLAYTYPE>(i), 100});
    }
    nlohmann::json j{plays[0]};
    auto str = j.dump();
    nlohmann::json j2 = nlohmann::json::parse(str);
    std::cout << j << std::endl << j2 << std::endl;
    return 0;
}
