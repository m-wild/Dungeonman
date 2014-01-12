/**
 * main.cpp
 * Thy Dungeonman clone
 *
 * COPYRIGHT 2013 Michael Wildman
 * All rights reserved
 */

#include<iostream>
#include<string>
#include<algorithm>
#include<cstddef>
using namespace std;

// variables
string areaDescription[4];
string verb;
string noun;
string input;
int score;
bool gameRunning;
bool haveScroll;
bool haveTrinket;
bool lookedScroll;
int getFlaskCount;
enum Location { MAIN, NORTH, SOUTH, DENNIS };
Location loc;

// functions
void play();
void look();
bool get();
void getCommand();
bool again();
void go();
void talk();
bool give();
void help() { printf(areaDescription[loc].c_str()); }
void invalidEntry() { printf("That does not computeth.\nType HELP if thou needs of it.\n"); }

// reset variables for a new game
void reset() {
	score = 0;
	gameRunning = true;
	haveScroll = false;
	haveTrinket = false;
	lookedScroll = false;
	getFlaskCount = 0;

	system("cls");
	printf("     Thy Dungeonman    \n\n");

	printf("   /\\      /\\     /\\   \n");
	printf("   ||/----\\||     ||   \n");
	printf("   \\_------_/     ||   \n");
	printf("    / o  o \\      ||   \n");
	printf("    /  ||  \\   o--||--o\n");
	printf("    / ---- \\    \\----/ \n");
	printf("    /\\/\\/\\/\\      ||   \n");
	printf("                  oo   \n\n");

	printf(" Push thy Enter key... \n");


	getline(cin, input);
	printf("\n\nTHY DUNGEONMAN\nYOU ARE THY DUNGEONMAN\n\n");

	areaDescription[MAIN] = "Ye find yeself in yon dungeon. Ye see a SCROLL. Behind ye scroll is a FLASK.\nObvious exits are NORTH, SOUTH and DENNIS.\n";
	areaDescription[NORTH] = "Ye thou now be at parapets. Ye see a ROPE.\nObvious exits are SOUTH.\n";
	areaDescription[SOUTH] = "Ye stand yeself close to a yet-unnamed escarpment.\nNonetheless, ye spies a TRINKET.n\Obvious exits are NORTH.\n";
	areaDescription[DENNIS] = "Ye arrive at Dennis.\nHe wears a sporty frock coat and a long jimberjam. He paces about nervously.\nObvious exits are NOT DENNIS.\n";
	loc = MAIN;

	help();
}

// program entry point
int main() {
	do{
		reset();
		play();
		gameRunning = again();
	} while (gameRunning);
	return(0);
}

// main game loop
void play() {	
	while (true) {
		printf("\nWhat wouldst thou deau?\n");
		getCommand();
		system("cls");
		// parse input
		if (verb == "HELP" || verb == "HELPETH") {
			help();
		} else if (verb == "LOOK") {
			if (noun == "") {
				help();
			} else {
				look();
			}
		} else if (verb == "DIE") {
			score -= 100;
			printf("That wasn't very smart. Your score was: %d.\n", score);
			break;
		} else if (verb == "GET") {
			if (!get())
				break;
		} else if (verb =="DANCE") {
			printf("Thou shaketh it a little, and it feeleth all right.\n");
		} else if (verb == "GO") {
			go();
		} else if (loc == DENNIS && verb == "NOT" && noun == "DENNIS") {
			loc = MAIN;
			printf("You go NOT DENNIS.\n");
			help();
		} else if (verb == "TALK") {
			talk();
		} else if (verb == "GIVE") {
			if (give())
				break;
		} else if (verb == "SMELL" || verb == "SNIFF") {
			printf("You smell a Wumpus.\n");
		} else {
			invalidEntry();
		}
	}
}

// get user input
void getCommand() {
	noun = verb = "";
	printf("> ");
	getline(cin, input);	
	transform(input.begin(), input.end(), input.begin(), toupper);
	size_t pos = input.find_first_of(' ');
	verb = input.substr(0, pos);
	if (pos != string::npos)
		noun = input.substr(pos+1, string::npos);
	printf("\n");
}

// look decision tree
void look() {
	if (loc == MAIN) {
		if (noun == "SCROLL") {
			if (!haveScroll) {
				printf("Parchment, definitely parchment. I'd recognize it anywhere.\n");
			} else if (!lookedScroll) {
				printf("Ye seeth nothing wheretofore it went ZAP.\n");
				lookedScroll = true;
			} else {
				printf("There is definitely no YE SCROLL, so drop it.\n");
				areaDescription[MAIN] += "There is definitely no YE SCROLL, so drop it.\n";
			}
		} else if (noun == "FLASK") {
			printf("Looks like you could quaff some serious mead out of that thing.\n");
		} else {
			invalidEntry();
		}
	} else if (loc == NORTH) {
		if (noun == "PARAPETS") {
			printf("Well, they're parapets. This much we know for sure.\n");
		} else if (noun == "ROPE") {
			printf("It looks okay. You've seen better.\n");
		} else {
			invalidEntry();
		}
		
	} else if (loc == SOUTH) {
		if (noun == "TRINKET") {
			if (!haveTrinket) {
				printf("Quit looking! Just get it already.\n");
			} else {
				printf("Just a bulge in thou pouchel at thist point.\n");
			}
		} else {
			invalidEntry();
		}
	} else if (loc == DENNIS) {
		if (noun == "DENNIS") {
			printf("That jimberjam really makes the outfit.\n");
		} else if (noun == "JIMBERJAM") {
			printf("Man, that art a nice jimberjam.\n");
		}
	}
}

