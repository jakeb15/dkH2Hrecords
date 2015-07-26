// FantasySportsPlayerFinder.cpp : Defines the entry point for the console application.
//
//
// Add: Support to show difference of players from last find. - this will show me who just registered
//
// Factor into one fucntion for mlb nba hockey
//
//
//
#include "stdafx.h"
#define CURL_STATICLIB
#include <stdio.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string>
#include <iostream> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <MMSystem.h>
#include <iomanip>
#include <conio.h>
#include "Player.h"

using namespace std;

void getWebPage(char* s, char * temp);
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Hockey(string gameInfo,string gameName, int digits,std::vector<std::string> AllObservedPlayers);
void Basketball(string gameInfo, string gameName, int digits,std::vector<std::string> AllObservedPlayers,vector<Player> LIST,string sport);
void Baseball(string gameInfo,string gameName,int digits,std::vector<std::string> AllObservedPlayers);
void Controller(bool NHL, bool NBA, bool MLB, vector<Player> LIST);
std::vector<string> UniversalPlayerDatabase();
std::vector<Player> playerRecords(string sport);

//load a list of players that was last seen into an array. Used so I can see who is new from my last search and quickly find them
std::vector<Player> LastListofPlayers();


int _tmain(int argc, _TCHAR* argv[])
{
	/////settings....1 sport at a time.///////
	bool NHL = false;
	bool NBA = false;
	bool MLB = true;
	//////////////////////

	ofstream eraseList;
	vector<Player> LIST;


	while(true){
		if(GetAsyncKeyState(0xC0)){
			system("cls");
			LIST = LastListofPlayers();
		
			//erase list
			eraseList.open("C:\\progData\\Players\\000LastListGrabbed",std::ofstream::out );
			eraseList.close();

			Controller(NHL,NBA,MLB,LIST);
			Sleep(1500);
			cout<<endl << "waiting.."<<endl;
		}
	}

	
	

	
	return 0;
}


