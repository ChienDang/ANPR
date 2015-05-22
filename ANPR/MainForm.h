#pragma once
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\ml.h>
#include <opencv\cxcore.h>
#include <stdio.h>

#include "PlateFinder.h"
namespace ANPR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{

	private:
		IplImage *src;
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
			if (src){
				src = NULL;
			}
		}
	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::Button^  btnRecognise;
	private: System::Windows::Forms::PictureBox^  pbPlate;
	protected:



	private: System::Windows::Forms::PictureBox^  pbSrc;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->btnRecognise = (gcnew System::Windows::Forms::Button());
			this->pbPlate = (gcnew System::Windows::Forms::PictureBox());
			this->pbSrc = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPlate))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSrc))->BeginInit();
			this->SuspendLayout();
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(196, 320);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(75, 23);
			this->btnBrowse->TabIndex = 0;
			this->btnBrowse->Text = L"Browse...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// btnRecognise
			// 
			this->btnRecognise->Location = System::Drawing::Point(320, 320);
			this->btnRecognise->Name = L"btnRecognise";
			this->btnRecognise->Size = System::Drawing::Size(75, 23);
			this->btnRecognise->TabIndex = 1;
			this->btnRecognise->Text = L"Recognise";
			this->btnRecognise->UseVisualStyleBackColor = true;
			this->btnRecognise->Click += gcnew System::EventHandler(this, &MainForm::btnRecognise_Click);
			// 
			// pbPlate
			// 
			this->pbPlate->Location = System::Drawing::Point(12, 307);
			this->pbPlate->Name = L"pbPlate";
			this->pbPlate->Size = System::Drawing::Size(134, 50);
			this->pbPlate->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbPlate->TabIndex = 2;
			this->pbPlate->TabStop = false;
			// 
			// pbSrc
			// 
			this->pbSrc->Location = System::Drawing::Point(12, 12);
			this->pbSrc->Name = L"pbSrc";
			this->pbSrc->Size = System::Drawing::Size(383, 273);
			this->pbSrc->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbSrc->TabIndex = 3;
			this->pbSrc->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(431, 380);
			this->Controls->Add(this->pbSrc);
			this->Controls->Add(this->pbPlate);
			this->Controls->Add(this->btnRecognise);
			this->Controls->Add(this->btnBrowse);
			this->Name = L"MainForm";
			this->Text = L"ANPR";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPlate))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbSrc))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

		private: char* ConvertStringToChar(System::String ^str)
		{
					 char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
					 return str2;
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ dlg = gcnew OpenFileDialog;
				 dlg->Filter = "Image (*.bmp; *.jpg; *.png; *.jpeg) | *.bmp; *.jpg; *.png; *.jpeg| All files (*.*|*.*||";

				 if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
				 {
					 return;
				 }

				 src = cvLoadImage(ConvertStringToChar(dlg->FileName));

				 Bitmap ^bmp = gcnew Bitmap(dlg->FileName);
				 pbSrc->Image = bmp;
				 pbPlate->Refresh();
	}
	private: System::Void btnRecognise_Click(System::Object^  sender, System::EventArgs^  e) {
				 PlateFinder pf;

				 if (!src)
				 {
					 MessageBox::Show("No Image Loaded", "Error", MessageBoxButtons::OK);
					 return;
				 }

				 IplImage *resizeImg = cvCreateImage(cvSize(800, 600), src->depth, src->nChannels); // resize ảnh
				 cvResize(src, resizeImg);

				 //Convert sang ảnh xám
				 /*IplImage *grayImg = cvCreateImage(cvGetSize(resizeImg), IPL_DEPTH_8U, 1);
				 cvCvtColor(resizeImg, grayImg, CV_RGB2GRAY);
				 cvNormalize(grayImg, grayImg, 0, 255, CV_MINMAX);

				 pf.ImageRestoration(grayImg);*/
				 pf.FindPlate(resizeImg);
	}
};
}
