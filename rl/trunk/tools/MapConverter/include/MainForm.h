#pragma once

#include "MAPLoader.h"
#include "TextureManager.h"
#include "World.h"
#include "Entity.h"

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


    private: System::Windows::Forms::Label *  lblMap;
    private: System::Windows::Forms::TextBox *  txtBoxMapFile;
    private: System::Windows::Forms::Button *  btnOpenMap;
    private: System::Windows::Forms::OpenFileDialog *  openFileDialog;
    private: System::Windows::Forms::Button *  btnUmwandeln;

    private: System::Windows::Forms::StatusBarPanel *  statusBarPanel1;
    private: System::Windows::Forms::StatusBar *  statusBar;
    private: System::Windows::Forms::Button *  btnOpenWadDir;
    private: System::Windows::Forms::TextBox *  txtBoxWadDir;
    private: System::Windows::Forms::Label *  label1;
    private: System::Windows::Forms::FolderBrowserDialog *  openDirDialog;
    private: System::Windows::Forms::SaveFileDialog *  saveFileDialog;



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
            this->lblMap = new System::Windows::Forms::Label();
            this->txtBoxMapFile = new System::Windows::Forms::TextBox();
            this->btnOpenMap = new System::Windows::Forms::Button();
            this->openFileDialog = new System::Windows::Forms::OpenFileDialog();
            this->btnUmwandeln = new System::Windows::Forms::Button();
            this->statusBar = new System::Windows::Forms::StatusBar();
            this->statusBarPanel1 = new System::Windows::Forms::StatusBarPanel();
            this->btnOpenWadDir = new System::Windows::Forms::Button();
            this->txtBoxWadDir = new System::Windows::Forms::TextBox();
            this->label1 = new System::Windows::Forms::Label();
            this->openDirDialog = new System::Windows::Forms::FolderBrowserDialog();
            this->saveFileDialog = new System::Windows::Forms::SaveFileDialog();
            (__try_cast<System::ComponentModel::ISupportInitialize *  >(this->statusBarPanel1))->BeginInit();
            this->SuspendLayout();
            // 
            // lblMap
            // 
            this->lblMap->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->lblMap->Location = System::Drawing::Point(8, 8);
            this->lblMap->Name = S"lblMap";
            this->lblMap->Size = System::Drawing::Size(72, 16);
            this->lblMap->TabIndex = 1;
            this->lblMap->Text = S"MAP-Datei";
            // 
            // txtBoxMapFile
            // 
            this->txtBoxMapFile->Location = System::Drawing::Point(8, 24);
            this->txtBoxMapFile->Name = S"txtBoxMapFile";
            this->txtBoxMapFile->Size = System::Drawing::Size(352, 20);
            this->txtBoxMapFile->TabIndex = 2;
            this->txtBoxMapFile->Text = S"";
            // 
            // btnOpenMap
            // 
            this->btnOpenMap->Image = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"btnOpenMap.Image")));
            this->btnOpenMap->ImeMode = System::Windows::Forms::ImeMode::NoControl;
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
            this->openFileDialog->Title = S"Zu konvertierende Map laden";
            // 
            // btnUmwandeln
            // 
            this->btnUmwandeln->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->btnUmwandeln->Location = System::Drawing::Point(272, 96);
            this->btnUmwandeln->Name = S"btnUmwandeln";
            this->btnUmwandeln->Size = System::Drawing::Size(88, 24);
            this->btnUmwandeln->TabIndex = 4;
            this->btnUmwandeln->Text = S"Umwandeln..";
            this->btnUmwandeln->Click += new System::EventHandler(this, button2_Click);
            // 
            // statusBar
            // 
            this->statusBar->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->statusBar->Location = System::Drawing::Point(0, 131);
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
            // btnOpenWadDir
            // 
            this->btnOpenWadDir->Image = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"btnOpenWadDir.Image")));
            this->btnOpenWadDir->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->btnOpenWadDir->Location = System::Drawing::Point(368, 64);
            this->btnOpenWadDir->Name = S"btnOpenWadDir";
            this->btnOpenWadDir->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->btnOpenWadDir->Size = System::Drawing::Size(20, 20);
            this->btnOpenWadDir->TabIndex = 8;
            this->btnOpenWadDir->Click += new System::EventHandler(this, btnOpenWadDir_Click);
            // 
            // txtBoxWadDir
            // 
            this->txtBoxWadDir->Location = System::Drawing::Point(8, 64);
            this->txtBoxWadDir->Name = S"txtBoxWadDir";
            this->txtBoxWadDir->Size = System::Drawing::Size(352, 20);
            this->txtBoxWadDir->TabIndex = 7;
            this->txtBoxWadDir->Text = S"";
            // 
            // label1
            // 
            this->label1->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->label1->Location = System::Drawing::Point(8, 48);
            this->label1->Name = S"label1";
            this->label1->Size = System::Drawing::Size(96, 16);
            this->label1->TabIndex = 6;
            this->label1->Text = S"WAD-Verzeichnis";
            // 
            // openDirDialog
            // 
            this->openDirDialog->Description = S"Bitte das WAD-Verzeichnis wählen";
            this->openDirDialog->SelectedPath = S"C:\\";
            this->openDirDialog->ShowNewFolderButton = false;
            // 
            // saveFileDialog
            // 
            this->saveFileDialog->DefaultExt = S"HalfLife-Map-Dateien (*.map)|*.map";
            this->saveFileDialog->Filter = S"HalfLife-Map-Dateien (*.map)|*.map";
            this->saveFileDialog->Title = S"Konvertierte Map Speichern";
            // 
            // MainForm
            // 
            this->AutoScale = false;
            this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
            this->BackColor = System::Drawing::SystemColors::ActiveBorder;
            this->ClientSize = System::Drawing::Size(394, 155);
            this->Controls->Add(this->btnOpenWadDir);
            this->Controls->Add(this->txtBoxWadDir);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->statusBar);
            this->Controls->Add(this->btnUmwandeln);
            this->Controls->Add(this->btnOpenMap);
            this->Controls->Add(this->txtBoxMapFile);
            this->Controls->Add(this->lblMap);
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
                String* mapFile =  txtBoxMapFile->get_Text();
                String* wadDir = txtBoxWadDir->get_Text();
                String* wadString = "";

                if( System::IO::File::Exists( mapFile ) )
                {
                    if( System::IO::Directory::Exists( wadDir ) )
                    {
                        this->Cursor = Cursors::WaitCursor;

                        CMAPLoader* mapLoader = new CMAPLoader();
                        CTextureManager* textureManager = new CTextureManager();
                        CWorld* world = new CWorld();

                        
                        System::IO::StringWriter* sw  = new System::IO::StringWriter();
                        
                        try
                        {
                            mapLoader->LoadMAPFile(mapFile,world,textureManager);


                            if( world->GetWADString(&wadString) )
                            {
                                textureManager->LoadTextures(wadString, wadDir );                                
                            }

                            world->UpdateTexCoords(textureManager);


                            ArrayList* alist = world->GetEntities();

                            for( int i = 0; i < alist->Count; i++ )
                            {
                                CEntity* ent = __try_cast<CEntity*>(alist->get_Item(i));
                                String * entname = ent->GetName();
                                ent->WriteToStream(sw,textureManager);

                                LogManager::getSingletonPtr()->Log(entname, Color::Green);

                                if( entname->ToLower()->Equals(S"worldspawn") )
                                {
                                    LogManager::getSingletonPtr()->Log( String::Concat( S"skyname : ", ent->GetArgVal(S"skyname")), Color::Green);
                                }
                            }
                           
                            sw->Close();  
                            String* mapOutFile = sw->ToString();     
                             
                            System::Windows::Forms::DialogResult dr;
                            dr =   saveFileDialog->ShowDialog();

                            if ( dr == DialogResult::OK )
                            {
                                System::IO::StreamWriter* sr = System::IO::File::CreateText(saveFileDialog->get_FileName());
                                sr->WriteLine( mapOutFile );
                                sr->Close();
                            }
                        }
                        catch(Exception *e)
                        {
                            sw->Close();
                            MessageBox::Show( S"Konnte Map nicht konvertieren", S"Fehler" );
                        }

                        

                        this->Cursor = Cursors::Default;
                    }
                    else
                        MessageBox::Show( S"Wad-Verzeichnis existiert nicht", S"Fehler" );
                }
                else
                    MessageBox::Show( S"Map-Datei existiert nicht", S"Fehler" );
                
             }

    private: System::Void btnOpenMap_Click(System::Object *  sender, System::EventArgs *  e)
            {
                System::Windows::Forms::DialogResult dr;
                dr = openFileDialog->ShowDialog();

                if ( dr == DialogResult::OK )
                {
                    txtBoxMapFile->set_Text( openFileDialog->get_FileName() );
                    
                    int length = openFileDialog->get_FileName()->Length - System::IO::Path::GetFileName(openFileDialog->get_FileName())->Length;
                    String* justDir = openFileDialog->get_FileName()->Substring( 0, length );

                    openDirDialog->set_SelectedPath( justDir );
                    saveFileDialog->set_InitialDirectory( justDir );
                    saveFileDialog->set_FileName(String::Concat( System::IO::Path::GetFileNameWithoutExtension( openFileDialog->get_FileName() ) , "_q3.map" ) );

                    txtBoxWadDir->set_Text( justDir );
                }
            }

private: System::Void btnOpenWadDir_Click(System::Object *  sender, System::EventArgs *  e)
         {
                System::Windows::Forms::DialogResult dr;
                dr = openDirDialog->ShowDialog();

                if ( dr == DialogResult::OK )
                {
                    txtBoxWadDir->set_Text( openDirDialog->get_SelectedPath() );
                }
         }

};
}
