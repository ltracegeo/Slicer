// Qt includes
#include <QDebug>
#include <QString>
#include <QVariant>

// SlicerQt includes
//#include "qSlicerAbstractModule.h"
//#include "qSlicerCoreApplication.h"
//#include "qSlicerModuleManager.h"
#include "qSlicerScalarOverlayIO.h"
#include "qSlicerScalarOverlayIOOptionsWidget.h"

// Logic includes
#include "vtkSlicerModelsLogic.h"

// MRML includes
#include <vtkMRMLModelNode.h>
#include <vtkMRMLStorageNode.h>

//-----------------------------------------------------------------------------
qSlicerScalarOverlayIO::qSlicerScalarOverlayIO(QObject* _parent)
  :qSlicerIO(_parent)
{
}

//-----------------------------------------------------------------------------
QString qSlicerScalarOverlayIO::description()const
{
  return "Scalar Overlay";
}

//-----------------------------------------------------------------------------
qSlicerIO::IOFileType qSlicerScalarOverlayIO::fileType()const
{
  return qSlicerIO::ScalarOverlayFile;
}

//-----------------------------------------------------------------------------
QString qSlicerScalarOverlayIO::extensions()const
{
  return "*.*";
}

//-----------------------------------------------------------------------------
qSlicerIOOptions* qSlicerScalarOverlayIO::options()const
{
  qSlicerIOOptionsWidget* options = new qSlicerScalarOverlayIOOptionsWidget;
  qDebug() << "qSlicerScalarOverlayIO::options():" << this->mrmlScene();
  options->setMRMLScene(this->mrmlScene());
  return options;
}

//-----------------------------------------------------------------------------
bool qSlicerScalarOverlayIO::load(const IOProperties& properties)
{
  Q_ASSERT(properties.contains("fileName"));
  Q_ASSERT(properties.contains("modelNodeId"));
  QString fileName = properties["fileName"].toString();
  QString modelNodeId = properties["modelNodeId"].toString();
  vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(
      this->mrmlScene()->GetNodeByID(modelNodeId.toLatin1().data()));
  Q_ASSERT(modelNode);

  // FIXME: Use the following commented lines when qSlicerModelsIO.[x/cxx] will
  // be in the same directory than the Models module. Create its own logic for
  // now.
  //vtkSlicerModelsLogic* modelsLogic = vtkSlicerModelsLogic::SafeDownCast(
  //  qSlicerCoreApplication::application()->moduleManager()
  //  ->module("Models")->logic());
  vtkSlicerModelsLogic* modelsLogic = vtkSlicerModelsLogic::New();
  Q_ASSERT(modelsLogic);

  vtkMRMLStorageNode* node = modelsLogic->AddScalar(fileName.toLatin1().data(), modelNode);
  if (node)
    {
    this->setLoadedNodes(QStringList(QString(node->GetID())));
    }
  else
    {
    this->setLoadedNodes(QStringList());
    }
  modelsLogic->Delete();
  return node != 0;
}

