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
#include "hy_function.h"
#include "ui/UIText.h"

USING_NS_CC;
GameController* s_gameController = nullptr;

GameController::GameController():
_over(GAME_NONE),
_targetCount(0),
_timeLimit(0),
_mapSize(4),
_gameId(""),
_gameType(GAME_TYPE_NONE),
_gameSubType(GAME_SUBTYPE_NONE),
_curMusicIndex(0),
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
        
        jsonpath = FileUtils::getInstance()->fullPathForFilename("data/Music.json");
        contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
        _musicDoc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(_musicDoc.HasParseError());
        
        jsonpath = FileUtils::getInstance()->fullPathForFilename("data/ColorValue.json");
        contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
        _colorDoc.Parse<0>(contentStr.c_str());
        CC_BREAK_IF(_musicDoc.HasParseError());
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/textures.plist");
        for (int i = -12; i<= 24; i++) {
            CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(StringUtils::format("soumds/piano/%d.mp3",i).c_str());
        }
        
        changeLanguage();
        
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
    _over = GAME_NONE;
}

void GameController::startAgain(){
    _gameScene->restartGame();
    _over = GAME_NONE;
}
void GameController::setGame(const std::string& gameId){
    
    _curMusicVec = getMusicVec();
    _curMusicIndex = 0;
    
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
        case GAME_TYPE_ZEN:
            _timeLimit = 10;
            
            break;
        case GAME_TYPE_ARCADE:
            _speed = 1;
            break;
        case GAME_TYPE_RUSH:
            _speed = 1;
            break;
        case GAME_TYPE_RELAY:
            _targetCount = 50;
            _timeLimit = 10;
            break;
        case GAME_TYPE_ARCADE_2:
            _speed = 1;
            break;
        default:
            break;
    }

    switch (_gameSubType) {
        case GAME_SUBTYPE_time:
            _timeLimit = _gameValue;
            break;
        case GAME_SUBTYPE_add_time:
            _timeLimit = _gameValue;
            break;
        case GAME_SUBTYPE_TilesSize:
            _mapSize = _gameValue;
            break;
        case GAME_SUBTYPE_TilesCollected:
            _targetCount = _gameValue;
            break;
        case GAME_SUBTYPE_Faster:
            _speed = _gameValue;
        case GAME_SUBTYPE_Unstable:
            _speed = _gameValue;
        default:
            break;
    }
}

void GameController::addToken(int amount){
    int n = getUserData("token", DATA_INT).GetInt();
    setUserData("token",rapidjson::Value(amount+n));
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

const std::vector<int> GameController::getMusicVec(){
    std::vector<int> music;
    std::vector<std::string> monosyllable;
    if (_musicDoc.IsArray()) {
        int n = rand()%_musicDoc.Size();
        rapidjson::Value& arrayValue = _musicDoc[n];
        std::string str = arrayValue["music"].GetString();
        int start = arrayValue["scale_start"].GetInt();
        log("music:%s",arrayValue["tid"].GetString());
        hy_function::instance()->splite_string_to_vec(str.c_str(), monosyllable,'#');
        for (auto key:monosyllable) {
            int octave = 0;
            int i = 0;
            std::string str = &key[0];
            int num = (int)::atoi(str.c_str());
            while (i < key.size()) {
                if(key.find_first_of('+',i) != std::string::npos){
                    octave += 1;
                }
                if(key.find_first_of('-',i) != std::string::npos){
                    octave -= 1;
                }
                i++;
            }
            if (num != 0) {
                
                music.push_back(start + octave*12 + KEY[num-1]);
            }else{
                music.push_back(0);
            }
            
        }
    }
    return music;
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

std::string GameController::getRandomGame(){
    if (_doc.IsArray()) {
        int n = hy_function::instance()->randomFrom(0,_doc.Size()-1);
        rapidjson::Value& arrayValue = _doc[n];
        std::string s = arrayValue["id"].GetString();
        return s;
    }
    return  "";
}

void GameController::gameOver(GAME_RESULT result){
    if (result == GAME_SUCCESS) {
        addToken(20);
    }
    _over = result;
}

void GameController::playSoundEffect(const std::string& sound,bool isPiano){
    if (isPiano) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sounds/piano/%s",sound.c_str()).c_str());
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sounds/%s",sound.c_str()).c_str());
    }
}

void GameController::playPianoSount(){
    int index = _curMusicVec[_curMusicIndex];
    playSoundEffect(StringUtils::format("%d.mp3",index),true);
    _curMusicIndex++;
    if(_curMusicIndex == _curMusicVec.size()){
        _curMusicIndex = 0;
    }
}

float GameController::getUserResult(GAME_TYPE type,GAME_SUBTYPE subtype){
    return UserDefault::getInstance()->getFloatForKey(StringUtils::format("%d_%d",type,subtype).c_str());
}
void GameController::setUserResult(GAME_TYPE type,GAME_SUBTYPE subtype,float result){
    
    setUserData(StringUtils::format("%d_%d",type,subtype),rapidjson::Value(result));

}


void GameController::setUserData(const std::string& key,const rapidjson::Value& value){

    if(value.IsInt()){
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), value.GetInt());
    }else if(value.IsString()){
        UserDefault::getInstance()->setStringForKey(key.c_str(), value.GetString());
    }else if(value.IsDouble()){
        UserDefault::getInstance()->setDoubleForKey(key.c_str(), value.GetDouble());
    }
    UserDefault::getInstance()->flush();
}

