#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

class character {
	public:
		int id;
		string name;
		string anime;
		int matches;
		int wins;
		int votes_favour;
		int votes_against;
		double win_rate;
		double attendance;
		double balance;
		double rating;
		double geomean;
		
		character(int id_, string name_, string anime_) {
			id = id_;
			name = name_;
			anime = anime_;
			matches = 0;
			wins = 0;
			votes_favour = 0;
			votes_against = 0;
			win_rate = 0;
			attendance = 0;
			balance = 0;
			rating = 1500;
			geomean = 0;
		}
};

int main(int argc, char **argv) {
	
	//This section creates a vector with all the characters

	ifstream data(argv[1]);
	string line;
	
	vector<character> characters;

	while(getline(data, line)) { //loops through the txt file until there are no lines left

		line.erase(remove(line.begin(), line.end(), '\"'),line.end());

		string words[3]; //we have 3 words in the line
		
		int pos = 0; //we'll use this to help us loop through the lines
		
		for (int cnt = 0; cnt < 3; cnt++) {
        	pos = line.find(","); //the first thing after each word is a comma
        	words[cnt] = line.substr(0, pos); //separating the words and storing them in the array
        	line.erase(0, pos + 1); //deleting the current words from the string so we can continue
    	}
		
		words[2].pop_back(); //removing line ending
		characters.push_back(character{stoi(words[0]), words[1], words[2]}); //create the object for the new character and copy it to the vector
	}
	
	//This section loops through the matches and updates the respective stats

	ifstream matchups(argv[2]);

	const int K = 100;

	int A_id;
	int B_id;
	int A_idx;
	int B_idx;
	int A_votes;
	int B_votes;
	double A_cur_rating;
	double B_cur_rating;
	double A_wc;
	double B_wc;

	while(getline(matchups, line)) { //loops through the txt file until there are no lines left

		string words[4]; //we have 4 words in the line
		
		int pos = 0; //we'll use this to help us loop through the lines
		
		for (int cnt = 0; cnt < 4; cnt++) {
        	pos = line.find(","); //the first thing after each word is a comma
        	words[cnt] = line.substr(0, pos); //separating the words and storing them in the array
        	line.erase(0, pos + 1); //deleting the current words from the string so we can continue
    	}
		
		A_id = stoi(words[0]);
		A_votes = stoi(words[1]);
		
		B_id = stoi(words[2]);
		B_votes = stoi(words[3]);
		
		auto A_idx_pos = find_if(characters.begin(), characters.end(), [A_id](const auto &obj) -> bool {return obj.id == A_id;});
		A_idx = distance(characters.begin(), A_idx_pos);

		auto B_idx_pos = find_if(characters.begin(), characters.end(), [B_id](const auto &obj) -> bool {return obj.id == B_id;});
		B_idx = distance(characters.begin(), B_idx_pos);

		characters[A_idx].votes_favour += A_votes;
		characters[A_idx].votes_against += B_votes;

		characters[B_idx].votes_favour += B_votes;
		characters[B_idx].votes_against += A_votes;

		characters[A_idx].matches++;
		characters[B_idx].matches++;

		A_cur_rating = characters[A_idx].rating;
		B_cur_rating = characters[B_idx].rating;

		A_wc = 1.0 / (1 + pow(10, ((B_cur_rating - A_cur_rating) / 400.0)));
    	B_wc = 1.0 / (1 + pow(10, ((A_cur_rating - B_cur_rating) / 400.0)));

		//cout << "A's chance of victory: " << A_wc << endl;
		//cout << "B's chance of victory: " << B_wc << endl;

		if (A_votes > B_votes) {
			characters[A_idx].rating = A_cur_rating + K * B_wc * A_votes/(A_votes + B_votes);
			characters[B_idx].rating = B_cur_rating - K * B_wc * A_votes/(A_votes + B_votes);
			characters[A_idx].wins++;
		} else if (B_votes > A_votes) {
			characters[B_idx].rating = B_cur_rating + K * A_wc * B_votes/(B_votes + A_votes);
			characters[A_idx].rating = A_cur_rating - K * A_wc * B_votes/(B_votes + A_votes);
			characters[B_idx].wins++;
		} else {
			//do nothing
		}
	}

	for (auto &obj : characters) {
		obj.win_rate = double(obj.wins) / obj.matches;
		obj.attendance = double(obj.votes_favour) / obj.matches;
		obj.balance = double(obj.votes_favour) / obj.votes_against;
	}

	//This section of the code calculates the geometric mean

	for (auto &obj : characters) {
		obj.geomean = pow(obj.win_rate * obj.attendance * obj.balance * obj.rating, 1.0/4);
	}

	//This section sorts the characters vector from the highest geomean to the lowest

	sort(characters.begin(), characters.end(), [](const auto &lhs, const auto &rhs) {
		return lhs.geomean > rhs.geomean;
	});

	//this section writes the resulting vector to a CSV

	ofstream file_out("ranking.csv");
	int rank = 1;
	file_out << "Rank,ID,Name,Anime,Matches,Wins,Votes+,Votes-,WinRate,Attendance,Balance,Rating,Geomean" << endl;

	for (auto &obj : characters) {
		file_out << rank++ << ",";
		file_out << obj.id << ",";
		file_out << "\"" + obj.name + "\","; 
		file_out << "\"" + obj.anime + "\",";
		file_out << obj.matches << ",";
		file_out << obj.wins << ",";
		file_out << obj.votes_favour << ",";
		file_out << obj.votes_against << ",";
		file_out << obj.win_rate << ",";
		file_out << obj.attendance << ",";
		file_out << obj.balance << ",";
		file_out << obj.rating << ",";
		file_out << obj.geomean << endl;
	}

    return 0;
}
