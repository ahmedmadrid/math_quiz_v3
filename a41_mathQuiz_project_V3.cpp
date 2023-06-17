#include <iostream>
#include <cstdlib>
using namespace std;

/*
* module 1: read data from the user
*/

// data
enum enQuestionLevel { EASY = 1, MEDIUM = 2, HARD = 3, MIXED = 4 };

enum enOpType { ADD = 1, SUBTRACT = 2, MULTIPLY = 3, DIVIDE = 4, MIX = 5 };

enum enUserPassedFailed { PASSED = 1, FAILED = 2, DRAW = 3 };

// operations
int readRange(int from, int to, string message, string warning) {
	int num{};
	bool isCinFailed{};
	do {
		cout << message;
		cin >> num;

		/*
		* validation:
		* ignore strings
		*/

		isCinFailed = cin.fail();
		if (isCinFailed) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << warning;
		}
		else if (num < from || num > to)
			cout << warning;

	} while ((num < from || num > to) || isCinFailed);

	return num;
}


/*
* module 2: generate questions
*/

// data
struct stUser {
	int answer{};
	bool hasRightAnswer{};
	enUserPassedFailed isPAssed;
};

struct stQuestion {
	int number1{};
	int number2{};
	int answer{};
	enQuestionLevel questionLevel = enQuestionLevel::EASY;
	enOpType opType = enOpType::DIVIDE;
};

// operations
enQuestionLevel getQuestionLevel(int userChoice) {
	return (enQuestionLevel)userChoice;
}

enOpType getOpType(int userChoice) {
	return (enOpType)userChoice;
}

int randomNumber(int from, int to) {
	return rand() % (to - from + 1) + from;
}

int calculateAnswer(stQuestion question) {

	switch (question.opType)
	{
	case enOpType::ADD:
		question.answer = question.number1 + question.number2;
		break;
	case enOpType::SUBTRACT:
		question.answer = question.number1 - question.number2;
		break;
	case enOpType::MULTIPLY:
		question.answer = question.number1 * question.number2;
		break;
	default:
		question.answer = question.number1 / question.number2;
	}

	return question.answer;
}

string repeatString(string key, int numOfTimes) {
	string word{ "" };
	for (int time = 0; time < numOfTimes; time++)
		word += key;
	return word;
}

int getUserAnswer() {
	int answer{};
	cin >> answer;
	return answer;
}

stQuestion generateQuestion(enQuestionLevel level, enOpType opType) {
	stQuestion question;

	if (level == enQuestionLevel::MIXED)
		level = getQuestionLevel(randomNumber(1, 3));

	if (opType == enOpType::MIX)
		opType = getOpType(randomNumber(1, 4));

	question.questionLevel = level;
	question.opType = opType;

	switch (level)
	{
	case enQuestionLevel::EASY:
		question.number1 = randomNumber(1, 10);
		question.number2 = randomNumber(1, 10);
		break;
	case enQuestionLevel::MEDIUM:
		question.number1 = randomNumber(10, 50);
		question.number2 = randomNumber(10, 50);
		break;
	default:
		question.number1 = randomNumber(50, 100);
		question.number2 = randomNumber(50, 100);
	}

	// answer the question
	question.answer = calculateAnswer(question);
	return question;
}

struct stQuiz {
	// has # of questions
	stQuestion questions[100];
	// has a single user
	stUser user;

	enQuestionLevel questionLevel;
	enOpType opType;

	int numOfQuestions{};
	int numOfRightAnswers{};
	int numOfWrongAnswers{};
	bool isPassed{};
};

void generateQuestions(stQuiz& quiz) {

	for (int number = 0; number < quiz.numOfQuestions; number++) {
		quiz.questions[number] = generateQuestion(quiz.questionLevel, quiz.opType);
	}
}

string getOpSymbol(enOpType opType) {
	string symbols[] = { "+","-","*","/" };
	return symbols[opType - 1];
}

void printQuestion(stQuiz quiz, int questionNumber) {

	cout << "\nQuestion number [" << questionNumber+1 << "/" << quiz.numOfQuestions << "]\n";
	cout << quiz.questions[questionNumber].number1 << '\n';
	cout << '\n' << quiz.questions[questionNumber].number2 << '\n';
	cout << "   " << getOpSymbol(quiz.questions[questionNumber].opType) << '\n';
	cout << repeatString("_", 15) << endl;

}

