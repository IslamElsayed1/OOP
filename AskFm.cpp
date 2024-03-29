#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Question
{
   int id;
   int from;
   int to;
   string question;
};

struct Answer
{
   int id;
   int from;
   int to;
   int QorA_ID;
   string answer;
};

struct User
{
   int id;
   int password;
   string name;
};

class AskMe
{
private:
   vector<Question> questions;
   vector<Answer> answers;
   vector<User> users;

public:
   AskMe() {}

   void writeCountersToFile(int questionId, int answerId)
   {
      ofstream file("counters.txt", ios::trunc);
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      file << questionId << " " << answerId;
      file.close();
   }
   //======================================================================
   //======================================================================
   void readCountersFromFile(int &questionId, int &answerId)
   {

      ifstream file("counters.txt");
      if (file)
      {
         file >> questionId >> answerId;
         file.close();
      }
      else
      {
         cout << "Error opening file" << endl;
      }
   }

   //======================================================================
   //======================================================================
   void writeQuestionsToFile(Question newQuestion)
   {

      ofstream file("questions.txt", ios::app);
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      file << newQuestion.id << " " << newQuestion.from << " " << newQuestion.to << " " << newQuestion.question << endl;
      file.close();
   }

   //================================================================
   //================================================================
   void writeAnswersToFile(Answer newAnswer)
   {
      ofstream file("answers.txt", ios::app);
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      file << newAnswer.id << " " << newAnswer.from << " " << newAnswer.to << " " << newAnswer.QorA_ID << " " << newAnswer.answer << endl;
      file.close();
   }
   //================================================================
   //================================================================

   void writeUsersToFile(User newUser)
   {
      ofstream file("users.txt", ios::app);
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      file << newUser.id << " " << newUser.password << " " << newUser.name << endl;
      file.close();
   }

   //================================================================
   //================================================================

   void addQuestion(int from, int to, string questionText)
   {
      Question newQuestion;
      int questionId, answerId;
      readCountersFromFile(questionId, answerId);
      newQuestion.id = questionId;
      newQuestion.from = from;
      newQuestion.to = to;
      newQuestion.question = questionText;
      questions.push_back(newQuestion);
      writeQuestionsToFile(newQuestion);
      writeCountersToFile(newQuestion.id + 1, answerId);
   }

   //================================================================
   //================================================================

   void addAnswer(int from, int to, int questionIdToAddAnswerTo, string answerText)
   {

      Answer newAnswer;
      int questionId, answerId;
      readCountersFromFile(questionId, answerId);
      newAnswer.id = answerId;
      newAnswer.from = from;
      newAnswer.to = to;
      newAnswer.QorA_ID = questionIdToAddAnswerTo;
      newAnswer.answer = answerText;
      answers.push_back(newAnswer);
      writeAnswersToFile(newAnswer);

      writeCountersToFile(questionId, newAnswer.id + 1);
   }

   //================================================================
   //================================================================

   void addUser(int id, int password, string name)
   {
      User newUser;
      newUser.id = id;
      newUser.password = password;
      newUser.name = name;
      users.push_back(newUser);
      writeUsersToFile(newUser);
   }

   //================================================================
   //================================================================

   void loadUsersFromFile()
   {
      users.clear();
      ifstream file("users.txt");
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      User user;
      while (file >> user.id >> user.password)
      {
         getline(file, user.name);
         users.push_back(user);
      }
      file.close();
   }

   //================================================================
   //================================================================

   void loadQuestionFromFile()
   {
      questions.clear();
      ifstream file("questions.txt");
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      Question question;
      while (file >> question.id >> question.from >> question.to)
      {
         file.ignore();
         getline(file, question.question);
         questions.push_back(question);
      }
      file.close();
   }

   //================================================================
   //================================================================

   void loadAnswerFromFile()
   {
      answers.clear();
      ifstream file("answers.txt");
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      Answer answer;
      while (file >> answer.id >> answer.from >> answer.to >> answer.QorA_ID)
      {
         file.ignore();
         getline(file, answer.answer);
         answers.push_back(answer);
      }
      file.close();
   }

   //================================================================
   //================================================================
   void printQuestionAndAnswer(int questionId)
   {
      for (auto reply : answers)
      {
         if (reply.QorA_ID == questionId)
         {
            cout << "    Reply ID: " << reply.id << " To Q or A ID " << reply.QorA_ID << " From: " << reply.from << " To: " << reply.to << " Reply: " << reply.answer << "\n";
            printQuestionAndAnswer(reply.id);
         }
      }
   }

