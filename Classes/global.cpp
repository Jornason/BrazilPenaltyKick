//
//  global.cpp
//  BrazilPenaltyKick
//
//  Created by beckheng on 14-6-26.
//
//

#include "global.h"

#include "cocos2d.h"

#include "util/util.h"

USING_NS_CC;

using namespace yaoheng::lin;

DataRow drScore, drSetting;

void initDb()
{
	std::string sql;
	
	SqliteHandler dbSqlite(DB_FILE_PATH);
	
	dbSqlite.executeUpdate("CREATE TABLE IF NOT EXISTS settings(id INTEGER PRIMARY KEY AUTOINCREMENT, music_on INTEGER)");
	drSetting = dbSqlite.getFirstRow("SELECT * FROM settings");
	if (drSetting.size() == 0)
	{
		sql = "INSERT INTO settings(music_on) VALUES(1)";
		dbSqlite.executeUpdate(sql);
	}
	
	dbSqlite.executeUpdate("CREATE TABLE IF NOT EXISTS scores(id INTEGER PRIMARY KEY AUTOINCREMENT, lastest_score INTEGER, best_score INTEGER)");
	drScore = dbSqlite.getFirstRow("SELECT * FROM scores");
	if (drScore.size() == 0)
	{
		sql = "INSERT INTO scores(lastest_score, best_score) VALUES(0, 0)";
		dbSqlite.executeUpdate(sql);
	}
}

void loadScore()
{
	SqliteHandler dbSqlite(DB_FILE_PATH);
	drScore = dbSqlite.getFirstRow("SELECT * FROM scores");
}

void saveScore(int lastestScore, int bestScore)
{
	drScore["lastest_score"] = to_string2(lastestScore);
	drScore["best_score"] = to_string2(bestScore);
	
	SqliteHandler dbSqlite(DB_FILE_PATH);
	dbSqlite.executeUpdate(StringUtils::format("UPDATE scores SET lastest_score = %d, best_score = %d", lastestScore, bestScore));
}

void loadSetting()
{
	SqliteHandler dbSqlite(DB_FILE_PATH);
	drSetting = dbSqlite.getFirstRow("SELECT * FROM settings");
}

void saveSetting(int musicOn)
{
	drSetting["music_on"] = to_string2(musicOn);
	
	SqliteHandler dbSqlite(DB_FILE_PATH);
	dbSqlite.executeUpdate(StringUtils::format("UPDATE settings SET music_on = %d", musicOn));
}