bool validateUserAnswer(stQuiz& quiz, int questionNumber) {

	quiz.user.answer = getUserAnswer();

	if (quiz.questions[questionNumber].answer == quiz.user.answer) {
		quiz.user.hasRightAnswer = true;
		quiz.numOfRightAnswers++;
		return true;
	}
	else {
		quiz.user.hasRightAnswer = false;
		quiz.numOfWrongAnswers++;
		return false;
	}
}

void messageTheUser(stQuiz& quiz, int questionNumber) {
	
	if (validateUserAnswer(quiz, questionNumber))
		cout << "\nRight answer :-)\n";
	else {
		cout << "\nWrong answer :-(\n";
		cout << "\nThe right answer is " << quiz.questions[questionNumber].answer << endl;
	}
}

/*
* Module 3
*/

// data

enUserPassedFailed isUserPAssed(stQuiz& quiz) {
	// supposed to store userIsPassed
	if (quiz.numOfRightAnswers > quiz.numOfWrongAnswers)
		quiz.user.isPAssed = enUserPassedFailed::PASSED;
	else if(quiz.numOfRightAnswers < quiz.numOfWrongAnswers)
		quiz.user.isPAssed = enUserPassedFailed::FAILED;
	else
		quiz.user.isPAssed = enUserPassedFailed::DRAW;

	return quiz.user.isPAssed;
}

// operations

string userQuizState(enUserPassedFailed userState) {
	switch (userState)
	{
	case enUserPassedFailed::PASSED:
		return "Passed -:)";
	case enUserPassedFailed::FAILED:
		return "Failed -:(";
	default:
		return "Draw";
	}
}

// the idea is not eay or difficult. However, the games screen and final summary are two different entities!
// so we divided them into two seperate functions.

void gameScreen(stQuiz& quiz) {
	cout << repeatString("_", 50) << endl;
	
	cout << "\nFinal result is " << userQuizState(isUserPAssed(quiz)) << endl;

	cout << repeatString("_", 50) << endl;
}

string printQuizOpType(enOpType opType) {
	string opTypeArray[] = {"Add", "Subtract", "Multiply", "Divide", "MixOp"};
	return opTypeArray[opType - 1];
}

string printQuestionLevel(enQuestionLevel questionLevel) {
	string questionLevelArray[] = { "Easy", "Meduim", "Hard", "Mixed"};
	return questionLevelArray[questionLevel - 1];

}

void gameSummary(stQuiz quiz) {
	gameScreen(quiz);

	cout << "Number of questions " << quiz.numOfQuestions << endl;
	cout << "Question level\t" << printQuestionLevel(quiz.questionLevel) << endl;
	cout << "OpType\t\t" << printQuizOpType(quiz.opType) << endl;
	cout << "Number of right questions are " << quiz.numOfRightAnswers << endl;
	cout << "Number of wrong questions are " << quiz.numOfWrongAnswers << endl;
}

int main()
{
	srand((unsigned)time(NULL));

	stUser user;
	stQuestion q;

	q.number1 = 10;
	q.number2 = 2;
	q.opType = enOpType::ADD;
	q.questionLevel = enQuestionLevel::MEDIUM;
	q.answer = calculateAnswer(q);
	
	stQuestion ques;
	ques = generateQuestion(enQuestionLevel::EASY, enOpType::MIX);
	cout << "n1 " << ques.number1 << endl;
	cout << "n2 " << ques.number2 << endl;
	cout << "" << ques.answer << endl;
	string types[] = {"ADD", "S", "M", "D"," MIXED"};
	cout << "stQues: " << types[ques.opType - 1] << endl;
	
	stQuiz quiz;
	quiz.numOfQuestions = 4;
	quiz.opType = enOpType::MIX;
	quiz.questionLevel = enQuestionLevel::EASY;
	cout << "stQuiz: " << types[quiz.opType - 1] << endl;

	generateQuestions(quiz);

	cout << "generate question: \n";

	
	for (int i = 0; i < quiz.numOfQuestions; i++) {
		printQuestion(quiz, i);
		messageTheUser(quiz, i);
	} 

	

	gameSummary(quiz);

}

