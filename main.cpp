#include <stdlib.h>
#include <gtk/gtk.h>
#include <vector>
#include <time.h>
#include <map>

using namespace std;

class question
{
private:
   const gchar* q;
   const gchar* answs[3];
   gint correct;
public:
   question(gchar* _q, gchar* _ans0, gchar* _ans1, gchar* _ans2, gint _c)
   {
      q = _q;
      answs[0] = _ans0;
      answs[1] = _ans1;
      answs[2] = _ans2;
      correct = _c;
   }

   const gchar* getQuestion()
   {
      return q;
   }

   const gchar* getAnswer(int answerNumber)
   {
      return answs[answerNumber];
   }

   gint getCorrect()
   {
      return correct;
   }
};

//Globals{
//Vector of questions asked in current session
vector<question> questionsVector;

//Indicates which question from questionsVector is asked now
int currentQuestionNumber;

//How many questions will be in the test
int testSize;
//}Globals

//Function prototypes{
void addQuestion(vector<question> &v, gchar* q, gchar* ans0, gchar* ans1, gchar* ans2, gint c);

static gboolean startTest(GtkWidget *startButton, 
                             GdkEventButton *event,
                             gpointer data );

static gboolean previousQuestion(GtkWidget *buttonBack, 
                                 GdkEventButton *event,
                                 gpointer data );

static gboolean nextQuestion(GtkWidget *buttonForward, 
                             GdkEventButton *event,
                             gpointer data );

// static gboolean changeQuestion(GtkWidget *startButton, 
//                                GdkEventButton *event,
//                                gpointer data );
//}Function prototypes

//All GTK widgets{
GtkWidget *mainWindow;
GtkWidget *verticalBox;
GtkWidget *questionLabel;
GtkWidget *answerRadios[3];
GtkWidget *answerLabels[3];
GtkWidget *horizontalBox;
GtkWidget *buttonBack;
GtkWidget *buttonForward;
GtkWidget *buttonStart;
//}All GTK widgets

int main (int argc, char *argv[])
{

   //Create questions library
   vector<question> questionsLib;

   //Вопросы Димы{
   addQuestion(questionsLib,
            "Основным свойством типизированных файлов является... ?",
            "Последовательность однотипных компонентов в структуре данных",
            "Возможность заполнения структуры данных неоднотипными компонентами",
            "Разновидность реализации структур данных",
            0);
   addQuestion(questionsLib,
            "Второе название типизированных файлов?",
            "Элементарные файлы",
            "Структурированные файлы",
            "Файлы произвольного доступа",
            2);

   addQuestion(questionsLib,
            "Первое действие при работе с типизированными файлами?",
            "Связь имени файловой переменной с внешним именем файла (assign)",
            "Открытие файла для записи (rewrite)",
            "Открытие файла для чтения (reset)",
            0);

   addQuestion(questionsLib,
            "Оператор, служащий для установления положения указателя?",
            "putcur",
            "find",
            "seek",
            2);

   addQuestion(questionsLib,
            "Описание файлового типа для работы с типизированными файлами?",
            "file",
            "file of <тип элементов>",
            "file of <имя файла>",
            1);

   addQuestion(questionsLib,
            "Какое расширение имеют файлы, созданные программой в Pascal?",
            "Имеют расширение заданное пользователем",
            ".file",
            ".dat",
            0);

   addQuestion(questionsLib,
            "Какую позицию имеет начальный компонент в типизированных файлых?",
            "1",
            "0",
            "Заданную",
            1);

   addQuestion(questionsLib,
            "Какое значение выдает оператор конца файла (eof)?",
            "Строковое (string)",
            "Числовое (integer)",
            "Логическое (boolean(true/false))",
            2);

   addQuestion(questionsLib,
            "Какую позицию занимает указатель при использовании операторов reset/rewrite?",
            "Начальную",
            "Первую",
            "Следующую",
            0);

   addQuestion(questionsLib,
            "С помощью каких операторов осуществляется запись и чтение файла?",
            "wrt, rdf",
            "write, read",
            "writefile, readfile",
            1);
   //}Вопросы Димы

   //Fill questionsVector{
   testSize = 10;
   srand(time(NULL));
   vector<int> numbers;
   for(int i = 0; i < questionsLib.size(); i++)
   {
      numbers.push_back(i);
   }
   for(int i = 1; i < questionsLib.size(); i++)
   {
      int j = rand() % i;
      int h;
      h = numbers[j];
      numbers[j] = numbers[i];
      numbers[i] = h;
   }
   for(int i = 0; i < testSize; i++)
   {
      questionsVector.push_back(questionsLib[numbers[i]]);
   }
   //}Fill questionsVector

   //const char* mainFont = "Tahoma 20";

   currentQuestionNumber = 0;

   

   // Initialize GTK+{
   g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
   gtk_init (&argc, &argv);
   g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);
   // }Initialize GTK+

   // Create main window{
   mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_container_set_border_width (GTK_CONTAINER (mainWindow), 10);
   gtk_window_set_title (GTK_WINDOW (mainWindow), "Программа тестирования знаний языка Pascal");
   gtk_window_resize(GTK_WINDOW (mainWindow), 640, 480);
   gtk_window_set_position (GTK_WINDOW (mainWindow), GTK_WIN_POS_CENTER);
   gtk_widget_realize (mainWindow);
   g_signal_connect (mainWindow, "destroy", gtk_main_quit, NULL);
   //}Create main window

   // Create vertical box{
   verticalBox = gtk_vbox_new (FALSE, 6);
   gtk_container_add (GTK_CONTAINER (mainWindow), verticalBox);
   // }Create vertical box

   // Create question label{
   questionLabel = gtk_label_new (g_locale_to_utf8("Вас приветствует программа тестирования знаний языка программирования Pascal! Нажмите Начать тест, когда преподаватель скажет это сделать", -1, NULL, NULL, NULL));
   //gtk_widget_modify_font(questionLabel, pango_font_description_from_string(mainFont));
   gtk_box_pack_start (GTK_BOX (verticalBox), questionLabel, TRUE, FALSE, 10);
   gtk_label_set_line_wrap(GTK_LABEL(questionLabel),TRUE);
   // }Create question label

   // Create three answer variants{
   answerRadios[0] = gtk_radio_button_new(NULL);
   for(int i = 1; i < 3; i++)
   {
      answerRadios[i] = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(answerRadios[0]));
   }
   for(int i = 0; i < 3; i++)
   {
      answerLabels[i] = gtk_label_new (g_locale_to_utf8(questionsVector[currentQuestionNumber].getAnswer(i), -1, NULL, NULL, NULL));
   }
   for(int i = 0; i < 3; i++)
   {
      gtk_container_add(GTK_CONTAINER(answerRadios[i]), GTK_WIDGET(answerLabels[i]));
   }
   for(int i = 0; i < 3; i++)
   {
      gtk_box_pack_start (GTK_BOX (verticalBox), answerRadios[i], FALSE, FALSE, 10);
   }
   // }Create three answer variants

   // Create horizontal box{
   horizontalBox = gtk_hbox_new (FALSE, 6);
   gtk_box_pack_start (GTK_BOX (verticalBox), horizontalBox, FALSE, FALSE, 10);
   // }Create horizontal box

   // Create buttons ">>" and "<<"{
   buttonBack = gtk_button_new_with_label("<<");
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonBack, TRUE, TRUE, 10);
   g_signal_connect (buttonBack, "button_press_event", G_CALLBACK(previousQuestion), NULL);
   buttonForward = gtk_button_new_with_label(">>");
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonForward, TRUE, TRUE, 10);
   g_signal_connect (buttonForward, "button_press_event", G_CALLBACK(nextQuestion), NULL);
   // }Create buttons ">>" and "<<"

   // Create "Start" button{
   buttonStart = gtk_button_new_with_label(g_locale_to_utf8("Начать тест", -1, NULL, NULL, NULL));
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonStart, TRUE, TRUE, 10);
   g_signal_connect (buttonStart, "button_press_event", G_CALLBACK(startTest), NULL);
   // }Create "Start" button

   // Enter the main loop
   gtk_widget_show_all(mainWindow);
   for(int i = 0; i < 3; i++)
   {
      gtk_widget_hide(answerRadios[i]);
   }
   gtk_widget_hide(buttonBack);
   gtk_widget_hide(buttonForward);
   gtk_main ();
   return 0;
}

