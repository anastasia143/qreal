#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>
#include <QSplashScreen>
#include <QtGui>

#include "../pluginManager/editorManager.h"
#include "../pluginManager/toolPluginManager.h"
#include "propertyeditorproxymodel.h"
#include "gesturesPainterInterface.h"
#include "../dialogs/gesturesShow/gesturesWidget.h"
#include "../interpreters/visualDebugger/debuggerConnector.h"
#include "mainWindowInterpretersInterface.h"
#include "../kernel/settingsManager.h"

namespace Ui {
class MainWindowUi;
}

namespace qReal {

class EditorView;
class ListenerManager;
class VisualDebugger;

namespace models {
class Models;
}

namespace gui {
class ErrorReporter;
}

class MainWindow : public QMainWindow, public qReal::gui::MainWindowInterpretersInterface
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	EditorManager* manager();
	EditorView *getCurrentTab();
	ListenerManager *listenerManager();
	GesturesPainterInterface *gesturesPainter();
	QModelIndex rootIndex() const;

	QAction *actionDeleteFromDiagram() const;

	virtual void highlight(Id const &graphicalId, bool exclusive = true);
	virtual void dehighlight(Id const &graphicalId);
	virtual void dehighlight();
	virtual gui::ErrorReporter *errorReporter();
	void openShapeEditor(QPersistentModelIndex index, int role, QString const propertyValue);

signals:
	void gesturesShowed();
	void currentIdealGestureChanged();
	void rootDiagramChanged();

public slots:
	void deleteFromScene();

	void propertyEditorScrollTo(QModelIndex const &index);

	void activateItemOrDiagram(Id const &id, bool bl = true, bool isSetSel = true);
	void activateItemOrDiagram(QModelIndex const &idx, bool bl = true, bool isSetSel = true);
	virtual void selectItem(Id const &id);

	void selectItemWithError(Id const &id);

private slots:

	void setSceneFont();
	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(Id const &id);

	void settingsPlugins();

	void showAbout();
	void showHelp();

	void checkoutDialogOk();
	void checkoutDialogCancel();

	bool open(QString const &dirName);
	bool checkPluginsAndReopen();
	void saveAs();
	void saveAll();
	void fullscreen();
	bool openNewProject();
	void createProject();

	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void finalClose();
	void closeAllTabs();

	void sceneSelectionChanged();

	void doCheckout();
	void doCommit();
	void exportToXmi();
	void generateToJava();
	void parseJavaLibraries();
	void applySettings();

	void deleteFromScene(QGraphicsItem *target);

	void activateSubdiagram(QModelIndex const &idx);

	void debug();
	void debugSingleStep();
	void drawDebuggerStdOutput(QString output);
	void drawDebuggerErrOutput(QString output);
	void generateAndBuild();
	void startDebugger();
	void runProgramWithDebugger();
	void killProgramWithDebugger();
	void closeDebuggerProcessAndThread();
	void placeBreakpointsInDebugger();
	void goToNextBreakpoint();
	void goToNextInstruction();
	void configureDebugger();
	void setBreakpointAtStart();
	void startDebugging();
	void checkEditorForDebug(int index);

private slots:
	void deleteFromDiagram();
	void changeMiniMapSource(int index);
	void closeTab(int index);
	void closeTab(QModelIndex const &graphicsIndex);
//	void exterminate();
	void generateEditor();
	void generateEditorWithQRMC();
	void parseEditorXml();
	void generateToHascol();
	void parseHascol();
	void showPreferencesDialog();

	void connectActions();
	void connectDebugActions();

	void centerOn(Id const &id);
	void graphicalModelExplorerClicked(const QModelIndex &index);
	void logicalModelExplorerClicked(const QModelIndex &index);

	void openNewTab(const QModelIndex &index);
	void initCurrentTab(const QModelIndex &rootIndex);

	void showGestures();
	void showAlignment(bool isChecked);
	void switchGrid(bool isChecked);
	void switchAlignment(bool isChecked);
	void setShape(QString const &data, QPersistentModelIndex const &index, int const &role);

	void openShapeEditor();

	void setDiagramCreateFlag();
	void diagramInCreateListDeselect();
	void diagramInCreateListSelected(int num);

	void on_actionNew_Diagram_triggered();

	void updatePaletteIcons();

private:
	Ui::MainWindowUi *mUi;

	QCloseEvent *mCloseEvent;
	models::Models *mModels;
	EditorManager mEditorManager;
	ToolPluginManager mToolManager;
	ListenerManager *mListenerManager;
	PropertyEditorModel mPropertyModel;
//	PropertyEditorDelegate mDelegate;
	GesturesWidget *mGesturesWidget;

	QVector<bool> mSaveListChecked;
	bool mDiagramCreateFlag;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	DebuggerConnector *mDebuggerConnector;
	VisualDebugger *mVisualDebugger;
	gui::ErrorReporter *mErrorReporter;  // Has ownership

	/** @brief Fullscreen mode flag */
	bool mIsFullscreen;

	/** @brief Internal map table to store info what widgets should we hide/show */
	QMap<QString, bool> mDocksVisibility;

	QString mSaveDir;

	void createDiagram(const QString &idString);
	void loadNewEditor(QString const &directoryName, QString const &metamodelName,
			QString const &commandFirst, QString const &commandSecond, QString const &extension, QString const &prefix);

	void loadPlugins();

	QListWidget* createSaveListWidget();
	void suggestToSave();
	void suggestToCreateDiagram();

	virtual void closeEvent(QCloseEvent *event);
	void deleteFromExplorer(bool isLogicalModel);
	void keyPressEvent(QKeyEvent *event);
	QString getWorkingDir(QString const &dialogWindowTitle);

	int getTabIndex(const QModelIndex &index);

	void initGridProperties();
	void disconnectZoom(QGraphicsView* view);
	void connectZoom(QGraphicsView* view);
	void disconnectActionZoomTo(QWidget* widget);
	void connectActionZoomTo(QWidget* widget);
	void setConnectActionZoomTo(QWidget* widget);
	void clickErrorListWidget();

	void setShowGrid(bool isChecked);
	void setShowAlignment(bool isChecked);
	void setSwitchGrid(bool isChecked);
	void setSwitchAlignment(bool isChecked);

	void setIndexesOfPropertyEditor(Id const &id);

	/** @brief Check if we need to hide widget in fullscreen mode or not. If we do, hide it
		@param dockWidget QDockWidget to hide
		@param name Widget's name in internal map
	*/
	void hideDockWidget(QDockWidget *dockWidget, QString name);

	/** @brief Check if we need to show widget in fullscreen mode or not. If we do, show it
		@param dockWidget QDockWidget to show
		@param name Widget's name in internal map
	*/
	void showDockWidget(QDockWidget *dockWidget, QString name);

	QString getNextDirName(QString const &name);

	void initToolPlugins();
};
}
