#include "stdafx.h"
#include <iostream>
#include <string>
#include "Player.h"

using namespace std;

Player::Player(){
	name = "";
	wins = 0;
	playCount = 0;
}

void Player::setNotes(string x){
	Notes = x;
}

string Player::getNotes(){
	return Notes;
}



void Player::set_Name(string nameID){
	name = nameID;
}

void Player::set_wins(int t){
	wins = t;
}

string Player::get_name(){
	return name;
}

int Player::get_wins(){
	return wins;
}

int Player::get_playCount(){
	return playCount;
}

void Player::set_playCount(int t){
	playCount = t;
}

void Player::setTotalPts(float pts){
	TotalPts = pts;
}

float Player::getTotalPts(){
	return TotalPts;
}

void Player::setPTAverage(float pts){
	PtAverage = pts;
}
void Player::setskillLevel(int x){
	skillLevel = x;
}
int Player::getSkillLevel(){
	return skillLevel;
}
float Player::getPTAverage(){
	return PtAverage;
}

void Player::setPlusNegScore(int x){
	h2hPlusNegScore = x;
}
	
int Player::getPlusNegScore(){
	return h2hPlusNegScore;
}