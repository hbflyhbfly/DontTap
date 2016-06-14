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
#include "CCUserDefault.h"
class GameScene;
typedef enum {
    GAME_NONE = -1,
    GAME_FAIL = 0,
    GAME_SUCCESS = 1,
    GAME_TAP_MISTAKE = 2,
    GAME_OVER = 3
    
} GAME_RESULT;

typedef enum {
    DATA_INT = -1,
    DATA_STRING = 0,
    DATA_DOUBLE = 1,
    DATA_BOOL = 2
} DATA_TYPE;

typedef enum {
    MAIN_SCENE = 0,
    GAME_SCENE = 1
    
} GAME_SCENE_TYPE;

typedef enum{
    GAME_TYPE_NONE = 0,
    GAME_TYPE_CLASSICS = 1,
    GAME_TYPE_ARCADE = 2,
    GAME_TYPE_ZEN = 3,
    GAME_TYPE_RUSH = 4,
    GAME_TYPE_RELAY = 5,
    GAME_TYPE_ARCADE_2 = 6,
    GAME_TYPE_SHUFFLE = 7

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
    "Relay",
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
    "add_time",
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
static int KEY[] = {0,2,4,5,7,9,11};
class GameController:public cocos2d::Ref{
public:
    GameController();
    virtual ~GameController();
    static GameController* getInstance();
    
    virtual bool init();
    void initUser();
    void startGame(const std::string& gameId = "classic_25",bool isReplace = false);
    void setGame(const std::string& gameId = "classic_25");
    void gameOver(GAME_RESULT result);
    void startAgain();
    GAME_RESULT isGameOver(){return _over;};
    bool getGameDataFrom(const std::string& gameId,rapidjson::Value& value,rapidjson::Value& group);
    bool getGameData(const std::string& gameId,rapidjson::Value& value);
    bool getGroupWithType(GAME_TYPE gameType,rapidjson::Value& group);
    bool getCurrentGroup(rapidjson::Value& group);
    const std::vector<int> getMusicVec(int index = 0);
    bool getCanUseMusic(rapidjson::Value& group);
    const std::string& getGameId(){return _gameId;};
    GAME_SUBTYPE getSubType(){return _gameSubType;};
    GAME_TYPE getType(){return _gameType;};
    
    int getTargetCount(){return _targetCount;};
    float getTimeLimit(){return _timeLimit;};
    int getMapSize(){return _mapSize;};
    float getSpeed(){return _speed;};
    
    void toScene(GAME_SCENE_TYPE scene);
    int getToken();
    void addToken(int amount);
    void addTokenForAdReward(int amount);
    void playSoundEffect(const std::string& sound,bool isPiano);
    void playSoundForClick();
    void playPianoSount();
    float getUserResult(const std::string& id);
    void setUserResult(const std::string& id,float result);
    void setUserData(const std::string& key,const rapidjson::Value& value);
    rapidjson::Value getUserData(const std::string& key,DATA_TYPE type);
    std::string getRandomGame();
    void changeLanguage(const char* language = NULL);
    const char* getTidForKey(const char* key);
    void updateLanguage(cocos2d::Node* node);
    void setTidForNode(cocos2d::Node* node,const char* key = NULL);
    cocos2d::Color4F randomColor();
#pragma music
    void setMusic();
    bool getMusicData(int index,rapidjson::Value& value);
    bool getMusicData(const std::string& id,rapidjson::Value& value);
    int getMusicCount(){return _musicDoc.Size();};
    bool buyMusic(const std::string& id);
    bool haveBuy(const std::string& musicId);
    bool selectMusic(const std::string& musicId);
    bool isSelectedMusic(const std::string& musicId);
    void showAd();
    bool setPlayMode(bool flag = true);
    bool getPlayMode();
    int setPlayIndex(int index);
    int getPlayIndex();
private:
    GAME_RESULT _over;
    
    int _targetCount;
    float _timeLimit;
    int _mapSize;
    float _speed;
    std::vector<int> _curMusicVec;
    int _curMusicIndex;
    rapidjson::Document _doc;
    rapidjson::Document _musicDoc;
    rapidjson::Document _colorDoc;
    cocos2d::ValueMap _languagedict;
    std::string _gameId;
    GAME_TYPE _gameType;
    GAME_SUBTYPE _gameSubType;
    int _gameValue;
    GameScene* _gameScene;
    int _gameCount;
    int _gameIndex;
};
#endif /* GameController_hpp */


