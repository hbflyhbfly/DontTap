//
//  AdsMogoConstant.h
//  MyCppGame
//
//  Created by Castiel Chen on 15/5/13.
//
//

#ifndef __MyCppGame__AdsMogoConstant__
#define __MyCppGame__AdsMogoConstant__

//#import  "AdsMOGOContent.h"
#include <stdio.h>

#endif /* defined(__MyCppGame__AdsMogoConstant__) */

typedef enum {
    //图书
    CatType2dxBookReasoning =1 , //图书_推理侦探悬疑
    CatType2dxBookHorror = 2 ,    //图书_恐怖惊悚
    CatType2dxBookRomance  =3 , 	//图书_言情（古代、现代）
    CatType2dxBookHistory	=4 ,	//	图书_历史架空穿越
    CatType2dxBookCareer_Story =5 ,//	图书_职场商战
    CatType2dxBookOfficialdom=6 ,	//	图书_官场政治
    CatType2dxBookMilitary	=7 ,	//图书_军事战争
    CatType2dxBookFantasy		=8 ,	//图书_玄幻灵异
    CatType2dxBookScience_Fiction 	=9 ,//图书_科幻
    CatType2dxBookMartial_Arts_Novels =10 ,// 图书_武侠
    CatType2dxBookYouth =11 , //图书_青春校园
    CatType2dxBookLesbian =12 ,// 图书_同人耽美
    CatType2dxBookChineseClassics =13 , //图书_中国古典名著
    CatType2dxBookWorldClassics=14 , // 图书_世界名著
    CatType2dxBookLifeKnowledge=15 ,   //图书_生活百科
    CatType2dxBookSocial_Science =16 , //图书_社会科学
    CatType2dxBookCartoon  =17 ,		//图书_动漫绘画
    CatType2dxBookPhilosophy=18 ,   //图书_哲学宗教
    CatType2dxBookMarriage=19 ,  	//图书_两性婚恋
    CatType2dxBookFinancing=21 ,  //图书_经济管理、投资理财
    CatType2dxBookPsychology=22 ,  //图书_心理学
    CatType2dxBookChildren_Education=23 ,//图书_少儿教育
    CatType2dxBookParenting_Knowledge=24 , //图书_孕产育儿
    CatType2dxBookTravel=25 ,  //图书_旅游探险
    CatType2dxBookCooking=26 ,  //图书_美食烹饪
    CatType2dxBookHealth_Care=27 , //图书_养生保健
    CatType2dxBookFashion=28 ,  //图书_时尚美容
    CatType2dxBookHomeDecoration=29 ,  //图书_家居装饰
    CatType2dxBookExamination=30 ,  //图书_教辅考试
    CatType2dxBookForeignLanguage=31 , //图书_外语
    CatType2dxBookDictionary=32,  //图书_辞典工具书
    CatType2dxBookMotivationalBook=33 , //图书_成功励志
    CatType2dxBookMedicalScience=34 , //图书_医学
    CatType2dxBookScience=35 ,  //图书_科学技术
    CatType2dxBookComputer=36 ,  //图书_计算机与互联网
    CatType2dxBookArt=37,  //图书_艺术摄影
    CatType2dxBookSports=38, //图书_体育
    CatType2dxBookEntertainment=39 , //图书_娱乐休闲
    CatType2dxBookJournal=40,  //图书_期刊杂志
    // 听书
    CatType2dxListenBookCurrentPolitics=41,  //听书_时政热点
    CatType2dxListenBookAudiobooks=42,  //听书_有声小说
    CatType2dxListenBookLiterature=43,  //听书_文学名著
    CatType2dxListenBookChinese_Opera=44,  //听书_曲艺戏曲
    CatType2dxListenBookPop_Music=45,  //听书_潮流音乐
    CatType2dxListenBookCrosstalk=46,  //听书_相声评书
    CatType2dxListenBookKid=47,  //听书_少儿天地
    CatType2dxListenBookForeignLanguage=48,  //听书_外语学习
    CatType2dxListenBookEntertainment=49,  //听书_娱乐综艺
    CatType2dxListenBookHumanities=50,  //听书_人文社科
    CatType2dxListenBookCommerce=51,  //听书_商业财经
    CatType2dxListenBookIT=52,  //听书_IT科技
    CatType2dxListenBookProfessionalSkills=53,  //听书_职业技能
    CatType2dxListenBookFanmusic=54,  //听书_纯乐梵音
    CatType2dxListenBookHealthCare=55,  //听书_健康养生
    CatType2dxListenBookFashionLifestyle=56,  //听书_时尚生活
    CatType2dxListenBookEmotionalSubject=57,  //听书_情感话题
    CatType2dxListenBookRadioPlay=58,  //听书_广播剧
    CatType2dxListenBookRadioPrograms=59,  //听书_电台
    CatType2dxListenBookLectureRoom=60,  //听书_百家讲坛
    CatType2dxListenBookOthers=61,  //听书_其他
    
    //新闻
    CatType2dxNewsCurrent_Politics_And_Hotspot=62,    //新闻_时政要闻,
    CatType2dxNewsFinance = 63,    //新闻_财经
    CatType2dxNewsMilitary = 64,   //新闻_军事
    CatType2dxNewsHistory = 65,    //新闻_历史
    CatType2dxNewsSociety = 66,    //新闻_社会
    CatType2dxNewsScience = 67,    //新闻_科技
    CatType2dxNewsCulture = 68,    //新闻_文化
    CatType2dxNewsSports = 69,     //新闻_体育
    CatType2dxNewsEntertainment = 70,  //新闻_娱乐
    CatType2dxNewsEncyclopedia = 81,   //新闻_百科
    CatType2dxNewsStocks    = 71,      //新闻_股票
    CatType2dxNewsAutomobile = 72,     // 新闻_汽车
    CatType2dxNewsReal_Estate = 73,    // 新闻_房产
    CatType2dxNewsEducation = 74,      // 新闻_教育
    CatType2dxNewsDigital_Products = 75,// 新闻_数码
    CatType2dxNewsWoman = 76,  //新闻_女性
    CatType2dxNewsForum = 77,  //新闻_论坛
    CatType2dxNewsVideo = 78,  //新闻_视频
    CatType2dxNewsTravel = 79, //新闻_旅游
    CatType2dxNewsHome_Furnishings = 80,   //新闻_家居
    CatType2dxNewsGames=82,    //新闻_游戏
    CatType2dxNewsHealth = 83, //新闻_健康
    CatType2dxNewsCity = 84,   //新闻_城市
    CatType2dxNewsCharity = 85,    //新闻_公益
    CatType2dxNewsConstellation = 86,  //新闻_星座
    CatType2dxNewsLottery = 87,    //新闻_彩票
    CatType2dxNewsOthers = 88, //新闻_其他
    
    
    //视频
    CatType2dxVideoTv_Play=89,       //视频_娱乐,
    CatType2dxVideoMovie=90,         //视频_电影,
    CatType2dxVideoVariety_Show=91,  //视频_综艺,
    CatType2dxVideoMusic=92,         //视频_音乐,
    CatType2dxVideoDocumentary=93,   //视频_纪录片,
    CatType2dxVideoCartoon=94,       //视频_动漫,
    CatType2dxVideoEducation=95,     //视频_教育,
    CatType2dxVideoSports=96,        //视频_体育,
    CatType2dxVideoFinance=97,       //视频_财经,
    CatType2dxVideoInformation=98,   //视频_资讯,
    CatType2dxVideoEntertainment=99, //视频_娱乐,
    CatType2dxVideoAutomobile=100,   //视频_汽车,
    CatType2dxVideoTechnology=101,   //视频_科技,
    CatType2dxVideoLife=102,         //视频_生活,
    CatType2dxVideoGames=103,        //视频_游戏,
    CatType2dxVideoFashion=104,      //视频_时尚,
    CatType2dxVideoTravel=105,       //视频_旅游,
    CatType2dxVideoParent_Child=106, //视频_亲子,
    CatType2dxVideoComedy = 107,     //视频_搞笑,
    CatType2dxVideoMicro_Movie = 108,//视频_微电影,
    CatType2dxVideoInternet_Drama = 109,   //视频_网剧,
    CatType2dxVideoFlickr = 110,           //视频_拍客,
    CatType2dxVideoInitiative_Video = 111, //视频_创意视频,
    CatType2dxVideoAdvertisement = 112,    //视频_广告
    CatType2dxVideoOthers = 113,           //视频_其他
    
} CatType2dx;



class AdsMogoConstant {
public:
  static void  setTitle(char* title);
  static void  setUrl(char* url);
  static void  setCat(CatType2dx cat, ...);
  static void  setKeywords(char * keyworkd, ...);
};