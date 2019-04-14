#pragma once

#include "RenderWindow.h"
#include "..\SimPool3D\Physics.h"
#include "MMathObjects.h"
#include "DrawingHelper.h"

namespace Simulator
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class SimulatorWindow : public System::Windows::Forms::Form
	{	
	private:
		System::Windows::Forms::Form *  renderWindow;

		CPhysics * cPhysics;


	private: System::Windows::Forms::ComboBox *  comboObjects;
	private: System::Windows::Forms::Panel *  panelLeft;
	private: System::Windows::Forms::Panel *  panelTop;
			 ArrayList		*objects; // list of objects defined through user interface
			 ArrayList		*forces;  // list of forces defined through user interface
			 __value enum eSimulationMode { modePlay, modePause, modeStop } SimulationMode;
	private: System::Windows::Forms::Label *  label2;
	private: System::Windows::Forms::TextBox *  textNewForce;
	private: System::Windows::Forms::Button *  buttonAddNewForce;
	private: System::Windows::Forms::ComboBox *  comboForces;
	private: System::Windows::Forms::ComboBox *  comboNewForce;
	private: System::Windows::Forms::CheckBox *  checkStart;
	private: System::Windows::Forms::CheckBox *  checkPause;
	private: System::Windows::Forms::CheckBox *  checkStop;
	private: System::Windows::Forms::ToolTip *  toolTip1;
	private: System::Windows::Forms::Timer *  timerSimulation;

	private: CMathObjectRect* boundingbox;	// bounding box, describing currently selected object
			 CGrObject* grboundingbox;		// actual bounding box, drawn around currently selected object

	private: System::Windows::Forms::PropertyGrid *  propertyGrid2;

	public:
		SimulatorWindow(void)
		{
			InitializeComponent();
			renderWindow = new RenderWindow();
			renderWindow->MdiParent = this;
			renderWindow->ClientSize = System::Drawing::Size(this->ClientSize.Width-panelLeft->Width-6, this->ClientSize.Height-panelTop->Height-6);
			renderWindow->Show();
			System::Drawing::Image * image = Image::FromFile("Graphics\\litening.ico");
			this->imageList1->Images->Add(image);
			image = Image::FromFile("Graphics\\TRFFC10A.ICO");
			this->imageList1->Images->Add(image);
			image = Image::FromFile("Graphics\\TRFFC10B.ICO");
			this->imageList1->Images->Add(image);
			image = Image::FromFile("Graphics\\TRFFC10C.ICO");
			this->imageList1->Images->Add(image);
			this->buttonAddNewObject->ImageList = this->imageList1;
			this->buttonAddNewObject->ImageIndex = 0;
			this->buttonAddNewForce->ImageList = this->imageList1;
			this->buttonAddNewForce->ImageIndex = 0;
			this->checkStart->ImageList = this->imageList1;
			this->checkStart->ImageIndex = 1;
			this->checkPause->ImageList = this->imageList1;
			this->checkPause->ImageIndex = 2;
			this->checkStop->ImageList = this->imageList1;
			this->checkStop->ImageIndex = 3;
			this->checkStop->Checked = true;

			// initialize object list
			objects = new ArrayList();

			// initialize force list
			forces = new ArrayList();

			// stop simulation
			SimulationMode = modeStop;

			// initialize physics class instance
			cPhysics = new CPhysics();

			// initialize bounding box
			LPD3DXMESH pmesh = NULL;
			boundingbox = new CMathObjectRect();
			D3DXCreateBox(g_pD3DDevice, 1,1,1, &pmesh, NULL);
			grboundingbox = new CGrObject(pmesh, NULL);
			grboundingbox->pEntity = boundingbox;

			timerSimulation->Enabled = true;
			timerSimulation->Start();
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


	private: System::Windows::Forms::PropertyGrid *  propertyGrid1;

	private: System::Windows::Forms::MainMenu *  mainMenu1;
	private: System::Windows::Forms::MenuItem *  menuItem1;
	private: System::Windows::Forms::MenuItem *  menuItem2;
	private: System::Windows::Forms::MenuItem *  menuItem3;

	private: System::Windows::Forms::Label *  label1;


	private: System::Windows::Forms::ImageList *  imageList1;
	private: System::Windows::Forms::ComboBox *  comboNewObject;
	private: System::Windows::Forms::TextBox *  textNewObject;
	private: System::Windows::Forms::Button *  buttonAddNewObject;






	private: System::ComponentModel::IContainer *  components;

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
			this->components = (gcnew System::ComponentModel::Container());
			this->panelLeft = (gcnew System::Windows::Forms::Panel());
			this->propertyGrid2 = (gcnew System::Windows::Forms::PropertyGrid());
			this->comboForces = (gcnew System::Windows::Forms::ComboBox());
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
			this->comboObjects = (gcnew System::Windows::Forms::ComboBox());
			this->panelTop = (gcnew System::Windows::Forms::Panel());
			this->checkStop = (gcnew System::Windows::Forms::CheckBox());
			this->checkPause = (gcnew System::Windows::Forms::CheckBox());
			this->checkStart = (gcnew System::Windows::Forms::CheckBox());
			this->comboNewForce = (gcnew System::Windows::Forms::ComboBox());
			this->buttonAddNewForce = (gcnew System::Windows::Forms::Button());
			this->textNewForce = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboNewObject = (gcnew System::Windows::Forms::ComboBox());
			this->textNewObject = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonAddNewObject = (gcnew System::Windows::Forms::Button());
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->mainMenu1 = (gcnew System::Windows::Forms::MainMenu(this->components));
			this->menuItem1 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem3 = (gcnew System::Windows::Forms::MenuItem());
			this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->timerSimulation = (gcnew System::Windows::Forms::Timer(this->components));
			this->panelLeft->SuspendLayout();
			this->panelTop->SuspendLayout();
			this->SuspendLayout();
			// 
			// panelLeft
			// 
			this->panelLeft->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->panelLeft->BackColor = System::Drawing::Color::AliceBlue;
			this->panelLeft->Controls->Add(this->propertyGrid2);
			this->panelLeft->Controls->Add(this->comboForces);
			this->panelLeft->Controls->Add(this->propertyGrid1);
			this->panelLeft->Controls->Add(this->comboObjects);
			this->panelLeft->Location = System::Drawing::Point(0, 0);
			this->panelLeft->Name = L"panelLeft";
			this->panelLeft->Size = System::Drawing::Size(240, 569);
			this->panelLeft->TabIndex = 0;
			// 
			// propertyGrid2
			// 
			this->propertyGrid2->CommandsBackColor = System::Drawing::Color::AliceBlue;
			this->propertyGrid2->LineColor = System::Drawing::SystemColors::ScrollBar;
			this->propertyGrid2->Location = System::Drawing::Point(10, 443);
			this->propertyGrid2->Name = L"propertyGrid2";
			this->propertyGrid2->Size = System::Drawing::Size(220, 194);
			this->propertyGrid2->TabIndex = 3;
			this->propertyGrid2->ToolbarVisible = false;
			this->propertyGrid2->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &SimulatorWindow::propertyGrid2_PropertyValueChanged);
			// 
			// comboForces
			// 
			this->comboForces->Location = System::Drawing::Point(10, 415);
			this->comboForces->Name = L"comboForces";
			this->comboForces->Size = System::Drawing::Size(220, 24);
			this->comboForces->TabIndex = 2;
			this->comboForces->SelectedIndexChanged += gcnew System::EventHandler(this, &SimulatorWindow::comboForces_SelectedIndexChanged);
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->CommandsBackColor = System::Drawing::Color::AliceBlue;
			this->propertyGrid1->LineColor = System::Drawing::SystemColors::ScrollBar;
			this->propertyGrid1->Location = System::Drawing::Point(10, 102);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(220, 276);
			this->propertyGrid1->TabIndex = 1;
			this->propertyGrid1->ToolbarVisible = false;
			this->propertyGrid1->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &SimulatorWindow::propertyGrid1_PropertyValueChanged);
			// 
			// comboObjects
			// 
			this->comboObjects->Location = System::Drawing::Point(10, 74);
			this->comboObjects->Name = L"comboObjects";
			this->comboObjects->Size = System::Drawing::Size(220, 24);
			this->comboObjects->TabIndex = 0;
			this->comboObjects->SelectedIndexChanged += gcnew System::EventHandler(this, &SimulatorWindow::comboObjects_SelectedIndexChanged);
			// 
			// panelTop
			// 
			this->panelTop->BackColor = System::Drawing::Color::Silver;
			this->panelTop->Controls->Add(this->checkStop);
			this->panelTop->Controls->Add(this->checkPause);
			this->panelTop->Controls->Add(this->checkStart);
			this->panelTop->Controls->Add(this->comboNewForce);
			this->panelTop->Controls->Add(this->buttonAddNewForce);
			this->panelTop->Controls->Add(this->textNewForce);
			this->panelTop->Controls->Add(this->label2);
			this->panelTop->Controls->Add(this->comboNewObject);
			this->panelTop->Controls->Add(this->textNewObject);
			this->panelTop->Controls->Add(this->label1);
			this->panelTop->Controls->Add(this->buttonAddNewObject);
			this->panelTop->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelTop->Location = System::Drawing::Point(0, 0);
			this->panelTop->Name = L"panelTop";
			this->panelTop->Size = System::Drawing::Size(744, 55);
			this->panelTop->TabIndex = 5;
			// 
			// checkStop
			// 
			this->checkStop->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkStop->Location = System::Drawing::Point(826, 9);
			this->checkStop->Name = L"checkStop";
			this->checkStop->Size = System::Drawing::Size(48, 37);
			this->checkStop->TabIndex = 10;
			this->toolTip1->SetToolTip(this->checkStop, L"Stop the simulation, reseting all velocities");
			this->checkStop->Click += gcnew System::EventHandler(this, &SimulatorWindow::checkStop_Click);
			// 
			// checkPause
			// 
			this->checkPause->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkPause->Location = System::Drawing::Point(774, 9);
			this->checkPause->Name = L"checkPause";
			this->checkPause->Size = System::Drawing::Size(48, 37);
			this->checkPause->TabIndex = 9;
			this->toolTip1->SetToolTip(this->checkPause, L"Pause the simulation");
			this->checkPause->Click += gcnew System::EventHandler(this, &SimulatorWindow::checkPause_Click);
			// 
			// checkStart
			// 
			this->checkStart->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkStart->Location = System::Drawing::Point(722, 9);
			this->checkStart->Name = L"checkStart";
			this->checkStart->Size = System::Drawing::Size(48, 37);
			this->checkStart->TabIndex = 8;
			this->toolTip1->SetToolTip(this->checkStart, L"Start real-time simulation");
			this->checkStart->Click += gcnew System::EventHandler(this, &SimulatorWindow::checkStart_Click);
			// 
			// comboNewForce
			// 
			this->comboNewForce->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboNewForce->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"GLOBAL", L"OBJECT LOCAL" });
			this->comboNewForce->Location = System::Drawing::Point(456, 28);
			this->comboNewForce->Name = L"comboNewForce";
			this->comboNewForce->Size = System::Drawing::Size(120, 24);
			this->comboNewForce->TabIndex = 7;
			// 
			// buttonAddNewForce
			// 
			this->buttonAddNewForce->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->buttonAddNewForce->Location = System::Drawing::Point(595, 3);
			this->buttonAddNewForce->Name = L"buttonAddNewForce";
			this->buttonAddNewForce->Size = System::Drawing::Size(77, 34);
			this->buttonAddNewForce->TabIndex = 6;
			this->buttonAddNewForce->Text = L"Add";
			this->buttonAddNewForce->Click += gcnew System::EventHandler(this, &SimulatorWindow::buttonAddNewForce_Click);
			// 
			// textNewForce
			// 
			this->textNewForce->Location = System::Drawing::Point(456, 3);
			this->textNewForce->Name = L"textNewForce";
			this->textNewForce->Size = System::Drawing::Size(120, 22);
			this->textNewForce->TabIndex = 5;
			this->textNewForce->Text = L"force01";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(374, 10);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 18);
			this->label2->TabIndex = 4;
			this->label2->Text = L"New force:";
			// 
			// comboNewObject
			// 
			this->comboNewObject->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboNewObject->Items->AddRange(gcnew cli::array< System::Object^  >(8) {
				L"SPHERE", L"BOX", L"EDGE", L"LINE", L"CIRCLE",
					L"RECTANGLE", L"COMPOSITE PLANE", L"ARBITRARY"
			});
			this->comboNewObject->Location = System::Drawing::Point(96, 28);
			this->comboNewObject->Name = L"comboNewObject";
			this->comboNewObject->Size = System::Drawing::Size(120, 24);
			this->comboNewObject->TabIndex = 3;
			// 
			// textNewObject
			// 
			this->textNewObject->Location = System::Drawing::Point(96, 3);
			this->textNewObject->Name = L"textNewObject";
			this->textNewObject->Size = System::Drawing::Size(120, 22);
			this->textNewObject->TabIndex = 2;
			this->textNewObject->Text = L"object01";
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(10, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(86, 18);
			this->label1->TabIndex = 1;
			this->label1->Text = L"New object:";
			// 
			// buttonAddNewObject
			// 
			this->buttonAddNewObject->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->buttonAddNewObject->ImageIndex = 0;
			this->buttonAddNewObject->Location = System::Drawing::Point(230, 3);
			this->buttonAddNewObject->Name = L"buttonAddNewObject";
			this->buttonAddNewObject->Size = System::Drawing::Size(77, 34);
			this->buttonAddNewObject->TabIndex = 0;
			this->buttonAddNewObject->Text = L"Add";
			this->buttonAddNewObject->Click += gcnew System::EventHandler(this, &SimulatorWindow::buttonAddNewObject_Click);
			// 
			// imageList1
			// 
			this->imageList1->ColorDepth = System::Windows::Forms::ColorDepth::Depth8Bit;
			this->imageList1->ImageSize = System::Drawing::Size(16, 16);
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			// 
			// mainMenu1
			// 
			this->mainMenu1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(1) { this->menuItem1 });
			// 
			// menuItem1
			// 
			this->menuItem1->Index = 0;
			this->menuItem1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) { this->menuItem3, this->menuItem2 });
			this->menuItem1->Text = L"File";
			// 
			// menuItem3
			// 
			this->menuItem3->Index = 0;
			this->menuItem3->Text = L"Open...";
			// 
			// menuItem2
			// 
			this->menuItem2->Index = 1;
			this->menuItem2->Text = L"Exit";
			this->menuItem2->Click += gcnew System::EventHandler(this, &SimulatorWindow::menuItem2_Click);
			// 
			// timerSimulation
			// 
			this->timerSimulation->Tick += gcnew System::EventHandler(this, &SimulatorWindow::timerSimulation_Tick);
			// 
			// SimulatorWindow
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(6, 15);
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(744, 569);
			this->Controls->Add(this->panelTop);
			this->Controls->Add(this->panelLeft);
			this->IsMdiContainer = true;
			this->Menu = this->mainMenu1;
			this->Name = L"SimulatorWindow";
			this->Text = L"Physics Simulator";
			this->Resize += gcnew System::EventHandler(this, &SimulatorWindow::SimulatorWindow_Resize);
			this->panelLeft->ResumeLayout(false);
			this->panelTop->ResumeLayout(false);
			this->panelTop->PerformLayout();
			this->ResumeLayout(false);

		}

	private: System::Void menuItem2_Click(System::Object *  sender, System::EventArgs *  e)
			 {
				 Application::Exit();
			 }

