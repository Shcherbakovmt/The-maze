#include "MainForm.h"
#include "Class_Maze.hpp"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(cli::array <String^>^ args)
{
	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	MazeWinForms::MainForm form;
	Application::Run(% form);
}