   void printQuestionAndAnswer()
   {
      loadQuestionFromFile();
      loadAnswerFromFile();

      cout << "Questions and Answers:\n";

      for (auto question : questions)
      {
         cout << "Question ID: " << question.id << " From: " << question.from << " To: " << question.to << " Question: " << question.question << "\n";

         bool foundAnswer = false;
         for (auto answer : answers)
         {
            if (answer.QorA_ID == question.id)
            {
               foundAnswer = true;
               cout << "  Answer ID: " << answer.id << " To QID " << answer.QorA_ID << " From: " << answer.from << " To: " << answer.to << " Answer: " << answer.answer << "\n";

               printQuestionAndAnswer(answer.id);
            }
         }

         if (!foundAnswer)
         {
            cout << "  No answer found for this question.\n";
         }

         cout << "\n";
      }
   }

   //================================================================
   //================================================================

   bool findUserById(int userId, int password)
   {
      loadUsersFromFile();
      for (auto user : users)
      {
         if (user.id == userId && user.password == password)
         {
            return true;
         }
      }
      return false;
   }

   //================================================================
   //================================================================

   void deleteQuestion(int questionId)
   {
      questions.clear();
      ifstream file("questions.txt");
      if (!file)
      {
         cout << "Error opening file" << endl;
         return;
      }
      Question question;
      while (file >> question.id >> question.from >> question.to)
      {
         getline(file, question.question);
         if (question.id != questionId)
         {
            questions.push_back(question);
         }
      }
      file.close();

      ofstream outfile("questions.txt", ios::trunc);
      for (auto q : questions)
      {
         outfile << q.id << " " << q.from << " " << q.to << " " << q.question << "\n";
      }
      outfile.close();

      cout << "Question deleted successfully" << endl;
   }

   //================================================================
   //================================================================

   void displayUsers()
   {
      loadUsersFromFile();
      if (users.empty())
      {
         cout << "No users found" << endl;
         return;
      }
      cout << "List of users:\n";
      for (auto user : users)
      {
         cout << "ID: " << user.id << " Password: " << user.password << " Name: " << user.name << endl;
      }
   }

   //================================================================
   //================================================================

   void printMenu()
   {
      loadUsersFromFile();
      loadQuestionFromFile();
      loadAnswerFromFile();
      while (true)
      {
         cout << "\nMenu:" << endl;
         cout << "1- Print Questions and Answers" << endl;
         cout << "2- Answer Questions" << endl;
         cout << "3- Delete Question" << endl;
         cout << "4- Ask a Question" << endl;
         cout << "5- Display Users" << endl;
         cout << "6- Exit" << endl;

         int option;
         cout << "Enter option: ";
         cin >> option;

         int printFrom;
         int answerFrom, answerTo, QorA_ID;
         string answerText;
         int deleteId;
         int questionFrom, questionTo;
         string questionText;

         switch (option)
         {
         case 1:
            printQuestionAndAnswer();
            break;
         case 2:
            cout << "Enter from ID and to ID and QuestionId: ";
            cin >> answerFrom >> answerTo >> QorA_ID;
            cin.ignore();
            cout << "Enter the answer: ";
            getline(cin, answerText);
            addAnswer(answerFrom, answerTo, QorA_ID, answerText);
            break;
         case 3:
            cout << "Enter the question ID you want to delete: ";
            cin >> deleteId;
            deleteQuestion(deleteId);
            break;
         case 4:
            cout << "Enter from ID and to ID: ";
            cin >> questionFrom >> questionTo;
            cin.ignore();
            cout << "Enter the question: ";
            getline(cin, questionText);
            addQuestion(questionFrom, questionTo, questionText);
            break;
         case 5:
            displayUsers();
            break;
         case 6:
            return;
            break;
         default:
            cout << "Invalid option" << endl;
            break;
         }
      }
   }

   //================================================================
   //================================================================

   void login()
   {
      int userId, password;
      cout << "Enter User_ID and Password: ";
      cin >> userId >> password;

      if (findUserById(userId, password))
      {
         printMenu();
      }
      else
      {
         cout << "User not found" << endl;
      }
   }
};

int main()
{
   AskMe AskMe;
   cout << "1- Login\n";
   cout << "2- Sign Up\n";
   int option;
   cin >> option;

   string userName;
   switch (option)
   {
   case 1:
      AskMe.login();
      break;
   case 2:
      int userId, password;
      cout << "Enter User_ID: ";
      cin >> userId;
      cout << "Enter password: ";
      cin >> password;
      cout << "Enter user name: ";
      cin.ignore();
      getline(cin, userName);
      AskMe.addUser(userId, password, userName);
      cout << "User added successfully" << endl;
      break;
   default:
      cout << "Invalid option" << endl;
      break;
   }

   return 0;
}