private: System::Void buttonAddNewObject_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 int numvertices, vertexsize;
			 DWORD fvf;
			 D3DPRIMITIVETYPE primitivetype;
			 BYTE* buffer;
			 int numprimitives;

			 // adding new object to the list
			 if (comboNewObject->SelectedIndex < 0)
			 {
				 MessageBox(NULL, "Please select an object type.", "Error", MB_OK | MB_ICONEXCLAMATION);
				 comboNewObject->Focus();
				 return;
			 }
			 UIObject *object = new UIObject();
			 object->iObjType = comboNewObject->SelectedIndex+1;
			 object->Name = textNewObject->Text;
			 // check if an object with the same name already exists
			 for (int i = 0; i < objects->Count; i++)
			 {
				 if (((UIObject*)(objects->Item[i]))->Name->Equals(object->Name))
				 {
					 // we can't allow two objects with the same name
					 MessageBox(NULL, "Object with that name already exists.", "Duplicate name", MB_OK | MB_ICONEXCLAMATION);
					 return;
				 }
			 }
			 MMathObject *mmathobj;
			 LPD3DXMESH pmesh = NULL; // DirectX mesh data for the new object
			 CGrObject *grobject = NULL;
			 char tfname[13] = "ball_t10.bmp";
			 switch (object->iObjType)
			 {
				case OBJTYPE_SPHERE: // adding a new sphere
					object->pMathObject = new CMathObjectSphere();
					mmathobj = new MMathObjectSphere(object->Name); // create and initialize a managed object instance

					pmesh = CreateSphere(((MMathObjectSphere*)(mmathobj))->Radius); // create mesh

					// align CMathObject to MMathObject
					((MMathObjectSphere*)(mmathobj))->SyncMathObject(object->pMathObject, true);
					
					// create a CGrObject instance using pre-initialized mesh and a texture filename
					grobject = new CGrObject(pmesh, tfname);
					break;
				case OBJTYPE_PLANE: // adding a new composite plane
					object->pMathObject = new CMathObjectPlane();
					mmathobj = new MMathObjectPlane(object->Name); // create and initialize a managed object instance
					mmathobj->Color = System::Drawing::Color::White;

					// align CMathObject to MMathObject
					((MMathObjectPlane*)(mmathobj))->SyncMathObject(object->pMathObject, true);

					// create vertices for plane
					buffer = CreateRectangle((CMathObjectRect*)(object->pMathObject), &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives, mmathobj->Color);

					// create a CGrObject instance using pre-initialized vertex data
					grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
					grobject->SetCull(D3DCULL_NONE);
					break;
				case OBJTYPE_LINE: // adding a new line
					object->pMathObject = new CMathObjectLine();
					mmathobj = new MMathObjectLine(object->Name); // create and initialize a managed object instance

					// align CMathObject to MMathObject
					((MMathObjectLine*)(mmathobj))->SyncMathObject(object->pMathObject, true);

					// create vertices for plane
					buffer = CreateLine((CMathObjectLine*)(object->pMathObject), &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives);

					// create a CGrObject instance using pre-initialized vertex data
					grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
					grobject->SetCull(D3DCULL_NONE);
					break;
				case OBJTYPE_RECT: // adding a new rectangle
					object->pMathObject = new CMathObjectRect();
					mmathobj = new MMathObjectRect(object->Name); // create and initialize a managed object instance

					pmesh = CreateSphere(1); // create mesh

					// align CMathObject to MMathObject
					((MMathObjectRect*)(mmathobj))->SyncMathObject(object->pMathObject, true);

					// create a CGrObject instance using pre-initialized mesh and a texture filename
					grobject = new CGrObject(pmesh, tfname);
					break;
				case OBJTYPE_CIRCLE: // adding a new circle
					object->pMathObject = new CMathObjectCircle();
					mmathobj = new MMathObjectCircle(object->Name); // create and initialize a managed object instance

					// align CMathObject to MMathObject
					((MMathObjectCircle*)(mmathobj))->SyncMathObject(object->pMathObject, true);

					// create vertices for circle
					buffer = CreateCircle((CMathObjectCircle*)(object->pMathObject), &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives);

					// create a CGrObject instance using pre-initialized vertex data
					grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
					grobject->SetCull(D3DCULL_NONE);
					break;
				default:
					MessageBox(NULL, "Object type not supported in this version.", "Not supported", MB_OK | MB_ICONEXCLAMATION);
					return;
					break;
			 }
			 mmathobj->Type = object->iObjType;
			 object->pMMathObject = mmathobj;
			 grobject->pEntity = object->pMathObject; // link from CGrObject to CMathObject instance
			 AddObject(grobject);  // add grobject to grobject list used by Graphics module
			 object->pGrObject = grobject; // link from UI object to CGrObject instance

			 objects->Add(object); // finally, add initialized UI object to list of UI objects

			 cPhysics->AddEntity(object->pMathObject); // add math object to physics simulation
			 
			 // refresh combo box
			 comboObjects->DataSource = NULL;
			 comboObjects->DataSource = objects;
			 comboObjects->DisplayMember = "Name";
			 comboObjects->ValueMember = "Name";
			 comboObjects->SelectedIndex = objects->Count-1; // select newly added object
		 }



