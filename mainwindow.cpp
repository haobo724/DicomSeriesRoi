#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qdebug.h"
#include "myInteractor.h"
#include "drawline.h"
#include "myInteractionStyle.h"





void QuitFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);
void CoordinanteTrans(double& X, double& Y, const itk::Image< unsigned short, 3 >::SizeType size, double WindowSize[2], float cc, double distance[2]);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{
	
    ui->setupUi(this);
	connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(OpenImg()));
	connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(SaveImg()));
	connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(Next()));
	connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(SaveBox()));
	connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(LoadBox()));
	//connect(ui->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(Test()));

}
void MainWindow::OpenImg()//选定打开路径
{

	QString OpenFile, OpenFilePath;
	//QImage image;
	QDir dir;
	OpenFile = QFileDialog::getExistingDirectory(this,
		"please choose an image file");
	if (OpenFile.isEmpty() == true) return;//如果打开的地址不是空的，就返回
	QFileInfo OpenFileInfo;

	ui->label->setText(OpenFile);
	std::string path = ui->label->text().toUtf8().data();
	this->InputPath = path;

}
void MainWindow::SaveImg()//选定打开路径
{
	//std::cout << this->InputPath;
	QString SaveFile, SaveFilePath;

	QDir dir;
	SaveFile = QFileDialog::getExistingDirectory(this,
		"Please choose an image file");
	if (SaveFile.isEmpty() == true) return;//如果打开的地址不是空的，就返回
	QFileInfo OpenFileInfo;

	ui->label_2->setText(SaveFile);
	std::string path = ui->label_2->text().toUtf8().data();
	this->OutputPath = path;
}
void MainWindow::SaveBox()//保存所需box
{
	std::cout << "The ROI BOX are saved";
	
}



void MainWindow::LoadBox()//保存所需box
{
	bool saved = true;
	std::cout << "The ROI BOX are loaded";
	//this->savedG=saved;
	this->setsaved(saved);
}

MainWindow::~MainWindow()
{
	delete ui;
}

