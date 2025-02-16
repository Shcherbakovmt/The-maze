#pragma once
#include "Class_Maze.hpp"
namespace MazeWinForms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


	private: System::Windows::Forms::Button^ BtnLeft;
	private: System::Windows::Forms::Label^ Console;





	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->BtnLeft = (gcnew System::Windows::Forms::Button());
			this->Console = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// BtnLeft
			// 
			this->BtnLeft->Location = System::Drawing::Point(50, 237);
			this->BtnLeft->Name = L"BtnLeft";
			this->BtnLeft->Size = System::Drawing::Size(75, 23);
			this->BtnLeft->TabIndex = 2;
			this->BtnLeft->Text = L"Left";
			this->BtnLeft->UseVisualStyleBackColor = true;
			this->BtnLeft->Click += gcnew System::EventHandler(this, &MainForm::BtnLeft_Click);
			// 
			// Console
			// 
			this->Console->AutoSize = true;
			this->Console->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->Console->Location = System::Drawing::Point(81, 48);
			this->Console->Name = L"Console";
			this->Console->Size = System::Drawing::Size(52, 13);
			this->Console->TabIndex = 3;
			this->Console->Text = L"Welcome";
			this->Console->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->ClientSize = System::Drawing::Size(478, 313);
			this->Controls->Add(this->Console);
			this->Controls->Add(this->BtnLeft);
			this->Cursor = System::Windows::Forms::Cursors::PanWest;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"Maze";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void BtnLeft_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Console->Text = "Some new";
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
