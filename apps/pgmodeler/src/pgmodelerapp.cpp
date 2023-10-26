/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#include "pgmodelerapp.h"
#include "globalattributes.h"
#include "messagebox.h"
#include "attributes.h"
#include <QScreen>

PgModelerApp::PgModelerApp(int &argc, char **argv) : Application(argc,argv)
{
	QString plugin_name, plug_lang_dir, plug_lang_file;
	QStringList dir_list;
	QDir dir;

	try
	{
		//Creating the initial user's configuration
		createUserConfiguration(true);
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}

	//Changing the current working dir to the executable's directory in
	QDir::setCurrent(this->applicationDirPath());

	//Adding paths which executable will find plugins and it's dependecies
	this->addLibraryPath(this->applicationDirPath());

	//If pgModeler bundles plugins, add the root plugins path to lib search paths
	if(dir.exists(GlobalAttributes::getPluginsPath()))
		this->addLibraryPath(GlobalAttributes::getPluginsPath());

	//Check if the temporary dir exists, if not, creates it.
	if(!dir.exists(GlobalAttributes::getTemporaryPath()))
	{
		if(!dir.mkdir(GlobalAttributes::getTemporaryPath()))
		{
			Messagebox msg;
			msg.show(Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(GlobalAttributes::getTemporaryPath()),
												 ErrorCode::FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__));
		}
	}

	//Trying to identify if the user defined a custom UI language in the pgmodeler.conf file
	QString lang_id = GlobalAttributes::getConfigParamFromFile(Attributes::UiLanguage, GlobalAttributes::GeneralConf);

	if(lang_id.isEmpty())
		lang_id = QLocale::system().name();

	loadTranslation(lang_id);

	//Trying to load plugins translations
	dir_list=QDir(GlobalAttributes::getPluginsPath() +
								GlobalAttributes::DirSeparator,
								QString("*"), QDir::Name, QDir::AllDirs | QDir::NoDotAndDotDot).entryList();

	while(!dir_list.isEmpty())
	{
		plugin_name=dir_list.front();
		dir_list.pop_front();

		//Configure the path to "lang" subdir at current plugin directory
		plug_lang_dir=GlobalAttributes::getPluginsPath() +
					  GlobalAttributes::DirSeparator + plugin_name +
					  GlobalAttributes::DirSeparator + QString("lang") +
					  GlobalAttributes::DirSeparator;

		plug_lang_file=plugin_name + QString(".") + lang_id;
		loadTranslation(plug_lang_file, plug_lang_dir);
	}
}

bool PgModelerApp::notify(QObject *receiver, QEvent *event)
{
	try
	{
		return QApplication::notify(receiver,event);
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
		return false;
	}
	catch(...)
	{
		Messagebox msg_box;
		msg_box.show(tr("Unknown exception caught!"), Messagebox::ErrorIcon);
		return false;
	}
}