static gboolean startTest(GtkWidget *startButton, 
                          GdkEventButton *event,
                          gpointer data )
{
   gtk_widget_show_all(verticalBox);
   gtk_widget_hide(startButton);
   gtk_label_set_text(GTK_LABEL(questionLabel), g_locale_to_utf8(questionsVector[currentQuestionNumber].getQuestion(), -1, NULL, NULL, NULL));
   if(currentQuestionNumber == 0)
   {
      gtk_widget_set_sensitive(buttonBack, false);
   }
   return true;
}

static gboolean previousQuestion(GtkWidget *buttonBack, 
                                 GdkEventButton *event,
                                 gpointer data )
{
   if(currentQuestionNumber > 0)
   {
      currentQuestionNumber--;
   }
   //fill questionLabel with appropriate question
   gtk_label_set_text(GTK_LABEL(questionLabel), g_locale_to_utf8(questionsVector[currentQuestionNumber].getQuestion(), -1, NULL, NULL, NULL));

   //fill the array of answers with appropriate answer variants
   for(int i = 0; i < 3; i++)
   {
      gtk_label_set_text(GTK_LABEL(answerLabels[i]), g_locale_to_utf8(questionsVector[currentQuestionNumber].getAnswer(i), -1, NULL, NULL, NULL));
   }
   if(currentQuestionNumber == 0)
   {
      gtk_widget_set_sensitive(buttonBack, false);
   }

   if(currentQuestionNumber < testSize - 1)
   {
      gtk_widget_set_sensitive(buttonForward, true);
   }
   return true;
}

static gboolean nextQuestion(GtkWidget *buttonForward, 
                             GdkEventButton *event,
                             gpointer data )
{
   if(currentQuestionNumber < testSize - 1)
   {
      currentQuestionNumber++;
   }
   //fill questionLabel with appropriate question
   gtk_label_set_text(GTK_LABEL(questionLabel), g_locale_to_utf8(questionsVector[currentQuestionNumber].getQuestion(), -1, NULL, NULL, NULL));

   //fill the array of answers with appropriate answer variants
   for(int i = 0; i < 3; i++)
   {
      gtk_label_set_text(GTK_LABEL(answerLabels[i]), g_locale_to_utf8(questionsVector[currentQuestionNumber].getAnswer(i), -1, NULL, NULL, NULL));
   }
   if(currentQuestionNumber == testSize - 1)
   {
      gtk_widget_set_sensitive(buttonForward, false);
   }
   if(currentQuestionNumber > 0)
   {
      gtk_widget_set_sensitive(buttonBack, true);
   }
   return true;
}

void addQuestion(vector<question> &v, gchar* q, gchar* ans0, gchar* ans1, gchar* ans2, gint c)
{
   question * currentQuestion = new question(q, ans0, ans1, ans2, c);
   v.push_back(*currentQuestion);
}