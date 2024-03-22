//---------------------------------------------------------------------------

#include <vcl.h>
#include <ScktComp.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("main.cpp", fmMain);
USEFORM("about.cpp", fmAbout);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  //*************************************************************************   
  HWND  hMutex;
  try{   
      //Mutex of beyondcopy  
      hMutex   =   OpenMutex(MUTEX_ALL_ACCESS,0,"fmmain_beyondcopy");

      if (hMutex)//already run   
      {
          MessageBox(0, "Another instance of BeyondCopy is running.", "BeyondCopy",
              MB_OK + MB_ICONINFORMATION);
          return 1;//return now
      }
      else //new mutex  
          CreateMutex(0,0,"fmmain_beyondcopy");   
  //***********************************************************************  
        try
        {
            Application->Initialize();
            Application->Title = "BeyondCopy";
        


            Application->CreateForm(__classid(TfmMain), &fmMain);
		Application->CreateForm(__classid(TfmAbout), &fmAbout);
		Application->Run();
        }


        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }



        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
  //*******************************
  }
  __finally
  {
      ::ReleaseMutex(hMutex);
  }
  //******************************  

        return 0;
}
//---------------------------------------------------------------------------
