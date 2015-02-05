//https://github.com/hannahhoffman/Lab03

#include "Password.h"
#include "ListArray.h"
#include "ListArrayIterator.h"
using CSC2110::ListArrayIterator;
using CSC2110::ListArray;
using CSC2110:: Password;

#include <iostream>
using namespace std;

int Password::getNumMatches(String* curr_word, String* word_guess)
{
	//counter for number of matches that have been found
	int num_matches=0;
	for (int i=0; i<len; i++)
	{
		//iterate through each letter of the words and check each character (position specific)
		if (curr_word-> charAt(i) == word_guess-> charAt(i))
		{
			//if we find a match increment the counter
			num_matches++;
		}
	}
	return num_matches;
	//if (curr_word-> charAt(i) == word_guess-> charAt(i))
}

Password::Password() //constructor
{
	viable_words = new ListArray<String>();
	all_words = new ListArray<String>();
	len=0;
}
Password::~Password() //destructor (there is work to do here, delete the individual words)
{
	delete viable_words;
	delete all_words;
}
void Password::addWord(String* word) //add a word to the list of possible passwords
{
	if (len == 0) // if len is 0; ie this is the first word
	{
		len = word->length(); // set len to the length of the first 
	}

	else if (word->length() == len)// if the length of the word being added is the same as the length of the first word
	{
		viable_words-> add(word); // add to viable words list
		all_words-> add(word); // add to all words list
	}
	//there may be better solutions rather than simply ignoring words that are typed incorrectly, maybe tell the user when a word is excluded?
	else
	{
		cout<<"word ";
		word->displayString();
		cout<<" was excluded from list of possible passwords. (length incorrect)\n";
	}
}
void Password::guess(int try_password, int num_matches) //index of guessed word in the list of all words (1-based), number of matches reported by fallout 3, update viable passwords list
{
	// if it's not the word, take it off viable words
	// in other words; if getNumMatches!= num_matches remove from viable words

	String *word_guess = getOriginalWord(try_password);
	ListArrayIterator<String>* viable_iter = viable_words->iterator();
	ListArray<String>* temp = new ListArray<String>();

	while (viable_iter -> hasNext())
	{
		String* word_check =viable_iter-> next();
		if (getNumMatches(word_check, word_guess) == num_matches)
		{
			temp-> add(word_check);
		}
	}
	
	delete viable_words;
	viable_words = temp;
	//delete iter????
	//scope of temp expires and memory is deallocated automatically 
}
int Password::getNumberOfPasswordsLeft() //returns the number of possible passwords remaining
{
	//too easy, return the size of the array that holds the possible passwords
	return viable_words-> size();
}

void Password::displayViableWords() //display the current list of possible passwords
{
	//we need an iterator to pass through the list (it just works better that way)
	ListArrayIterator<String>* viable_iter = viable_words->iterator();
	
	//while there are more words to display
	while (viable_iter -> hasNext())
	{
		//get the word at the current iterator position
		String* cur_word= viable_iter-> next();
		//display that word, and then an endline character to make it look nice
		cur_word-> displayString();
		cout<<endl;
	}
}


String* Password::getOriginalWord(int index) //get a word from the original list of all passwords, 1-based
{
	// go to the original list and get the word
	return all_words-> get(index);

}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}