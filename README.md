Rendering DICOM Volume using VTK
===================

This is a template to render a volume from DICOM files using Ray Casting in VTK. The code uses `vtkGPUVolumeRayCastMapper` for the volume. Also, the scalar opacity, gradient and 1D transfer function. Files are read using `vtkDICOMImageReader` copied to a `vtkImageData`.

The code open the volume and display it (base code)

###### If you want to contribute to this project and make it better, your help is very welcome.