private: System::Void comboObjects_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e)
		 {
			 int index = comboObjects->SelectedIndex;
			 if (index < 0) 
			 {
				 propertyGrid1->SelectedObject = NULL;
				 return;
			 }
			 propertyGrid1->SelectedObject = ((UIObject*)(objects->Item[index]))->pMMathObject;
			 SetBoundingBox(((UIObject*)(objects->Item[index]))->pMathObject, ((UIObject*)(objects->Item[index]))->pGrObject);
		 }

private:
		// using a world object as a parameter, function adds a new bounding box around the
		// object and removes previous bounding box, if exists
		void SetBoundingBox(CMathObject* mathobj, CGrObject* grobject)
		{
			// re-initialize bounding box
			LPD3DXMESH pmesh = NULL;
			if (boundingbox == NULL)
				boundingbox = new CMathObjectRect();
			if (grboundingbox != NULL)
				ReleaseObject(grboundingbox);
			grboundingbox = NULL;
			D3DXVECTOR3 lowerleft, upperright;
			if ( grobject->GetBoundingBox(&lowerleft, &upperright) )
			{
				// create a box using the calculated bounding box dimensions
				D3DXCreateBox(g_pD3DDevice, fabs(D3DXVec3Dot( &(upperright-lowerleft), &(D3DXVECTOR3(1.05f,0,0)))),
					                        fabs(D3DXVec3Dot( &(upperright-lowerleft), &(D3DXVECTOR3(0,1.05f,0)))),
											fabs(D3DXVec3Dot( &(upperright-lowerleft), &(D3DXVECTOR3(0,0,1.05f)))),
											&pmesh, NULL);
				// align the box world coordinates with the object that it is bounded to
				boundingbox->SetPosition( &(mathobj->GetPosition()) );
				boundingbox->SetRotation( &(mathobj->GetRotation()) );

				// add the box into the list
				grboundingbox = new CGrObject(pmesh, NULL);
				grboundingbox->pEntity = boundingbox;
				grboundingbox->SetFillMode(D3DFILL_WIREFRAME);
				grboundingbox->SetCull(D3DCULL_NONE);
				AddObject(grboundingbox);
			}
		}

