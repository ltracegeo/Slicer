// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qSlicerSubjectHierarchyPluginHandler.h"
#include "qSlicerSubjectHierarchyLazyPlugin.h"
#include "qSlicerSubjectHierarchyDefaultPlugin.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLTextNode.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

// Slicer includes
#include "qSlicerApplication.h"
#include "qSlicerAbstractModuleWidget.h"
#include "qSlicerLayoutManager.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class qSlicerSubjectHierarchyLazyPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qSlicerSubjectHierarchyLazyPlugin);
protected:
  qSlicerSubjectHierarchyLazyPlugin* const q_ptr;
public:
  qSlicerSubjectHierarchyLazyPluginPrivate(qSlicerSubjectHierarchyLazyPlugin& object);
  ~qSlicerSubjectHierarchyLazyPluginPrivate() override;
  void init();
public:
  QIcon LazyNodeIcon;
};

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyLazyPluginPrivate methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLazyPluginPrivate::qSlicerSubjectHierarchyLazyPluginPrivate(qSlicerSubjectHierarchyLazyPlugin& object)
: q_ptr(&object)
{
  this->LazyNodeIcon = QIcon(":Icons/Lazy.png");
}

//------------------------------------------------------------------------------
void qSlicerSubjectHierarchyLazyPluginPrivate::init()
{
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLazyPluginPrivate::~qSlicerSubjectHierarchyLazyPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyLazyPlugin methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLazyPlugin::qSlicerSubjectHierarchyLazyPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qSlicerSubjectHierarchyLazyPluginPrivate(*this) )
{
  this->m_Name = QString("Lazy");

  Q_D(qSlicerSubjectHierarchyLazyPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyLazyPlugin::~qSlicerSubjectHierarchyLazyPlugin() = default;

//----------------------------------------------------------------------------
double qSlicerSubjectHierarchyLazyPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is NULL";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLTextNode"))
    {
    const char* isLazy = node->GetAttribute("LazyNode");
    if (isLazy != nullptr && !std::string{isLazy}.empty())
      {
      return 1.0;
      }
    }

  return 0.0;
}

//---------------------------------------------------------------------------
double qSlicerSubjectHierarchyLazyPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  // Table
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLTextNode"))
    {
    const char* isLazy = associatedNode->GetAttribute("LazyNode");
    if (isLazy != nullptr && !std::string{isLazy}.empty())
      {
      return 1.0;
      }
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qSlicerSubjectHierarchyLazyPlugin::roleForPlugin()const
{
  return "Lazy";
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyLazyPlugin::icon(vtkIdType itemID)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  Q_D(qSlicerSubjectHierarchyLazyPlugin);

  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    return d->LazyNodeIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyLazyPlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qSlicerSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//---------------------------------------------------------------------------
QString qSlicerSubjectHierarchyLazyPlugin::tooltip(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QString("Invalid!");
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QString("Error!");
    }

  QString tooltipString;

  vtkMRMLTextNode* textNode = vtkMRMLTextNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (textNode)
    {
    std::stringstream textNodeInfo;
    textNodeInfo << "Virtual node" << std::endl;
    if (textNode->GetEncoding() != VTK_ENCODING_NONE)
      {
      textNodeInfo << std::endl << textNode->GetText();
      }
    QString textInfo = QString::fromStdString(textNodeInfo.str());
    tooltipString.append(textInfo);
    }

  return tooltipString;
}

//---------------------------------------------------------------------------
void qSlicerSubjectHierarchyLazyPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene!";
    return;
    }
  if (this->getDisplayVisibility(itemID) == visible)
    {
    return;
    }

  
  const char* currentLazyNodeIdStr = shNode->GetAttribute("CurrentLazyNode");
  vtkIdType currentLazyNodeId = vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID;
  if (currentLazyNodeIdStr != nullptr && !std::string{currentLazyNodeIdStr}.empty())
    {
    currentLazyNodeId = std::stoi(currentLazyNodeIdStr);
    }
  
  if (visible)
    {
    // Show the new lazy node
    shNode->SetAttribute("CurrentLazyNode", std::to_string(itemID).c_str());
    shNode->InvokeEvent("CurrentLazyNodeChanged");
    if (currentLazyNodeId != vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
      {
      // Update icon of the previously shown lazy node
      shNode->ItemModified(currentLazyNodeId);
      }
    }
  else
    {
    // Hide
    if (currentLazyNodeId == itemID)
      {
      // Hide the currently shown lazy node
      shNode->SetAttribute("CurrentLazyNode", "");
      shNode->InvokeEvent("CurrentLazyNodeChanged");
      }
    }

  // Trigger icon update
  shNode->ItemModified(itemID);
}

//-----------------------------------------------------------------------------
int qSlicerSubjectHierarchyLazyPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return -1;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return -1;
    }

 const char* currentLazyNodeIdStr = shNode->GetAttribute("CurrentLazyNode");
 vtkIdType currentLazyNodeId = vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID;
 if (currentLazyNodeIdStr != nullptr && !std::string{currentLazyNodeIdStr}.empty())
    {
    currentLazyNodeId = std::stoi(currentLazyNodeIdStr);
    }
  return currentLazyNodeId == itemID ? 1 : 0;
}
