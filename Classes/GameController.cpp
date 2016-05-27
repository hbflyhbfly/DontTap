//
//  GameController.cpp
//  DontTap
//
//  Created by Syuuhi on 5/7/16.
//
//
#include "audio/include/SimpleAudioEngine.h"
#include "GameController.hpp"
#include "GameScene.hpp"
#include "MainScene.hpp"
USING_NS_CC;
GameController* s_gameController = nullptr;

GameController::GameController():
_over(false),
_targetCount(0),
_timeLimit(0),
_mapSize(4),
_gameId(""),
_gameType(GAME_TYPE_NONE),
_gameSubType(GAME_SUBTYPE_NONE),
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
        std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("data/GameMode.json");
        std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
        _doc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(_doc.HasParseError());
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/textures.plist");
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("");
        ret = true;
    } while (0);
    return true;
}

void GameController::toScene(GAME_SCENE_TYPE sceneType){
    switch (sceneType) {
        case MAIN_SCENE:
            Director::getInstance()->replaceScene(MainScene::createScene());
            break;
        case GAME_SCENE:
            Director::getInstance()->replaceScene(GameScene::createScene()->_scene);
            break;
        default:
            break;
    }
}
void GameController::startGame(const std::string& gameId,bool isReplace){
    this->setGame(gameId);
    if (isReplace) {
        _gameScene = GameScene::createScene();
        Scene* scene = Scene::create();
        scene->addChild(_gameScene);
        Director::getInstance()->replaceScene(scene);
    }else{
        _gameScene->restartGame();
    }
    _over = false;
}

void GameController::startAgain(){
    _gameScene->restartGame();
    _over = false;
}
void GameController::setGame(const std::string& gameId){
    
    rapidjson::Value gameData;
    getGameData(gameId,gameData);
    
    _gameId = gameData["id"].GetString();
    _gameValue = gameData["value"].GetInt();
    
    for(int i = 0;i < TYPE_STR_VEC.size();i++){
        if (gameData["mode"].GetString() == TYPE_STR_VEC[i]) {
            _gameType = GAME_TYPE(i);
            break;
        }
    }
    
    for(int i = 0;i < SUBTYPE_STR_VEC.size();i++){
        if (gameData["sub_mode"].GetString() == SUBTYPE_STR_VEC[i]) {
            _gameSubType = GAME_SUBTYPE(i);
            break;
        }
    }
    
    _mapSize = 4;

    switch (_gameType) {
        case GAME_TYPE_CLASSICS:
            _targetCount = 25;
            break;
        case GAME_TYPE_ARCADE:
            _speed = 1;
            break;
        case GAME_TYPE_ZEN:
            _timeLimit = 10;
            
            break;
        case GAME_TYPE_RUSH:
            
            break;
        case GAME_TYPE_RELAY:
            
            break;
        case GAME_TYPE_ARCADE_2:
            
            break;
        default:
            break;
    }

    switch (_gameSubType) {
        case GAME_SUBTYPE_time:
            _timeLimit = _gameValue;
            break;
        case GAME_SUBTYPE_TilesSize:
            _mapSize = _gameValue;
            break;
        default:
            break;
    }
}

bool GameController::getGameData(const std::string& gameId,rapidjson::Value& value){
    return getGameDataFrom(gameId, value,_doc);
}

bool GameController::getGameDataFrom(const std::string& gameId,rapidjson::Value& value,rapidjson::Value& group){
    if (group.IsArray()) {
        for (int i =0; i<_doc.Size(); i++) {
            rapidjson::Value& arrayValue = _doc[i];
            std::string s = arrayValue["id"].GetString();
            if(gameId == s){
                value.CopyFrom(arrayValue, _doc.GetAllocator());
                break;
            }
        }
    }
    return true;
}

bool GameController::getCurrentGroup(rapidjson::Value& group){
    return getGroupWithType(_gameType,group);
}

bool GameController::getGroupWithType(GAME_TYPE gameType,rapidjson::Value& group
){
    if (_doc.IsArray()) {
        for (int i =0; i<_doc.Size(); i++) {
            rapidjson::Value& arrayValue = _doc[i];
            std::string s = arrayValue["mode"].GetString();

            if(TYPE_STR_VEC[gameType] == s){
                rapidjson::Value newValue(arrayValue,_doc.GetAllocator());

                group.PushBack(newValue, _doc.GetAllocator());
            }
        }
    }
    
    return true;
}

void GameController::gameOver(){
    _over = true;
}