int MainWindow::Next(){
	const int Dimension = 3;
	const int OutputDimension = 2;
	using PixelType = unsigned short;
	using ImageType = itk::Image< PixelType, Dimension >;
	using OutputPixelType = unsigned short;
	using Image2DType = itk::Image< OutputPixelType, OutputDimension >;
	using ReaderType = itk::ImageSeriesReader< ImageType >;

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	ReaderType::Pointer reader = ReaderType::New();
	using InputNamesGeneratorType = itk::GDCMSeriesFileNames;

	InputNamesGeneratorType::Pointer inputNames = InputNamesGeneratorType::New();
	inputNames->SetUseSeriesDetails(true);
	inputNames->AddSeriesRestriction("0008|0021");
	inputNames->SetGlobalWarningDisplay(false);

	inputNames->SetInputDirectory(InputPath);
	ReaderType::FileNamesContainer filenames =
		inputNames->GetInputFileNames();
	reader->SetImageIO(gdcmImageIO);
	reader->SetFileNames(filenames);


	std::cout << "The input series in directory " << InputPath
		<< " has " << filenames.size() << " files" << std::endl;


	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject &excp)
	{
		std::cerr << "Exception thrown while reading the series" << std::endl;
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}
	if (InputPath !="") {
		std::cout << "Successfully Loaded"<<"Path is："<<InputPath;
	}
	ImageType::RegionType region =
		reader->GetOutput()->GetLargestPossibleRegion();
	const ImageType::SizeType size = region.GetSize();
	//////////////////////////////////////////////////////ITK TO VTK //////////////////////////////////////////////////////////
	using ITKTOVTKFilterType = itk::ImageToVTKImageFilter<ImageType>;
	ITKTOVTKFilterType::Pointer VTKFilter = ITKTOVTKFilterType::New();
	VTKFilter->SetInput(reader->GetOutput());
	VTKFilter->Update();

	vtkSmartPointer<vtkImageFlip> imageflip = vtkSmartPointer<vtkImageFlip>::New();
	imageflip->SetInputData(VTKFilter->GetOutput());
	imageflip->SetFilteredAxes(1);//Y轴
	imageflip->Update();
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();


	imageViewer->SetInputConnection(imageflip->GetOutputPort());
	imageViewer->GetRenderWindow()->SetWindowName("ROI");
	//// slice status message
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg = StatusMessage::Format(imageViewer->GetSliceMin(), imageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	//// usage hint message
	//vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	//usageTextProp->SetFontFamilyToCourier();
	//usageTextProp->SetFontSize(14);
	//usageTextProp->SetVerticalJustificationToTop();
	//usageTextProp->SetJustificationToLeft();

	//vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	//usageTextMapper->SetInput("");
	//usageTextMapper->SetTextProperty(usageTextProp);

	//vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	//usageTextActor->SetMapper(usageTextMapper);
	//usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	//usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);//文字位置

	std::string ColorLevel;
	std::string ColorLeveltagkey = "0028|1050";
	std::string ColorWindow;
	std::string ColorWindowtagkey = "0028|1051";
	gdcmImageIO->GetValueFromTag(ColorLeveltagkey, ColorLevel);
	gdcmImageIO->GetValueFromTag(ColorWindowtagkey, ColorWindow);
	std::cout << "ColorLevel :" << ColorLevel << std::endl
		<< " ColorWindow :" << ColorWindow << std::endl;
	int intColorLevel = atoi(ColorLevel.c_str());
	int intColorWindow = atoi(ColorWindow.c_str());
	if (intColorLevel > 0 && intColorWindow > 0) {
		imageViewer->SetColorLevel(intColorLevel);
		imageViewer->SetColorWindow(intColorWindow);

	}
	else {
		imageViewer->SetColorLevel(952);
		imageViewer->SetColorWindow(1201);
		std::cout << "Set default 952,1201" << std::endl;

	}

	// initialize rendering and interaction
	imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);// imageviewer里面封装了render和renderwindow所以可以直接用getter调用，或者如下图单独拿出来
	imageViewer->GetRenderer()->ResetCamera();
	/*vtkSmartPointer<vtkRenderer> renderer= vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.2, 0.7, 0.4);
	imageViewer->SetRenderer(renderer); *///这样单独把render拿出来当然可以使用，但是其他的东西比如mapper actor都要重新和他关联 否则就没关联上，actor和render已经关联过了，在这如果强行单独拿出来那就要再弄一次
	imageViewer->Render();//->GetRenderWindow()加上这个立马失效，很迷
	int extent[6];
	VTKFilter->GetOutput()->GetExtent(extent);

	double origin[3];
	VTKFilter->GetOutput()->GetOrigin(origin);

	double spacing[3];
	VTKFilter->GetOutput()->GetSpacing(spacing);

	vtkCamera* camera = imageViewer->GetRenderer()->GetActiveCamera();
	camera->ParallelProjectionOn();
	float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
	float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
	//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
	float yd = (extent[3] - extent[2] + 1)*spacing[1];

	float d = camera->GetDistance();
	camera->SetParallelScale(0.5f*static_cast<float>(yd));
	camera->SetFocalPoint(xc, yc, 0.0);
	camera->SetPosition(xc, yc, +d);

	// create an interactor with our own style (inherit from vtkInteractorStyleImage)
	// in order to catch mousewheel and key events  AND PIXEL POSITION
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
		vtkSmartPointer<myVtkInteractorStyleImage>::New();

	// make imageviewer2 and sliceTextMapper visible to our interactorstyle
	// to enable slice status message updates when scrolling through the slices
	myInteractorStyle->SetImageViewer(imageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);
	myInteractorStyle->seta(this);

	imageViewer->SetupInteractor(renderWindowInteractor);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
	// renderWindowInteractor->SetRenderWindow(imageViewer->GetRenderWindow()); 写不写都一样
	// add slice status message and usage hint message to the renderer
	imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	//imageViewer->GetRenderer()->AddActor2D(usageTextActor);

	////////////////////////////////Call back part start///////////////////////////////////////////////
	

	vtkSmartPointer<vtkMycallBackRightClick> RightClick =
		vtkSmartPointer<vtkMycallBackRightClick>::New();

	RightClick->seta(this);
	RightClick->SetInteractor(renderWindowInteractor);

	renderWindowInteractor->AddObserver(vtkCommand::LeftButtonReleaseEvent, RightClick);
	renderWindowInteractor->AddObserver(vtkCommand::MouseMoveEvent, RightClick);
	renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent, RightClick);


	vtkSmartPointer<DrawLineCommand> drawLineCallBack =vtkSmartPointer<DrawLineCommand>::New();

	drawLineCallBack->SetInteractor(renderWindowInteractor);
	drawLineCallBack->seta(this);
	renderWindowInteractor->AddObserver(vtkCommand::LeftButtonReleaseEvent, drawLineCallBack);
	renderWindowInteractor->AddObserver(vtkCommand::MouseMoveEvent, drawLineCallBack);
	renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent, drawLineCallBack);



	vtkSmartPointer<vtkCallbackCommand> Quit = vtkSmartPointer<vtkCallbackCommand>::New();
	Quit->SetCallback(QuitFunction);
	imageViewer->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, Quit);

	////////////////////////////////Call back part END///////////////////////////////////////////////
	//imageViewer->GetRenderWindow()->Render();
	double size1 = size[1];
	double size0 = size[0];
	


	imageViewer->SetSize(size0 / 3, size1 / 3);
	imageViewer->UpdateDisplayExtent();
	double distance[2];
	distance[0] = myInteractorStyle->GetDistance0();
	distance[1] = myInteractorStyle->GetDistance1();

	renderWindowInteractor->Render();
	renderWindowInteractor->Start();
	renderWindowInteractor->Render();



	//std::cout << "Image Size is:" << size[0] << "," << size[1] << "," << size[2] << std::endl;

	//std::cout << "Now WindowSize：" << imageViewer->GetRenderWindow()->GetSize()[0] << "," << imageViewer->GetRenderWindow()->GetSize()[1] << std::endl;

	double StartPosition[2];
	double EndPosition[2];
	double WindowSize[2];
	int *dim = imageViewer->GetInput()->GetDimensions();
	vtkSmartPointer<vtkCoordinate> c = vtkCoordinate::New();
	c->SetValue(0, 0, 0);
	int *res = c->GetComputedDisplayValue(imageViewer->GetRenderer());
		int a2 = res[0];
		c->SetValue(dim[0], 0, 0);
		res = c->GetComputedDisplayValue(imageViewer->GetRenderer());
		
	float cc= static_cast<float>(res[0] - a2) / dim[0];//3.91
	std::cout << "Zoom Size is:" << cc << std::endl;
	StartPosition[0] = RightClick->GetPositionXS();
	StartPosition[1] = RightClick->GetPositionYS();

	EndPosition[0] = RightClick->GetPositionX();
	EndPosition[1] = RightClick->GetPositionY();
	WindowSize[0] = RightClick->GetSizeX();//windowsize
	WindowSize[1] = RightClick->GetSizeY();


	///////////////////////////////////ROI//////////////////////////////////////////////////////////
	using FilterType = itk::RegionOfInterestImageFilter< ImageType, ImageType >;//imagetype is 3D
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(reader->GetOutput());//reader is seriesreader

	CoordinanteTrans(StartPosition[0], StartPosition[1], size, WindowSize,cc, distance);
	CoordinanteTrans(EndPosition[0], EndPosition[1], size, WindowSize,cc, distance);
	int PixelonX = EndPosition[0] - StartPosition[0];
	int PixelonY = EndPosition[1] - StartPosition[1];
	EndPosition[0] = EndPosition[0] + PixelonX * (cc / 3.91 - 1);
		EndPosition[1]= EndPosition[1] + PixelonY * (cc / 3.91 - 1);
		 PixelonX = EndPosition[0] - StartPosition[0];
		 PixelonY = EndPosition[1] - StartPosition[1];
	std::string outputDirectory = OutputPath;
	itksys::SystemTools::MakeDirectory(outputDirectory);
	using NameGeneratorType = itk::NumericSeriesFileNames;
	NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();//point
	//std::vector<std::string> intput= filenames;
	std::string inputname = filenames.begin()->c_str();
	inputname = inputname.replace(inputname.find(".IMA"), 4, "");
	inputname = inputname.replace(inputname.find(InputPath), InputPath.size(), "");
	std::string imagesize = std::to_string(PixelonX)+"x"+ std::to_string(PixelonY);
	std::string format = "_%03d_Crop_";
	std::string extend = ".IMA";
	std::string Outputnames = outputDirectory + inputname + format+ imagesize+ extend;


	nameGenerator->SetSeriesFormat(Outputnames.c_str());
	//std::cout << "After CoordinanteTrans,start:" << StartPosition[0] << "," << StartPosition[1] << std::endl;
	//std::cout << "After CoordinanteTrans,end:" << EndPosition[0] << "," << EndPosition[1] << std::endl;
	//std::cout << "image:" << EndPosition[0]- StartPosition[0]<<"x"<< EndPosition[1]- StartPosition[1]<<std::endl;

	const auto startx = static_cast<itk::IndexValueType>(StartPosition[0]);
	const auto endx = static_cast<itk::IndexValueType>(EndPosition[0]);
	const auto starty = static_cast<itk::IndexValueType>(StartPosition[1]);
	const auto endy = static_cast<itk::IndexValueType>(EndPosition[1]);

	ImageType::IndexType start;
	start[0] = startx;
	start[1] = starty;
	start[2] = 0;

	ImageType::IndexType end;
	end[0] = endx;
	end[1] = endy;
	end[2] = size[2];

	ImageType::SizeType size2;


	size2[0] = abs(end[0] - start[0]) + 1;
	size2[1] = abs(end[1] - start[1]) + 1;
	size2[2] = size[2];
	region.SetIndex(start);
	//region.SetUpperIndex(end);
	region.SetSize(size2);

	filter->SetRegionOfInterest(region);
	filter->Update();

	const unsigned int firstSlice = start[2];
	const unsigned int lastSlice = start[2] + size[2] - 1;
	nameGenerator->SetStartIndex(firstSlice);
	nameGenerator->SetEndIndex(lastSlice);
	nameGenerator->SetIncrementIndex(1);

	using WriterType = itk::ImageSeriesWriter<ImageType, Image2DType>;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(filter->GetOutput());
	writer->SetImageIO(gdcmImageIO);
	writer->SetFileNames(nameGenerator->GetFileNames());
	writer->SetMetaDataDictionaryArray(reader->GetMetaDataDictionaryArray());
	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & error)
	{
		std::cerr << "Error: " << error << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}

