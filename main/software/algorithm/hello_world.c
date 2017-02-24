/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>




int main()
{


	int solutions[25] = {0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0};


	int engineeringScore = 0;
		int artistScore = 0;
		int environmentScore = 0;
		int teacherScore = 0;
		int musicianScore = 0;
		int athleteScore = 0;
		int constructionWorkerScore = 0;
		int policeScore = 0;
		int politicianScore = 0;
		int doctorScore = 0;
		int lawyerScore = 0;
		int scientistScore = 0;

	//Sample QuestionNo.  {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
	int engineering[25] = {0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0};
	int artist[25] =      {0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int environment[25] = {0,0,0,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0};
	int teacher[25] =     {0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int musician[25] =    {0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int athlete[25] =     {0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0};
	int Construct[25] =   {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1};
	int police[25] =      {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0};
	int politician[25] =  {1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int doctor[25] =      {1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,0};
	int lawyer[25] =      {1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0};
	int scientist[25] =   {0,1,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0};

	int i = 0;
	//Assign score to all the professions based on the input by the user
	for( i = 0; i < 25; i++ )
	{

	engineeringScore = engineeringScore + (engineering[i] & solutions[i]);
	artistScore = artistScore + (artist[i] & solutions[i]);
	environmentScore = environmentScore + (environment[i] & solutions[i]);
	teacherScore = teacherScore + (teacher[i] & solutions[i]);
	musicianScore = musicianScore + (musician[i] & solutions[i]);
	athleteScore =  athleteScore + (athlete[i] & solutions[i]);
	constructionWorkerScore = constructionWorkerScore + (Construct[i] & solutions[i]);
	policeScore = policeScore + (police[i] & solutions[i]);
	politicianScore = politicianScore + (politician[i] & solutions[i]);
	doctorScore = doctorScore + (doctor[i] & solutions[i]);
	lawyerScore = lawyerScore + (lawyer[i] & solutions[i]);
	scientistScore = scientistScore + (scientist[i] & solutions[i]);

	}

	void priorityPrint(char profession[25]){
		printf("%s", profession);
	}
/*
//Check for the profession
	int score;
	int priority = 1;
	int flag = 0;

	for( score = 25; score > 0; score++){

		if(priority > 0){

			if(engineeringScore == score){
				priorityPrint("engineering");
				flag = 1;
			}

			if(artistScore == score){
				priorityPrint("artist");
				flag = 1;
			}

			if(environmentScore == score){
				priorityPrint("environment");
				flag = 1;
			}

			if(teacherScore == score){
				priorityPrint("teacher");
				flag = 1;
			}

			if(musicianScore == score){
				priorityPrint("musician");
				flag = 1;
			}

			if(athleteScore == score){
				priorityPrint("athlete");
				flag = 1;
			}

			if(constructionWorkerScore == score){
				priorityPrint("ConstructionWorker");
				flag = 1;
			}

			if(policeScore == score){
				priorityPrint("police");
				flag = 1;
			}

			if(politicianScore == score){
				priorityPrint("politician");
				flag = 1;
			}

			if(doctorScore == score){
				priorityPrint("doctor");
				flag = 1;
			}

			if(lawyerScore == score){
				priorityPrint("lawyer");
				flag = 1;
			}

			if(scientistScore == score){
				priorityPrint("scientist");
				flag = 1;
			}

			priority = priority + flag;
			if(priority > 3) priority  = 0;
		}

	}

	*/

	return (0);
}
