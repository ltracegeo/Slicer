#ifndef __qSlicerSubjectHierarchyLazyPlugin_h
#define __qSlicerSubjectHierarchyLazyPlugin_h

// SubjectHierarchy includes
#include "qSlicerSubjectHierarchyAbstractPlugin.h"

#include "qSlicerTextsSubjectHierarchyPluginsExport.h"

class qSlicerSubjectHierarchyLazyPluginPrivate;
class vtkMRMLNode;
class vtkMRMLSubjectHierarchyNode;

/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class Q_SLICER_TEXTS_SUBJECT_HIERARCHY_PLUGINS_EXPORT qSlicerSubjectHierarchyLazyPlugin : public qSlicerSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qSlicerSubjectHierarchyAbstractPlugin Superclass;
  qSlicerSubjectHierarchyLazyPlugin(QObject* parent = nullptr);
  ~qSlicerSubjectHierarchyLazyPlugin() override;

public:
  /// Determines if a data node can be placed in the hierarchy using the actual plugin,
  /// and gets a confidence value for a certain MRML node (usually the type and possibly attributes are checked).
  /// \param node Node to be added to the hierarchy
  /// \param parentItemID Prospective parent of the node to add.
  ///   Default value is invalid. In that case the parent will be ignored, the confidence numbers are got based on the to-be child node alone.
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   node, and 1 means that the plugin is the only one that can handle the node (by node type or identifier attribute)
  double canAddNodeToSubjectHierarchy(
    vtkMRMLNode* node,
    vtkIdType parentItemID=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID )const override;

  /// Determines if the actual plugin can handle a subject hierarchy item. The plugin with
  /// the highest confidence number will "own" the item in the subject hierarchy (set icon, tooltip,
  /// set context menu etc.)
  /// \param item Item to handle in the subject hierarchy tree
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   item, and 1 means that the plugin is the only one that can handle the item (by node type or identifier attribute)
  double canOwnSubjectHierarchyItem(vtkIdType itemID)const override;

  /// Get role that the plugin assigns to the subject hierarchy item.
  ///   Each plugin should provide only one role.
  Q_INVOKABLE const QString roleForPlugin()const override;

  /// Get icon of an owned subject hierarchy item
  /// \return Icon to set, empty icon if nothing to set
  QIcon icon(vtkIdType itemID) override;
  /// Get visibility icon for a visibility state
  QIcon visibilityIcon(int visible) override;

  /// Generate tooltip for a owned subject hierarchy item
  QString tooltip(vtkIdType itemID)const override;

  /// Set display visibility of a owned subject hierarchy item
  void setDisplayVisibility(vtkIdType itemID, int visible) override;

  /// Get display visibility of a owned subject hierarchy item
  /// \return Display visibility (0: hidden, 1: shown, 2: partially shown)
  int getDisplayVisibility(vtkIdType itemID)const override;

protected:
  QScopedPointer<qSlicerSubjectHierarchyLazyPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerSubjectHierarchyLazyPlugin);
  Q_DISABLE_COPY(qSlicerSubjectHierarchyLazyPlugin);
};

#endif
