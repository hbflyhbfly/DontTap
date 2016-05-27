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
class GameScene;

typedef enum{
    GAME_TYPE_NONE = 0,
    GAME_TYPE_CLASSICS = 1,
    GAME_TYPE_ARCADE = 2,
    GAME_TYPE_ZEN = 3,
    GAME_TYPE_RUSH = 4,
    GAME_TYPE_RELAY = 5,
    GAME_TYPE_ARCADE_2 = 6,
    GAME_TYPE_ARCADE_3 = 7

} GAME_TYPE;
typedef enum{
    GAME_SUBTYPE_NONE = 0,
    GAME_SUBTYPE_TilesCollected = 1,
    GAME_SUBTYPE_Plus = 2,
    GAME_SUBTYPE_TilesSize = 3,
    GAME_SUBTYPE_Reverse = 4,
    GAME_SUBTYPE_Normal = 5,
    GAME_SUBTYPE_Faster = 6,
    GAME_SUBTYPE_time = 7,
    GAME_SUBTYPE_add_time = 8,
    GAME_SUBTYPE_Bomb = 9,
    GAME_SUBTYPE_Double = 10,
    GAME_SUBTYPE_Bilayer = 11,
    GAME_SUBTYPE_Mist = 12,
    GAME_SUBTYPE_Unstable = 13,
    GAME_SUBTYPE_Rotatory = 14,
    GAME_SUBTYPE_Shaky = 15,
    GAME_SUBTYPE_Cloudy = 16,
    GAME_SUBTYPE_Shift = 17
} GAME_SUBTYPE;

static std::vector<std::string> TYPE_STR_VEC{
    "None",
    "Classic",
    "Arcade",
    "Zen",
    "Rush",
    "Zen",
    "Arcade-2",
    "Shuffle"
};

static std::vector<std::string> SUBTYPE_STR_VEC{
    "None",
    "TilesCollected",
    "Plus",
    "TilesSize",
    "Reverse",
    "Normal",
    "Faster",
    "time",
    "time",
    "Bomb",
    "Double",
    "Bilayer",
    "Mist",
    "Unstable",
    "Rotatory",
    "Shaky",
    "Cloudy",
    "Shift"
    };
class GameController:public cocos2d::Ref{
public:
    GameController();
    virtual ~GameController();
    static GameController* getInstance();
    
    virtual bool init();
    void starGame(const std::string& gameId = "classic_25",bool isReplace = false);
    void setGame(const std::string& gameId = "classic_25");
    void gameOver();
    bool isGameOver(){return _over;};
    bool getGameDataFrom(const std::string& gameId,rapidjson::Value& value,rapidjson::Value& group);
    bool getGameData(const std::string& gameId,rapidjson::Value& value);
    bool getGroupWithType(GAME_TYPE gameType,rapidjson::Value& group);
    bool getCurrentGroup(rapidjson::Value& group);
    const std::string& getGameId(){return _gameId;};
    GAME_SUBTYPE getSubType(){return _gameSubType;};
    GAME_TYPE getType(){return _gameType;};
    
    int getTargetCount(){return _targetCount;};
    float getTimeLimit(){return _timeLimit;};
    int getMapSize(){return _mapSize;};
    float getSpeed(){return _speed;};

private:
    bool _over;
    
    int _targetCount;
    float _timeLimit;
    int _mapSize;
    float _speed;
    
    rapidjson::Document _doc;
    std::string _gameId;
    GAME_TYPE _gameType;
    GAME_SUBTYPE _gameSubType;
    int _gameValue;
    GameScene* _gameScene;
};
#endif /* GameController_hpp */


