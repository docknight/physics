#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "..\SimPool3D\Graphics.h"

namespace Simulator
{
	/// <summary> 
	/// Summary for RenderWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class RenderWindow : public System::Windows::Forms::Form
	{
	public:
		bool bInitialized;
		RenderWindow(void)
		{
			bInitialized = false;
			InitializeComponent();
		}

		void StartTimer()
		{
			timer1->Start();
		}

		void StopTimer()
		{
			timer1->Stop();
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Timer *  timer1;
	private: System::ComponentModel::IContainer *  components;

			 bool bSimulation;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = new System::ComponentModel::Container();
			this->timer1 = new System::Windows::Forms::Timer(this->components);
			// 
			// timer1
			// 
			this->timer1->Interval = 50;
			this->timer1->Tick += new System::EventHandler(this, timer1_Tick);
			// 
			// RenderWindow
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->BackColor = System::Drawing::SystemColors::WindowFrame;
			this->ClientSize = System::Drawing::Size(376, 325);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = S"RenderWindow";
			this->Text = S"RenderWindow";
			this->Load += new System::EventHandler(this, RenderWindow_Load);

		}		

	private: System::Void RenderWindow_Load(System::Object *  sender, System::EventArgs *  e)
			 {
				 this->StartPosition = FormStartPosition::Manual;
				 Location = System::Drawing::Point(200, 1);
				 InitGraphics();
				 HWND handle = (HWND)(this->Handle.ToInt32()); // TODO: .ToInt64() if running on 64-bit HW
				 if (InitD3D(handle, false) != S_OK)
				 {
					 MessageBox(handle, "Can not initialize Direct3D", "Error", MB_OK);
					 bInitialized = true;
					 return;
				 }
				 bInitialized = true;
			 }

	private: System::Void timer1_Tick(System::Object *  sender, System::EventArgs *  e)
			 {
				 //Render();
			 }



	};
}