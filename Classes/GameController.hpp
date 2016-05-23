//
//  GameController.hpp
//  DontTap
//
//  Created by Syuuhi on 5/7/16.
//
//

#ifndef GameController_hpp
#define GameController_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "json/document.h"

typedef enum{
    GAME_TYPE_NONE,
    GAME_TYPE_CLASSICS,
    GAME_TYPE_ARCADE,
    GAME_TYPE_MEDIATION,
    GAME_TYPE_SPEED,
    GAME_TYPE_RELAY,
    GAME_TYPE_ARCADE_PLUS,
    GAME_TYPE_RANMDOM
} GAME_TYPE;
typedef enum{
    GAME_SUBTYPE_NONE,
    GAME_SUBTYPE_TIME,
    GAME_SUBTYPE_MAP_SIZE,
    GAME_SUBTYPE_UNCONTINUOUS,
    GAME_SUBTYPE_TAP_COUNT,

} GAME_SUBTYPE;
class GameController:public cocos2d::Ref{
public:
    GameController();
    virtual ~GameController();
    static GameController* getInstance();
    virtual bool init();
    void starGame(const std::string& gameId = "");
    void setGame(const std::string& gameId = "");
    void gameOver();
    bool isGameOver(){return _over;};
private:
    bool _over;
    int _targetCount;
    float _timeLimit;
    int _mapWidth;
    int _mapHeigh;
    std::string _gameId;
    GAME_TYPE _gameType;
    GAME_SUBTYPE _gameSubType;
    rapidjson::Document _doc;
    
};
#endif /* GameController_hpp */


