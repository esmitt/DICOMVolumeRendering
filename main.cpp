#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPointData.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkFloatArray.h>
#include <vtkImageReader.h>
#include <string>

int main(int argc, char *argv[])
{
  //std::string strFolder = "\\DICOM\\D201510\\DD1412"; //put your directory path here
  
  //vtkNew<vtkImageData> imageData;
  
	//vtkNew<vtkDICOMImageReader> reader;
 // // read the dicom dir
 // reader->SetDirectoryName(strFolder.c_str());
 // reader->Update();
 // imageData->ShallowCopy(reader->GetOutput());

	std::string strFile = "C:\\Users\\eramirez\\Desktop\\Skeletonization\\DistalGlobalSegAprox_16_01_2018_Complex0.raw"; //put your directory path here
	vtkSmartPointer<vtkImageReader> reader = vtkSmartPointer<vtkImageReader>::New();
	reader->SetFileName(strFile.c_str());
	reader->SetDataScalarTypeToFloat();
	reader->SetFileDimensionality(3);
	reader->SetDataExtent(0, 286, 0, 380, 0, 643);
	reader->SetHeaderSize(0);
	reader->Update();

  // properties options
  vtkNew<vtkVolumeProperty> volumeProperty;
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

  // get the real range in hounsfield 
  vtkDataArray* arr = reader->GetOutput()->GetPointData()->GetScalars();
  double range[2];
  arr->GetRange(range);

  // 1D transfer functions
  vtkNew<vtkColorTransferFunction> colorTF;
  colorTF->AddRGBPoint(-200, 0.0, 0.0, 0.0);
  colorTF->AddRGBPoint(110, 0.4, 0.4, 1.0);
  colorTF->AddRGBPoint(512, 1.0, 1.0, 1.0);
  colorTF->AddRGBPoint(range[1], 0.9, 0.1, 0.3);

  vtkNew<vtkPiecewiseFunction> scalarTF;
  scalarTF->AddPoint(-200, 0.00);
  scalarTF->AddPoint(110, 0.00);
  scalarTF->AddPoint(512, 0.5);
  scalarTF->AddPoint(range[1], 0.9);

  vtkNew<vtkPiecewiseFunction> gradientTF;
  gradientTF->AddPoint(-200, 0.0);
  gradientTF->AddPoint(range[1] / 4.0, 1.0);

  volumeProperty->SetScalarOpacity(scalarTF);
  volumeProperty->SetGradientOpacity(gradientTF);
  volumeProperty->SetColor(colorTF);

  // setup rendering context
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetSize(512, 512);
  renderWindow->SetMultiSamples(0);

  // mapping data
  vtkNew<vtkGPUVolumeRayCastMapper> mapper;
  mapper->SetInputConnection(reader->GetOutputPort());
  mapper->SetBlendModeToComposite();
  mapper->SetUseJittering(1);

  // renderer and volume
  vtkNew<vtkRenderer> renderer;
  renderWindow->AddRenderer(renderer);
  renderer->SetBackground(0.03, 0.33, 0.33);

  vtkNew<vtkVolume> volume;
  volume->SetMapper(mapper);
  volume->SetProperty(volumeProperty);
  renderer->AddVolume(volume);

  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(1.3);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(renderWindow);

  vtkNew<vtkInteractorStyleTrackballCamera> style;
  interactor->SetInteractorStyle(style);

  renderWindow->Render();
  interactor->Start();

  return EXIT_SUCCESS;
}