void QuitFunction(vtkObject * caller, long unsigned int eventId, void * clientData, void * callData)
{
	vtkRenderWindowInteractor *iren =
		static_cast<vtkRenderWindowInteractor*>(caller);
	std::string key = iren->GetKeySym();
	if (key == "Q")
	{
		iren->GetRenderWindow()->Finalize();

		// Stop the interactor
		iren->TerminateApp();
		std::cout << "Closing window..." << std::endl;

	}
};
void CoordinanteTrans(double& X, double& Y, const itk::Image< unsigned short, 3 >::SizeType size, double WindowSize[2],float cc,double distance[2]) 
{

	double scale[2];
	double WindowMidX = WindowSize[0] / 2;
	double WindowMidY = WindowSize[1] / 2;

	cc = static_cast<double>(cc / 3.91964);
	scale[0] = size[0] / WindowSize[0];
	scale[1] = size[1] / WindowSize[1];
	double DistanceCenterX = X - WindowMidX;
	double DistanceCenterY = Y - WindowMidY;
	distance[0] = distance[0] / cc;
	distance[1] = distance[1] / cc;

	 DistanceCenterX = DistanceCenterX/ cc;
	 DistanceCenterY = DistanceCenterY/ cc;

	 WindowMidX = WindowMidX + distance[0];
	 WindowMidY = WindowMidY + distance[1];

	 X = WindowSize[0]/2 + DistanceCenterX;
	 Y = WindowSize[1]/2 + DistanceCenterY;

	

	X = X * scale[0];
	Y = (1 - Y / WindowSize[1])*size[1];
	


	


}