private: System::Void SimulatorWindow_Resize(System::Object *  sender, System::EventArgs *  e)
		{
			renderWindow->ClientSize = System::Drawing::Size(this->ClientSize.Width-panelLeft->Width-6, this->ClientSize.Height-panelTop->Height-6);
		}

private: System::Void propertyGrid1_PropertyValueChanged(System::Object *  s, System::Windows::Forms::PropertyValueChangedEventArgs *  e)
		 {
			 int numvertices, vertexsize;
			 DWORD fvf;
			 D3DPRIMITIVETYPE primitivetype;
			 BYTE* buffer;
			 int numprimitives;
			 CMathObject* pmathobj;

			 if (SimulationMode = modeStop)
			 {
				 // if simulation is not running, immediately synchronize MMathObject with CMathObject for the
				 // currently edited entity
				 MMathObject* mmathobj = (MMathObject*)(propertyGrid1->SelectedObject);
				 // to find the matching CMathObject, seek into UIObject list to find the
				 // currently edited MMathObject
				 bool bFound = false;
				 for (int i=0; i<objects->Count; i++)
				 {
					 if ( ((UIObject*)(objects->Item[i]))->pMMathObject == mmathobj)
					 {
						 bFound = true;
						 break;
					 }
				 }
				 if (bFound == false)
				 {
					 MessageBox(NULL, "Lost connection to UIObject", "Internal Error", MB_OK | MB_ICONEXCLAMATION);
					 return;
				 }

				 // stop rendering while changing objects
				 //((RenderWindow*)renderWindow)->StopTimer();
				 timerSimulation->Stop();

				 bool bNeedReload = mmathobj->SyncMathObject(((UIObject*)(objects->Item[i]))->pMathObject, true);
				 // objects synchronized, but if geometry properties have been changed, object
				 // must be reloaded
				 if (bNeedReload)
				 {
					ReleaseObject(((UIObject*)(objects->Item[i]))->pGrObject); // release grobject and remove it from the grobject list
					LPD3DXMESH pmesh; // DirectX mesh data for the new object
					char tfname[13] = "ball_t10.bmp";
					CGrObject *grobject = NULL;
					CMathObject* mathobjdiv;
					switch (((UIObject*)(objects->Item[i]))->iObjType)
					{
						case OBJTYPE_SPHERE:
							pmesh = CreateSphere(((MMathObjectSphere*)(mmathobj))->Radius);
							if ( pmesh == NULL )
							{
								MessageBox(NULL, "Can't create mesh", "Error", MB_OK | MB_ICONEXCLAMATION);
								return;
							}
							grobject = new CGrObject(pmesh, tfname);
							AddObject(grobject); // add new grobject back to the list
							break;
						case OBJTYPE_PLANE:
							// create vertices for plane
							pmathobj = ((UIObject*)(objects->Item[i]))->pMathObject;
							buffer = CreateRectangle((CMathObjectRect*)pmathobj, &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives, ((UIObject*)(objects->Item[i]))->pMMathObject->Color);

							// create a CGrObject instance using pre-initialized vertex data
							grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
							grobject->SetCull(D3DCULL_NONE);
							AddObject(grobject); // add new grobject back to the list
							// also, re-create all plane elements
							mathobjdiv = NULL;
							for (int j = 0; j < ((MMathObjectPlane*)(((UIObject*)(objects->Item[i]))->pMMathObject))->Elements->Count; j++)
							{
								MMathObject* obj = ((MMathObjectPlane*)(((UIObject*)(objects->Item[i]))->pMMathObject))->Elements->Item[j];
								switch (obj->Type)
								{
									case OBJTYPE_CIRCLE:
										{
											MMathElementCircle* circle = (MMathElementCircle*)obj;
											ReleaseObject(circle->pGrObject);
											// get the matching CMathObject element
											// TODO: it is assumed that CMathObject plane elements are in the same order as
											// MMathObjectPlane elements; there is no other way to obtain the correct element,
											// since MMathObject does not store a link to CMathObject
											mathobjdiv = ((CMathObjectPlane*)(pmathobj))->GetNextElement(mathobjdiv);
											BYTE* buffercircle = CreateCircle((CMathObjectCircle*)mathobjdiv, &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives);
											// create a CGrObject instance using pre-initialized vertex data
											CGrObject* grobjectdiv = new CGrObject(buffercircle, numvertices, vertexsize, fvf, primitivetype, numprimitives);
											circle->pGrObject = grobjectdiv;
											grobjectdiv->SetCull(D3DCULL_NONE);
											grobjectdiv->pEntity = mathobjdiv; // link from CGrObject to CMathObject instance
											AddObject(grobjectdiv); // add new grobject back to the list
										}
										break;
									case OBJTYPE_RECT:
										// TODO: write me, see circle above!
										{
										}
										break;
								}
							}
							break;
						case OBJTYPE_CIRCLE:
							// create vertices for circle
							pmathobj = ((UIObject*)(objects->Item[i]))->pMathObject;
							buffer = CreateCircle((CMathObjectCircle*)pmathobj, &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives);

							// create a CGrObject instance using pre-initialized vertex data
							grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
							grobject->SetCull(D3DCULL_NONE);
							AddObject(grobject); // add new grobject back to the list
							break;
						case OBJTYPE_LINE:
							// create vertices for circle
							pmathobj = ((UIObject*)(objects->Item[i]))->pMathObject;
							buffer = CreateLine((CMathObjectLine*)pmathobj, &numvertices, &vertexsize, &fvf, &primitivetype, &numprimitives);

							// create a CGrObject instance using pre-initialized vertex data
							grobject = new CGrObject(buffer, numvertices, vertexsize, fvf, primitivetype, numprimitives);
							grobject->SetCull(D3DCULL_NONE);
							AddObject(grobject); // add new grobject back to the list
							break;
					}
					grobject->pEntity = ((UIObject*)(objects->Item[i]))->pMathObject; // link from CGrObject to CMathObject instance
					((UIObject*)(objects->Item[i]))->pGrObject = grobject; // re-link from UI object to CGrObject instance
				 }

				 SetBoundingBox(((UIObject*)(objects->Item[i]))->pMathObject, ((UIObject*)(objects->Item[i]))->pGrObject);
				 timerSimulation->Start();
			 }
		 }
		
		 LPD3DXMESH CreateSphere(float r)
		 {
			LPD3DXMESH pmesh = NULL;
			if ( D3DXCreateSphere(g_pD3DDevice, r, 40,40, &pmesh, NULL) != D3D_OK )
			{
				MessageBox(NULL, "Can't create mesh", "Error", MB_OK | MB_ICONEXCLAMATION);
				return pmesh;
			}
			// create a mesh with texture coordinates
			LPD3DXMESH texMesh;
			if (FAILED(pmesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM,FVF_VERTEX,g_pD3DDevice,&texMesh)))
				; // failed, return un-textured mesh
			else
			{
				// finished with the original mesh, release it
				pmesh->Release();
				// lock the vertex buffer
				_VERTEX *pVerts;
				if (SUCCEEDED(texMesh->LockVertexBuffer(0,(void **) &pVerts))) 
				{
					// get vertex count
					int numVerts=texMesh->GetNumVertices();
					// loop through the vertices
					for (int i=0;i<numVerts;i++) 
					{
						// calculate texture coordinates
						pVerts->tu=asinf(pVerts->norm.x)/D3DX_PI+0.5f;
						pVerts->tv=1-(asinf(pVerts->norm.y)/D3DX_PI+0.5f);    
						// go to next vertex
						pVerts++;
					}
					// unlock the vertex buffer
					texMesh->UnlockVertexBuffer();
				}
				pmesh = texMesh;
			}
			return pmesh;
		 }

