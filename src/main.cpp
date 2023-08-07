#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include <random>
#include <string>
#include <json/json.h>

#if defined(_WIN32) || defined(WIN32)
#define clear "cls"
#else
#define clear "clear"
#endif

const char* saveFile = "StockSave";

int money{5};
int stockOwned{0};
int stockPrice{1};

namespace utils{
    std::random_device rd;
    std::default_random_engine engine(rd());

    enum colors{
        RED,
        GREEN,
        BLUE,
        YELLOW,
        MAGENTA,
        CYAN,
        WHITE,
        BLACK
    };

    int getRandom(const int minVal = 0, const int maxVal = 0){
        if (minVal == 0 && maxVal == 0){
            std::uniform_int_distribution<int> uniDist(-25, 25);
            return uniDist(engine);
        }
        else {
            std::uniform_int_distribution<int> uniDist(minVal, maxVal);
            return  uniDist(engine);
        }
    }

    template<typename T>
    const char* convToChar(T type) {
        std::string conv = std::to_string(type);
        const char* c = conv.c_str();
        return c;
    }

    void printCol(const colors&& color, const unsigned int&& strings, const char* string, ...){
        std::string colorCode;

        switch (color){
        case BLACK:
            colorCode = "\033[1;30m";
            break;
        case RED:
            colorCode = "\033[1;31m";
            break;
        case GREEN:
            colorCode = "\033[1;32m";
            break;
        case YELLOW:
            colorCode = "\033[1;33m";
            break;
        case BLUE:
            colorCode = "\033[1;34m";
            break;
        case MAGENTA:
            colorCode = "\033[1;35m";
            break;
        case CYAN:
            colorCode = "\033[1;36m";
            break;
        case WHITE:
            colorCode = "\033[1;37m";
            break;
        default:
            break;
        }

        va_list args;
        va_start(args, string);

        std::string msg = colorCode + std::string(string);

        for (int i = 1; i < strings; ++i){
            const char* newString = va_arg(args, const char*);
            msg += newString;
        }

        va_end(args);

        msg += "\033[0m";
        std::cout << msg << std::endl;
    }
}


void loadSave(bool save = true){
    if (save){
        Json::Value data;
        data["Money"] = money;
        data["StockOwned"] = stockOwned;
        data["StockPrice"] = stockPrice;

        std::ofstream stream(saveFile, std::ios::trunc);

        if (stream.is_open()){
            Json::StreamWriterBuilder writer;
            stream << Json::writeString(writer, data);
            stream.close();
        }
    }
    else{
        std::ifstream stream;
        stream.open(saveFile);

        if (!stream){
            std::cerr << "Could not open save file" << std::endl;
            return;
        }

        Json::CharReaderBuilder reader;
        Json::Value data;
        std::string errs;

        if (Json::parseFromStream(reader, stream, &data, &errs)){
            money = data["Money"].asInt();
            stockOwned = data["StockOwned"].asInt();
            stockPrice = data["StockPrice"].asInt();
        }
        else
            std::cerr << "Error parsing JSON: " << errs << std::endl;
        
        stream.close();
    }
}

int main(void){
    std::system(clear);
    utils::printCol(utils::WHITE, 1, "Simple Stock V0.1");

    int randNum{0};

    loadSave(false);

    bool running{true};

    while (running){
        utils::printCol(utils::RED, 1, "key buy: B | key sell: S | key term: T | Anything else will skip this turn\n");
        utils::printCol(utils::GREEN, 2, "Money: ", utils::convToChar(money));
        utils::printCol(utils::GREEN, 2, "Stock Owned: ", utils::convToChar(stockOwned));
        utils::printCol(utils::RED, 2, "Price: ", utils::convToChar(stockPrice));

        std::cout << ">> ";

        char input;
        std::cin >> input;

        switch(input){
        case 't':
            running = false;
            break;
        case 'b':
            if (money >= stockPrice){
                money -= stockPrice;
                ++stockOwned;
            }
            break;
        case 's':
            if (stockOwned > 0){
                money += stockPrice;
                --stockOwned;
            }
            break;
        }

        if (randNum >= 0)
            randNum = utils::getRandom(-3, 12);
        else
            randNum = utils::getRandom(-12, 3);

        stockPrice += randNum;

        if (stockPrice < 1)
            stockPrice = 1;

        system(clear);
    }

    loadSave();
    return 0;
}