rapidjson::Value GameController::getUserData(const std::string& key,DATA_TYPE type){
    rapidjson::Value value;
    if (type == DATA_INT) {
        value.SetInt(UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0));
    }else if(type == DATA_STRING){
        std::string s = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
        value.SetString(s.c_str(),(int)s.length());
    }else if(type == DATA_DOUBLE){
        value.SetDouble(UserDefault::getInstance()->getDoubleForKey(key.c_str(), 0));
    }
    return value;
}

void GameController::changeLanguage(const char* language){
    if (language == NULL) {
        language = Application::getInstance()->getCurrentLanguageCode();
    }
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("localizations/%s.plist",language));
    _languagedict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
    
}

void GameController::updateLanguage(Node* node){
    setTidForNode(node);
    Vector<Node*> children = node->getChildren();
    for (auto child:children) {
        updateLanguage(child);
    }
}

void GameController::setTidForNode(Node* node,const char* key){
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    auto label = dynamic_cast<Label*>(node);
    auto textFnt = dynamic_cast<cocos2d::ui::TextBMFont*>(node);
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    if (key == NULL) {
        if (button) button->setTitleText(getTidForKey(button->getTitleText().c_str()));
        if (label) label->setString(getTidForKey(label->getString().c_str()));
        if (textFnt) textFnt->setString(getTidForKey(textFnt->getString().c_str()));
        if (text) text->setString(getTidForKey(text->getString().c_str()));
    }else{
        if (button) button->setTitleText(getTidForKey(key));
        if (label) label->setString(getTidForKey(key));
        if (textFnt) textFnt->setString(getTidForKey(key));
        if (text) text->setString(getTidForKey(key));
    }
    
}
const char* GameController::getTidForKey(const char* key){
    const char* tid = key;

    if(_languagedict.find(key) != _languagedict.end()){
        tid = _languagedict[key].asString().c_str();

    }
    return tid;
}

Color4F GameController::randomColor(){
    if (_colorDoc.IsArray()) {
        int n = hy_function::instance()->randomFrom(0,_colorDoc.Size()-1);
        rapidjson::Value& arrayValue = _colorDoc[n];
        int r = arrayValue["color_red"].GetInt();
        int g = arrayValue["color_green"].GetInt();
        int b = arrayValue["color_blue"].GetInt();
        
        return Color4F(Color4B(r,g,b,255));
    }
    return Color4F::BLACK;
}
