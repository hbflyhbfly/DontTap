//
//  GameController.cpp
//  DontTap
//
//  Created by Syuuhi on 5/7/16.
//
//

#include "GameController.hpp"
USING_NS_CC;
GameController* s_gameController = nullptr;

GameController::GameController():
_over(false),
_targetCount(0),
_timeLimit(0),
_mapWidth(5),
_mapHeigh(5),
_gameId(""),
_gameType(GAME_TYPE::GAME_TYPE_NONE),
_gameSubType(GAME_SUBTYPE::GAME_SUBTYPE_NONE),
_doc(nullptr)
{
    
}

GameController::~GameController(){
    
}

GameController* GameController::getInstance(){
    if (s_gameController == nullptr) {
        s_gameController = new GameController;
        s_gameController->init();
    }
    return s_gameController;
}

bool GameController::init(){
    bool ret = false;
    do {
        std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("data/game_data.json");
        std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
        _doc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(_doc.HasParseError());
        
        ret = true;
    } while (0);
    return true;
}

void GameController::setGame(const std::string& gameId){
    _gameId = gameId;
}

void GameController::gameOver(){
    _over = true;
}
