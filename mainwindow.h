#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <Qlabel>
#include <QLineEdit>
#include <QImage>
#include <QFileInfo>
#include <QFileDialog>

#include "itkIdentityTransform.h"
#include "itkImageFileReader.h"
#include "itkVersion.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkImageToVTKImageFilter.h"
#include "gdcmUIDGenerator.h"
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkCamera.h>
#include <vtkImageActor.h>
#include <vtkCallbackCommand.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include "vtkImageMapper3D.h"
#include <vtkInteractorStyleTrackballCamera.h>  
#include <vtkPolyDataMapper.h> 
#include <vtkObjectFactory.h>  
#include <vtkLine.h>  
#include <vtkWorldPointPicker.h>
#include <vtkProperty.h>  
#include <vtkPolyLine.h>  
#include <vtkDataSetMapper.h>  
#include <vtkLineSource.h>  
#include <vtkRendererCollection.h>  
#include <vtkCellPicker.h>  
#include "vtkAutoInit.h"   
#include <string>
#include <stack>

// needed to easily convert int to std::string
#include <sstream>
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
#include "vtkImageFlip.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	double LinePixelonXg;
	double LinePixelonYg;
	double PositionX;
	double PositionY;

	double  LinePixelonXP;
	double LinePixelonYP;
	bool savedG=false;
	double WinsizeX = 1;
	double WinsizeY = 1;
	int p=1;
		
	void SetP(int X) {
		p = X;
	}
	int GetP() {
	return p;
	}



    void setLinePixelonXg(double X) {
		LinePixelonXg=X;
	}
	void setLinePixelonYg(double Y) {
		LinePixelonYg=Y;
	}

	void setLinePixelonXP(double X) {
		LinePixelonXP = X;
	}
	void setLinePixelonYP(double Y) {
		LinePixelonYP = Y;
	}

	void SetPositionX(double X) {
		PositionX = X;
	}
	void SetPositionY(double Y) {
		PositionY = Y;
	}



	void SetwinSizeX(double X) {
		WinsizeX = X;
	}
	void SetwinSizeY(double Y) {
		WinsizeY = Y;
	}

	double GetPositionX( ) {
		return PositionX ;
	}
	double GetPositionY() {
		return PositionY ;
	}

	double  getWinSizeX() {
		return WinsizeX;
	}
	double  getWinSizeY() {
		return WinsizeY;
	}
	double getLinePixelonXg() {
		return LinePixelonXg;
	}
	double getLinePixelonYg() {
		return LinePixelonYg;
	}

	double getLinePixelonXP() {
		return LinePixelonXP;
	}
	double getLinePixelonYP() {
		return LinePixelonYP;
	}

	void setsaved(bool saved) {
		
	    this->savedG = true;
	
	}
	bool getsaved() {
		
		return savedG;
	}

	
	

private:
    Ui::MainWindow *ui;
	std::string InputPath;
	std::string OutputPath;
	std::stack<std::string> box;
	


private slots:
	void OpenImg();
	void SaveImg();
	void SaveBox();
	void LoadBox();
	int Next();
};



#endif // MAINWINDOW_H
