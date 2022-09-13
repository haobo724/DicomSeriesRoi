#include "mainwindow.h"
class vtkMycallBackRightClick : public vtkCommand {
public:
	static vtkMycallBackRightClick *New() {
		vtkMycallBackRightClick *cb = new vtkMycallBackRightClick;
		return cb;
	}
	vtkImageViewer2* _ImageViewer;
	MainWindow *a;
	vtkRenderWindowInteractor* iren;
	bool saved = false;
	void seta(MainWindow* pointer) {

		a = pointer;
	}
	void vtkMycallBackRightClick::SetInteractor(vtkRenderWindowInteractor* interactor)
	{
		iren = interactor;
	}


protected:
	double PositionE[2] = { 0,0 };
	double PositionS[2] = { 0,0 };
	double Size[2] = { 1,1 };
	double Oldsize[2] = { 1,1 };
	double LinePixelonX = 0;
	double LinePixelonY = 0;
	float zoom;
public:
	int p = 1;
	virtual void Execute(vtkObject *(caller),
		unsigned long(eventId),
		void *vtkNotUsed(callData)) {

		switch (eventId)
		{
		case vtkCommand::LeftButtonPressEvent :
			if (a->GetP() == 1) {
				PositionS[0] = iren->GetEventPosition()[0];
				PositionS[1] = iren->GetEventPosition()[1];
				//std::cout << "Start Point:" << PositionS[0] << "," << PositionS[1] << std::endl;
				
			}
				break;

		case vtkCommand::MouseMoveEvent:
			if (a->GetP() == 0)
			{

				if (!(a->getsaved()))
				{
					PositionE[0] = iren->GetEventPosition()[0];
					PositionE[1] = iren->GetEventPosition()[1];
					Size[0] = iren->GetRenderWindow()->GetSize()[0];
					Size[1] = iren->GetRenderWindow()->GetSize()[1];
					//std::cout << "End Point:" << PositionE[0] << "," << PositionE[1] << std::endl;
					a->SetPositionX(PositionE[0]);
					a->SetPositionY(PositionE[1]);
					a->SetwinSizeX(Size[0]);//把old size上传
					a->SetwinSizeY(Size[1]);
					LinePixelonX = abs(PositionE[0] - PositionS[0]);//在当前size下x轴像素数量，相对于窗口大小，而不是真实像素数
					LinePixelonY = abs(PositionE[1] - PositionS[1]);
					a->setLinePixelonXP(LinePixelonX);
					a->setLinePixelonYP(LinePixelonY);
					//std::cout << "windowsize:" << Size[0] << "," << Size[1] << std::endl;
				

				}
				else {
					LinePixelonX = a->getLinePixelonXP();
					LinePixelonY = a->getLinePixelonYP();
					//std::cout << "右键锁定" << std::endl;
					Size[0] = iren->GetRenderWindow()->GetSize()[0];
					Size[1] = iren->GetRenderWindow()->GetSize()[1];

					Oldsize[0] = a->getWinSizeX();
					Oldsize[1] = a->getWinSizeY();

					/*PositionE[0] = PositionS[0]+ LinePixelonX * (Size[0] / Oldsize[0]);
					PositionE[1] = PositionS[1] - LinePixelonY* (Size[1] / Oldsize[1]);*/

					PositionE[0] = PositionS[0] + LinePixelonX;
					PositionE[1] = PositionS[1] - LinePixelonY;
					//std::cout << "End Point:" << PositionE[0] << "," << PositionE[1] << std::endl;
					

				}
			}
			break;

		//case vtkCommand::LeftButtonReleaseEvent:
		//	std::cout << "hi there" << std::endl;
		//	if (p == 1) {
		//		p = 0;
		//	}
		//	else {
		//		p = 1;
		//	}
		//	break; 



		}
	}

	
	
	void SetImageViewer(vtkImageViewer2* imageViewer) {
		_ImageViewer = imageViewer;
		
	}


	double GetPositionX() {
		if (PositionE[0] > PositionS[0]) {
			return PositionE[0];
		}
		else {
			return PositionS[0];
		}
	}
	double GetPositionY() {
		if (PositionE[1] < PositionS[1]) {
			return PositionE[1];
		}
		else {
			return PositionS[1];
		}
	}

	double GetSizeX() {
		return Size[0];
	}
	double GetSizeY() {
		return Size[1];
	}
	double GetPositionXS() {
		if (PositionE[0] < PositionS[0]) {
			return PositionE[0];
		}
		else {
			return PositionS[0];
		}
	}
	double GetPositionYS() {
		if (PositionE[1] > PositionS[1]) {
			return PositionE[1];
		}
		else {
			return PositionS[1];
		}
	}
};