#include <string>
#include <iostream>

using namespace std;

class Player{
	string name;
	int wins;
	int playCount;
	int skillLevel;
	float TotalPts;
	float PtAverage;
	int h2hPlusNegScore;
	string Notes;


public:
	Player();
	void setTotalPts(float pts);
	void setPTAverage(float pts);
	void setskillLevel(int x);
	int getSkillLevel();
	float getPTAverage();
	float getTotalPts();
	void setPlusNegScore(int x);
	int getPlusNegScore();
	void setNotes(string x);
	string getNotes();
	void set_Name(string);
	void set_wins(int);
	string get_name();
	int get_wins();
	void set_playCount(int);
	int get_playCount();
};