// get decision tree; return true = alive, false = gameover
bool get() {
	if (noun == "DAGGER") {
		printf("Yeah, okay.\n");
		score += 25;	// usable infinitely
		return true;
	}
	if (loc == MAIN) {
		if (noun == "SCROLL") {
			if (!haveScroll) {
				printf("Ye takes the scroll and reads of it.\nIt doth say:\nBEWARE, READER OF THE SCROLL, DANGER AWAITS TO THE-\nThe SCROLL disappears in thy hands with ye olde ZAP!\n");
				haveScroll = true;
				score += 2;
				areaDescription[MAIN] = "Ye find yeself in yon dungeon. Back yonder there is a FLASK.\nObvious exits are NORTH, SOUTH and DENNIS.\n";
			} else {
				printf("Ye doth suffer from memory loss. YE SCROLL is no more. Honestly.\n");
				score--;
			}
		} else if (noun == "FLASK") {
			if (getFlaskCount < 2) {
				printf("Ye cannot get the FLASK.\nIt is firmly bolted to a wall which is bolted to the rest of the dungeon\nwhich is probably bolted to a castle.\nNever you mind.\n");
				score++;
				getFlaskCount++;
			} else {
				score -= 1000;
				printf("Okay, okay. You unbolt yon FLASK and hold it aloft.\nA great shaking begins.\nThe dungeon ceiling collapses down on you, crushing you in twain.\nApparently, this was a load-bearing FLASK.\n\nYour score was: %d\n", score);
				return false;
			}
		} else {
			printf("Thou cannotst get that. Quit making stuffeth up!\n");
		}
	} else if (loc == NORTH) {
		if (noun == "ROPE") {
			score--;
			printf("You attempt to take ye ROPE but alas it is enchanted!\nIt glows a mustard red and smells like a public privy.\nThe ROPE wraps round your neck and hangs you from parapets.\nWith your last breath, you wonder what parapets are.\nGAME OVER. Your score was: %d\n", score);
			return false;
		}
	} else if (loc == SOUTH) {
		if (noun == "TRINKET") {
			if (!haveTrinket) {
				printf("Ye getsts yon TRINKET and discover it to be a bauble.\nYou rejoice at your good fortune. You shove the TRINKET in your pouchel.\nIt kinda hurts.\n");
				score += 2;
				areaDescription[SOUTH] = "Ye stand high above a canyon-like depression.\nObvious exits are NORTH.\n";
				haveTrinket = true;
			} else {
				printf("Sigh. The trinket is in thou pouchel. Recallest thou?\n");
				score--;
				areaDescription[SOUTH] += "I shouldn't have to tell ye there is no TRINKET.\n";
			}
		}
	}

	return true;
}

// traversal tree
void go() {
	if (loc == MAIN && noun == "NORTH") {
		loc = NORTH;
		printf("You go NORTH through yon corrider.\nYou arrive at parapets. Ye see a rope.\nObvious exits are SOUTH.\n");
	} else if (loc == MAIN && noun == "SOUTH") {
		loc = SOUTH;
		printf("You head south to an enbankment.\nOr maybe a chasm. You can't decide which.\nAnyway, ye spies a TRINKET.\nObvious exits are NORTH.\n");
	} else if (loc == MAIN && noun == "DENNIS") {
		loc = DENNIS;
		help();
	} else if (loc == NORTH && noun == "SOUTH") {
		loc = MAIN;
		printf("You go SOUTH back through yon corridor.\n");
		help();
	} else if (loc == SOUTH && noun == "NORTH") {
		loc = MAIN;
		printf("You go NORTH.\n");
		help();
	} else {
		printf("Thou cannotst go there. Who do you think thou art? A magistrate?!\n");
	}
}

// talk to dennis... or no one
void talk() {
	if (loc == DENNIS) {
		printf("You engage Dennis in leisurely discussion.\nYe learns that his jimberjam was purchased on sale at a discount market\nand that he enjoys pacing about nervously.\nYou become bored and begin thinking about parapets.\n");
	} else {
		printf("Who is %s? Your new boyfriend?\nSomebody from work you don't want me to meeteth?\n", noun.c_str());
	}
}

// give dennis the trinket; return true = winner
bool give() {
	if (loc == DENNIS && (noun == "TRINKET" || noun == "TRINKET TO DENNIS") && haveTrinket) {
		printf("A novel idea!\nYou givst the TRINKET to Dennis and he happily agrees to tell you what parapets are.\nWith this new knowledge, ye escapes from yon dungeon in order to search for new dungeons and to remain...\nTHY DUNGEONMAN!!\n\nYou hath won! Congraturation!!\nYour score was: %d\n", score);
		return true;
	} else {
		printf("Thou don'tst have a %s to give. Go back to your tiny life.", noun.c_str());
		return false;
	}
}

// play again?
bool again() {
	while (true) {
		printf("Play again? [Y/N]\n");
		getCommand();
		if (verb == "N") {
			return false;
		} else if (verb == "Y") {
			return true;
		}
	}
}