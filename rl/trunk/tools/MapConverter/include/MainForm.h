#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapConverter
{
	/// <summary> 
	/// Zusammenfassung für MainForm
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public __gc class MainForm : public System::Windows::Forms::Form
	{
	public: 
		MainForm(void)
		{
			InitializeComponent();
            
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
    private: System::Windows::Forms::PictureBox *  picPantheon;
    private: System::Windows::Forms::Label *  lblMap;
    private: System::Windows::Forms::TextBox *  txtBoxMapFile;
    private: System::Windows::Forms::Button *  btnOpenMap;
    private: System::Windows::Forms::OpenFileDialog *  openFileDialog;
    private: System::Windows::Forms::Button *  btnUmwandeln;

    private: System::Windows::Forms::StatusBarPanel *  statusBarPanel1;
    private: System::Windows::Forms::StatusBar *  statusBar;









	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
            System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(MapConverter::MainForm));
            this->picPantheon = new System::Windows::Forms::PictureBox();
            this->lblMap = new System::Windows::Forms::Label();
            this->txtBoxMapFile = new System::Windows::Forms::TextBox();
            this->btnOpenMap = new System::Windows::Forms::Button();
            this->openFileDialog = new System::Windows::Forms::OpenFileDialog();
            this->btnUmwandeln = new System::Windows::Forms::Button();
            this->statusBar = new System::Windows::Forms::StatusBar();
            this->statusBarPanel1 = new System::Windows::Forms::StatusBarPanel();
            (__try_cast<System::ComponentModel::ISupportInitialize *  >(this->statusBarPanel1))->BeginInit();
            this->SuspendLayout();
            // 
            // picPantheon
            // 
            this->picPantheon->BackColor = System::Drawing::SystemColors::Window;
            this->picPantheon->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->picPantheon->Image = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"picPantheon.Image")));
            this->picPantheon->Location = System::Drawing::Point(8, 8);
            this->picPantheon->Name = S"picPantheon";
            this->picPantheon->Size = System::Drawing::Size(72, 72);
            this->picPantheon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->picPantheon->TabIndex = 0;
            this->picPantheon->TabStop = false;
            // 
            // lblMap
            // 
            this->lblMap->Location = System::Drawing::Point(88, 8);
            this->lblMap->Name = S"lblMap";
            this->lblMap->Size = System::Drawing::Size(72, 16);
            this->lblMap->TabIndex = 1;
            this->lblMap->Text = S"MAP-Datei";
            // 
            // txtBoxMapFile
            // 
            this->txtBoxMapFile->Location = System::Drawing::Point(88, 24);
            this->txtBoxMapFile->Name = S"txtBoxMapFile";
            this->txtBoxMapFile->Size = System::Drawing::Size(272, 20);
            this->txtBoxMapFile->TabIndex = 2;
            this->txtBoxMapFile->Text = S"";
            // 
            // btnOpenMap
            // 
            this->btnOpenMap->Image = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"btnOpenMap.Image")));
            this->btnOpenMap->Location = System::Drawing::Point(368, 24);
            this->btnOpenMap->Name = S"btnOpenMap";
            this->btnOpenMap->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->btnOpenMap->Size = System::Drawing::Size(20, 20);
            this->btnOpenMap->TabIndex = 3;
            this->btnOpenMap->Click += new System::EventHandler(this, btnOpenMap_Click);
            // 
            // openFileDialog
            // 
            this->openFileDialog->DefaultExt = S"map";
            this->openFileDialog->Filter = S"HalfLife-Map-Dateien (*.map)|*.map";
            this->openFileDialog->ReadOnlyChecked = true;
            this->openFileDialog->RestoreDirectory = true;
            // 
            // btnUmwandeln
            // 
            this->btnUmwandeln->Location = System::Drawing::Point(301, 56);
            this->btnUmwandeln->Name = S"btnUmwandeln";
            this->btnUmwandeln->Size = System::Drawing::Size(88, 24);
            this->btnUmwandeln->TabIndex = 4;
            this->btnUmwandeln->Text = S"Umwandeln..";
            this->btnUmwandeln->Click += new System::EventHandler(this, button2_Click);
            // 
            // statusBar
            // 
            this->statusBar->Location = System::Drawing::Point(0, 83);
            this->statusBar->Name = S"statusBar";
            System::Windows::Forms::StatusBarPanel* __mcTemp__1[] = new System::Windows::Forms::StatusBarPanel*[1];
            __mcTemp__1[0] = this->statusBarPanel1;
            this->statusBar->Panels->AddRange(__mcTemp__1);
            this->statusBar->ShowPanels = true;
            this->statusBar->Size = System::Drawing::Size(394, 24);
            this->statusBar->SizingGrip = false;
            this->statusBar->TabIndex = 5;
            // 
            // statusBarPanel1
            // 
            this->statusBarPanel1->AutoSize = System::Windows::Forms::StatusBarPanelAutoSize::Spring;
            this->statusBarPanel1->Text = S"Bitte zu konvertierende Map wählen";
            this->statusBarPanel1->Width = 394;
            // 
            // MainForm
            // 
            this->AutoScale = false;
            this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
            this->BackColor = System::Drawing::SystemColors::ActiveBorder;
            this->ClientSize = System::Drawing::Size(394, 107);
            this->Controls->Add(this->statusBar);
            this->Controls->Add(this->btnUmwandeln);
            this->Controls->Add(this->btnOpenMap);
            this->Controls->Add(this->txtBoxMapFile);
            this->Controls->Add(this->lblMap);
            this->Controls->Add(this->picPantheon);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
            this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
            this->MaximizeBox = false;
            this->Name = S"MainForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = S"Pantheon MapConverter";
            this->Closing += new System::ComponentModel::CancelEventHandler(this, MainForm_Closing);
            (__try_cast<System::ComponentModel::ISupportInitialize *  >(this->statusBarPanel1))->EndInit();
            this->ResumeLayout(false);

        }		


  
    private: System::Void MainForm_Closing(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e)
             {
             }



    private: System::Void button2_Click(System::Object *  sender, System::EventArgs *  e)
             {              
                if( System::IO::File::Exists( txtBoxMapFile->get_Text() ) )
                {
                    MessageBox::Show( "Datei existiert" );

                }
             }

    private: System::Void btnOpenMap_Click(System::Object *  sender, System::EventArgs *  e)
            {
                System::Windows::Forms::DialogResult dr;
                dr = openFileDialog->ShowDialog();

                if ( dr == DialogResult::OK )
                {
                    txtBoxMapFile->set_Text( openFileDialog->get_FileName() );
                }
            }
};
}
