#include <iostream>
#include <vector>
#include <cstring> // String
#include <fstream>
#include <cstdlib> // (rand function)
#include <ctime> // Pseudo-random number generation

using namespace std;
void loadBrain(); // Brain Loading
void saveToBrain(string message); // Saves messages to brain
string userInput(); // Ask for user's input
string botOutput(string mes); // Ask for the bot's reply
void replyWith(string message); // MISC: Decorates text with "BOT: {message}"

// Levenshtein Algorithm
unsigned int levenshtein_distance(string s1, string s2);
/* Memory */
vector<string> memory;

/* Main Function */
int main()
{
  string message, reply;
  /* Initialize Brain */
  loadBrain();
  while(1) // Loop Indefinitely
  {
    /* Ask for user input */
    message = userInput();
    /* Get a reply from bot */
    reply = botOutput(message);
    replyWith(reply);
  }
}
/** Loads the brain from file
* @param NONE
* @return NONE
*/
void loadBrain()
{
  cout << "[BOT CONSOLE] > INITIALIZING BRAIN FILE\n";
  string tmp;
  /* Load the file itself */
  ifstream brainFile("brain.txt");// Bagong simula...
  /* Read the file line by line */
  while(getline(brainFile, tmp))
    memory.push_back(tmp);
  /* Close the file after reading */
  brainFile.close(); // Tutal tapos na siya sayo
  cout << "[BOT CONSOLE] > Brain File loaded!\n"
   << "@exit to end chat session :)\n" // Lahat ng bagay may katapusan :(
   << "BOT: Hi!~\n"; // Pero lahat ay nagsisimula sa simpleng "hi!"
}
/** Appends the message into the file
* @param {string} message to Append
*/
void saveToBrain(string message)
{
  /* Append the message to the local file */
  ofstream brainFile("brain.txt", ios::app);
  brainFile << message << "\n";
  brainFile.close();

  /* Push the message to the memory */
  /*
  To prevent Segmentation fault
  if memory is undefined
  */
  memory.push_back(message);
}
/** Ask for user input
* @param NONE
* @return {string} User's input
*/
string userInput()
{
  string mes;
  do {
    cout << "YOU: ";
    getline(cin, mes);
    // Check for Commands
    if(mes == "@exit") exit(0); // Exit if the user said so :)
  } while(mes == "");
  return mes;
}
/** Generate Reply
* @param {string} The user's message
* @return {string} Bot's Reply
*/
string botOutput(string mes) // Use reference instead of storing variable
{
  /* Configuration */
  float errorRate, errorThreshold = 0.40; // 40% Allowable Error
  int dist; // Edit Distance
  vector<string> replies; // Used for storing reply candidates
  /* Step 1: Generate Replies */
  for(int i=0; i< memory.size(); i++)
  {
    dist = levenshtein_distance(mes, memory[i]);
    errorRate = (float)dist / (float)mes.size();

    if(errorRate <= errorThreshold) { // Push reply candidates
      if(memory.size() > i+1) {// Check if the next line is the end of file
        if(memory[i+1] != "") {// Check if the next line is not empty
          replies.push_back(memory[i+1]); // OKAY!
        }
      }
    }
  }

  /* Step 2: Validate Replies */
  srand(time(NULL));// Seed Random Function (PRNG)
  if(replies.size() == 0) // ERROR: No Reply Candidate found!
  {
    /* Step 0: Learn from the user */
      saveToBrain(mes);
    // Step 1: Return the user's input
      replyWith(mes+"?");
    // Step 2: Learn from the user's reply
      string reply = userInput();
    // Step 3: Save it to the file
      saveToBrain(reply);
    // Step 4: Divert! If Possible! :)
    if(memory.size() > 0) {
      int randomNumber = rand() % memory.size();
      saveToBrain(memory[randomNumber]);
      return memory[randomNumber];
    }

  } else { // FOUND a reply candidate
    /* Action: Reply */
    int randomNumber = rand() % replies.size();
    return(replies[randomNumber]);
  }
}
/** Send Message
* @param {string} Message to Send
* @return NONE
*/
void replyWith(string message) //  = Reference
{
  cout << "BOT: " << message << "\n";
}

/************************************************************************************************/
/***************************************** DO NOT TOUCH *****************************************/
/************************************************************************************************/
unsigned int levenshtein_distance(string s1, string s2)
{
	const size_t len1 = s1.size(), len2 = s2.size();
	vector<unsigned int> col(len2+1), prevCol(len2+1);
	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;// height
    for (unsigned int j = 0; j < len2; j++)
			col[j+1] = min(min(prevCol[j + 1] + 1, col[j] + 1), prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
		col.swap(prevCol);
	}
	return prevCol[len2];
}
