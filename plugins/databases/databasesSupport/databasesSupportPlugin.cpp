/* Copyright 2014-2016 Anastasia Semenova
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "databasesSupportPlugin.h"

namespace qReal {

void loadTranslators(const QString &locale)
{
	QDir translationsDirectory(qReal::PlatformInfo::invariantSettingsPath("pathToTranslations") + "/" + locale);
	QDirIterator directories(translationsDirectory, QDirIterator::Subdirectories);
	while (directories.hasNext()) {
		for (const QFileInfo &translatorFile : QDir(directories.next()).entryInfoList(QDir::Files)) {
			QTranslator *translator = new QTranslator(qApp);
			translator->load(translatorFile.absoluteFilePath());
			QCoreApplication::installTranslator(translator);
		}
	}
}
namespace databasesSupport {

DatabasesSupportPlugin::DatabasesSupportPlugin()
	: mPreferencesPage(new DatabasesPreferencesPage())
	, mGenerateCodeAction(NULL)
	, mCheckCorectnessAction(NULL)
	, mGenerateSchemaWidget(new GenerateSchemaWidget())
{
	connect(mGenerateSchemaWidget, SIGNAL(accepted()), this, SLOT(generateSchema()));
}

DatabasesSupportPlugin::~DatabasesSupportPlugin()
{
	delete mDatabasesGenerator;
	delete mDatabasesReverseEngineer;
	delete mGenerateSchemaWidget;
}

void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	loadTranslators(QLocale::system().name());
	/*QDir translationsDirectory(qReal::PlatformInfo::invariantSettingsPath("pathToTranslations") + "/" + locale);
	QDirIterator directories(translationsDirectory, QDirIterator::Subdirectories);
	while (directories.hasNext()) {
		for (const QFileInfo &translatorFile : QDir(directories.next()).entryInfoList(QDir::Files)) {
			QTranslator *translator = new QTranslator(qApp);
			translator->load(translatorFile.absoluteFilePath());
			QCoreApplication::installTranslator(translator);
		}
	}

	mAppTranslator.load("databasesSupport_" + QLocale::system().name());
	QCoreApplication::installTranslator(&mAppTranslator);*/

	mDatabasesGenerator = new DatabasesGenerator(configurator, mPreferencesPage);
	mDatabasesReverseEngineer = new DatabasesReverseEngineer(configurator, mPreferencesPage);
	initActions();
}

QPair<QString, qReal::gui::PreferencesPage *> DatabasesSupportPlugin::preferencesPage()
{
	return qMakePair(tr("Databases"), static_cast<qReal::gui::PreferencesPage *>(mPreferencesPage));
}

qReal::Customizer* DatabasesSupportPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<qReal::ActionInfo> DatabasesSupportPlugin::actions()
{
	return mActionInfos;
}

void DatabasesSupportPlugin::initActions()
{
	mCheckCorectnessAction = new QAction(tr("Check correctness of the logical schema"), NULL);
	mCheckCorectnessAction->setShortcut(QKeySequence(Qt::Key_F5));
	connect(mCheckCorectnessAction, SIGNAL(triggered()), this, SLOT(checkCorrectness()));
	mActionInfos << ActionInfo(mCheckCorectnessAction, "generators", "tools");

	mGeneratePhysicalModelAction = new QAction(tr("Generate physical model"), NULL);
	mGeneratePhysicalModelAction->setShortcut(QKeySequence(Qt::Key_F6));
	connect(mGeneratePhysicalModelAction, SIGNAL(triggered()), this, SLOT(generatePhysicalModel()));
	mActionInfos << ActionInfo(mGeneratePhysicalModelAction, "generators", "tools");

	mGenerateCodeAction = new QAction(tr("Generate SQL code"), NULL);
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::Key_F7));
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));
	mActionInfos << ActionInfo(mGenerateCodeAction, "generators", "tools");

	mGenerateSchemaAction = new QAction(tr("Generate Schema from file"), NULL);
	mGenerateSchemaAction->setShortcut(QKeySequence(Qt::Key_F8));
	connect(mGenerateSchemaAction, SIGNAL(triggered()), mGenerateSchemaWidget, SLOT(open()));
	mActionInfos << ActionInfo(mGenerateSchemaAction, "generators", "tools");
}

void DatabasesSupportPlugin::checkCorrectness()
{
	mDatabasesGenerator->checkLogicalModelCorrectness();
}

void DatabasesSupportPlugin::generateCode()
{
	mDatabasesGenerator->generateSQLCode();
}

void DatabasesSupportPlugin::generatePhysicalModel()
{
	mDatabasesGenerator->generatePhysicalModel();
}

void DatabasesSupportPlugin::generateSchema()
{
	mDatabasesReverseEngineer->generateSchema(mGenerateSchemaWidget->getDbms(), mGenerateSchemaWidget->getFilePath());
}

}
}
