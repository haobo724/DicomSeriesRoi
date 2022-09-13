#include "mainwindow.h"
// helper class to format slice status message
class StatusMessage {
public:
	static std::string Format(int slice, int maxSlice) {
		std::stringstream tmp;
		tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
		return tmp.str();
	}
};

// Define own interaction style
class myVtkInteractorStyleImage : public vtkInteractorStyleImage//继承于vtkInteractorStyleImage这个父类
{
public:
	static myVtkInteractorStyleImage *New();
	vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
	vtkImageViewer2* _ImageViewer;
	vtkTextMapper* _StatusMapper;
	int _Slice;
	int _MinSlice;
	int _MaxSlice;
	double origin[3];
	double end[3];
	double distance[2] = {0,0};


public:
	MainWindow *a;

	void seta(MainWindow* pointer) {

		a = pointer;

	}

	void SetImageViewer(vtkImageViewer2* imageViewer) {
		_ImageViewer = imageViewer;
		_MinSlice = _ImageViewer->GetSliceMin();
		_MaxSlice = _ImageViewer->GetSliceMax();
		_Slice = _MinSlice;
		cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
	}

	void SetStatusMapper(vtkTextMapper* statusMapper) {
		_StatusMapper = statusMapper;
	}
	int GetSlice() {
		return _Slice;

	}

	void switchstaus(){
	
		if (a->GetP() == 1) {
			a->SetP(0);
		}
		else {
			a->SetP(1);
		}
	
	
	}


public:
	void MoveSliceForward() {
		if (_Slice < _MaxSlice) {
			_Slice += 1;
			cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
			_ImageViewer->SetSlice(_Slice);

			std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
			_StatusMapper->SetInput(msg.c_str());
			_ImageViewer->Render();
		}
	}

	void MoveSliceBackward() {
		if (_Slice > _MinSlice) {
			_Slice -= 1;
			cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
			_ImageViewer->SetSlice(_Slice);
			std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
			_StatusMapper->SetInput(msg.c_str());
			_ImageViewer->Render();
		}
	}


	virtual void OnKeyDown() {
		std::string key = this->GetInteractor()->GetKeySym();//这个this指这个类myVtkInteractorStyleImage，当interactor被调用即style被调用，
															//在这里style里面有个默认事件是按键按下，如果是这个事件，
															//那么this style会获取它所属于的interactor，然后这个interactor会获取所按下的key值
		if (key.compare("Up") == 0) {
			//cout << "Up arrow key was pressed." << endl;
			MoveSliceForward();
		}
		else if (key.compare("Down") == 0) {
			//cout << "Down arrow key was pressed." << endl;
			MoveSliceBackward();
		}
		else if ((key.compare("A") == 0 )|( key.compare("a") == 0)) {
			
			_ImageViewer->SetColorLevel(_ImageViewer->GetColorLevel()+50);
			std::cout << "Current ColorLevel " << _ImageViewer->GetColorLevel() << std::endl;
			std::cout << "Current ColorWindow " << _ImageViewer->GetColorWindow() << std::endl;
		

			_ImageViewer->Render();

		}
		else if ((key.compare("D") == 0) | (key.compare("d") == 0)) {
		
			_ImageViewer->SetColorLevel(_ImageViewer->GetColorLevel()-50);
			std::cout << "Current ColorLevel " << _ImageViewer->GetColorLevel() << std::endl;
			std::cout << "Current ColorWindow " << _ImageViewer->GetColorWindow() << std::endl;
			_ImageViewer->Render();

		}
		else if ((key.compare("W") == 0) | (key.compare("w") == 0)) {
			
			_ImageViewer->SetColorWindow(_ImageViewer->GetColorWindow()+50);
			std::cout << "Current ColorLevel " << _ImageViewer->GetColorLevel() << std::endl;
			std::cout << "Current ColorWindow " << _ImageViewer->GetColorWindow() << std::endl;
			_ImageViewer->Render();

		}
		else if ((key.compare("S") == 0) | (key.compare("s") == 0)) {
		
			_ImageViewer->SetColorWindow(_ImageViewer->GetColorWindow()-50);
			std::cout << "Current ColorLevel " << _ImageViewer->GetColorLevel() << std::endl;
			std::cout << "Current ColorWindow " << _ImageViewer->GetColorWindow() << std::endl;
			_ImageViewer->Render();

		}

		// forward event
		vtkInteractorStyleTrackballCamera::OnKeyDown();
	}
	virtual void OnLeftButtonDown() {
		
		vtkInteractorStyleImage::OnLeftButtonDown();


	}

	virtual void OnLeftButtonUp() {
		
		switchstaus();
	

		vtkInteractorStyleImage::OnLeftButtonUp();


	}


	virtual void OnRightButtonDown()
	{
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();

		//std::cout << "RightButton is locked" << std::endl;

	}


	virtual void OnMiddleButtonDown()
	{
	
		origin[0] = this->GetInteractor()->GetEventPosition()[0];
		origin[1] = this->GetInteractor()->GetEventPosition()[1];
		origin[2] = this->GetInteractor()->GetEventPosition()[2];
		std::cout << "Move GetPosition from:" << origin[0] << "," << origin[1] << std::endl;

		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	}

	virtual void OnMiddleButtonUp()
	{
	
		end[0] = this->GetInteractor()->GetEventPosition()[0];
		end[1] = this->GetInteractor()->GetEventPosition()[1];
		end[2] = this->GetInteractor()->GetEventPosition()[2];
		std::cout << "TO:" << end[0] << "," << end[1] << std::endl;
		
	     SetDistance0(distance[0]+end[0] - origin[0]);
		 SetDistance1(distance[1]+end[1] - origin[1]);
		
		vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
	}

	virtual void OnMouseWheelForward() {
		//std::cout << "Scrolled mouse wheel forward." << std::endl;
		MoveSliceForward();
		// don't forward events, otherwise the image will be zoomed 
		// in case another interactorstyle is used (e.g. trackballstyle, ...)
		// vtkInteractorStyleImage::OnMouseWheelForward();
	}


	virtual void OnMouseWheelBackward() {
		//std::cout << "Scrolled mouse wheel backward." << std::endl;
		if (_Slice > _MinSlice) {
			MoveSliceBackward();
		}
		// don't forward events, otherwise the image will be zoomed 
		// in case another interactorstyle is used (e.g. trackballstyle, ...)
		// vtkInteractorStyleImage::OnMouseWheelBackward();
	}


	void SetDistance0(double X) {
		distance[0] = X;
	}
	void SetDistance1(double Y) {
		distance[1] = Y;
	}

	double GetDistance0() {
		return distance[0];
	}
	double GetDistance1() {
		return distance[1];
	}
};

vtkStandardNewMacro(myVtkInteractorStyleImage); 
