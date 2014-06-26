//
//  global.h
//  BrazilPenaltyKick
//
//  Created by beckheng on 14-6-26.
//
//

#ifndef __BrazilPenaltyKick__global__
#define __BrazilPenaltyKick__global__

#include <iostream>

#include "db/SqliteHandler.h"

#define DB_FILE_PATH FileUtils::getInstance()->getWritablePath()+"BrazilPenaltyKick.db3"

extern DataRow drScore;
extern DataRow drSetting;

void initDb(); // 初始化数据库
void loadScore(); // 加载分数
void saveScore(int lastestScore, int bestScore); // 保存分数
void loadSetting(); // 加载设置
void saveSetting(int musicOn); // 保存设置

#endif /* defined(__BrazilPenaltyKick__global__) */
