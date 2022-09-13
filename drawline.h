#include "mainwindow.h"

class DrawLineCommand : public vtkCommand
{
public:
	static DrawLineCommand *New()
	{
		return new DrawLineCommand;
	}


	double Position[3] = { 0,0,0 };
	double Position1[3] = { 0,0,0 };
	double LinePixelonX = 0.0;
	double LinePixelonY = 0.0;
	double ptA[3] = { 0,0,200 }, ptC[3] = { 0,0,200 }, ptB[3] = { 0,0,200 }, ptD[3] = { 0,0,200 };
	double pickPos[3] = { 0,0,100 };
	double pickPos1[3] = { 0,0,100 };
	int p = 1;
	MainWindow *a;

	vtkRenderWindowInteractor* iren;
	bool saved = false;
	void seta(MainWindow* pointer) {

		a = pointer;

	}


	double GetLinePixelonX()
	{
		return LinePixelonX;

	}

	double GetLinePixelonY()
	{
		return LinePixelonY;

	}
	void DrawLineCommand::SetInteractor(vtkRenderWindowInteractor* interactor)
	{
		iren = interactor;
	}
	virtual void Execute(vtkObject* caller, unsigned long eventId, void*)
	{


		saved = a->getsaved();

		vtkSmartPointer<vtkWorldPointPicker>pointPicker = vtkSmartPointer<vtkWorldPointPicker>::New();

		iren->SetPicker(pointPicker);

		switch (eventId)
		{
		case vtkCommand::LeftButtonPressEvent :
	
				iren->GetPicker()->Pick(iren->GetEventPosition()[0],
					iren->GetEventPosition()[1], iren->GetEventPosition()[2],
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

				iren->GetPicker()->GetPickPosition(pickPos);
				ptA[0] = pickPos[0];
				ptA[1] = pickPos[1];
				ptA[2] = pickPos[2] + 200;
				//std::cout << "世界坐标:" << pickPos[0] << "," << pickPos[1] << "," << pickPos[2] << std::endl;
				
				break;

	    /*case vtkCommand::LeftButtonReleaseEvent:
			if (p == 1) {
				p = 0;
			}
			else {
				p = 1;
			}
		
			break;*/

		 case vtkCommand::MouseMoveEvent:
			if ((a->GetP())==0)
			{
				iren->GetPicker()->Pick(iren->GetEventPosition()[0],
					iren->GetEventPosition()[1], iren->GetEventPosition()[2],
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
				iren->GetPicker()->GetPickPosition(pickPos1);
				
				//std::cout << "右键:" << pickPos1[0] << "," << pickPos1[1] << "," << pickPos1[2] << std::endl;

				///////////////////////////////////////A-C-B-D-A 顺时针

				if (saved == 0)
				{

					ptB[0] = pickPos1[0];////B
					ptB[1] = pickPos1[1];
					ptB[2] = pickPos1[2] + 200;

					ptC[0] = ptB[0];///C
					ptC[1] = ptA[1];
					ptC[2] = ptB[2];

					ptD[0] = ptA[0];////D
					ptD[1] = ptB[1];
					ptD[2] = ptB[2];

					line->SetPoint1(ptA);
					line->SetPoint2(ptC);
					line->Update();///A-----C

					line1->SetPoint1(ptC);
					line1->SetPoint2(ptB);
					line1->Update();

					line2->SetPoint1(ptB);
					line2->SetPoint2(ptD);
					line2->Update();

					line3->SetPoint1(ptD);
					line3->SetPoint2(ptA);
					line3->Update();

					mapper->SetInputData(line->GetOutput());
					mapper->Update();
					mapper1->SetInputData(line1->GetOutput());
					mapper1->Update();
					mapper2->SetInputData(line2->GetOutput());
					mapper2->Update();
					mapper3->SetInputData(line3->GetOutput());
					mapper3->Update();

					actor->SetMapper(mapper);
					actor1->SetMapper(mapper1);
					actor2->SetMapper(mapper2);
					actor3->SetMapper(mapper3);

					actor->GetProperty()->SetColor(1, 0, 0);
					actor1->GetProperty()->SetColor(1, 0, 0);
					actor2->GetProperty()->SetColor(1, 0, 0);
					actor3->GetProperty()->SetColor(1, 0, 0);

					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor1);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor2);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor3);

					iren->GetRenderWindow()->Render();

					LinePixelonX = pickPos1[0] - pickPos[0];
					LinePixelonY = pickPos1[1] - pickPos[1];

					a->setLinePixelonXg(LinePixelonX);
					a->setLinePixelonYg(LinePixelonY);
				
					//std::cout << LinePixelonX << std::endl;
					//std::cout << LinePixelonY << std::endl;
				}

				else
				{
					//std::cout << "ROI BOX loaded" << std::endl;
					//LinePixelonX = a->getLinePixelonXg();
					//LinePixelonY = a->getLinePixelonYg();
					//std::cout << LinePixelonX << std::endl;
					//std::cout << LinePixelonY << std::endl;
					LinePixelonX = a->getLinePixelonXg();
					LinePixelonY = a->getLinePixelonYg();

					ptC[0] = ptA[0] + LinePixelonX;
					ptC[1] = ptA[1];
					ptC[2] = ptA[2];

					ptB[0] = ptA[0] + LinePixelonX;
					ptB[1] = ptA[1] + LinePixelonY;
					ptB[2] = ptA[2];

					ptD[0] = ptA[0];
					ptD[1] = ptA[1] + LinePixelonY;
					ptD[2] = ptA[2];

					line->SetPoint1(ptA);
					line->SetPoint2(ptC);
					line->Update();///A-----C

					line1->SetPoint1(ptC);
					line1->SetPoint2(ptB);
					line1->Update();

					line2->SetPoint1(ptB);
					line2->SetPoint2(ptD);
					line2->Update();

					line3->SetPoint1(ptD);
					line3->SetPoint2(ptA);
					line3->Update();

					mapper->SetInputData(line->GetOutput());
					mapper->Update();
					mapper1->SetInputData(line1->GetOutput());
					mapper1->Update();
					mapper2->SetInputData(line2->GetOutput());
					mapper2->Update();
					mapper3->SetInputData(line3->GetOutput());
					mapper3->Update();

					actor->SetMapper(mapper);
					actor1->SetMapper(mapper1);
					actor2->SetMapper(mapper2);
					actor3->SetMapper(mapper3);

					actor->GetProperty()->SetColor(1, 0.5, 0);
					actor1->GetProperty()->SetColor(1, 0.5, 0);
					actor2->GetProperty()->SetColor(1, 0.5, 0);
					actor3->GetProperty()->SetColor(1, 0.5, 0);

					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor1);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor2);
					iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor3);
					iren->GetRenderWindow()->Render();


					
				}

			}
			break;

			
		}

	}



	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> actor3 = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
	vtkSmartPointer<vtkLineSource> line1 = vtkSmartPointer<vtkLineSource>::New();
	vtkSmartPointer<vtkLineSource> line2 = vtkSmartPointer<vtkLineSource>::New();
	vtkSmartPointer<vtkLineSource> line3 = vtkSmartPointer<vtkLineSource>::New();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();


}; 
