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
//Create vector of questions asked in current session
vector<question> questionsVector;

int currentQuestionNumber;
//}Globals

//Function prototypes{
void addQuestion(vector<question> &v, gchar* q, gchar* ans0, gchar* ans1, gchar* ans2, gint c);

static gboolean startTest(GtkWidget *startButton, 
                             GdkEventButton *event,
                             gpointer data );

// static gboolean changeQuestion(GtkWidget *startButton, 
//                                GdkEventButton *event,
//                                gpointer data );
//}Function prototypes

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
   int testSize = 10;
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

   GtkWidget *mainWindow = NULL;

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
   GtkWidget *verticalBox = NULL;
   verticalBox = gtk_vbox_new (FALSE, 6);
   gtk_container_add (GTK_CONTAINER (mainWindow), verticalBox);
   // }Create vertical box

   // Create question label{
   GtkWidget *questionLabel = NULL;
   questionLabel = gtk_label_new (g_locale_to_utf8("Вас приветствует программа тестирования знаний языка программирования Pascal! Нажмите Начать тест, когда преподаватель скажет это сделать", -1, NULL, NULL, NULL));
   //gtk_widget_modify_font(questionLabel, pango_font_description_from_string(mainFont));
   gtk_box_pack_start (GTK_BOX (verticalBox), questionLabel, TRUE, FALSE, 10);
   gtk_label_set_line_wrap(GTK_LABEL(questionLabel),TRUE);
   // }Create question label

   // Create three answer variants{
   GtkWidget *answers[3];
   answers[0] = gtk_radio_button_new_with_label(NULL,questionsVector[currentQuestionNumber].getAnswer(0));
   for(int i = 1; i < 3; i++)
   {
      answers[i] = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(answers[0]),questionsVector[currentQuestionNumber].getAnswer(i));
   }
   for(int i = 0; i < 3; i++)
   {
      gtk_box_pack_start (GTK_BOX (verticalBox), answers[i], FALSE, FALSE, 10);
   }
   // }Create three answer variants

   // Create horizontal box{
   GtkWidget *horizontalBox;
   horizontalBox = gtk_hbox_new (FALSE, 6);
   gtk_box_pack_start (GTK_BOX (verticalBox), horizontalBox, FALSE, FALSE, 10);
   // }Create horizontal box

   // Create buttons ">>" and "<<"{
   GtkWidget *buttonBack;
   buttonBack = gtk_button_new_with_label("<<");
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonBack, TRUE, TRUE, 10);
   GtkWidget *buttonForward;
   buttonForward = gtk_button_new_with_label(">>");
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonForward, TRUE, TRUE, 10);
   // }Create buttons ">>" and "<<"

   // Create "Start" button{
   GtkWidget *buttonStart;
   buttonStart = gtk_button_new_with_label(g_locale_to_utf8("Начать тест", -1, NULL, NULL, NULL));
   gtk_box_pack_start (GTK_BOX (horizontalBox), buttonStart, TRUE, TRUE, 10);
   vector<GtkWidget*> labelAndRadios; //startTest function needs either questionLabel or array of radioButtons
   labelAndRadios.push_back(questionLabel);
   labelAndRadios.push_back(verticalBox);
   //g_signal_connect (buttonStart, "button_press_event", G_CALLBACK(showVariants), verticalBox);
   g_signal_connect (buttonStart, "button_press_event", G_CALLBACK(startTest), labelAndRadios);
   // }Create "Start" button

   // Enter the main loop
   gtk_widget_show_all(mainWindow);
   for(int i = 0; i < 3; i++)
   {
      gtk_widget_hide(answers[i]);
   }
   gtk_widget_hide(buttonBack);
   gtk_widget_hide(buttonForward);
   gtk_main ();
   return 0;
}

// static gboolean changeQuestion(GtkWidget *startButton, 
//                                GdkEventButton *event,
//                                gpointer data )
// {
//    gtk_label_set_text(GTK_LABEL(data), g_locale_to_utf8(questionsVector[currentQuestionNumber].getQuestion(), -1, NULL, NULL, NULL));
//    return true;
// }

static gboolean startTest(GtkWidget *startButton, 
                             GdkEventButton *event,
                             gpointer data )
{
   gtk_widget_show_all(((vector<GtkWidget*>)data)[1]);
   gtk_widget_hide(startButton);
   gtk_label_set_text(GTK_LABEL(((vector<GtkWidget*>)data)[0]), g_locale_to_utf8(questionsVector[currentQuestionNumber].getQuestion(), -1, NULL, NULL, NULL));
   return true;
}

void addQuestion(vector<question> &v, gchar* q, gchar* ans0, gchar* ans1, gchar* ans2, gint c)
{
   question * currentQuestion = new question(q, ans0, ans1, ans2, c);
   v.push_back(*currentQuestion);
}