void getWebPage(char* s, char * temp){
	CURL *curl;
	CURLcode res;
    FILE *fp;
	char *url = s;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(temp,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
	    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER, false); // doesnt verify SSL otherwise erro 60
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void Controller(bool NHL, bool NBA, bool MLB,vector<Player> LIST){
	string sport = "hello"; // temp
	string gamestringName1;
	string gamestringName2;
	string gamestringName3;
	string gamestringName4;
	string gamestringName5;

	if(MLB==true){
		sport = "00MLBplayerRecords";
		gamestringName1 = "MLB $1 Head-to-Head vs. ";
		gamestringName2 = "MLB $2 Head-to-Head vs. ";
		gamestringName3 = "MLB $5 Head-to-Head vs. ";
		gamestringName4 = "MLB $10 Head-to-Head vs. ";
		gamestringName5 = "MLB $20 Head-to-Head vs. ";
	}
	else if(NBA == true){
		sport = "00NBAplayerRecords";
		gamestringName1 = "NBA $1 Head-to-Head vs. ";
		gamestringName2 = "NBA $2 Head-to-Head vs. ";
		gamestringName3 = "NBA $5 Head-to-Head vs. ";
		gamestringName4 = "NBA $10 Head-to-Head vs. ";
		gamestringName5 = "NBA $20 Head-to-Head vs. ";
	}
	else if(NHL == true){
		sport = "00NHLplayerRecords";
		gamestringName1 = "NHL $1 Head-to-Head vs. ";
		gamestringName2 = "NHL $2 Head-to-Head vs. ";
		gamestringName3 = "NHL $5 Head-to-Head vs. ";
		gamestringName4 = "NHL $10 Head-to-Head vs. ";
		gamestringName5 = "NHL $20 Head-to-Head vs. ";
	}
	
	cout <<"********grabbing Web Data**********"<<endl;
	getWebPage("https://www.draftkings.com/contest-lobby","DKLobbyData.txt");
	cout << "+++++++++Web Data Grabbed+++++++"<<endl;
	
	ifstream infile("DKLobbyData.txt");
	string line;
	string templine;
	long int found;
	int single = 24; // refers to $1
	int duble = 25; // refers to $10
	int triple = 26; // refers to $100

	std::vector<std::string> AllObservedPlayers = UniversalPlayerDatabase();

	//erases Last Player list so that a new one can be wrote to
	ofstream eraseFile;
	eraseFile.open("C:\\progData\\Players\\000LastListGrabbed",std::ofstream::out);
	eraseFile.close();


	while(getline(infile,line)){
		found = line.find("var packagedContests");
		if(found > 0){
			cout << "****Searching Head-to-Head games......*******"<<endl;
			
				cout << "************ $1 ************"<<endl;
				Basketball(line,gamestringName1,single,AllObservedPlayers,LIST,sport);
				cout << "************ $2 ************"<<endl;
				Basketball(line,gamestringName2,single,AllObservedPlayers,LIST,sport);
				cout << "************ $5 ************"<<endl;
				Basketball(line,gamestringName3,single,AllObservedPlayers,LIST,sport);
				cout << "************ $10 ************"<<endl;
				Basketball(line,gamestringName4,duble,AllObservedPlayers,LIST,sport);
				cout << "************ $20 ************"<<endl;
				Basketball(line,gamestringName5,duble,AllObservedPlayers,LIST,sport);
		

		}
	}

}


void Hockey(string gameInfo,string gameName,int digits,std::vector<std::string> AllObservedPlayers){
	string player = "";
	string data ="";
	int pos = 0;
	bool space = false;
	bool oneGame = false;
	bool PlayerObserved = false;
	////////// color ////////////////////////
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
	//////////////////////////////////////////

	std::vector<Player> playerDB  = playerRecords("00NHLplayerDB");

	ofstream myFile;
	myFile.open("C:\\progData\\Players\\00UniversalPlayers",std::ofstream::out | std::ofstream::app);

	

	long int h2h = 1;
	while(h2h > 0){
		h2h = gameInfo.find(gameName,pos);
		for(int x = h2h+digits; x < gameInfo.length(); x++){
		    if(gameInfo[x]!=' ' && gameInfo[x]!='"'){
				player = player + gameInfo[x];
			}
			else if(gameInfo[x]==' '){
				space = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else if(gameInfo[x]=='"' && space == false){
				oneGame = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else{
				pos =x;
				x = x + gameInfo.length();
			}
		}
		
		cout <<setw(20) <<player;
		
		if(oneGame ==true){
			cout << "*";
		}

		for (std::vector<std::string>::iterator it = AllObservedPlayers.begin() ; it != AllObservedPlayers.end(); ++it){
			if(it->compare(player)==0){
				PlayerObserved = true;
				
			}
		}

		if(PlayerObserved == false){
			cout << "(New)";
			myFile << player <<endl;
		}
		
		for (std::vector<Player>::iterator it = playerDB.begin() ; it != playerDB.end(); ++it){
			if(it->get_name().compare(player)==0){
				if(it->getSkillLevel() < 0){
					SetConsoleTextAttribute ( h,  FOREGROUND_GREEN  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel() << " avgPts: " <<setw(8)<< it->getPTAverage();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else if(it->getSkillLevel() > 5 && it->getPlusNegScore() < 0){
					SetConsoleTextAttribute ( h,  FOREGROUND_RED  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel() << " avgPts: " <<setw(8)<< it->getPTAverage();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else if(it->get_playCount() > 4 && (it->get_playCount() / 2) > it->getSkillLevel()){
					SetConsoleTextAttribute ( h,  FOREGROUND_BLUE  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel() << " avgPts: " <<setw(8)<< it->getPTAverage();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else{
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel() << " avgPts: " <<setw(8)<< it->getPTAverage();
				}
			}
		}

		cout <<endl;
		player = "";
		oneGame = false;
		space = false;
		PlayerObserved = false;

	}
	myFile.close();
}


void Basketball(string gameInfo,string gameName,int digits,std::vector<std::string> AllObservedPlayers, vector<Player> list,string sport){
	string player = "";
	string data ="";
	int pos = 0;
	bool space = false;
	bool oneGame = false;
	bool PlayerObserved = false;
	////////// color ////////////////////////
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
	//////////////////////////////////////////
	//Temp now equals all players from previous find
	vector<Player> Temp = list;

	

	ofstream lastGrab;
	lastGrab.open("C:\\progData\\Players\\000LastListGrabbed",std::ofstream::out  | std::ofstream::app);

	

	////////////////////////

	std::vector<Player> playerDB  = playerRecords(sport);

	ofstream myFile;
	myFile.open("C:\\progData\\Players\\00UniversalPlayers",std::ofstream::out | std::ofstream::app);


	long int h2h = 1;
	while(h2h > 0){
		h2h = gameInfo.find(gameName,pos);
		for(int x = h2h+digits; x < gameInfo.length(); x++){
		    if(gameInfo[x]!=' ' && gameInfo[x]!='"'){
				player = player + gameInfo[x];
			}
			else if(gameInfo[x]==' '){
				space = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else if(gameInfo[x]=='"' && space == false){
				oneGame = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else{
				pos =x;
				x = x + gameInfo.length();
			}
		}
		
		///checks to see if player exists from last search//
		bool pFound = false;
		for (std::vector<Player>::iterator it = Temp.begin() ; it != Temp.end(); ++it){
			if(player.compare(it->get_name())==0 && pFound == false){
				//cout <<setw(20) <<player;
				pFound = true;
			}
		
		}

		if(pFound == true){
			cout <<setw(20) <<player;
		}
		else{
			SetConsoleTextAttribute ( h,  FOREGROUND_GREEN  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
			cout <<setw(20) <<player;
			SetConsoleTextAttribute ( h, wOldColorAttrs);
		}
		
		lastGrab << player <<endl;
		///End

		if(oneGame ==true){
			cout << "*";
		}

		for (std::vector<std::string>::iterator it = AllObservedPlayers.begin() ; it != AllObservedPlayers.end(); ++it){
			if(it->compare(player)==0){
				PlayerObserved = true;
				
			}
		}

		if(PlayerObserved == false){
			cout << "(New)";
			myFile << player <<endl;
		}

		
		for (std::vector<Player>::iterator it = playerDB.begin() ; it != playerDB.end(); ++it){
			if(it->get_name().compare(player)==0){
				if(it->getSkillLevel() < 0){
					SetConsoleTextAttribute ( h,  FOREGROUND_GREEN  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else if(it->getSkillLevel() > 5 && it->getPlusNegScore() < 0){
					SetConsoleTextAttribute ( h,  FOREGROUND_RED  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else if(it->get_playCount() > 4 && (it->get_playCount() / 2) > it->getSkillLevel()){
					SetConsoleTextAttribute ( h,  FOREGROUND_BLUE  | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY );
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel();
					SetConsoleTextAttribute ( h, wOldColorAttrs);
				}
				else{
					cout  <<"  W/L:" << setw(4)<<it->getPlusNegScore() << " GC:" <<setw(3)<< it->get_playCount() << " " << "sLvL:"<<setw(3)<< it->getSkillLevel();
				}
			}
		}

		cout <<endl;
		player = "";
		oneGame = false;
		space = false;
		PlayerObserved = false;

	}
	lastGrab.close();
	myFile.close();
}

void Baseball(string gameInfo,string gameName,int digits,std::vector<std::string> AllObservedPlayers){
	string player = "";
	string data ="";
	int pos = 0;
	bool space = false;
	bool oneGame = false;
	bool PlayerObserved = false;
	ofstream myFile;
	myFile.open("C:\\progData\\Players\\00UniversalPlayers",std::ofstream::out | std::ofstream::app);

	long int h2h = 1;
	while(h2h > 0){
		h2h = gameInfo.find(gameName,pos);
		for(int x = h2h+digits; x < gameInfo.length(); x++){
		    if(gameInfo[x]!=' ' && gameInfo[x]!='"'){
				player = player + gameInfo[x];
			}
			else if(gameInfo[x]==' '){
				space = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else if(gameInfo[x]=='"' && space == false){
				oneGame = true;
				pos =x;
				x = x + gameInfo.length();
			}
			else{
				pos =x;
				x = x + gameInfo.length();
			}
		}
		
		cout << player;
		
		if(oneGame ==true){
			cout << "<******";
		}

		for (std::vector<std::string>::iterator it = AllObservedPlayers.begin() ; it != AllObservedPlayers.end(); ++it){
			if(it->compare(player)==0){
				PlayerObserved = true;
				
			}
		}

		if(PlayerObserved == false){
			cout << "(New)";
			myFile << player <<endl;
		}
		
		cout <<endl;
		player = "";
		oneGame = false;
		space = false;
		PlayerObserved = false;

	}
	myFile.close();
}

std::vector<string> UniversalPlayerDatabase(){
	ifstream DKPfile("C:\\progData\\Players\\00UniversalPlayers");
	string templine = "";
	std::vector<std::string> Database;

		while(getline(DKPfile,templine)){
			Database.push_back(templine); // loads Database 
		}
		DKPfile.close();

	return Database;
}

std::vector<Player> playerRecords(string sport){
	string templine;
	string playerName = "";
	string AVGScore ="";
	string GameCount ="";
	string TotPTS = "";
	string skillLVL = "";
	string PlusNegScore = "";
	//Player temp;
	int placeHolder = 0;
	std::vector<Player> winHistorydata;
	string sportRecord = "C:\\progData\\Players\\" + sport;
	ifstream WinHistory(sportRecord);//file where all my winhistory is kept formatted like [player] [+/-] [c:gameCount]
	// parse win file into Player class	
	while(getline(WinHistory,templine)){
		Player* temp = new Player();
		//Player Name
		for(int x = 0; x <templine.length() ; x++){
				if(templine[x]!=' '){
					playerName = playerName+templine[x];
				}
				else{
					placeHolder = x;
					x= x + templine.length();
				}
		}
		//Average Score
		for(int x = placeHolder+1; x<templine.length();x++){
				if(templine[x]!=' '){
					AVGScore = AVGScore + templine[x];	
				}
				else{
					placeHolder = x;
					x= x + templine.length();
				}
		}
		//Game count
		for(int x = placeHolder+1; x < templine.length();x++){
				if(templine[x]!=' '){
					GameCount = GameCount+templine[x];
				}
				else{
					placeHolder = x;
					x=x + templine.length();
				}
		}
		//Total Points
		for(int x = placeHolder+1; x < templine.length();x++){
				if(templine[x]!=' '){
					TotPTS = TotPTS + templine[x];
				}
				else{
					placeHolder = x;
					x=x + templine.length();
				}
		
		}
		//SkillLevel
		for(int x = placeHolder+1; x < templine.length();x++){
				if(templine[x]!=' '){
					skillLVL = skillLVL + templine[x];
				}
				else{
					placeHolder = x;
					x=x + templine.length();
				}
		
		}

		//H2HPlusNeg
		for(int x = placeHolder+1; x < templine.length();x++){
				if(templine[x]!='\n'){
					PlusNegScore = PlusNegScore + templine[x];
				}
				else{
					placeHolder = x;
					x=x + templine.length();
				}
		
		}
		//Name::AverageScore::Gamecount::TotPts::SkillLVL::h2hScore
			temp->set_Name(playerName);
			temp->setPTAverage(atof(AVGScore.c_str()));
			temp->set_playCount(atoi(GameCount.c_str()));
			temp->setTotalPts(atof(TotPTS.c_str()));
			temp->setskillLevel(atoi(skillLVL.c_str()));
			temp->setPlusNegScore(atoi(PlusNegScore.c_str()));
			//temp.set_wins(atoi(PlayerScore.c_str()));
			winHistorydata.push_back(*temp); // loads Database 
			playerName="";
			AVGScore="";
			TotPTS ="";
			skillLVL = "";
			PlusNegScore="";
			GameCount="";
	}
		WinHistory.close();

		return winHistorydata;
}

std::vector<Player> LastListofPlayers(){
	ifstream LastPlayerList("C:\\progData\\Players\\000LastListGrabbed");
	string templine = "";
	string player = "";
	int pos = 0;
	vector<Player> List;
	Player tempPlayer;

	while(getline(LastPlayerList,templine)){
		for(int x = 0; x <templine.length() ; x++){
			if(templine[x]!='\n'){
				player = player + templine[x];
			}	
			else{
				x = x + templine.length();
			}
		}
		tempPlayer.set_Name(player);
		List.push_back(tempPlayer);
		player = "";
	}


return List;
}