private: System::Void buttonAddNewForce_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 // adding new force to the list
			 if (comboNewForce->SelectedIndex < 0)
			 {
				 MessageBox(NULL, "Please select a force type.", "Error", MB_OK | MB_ICONEXCLAMATION);
				 comboNewForce->Focus();
				 return;
			 }
			 UIForce *force = new UIForce();
			 force->iForceType = comboNewForce->SelectedIndex+1;
			 force->Name = textNewForce->Text;
			 MForce *mforce;
			 force->pForce = (CForce*)malloc(sizeof(CForce));
			 mforce = new MForce(force->iForceType, objects); // create and initialize a managed object instance
			 
			 // align CForce to MForce
			 mforce->Type = force->iForceType;
			 force->pMForce = mforce;
			 force->SyncForce();

			 forces->Add(force); // finally, add initialized UI force object to list of UI force objects

			 cPhysics->AddForce(force->pForce); // add force to physics simulation
			 
			 // refresh combo box
			 comboForces->DataSource = NULL;
			 comboForces->DataSource = forces;
			 comboForces->DisplayMember = "Name";
			 comboForces->ValueMember = "Name";
			 comboForces->SelectedIndex = forces->Count-1; // select newly added force
		 }

private: System::Void comboForces_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e)
		 {
			 int index = comboForces->SelectedIndex;
			 if (index < 0) 
			 {
				 propertyGrid2->SelectedObject = NULL;
				 return;
			 }
			 propertyGrid2->SelectedObject = ((UIForce*)(forces->Item[index]))->pMForce;
		 }

private: System::Void propertyGrid2_PropertyValueChanged(System::Object *  s, System::Windows::Forms::PropertyValueChangedEventArgs *  e)
		 {
			 if (SimulationMode = modeStop)
			 {
				 // if simulation is not running, immediately synchronize MForce with CForce for the
				 // currently edited force
				 MForce* mforce = (MForce*)(propertyGrid2->SelectedObject);
				 // to find the matching CForce, seek into UIForce list to find the
				 // currently edited MForce
				 bool bFound = false;
				 for (int i=0; i<forces->Count; i++)
				 {
					 if ( ((UIForce*)(forces->Item[i]))->pMForce == mforce)
					 {
						 bFound = true;
						 break;
					 }
				 }
				 if (bFound == false)
				 {
					 MessageBox(NULL, "Lost connection to UIForce", "Internal Error", MB_OK | MB_ICONEXCLAMATION);
					 return;
				 }

				 // stop rendering while changing objects
				 //((RenderWindow*)renderWindow)->StopTimer();
				 timerSimulation->Stop();

				 ((UIForce*)(forces->Item[i]))->SyncForce();

				 //((RenderWindow*)renderWindow)->StartTimer();
				 timerSimulation->Start();
			 }
		 }





private: System::Void checkStart_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (checkStart->Checked == false)
			 {
				 checkStart->Checked = true;
				 return;
			 }
			 if (checkStart->Checked == true && checkPause->Checked == true)
			 {
				 checkStart->Checked = false;
				 return;
			 }
			 checkPause->Checked = false;
			 checkStop->Checked = false;
			 
			 ReleaseObject(grboundingbox);
			 cPhysics->StartSim();
		 }

private: System::Void checkPause_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (checkStart->Checked == false && checkPause->Checked == true)
			 {
				 // revert checked status if simulation is not active
				 checkPause->Checked = !checkPause->Checked;
				 return;
			 }
			 checkStart->Checked = !checkPause->Checked;
		 }

private: System::Void checkStop_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (checkStop->Checked == false)
			 {
				 checkStop->Checked = true;
				 return;
			 }
			 checkPause->Checked = false;
			 checkStart->Checked = false;
		 }

private: System::Void timerSimulation_Tick(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (checkStart->Checked == true)
			 {
				 cPhysics->StepSimulation(timerSimulation->Interval/1000.0f);
			 }
			 if (((RenderWindow*)renderWindow)->bInitialized)
				Render();
		 }

};


}


